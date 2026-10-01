// Applies build/types.json (tools/ghidratypes.py) to the program: struct,
// union and enum layouts, matched functions' signatures and class namespaces,
// and the types of globals every file agrees on.
// Args: <types.json>
// @category ByteTactics

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.listing.Function.FunctionUpdateType;
import ghidra.program.model.symbol.Namespace;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.program.model.util.CodeUnitInsertionException;
import ghidra.program.model.data.DataUtilities.ClearDataMode;
import ghidra.util.data.DataTypeParser;
import ghidra.util.data.DataTypeParser.AllowedDataTypes;

public class BtImportTypes extends GhidraScript {
    private DataTypeManager dtm;
    private JsonObject defs;
    private final Map<String, DataType> named = new HashMap<>();
    private final Map<String, DataType> functionTypes = new HashMap<>();
    private final Set<String> filled = new HashSet<>();
    private int fieldErrors = 0;

    @Override
    public void run() throws Exception {
        JsonObject root = JsonParser.parseString(
            Files.readString(Paths.get(getScriptArgs()[0]))).getAsJsonObject();
        dtm = currentProgram.getDataTypeManager();
        defs = root.getAsJsonObject("types");

        for (String name : defs.keySet()) {
            JsonObject t = defs.getAsJsonObject(name);
            CategoryPath cat = category(name);
            String leaf = clean(leafName(name));
            DataType dt;
            switch (t.get("kind").getAsString()) {
                case "enum":
                    EnumDataType e = new EnumDataType(cat, leaf, t.get("size").getAsInt(), dtm);
                    for (JsonElement v : t.getAsJsonArray("values")) {
                        JsonArray pair = v.getAsJsonArray();
                        try {
                            e.add(pair.get(0).getAsString(), pair.get(1).getAsLong());
                        }
                        catch (IllegalArgumentException ex) {
                            fieldErrors++;
                        }
                    }
                    dt = e;
                    break;
                case "union":
                    dt = new UnionDataType(cat, leaf, dtm);
                    break;
                default:
                    dt = new StructureDataType(cat, leaf, 0, dtm);
            }
            named.put(name, dtm.addDataType(dt, DataTypeConflictHandler.REPLACE_HANDLER));
        }
        for (String name : defs.keySet()) {
            fill(name);
        }
        println("ByteTactics: " + named.size() + " types, " + fieldErrors + " fields not placed");

        int applied = 0, missing = 0, failed = 0;
        for (JsonElement el : root.getAsJsonArray("functions")) {
            JsonObject f = el.getAsJsonObject();
            Address addr = toAddr(Long.decode(f.get("address").getAsString()));
            Function fn = getFunctionAt(addr);
            if (fn == null) {
                missing++;
                continue;
            }
            try {
                applySignature(fn, f);
                applied++;
            }
            catch (Exception ex) {
                println("ByteTactics: " + f.get("address").getAsString() + ": " + ex.getMessage());
                failed++;
            }
        }
        println("ByteTactics: " + applied + " signatures applied, " + missing +
            " with no function, " + failed + " failed");

        int typed = 0;
        DataTypeParser parser = new DataTypeParser(dtm, dtm, null, AllowedDataTypes.FIXED_LENGTH);
        for (JsonElement el : root.getAsJsonArray("globals")) {
            JsonObject g = el.getAsJsonObject();
            DataType dt = globalType(parser, g.get("type").getAsString(), g.get("size").getAsInt());
            if (dt == null) {
                continue;
            }
            Address addr = toAddr(Long.decode(g.get("address").getAsString()));
            Data existing = getDataAt(addr);
            if (existing != null && existing.getDataType().isEquivalent(dt)) {
                typed++;
                continue;
            }
            try {
                DataUtilities.createData(currentProgram, addr, dt, -1,
                    ClearDataMode.CLEAR_ALL_UNDEFINED_CONFLICT_DATA);
                typed++;
            }
            catch (CodeUnitInsertionException ex) {
                // Ghidra already put something defined there; keep it.
            }
        }
        println("ByteTactics: " + typed + " globals typed");
    }

    private void fill(String name) {
        if (!filled.add(name)) {
            return;
        }
        JsonObject t = defs.getAsJsonObject(name);
        String kind = t.get("kind").getAsString();
        if (kind.equals("enum") || t.has("opaque")) {
            return;
        }
        JsonArray fields = t.getAsJsonArray("fields");
        // Members held by value must have their final size before they are placed.
        for (JsonElement f : fields) {
            for (String dep : byValue(f.getAsJsonObject().get("type"))) {
                fill(dep);
            }
        }
        Composite built = kind.equals("union")
                ? new UnionDataType(category(name), clean(leafName(name)), dtm)
                : new StructureDataType(category(name), clean(leafName(name)), t.get("size").getAsInt(), dtm);
        for (JsonElement el : fields) {
            JsonObject f = el.getAsJsonObject();
            DataType dt = convert(f.get("type"));
            String fname = clean(f.get("name").getAsString());
            int off = f.get("off").getAsInt();
            try {
                if (f.has("bits")) {
                    JsonArray bits = f.getAsJsonArray("bits");
                    int width = f.get("width").getAsInt();
                    if (built instanceof Structure s) {
                        s.insertBitFieldAt(off, width, bits.get(0).getAsInt(), dt,
                            bits.get(1).getAsInt(), fname, null);
                    }
                    else {
                        ((Union) built).addBitField(dt, bits.get(1).getAsInt(), fname, null);
                    }
                }
                else if (dt.getLength() <= 0) {
                    fieldErrors++;
                }
                else if (built instanceof Structure s) {
                    s.replaceAtOffset(off, dt, dt.getLength(), fname, null);
                }
                else {
                    built.add(dt, dt.getLength(), fname, null);
                }
            }
            catch (Exception ex) {
                fieldErrors++;
            }
        }
        ((Composite) named.get(name)).replaceWith(built);
    }

    private List<String> byValue(JsonElement e) {
        List<String> out = new ArrayList<>();
        if (e.isJsonObject()) {
            JsonObject o = e.getAsJsonObject();
            if (o.has("ref")) {
                out.add(o.get("ref").getAsString());
            }
            else if (o.has("arr")) {
                out.addAll(byValue(o.get("arr")));
            }
        }
        return out;
    }

    private DataType convert(JsonElement e) {
        if (e.isJsonPrimitive()) {
            switch (e.getAsString()) {
                case "void": return VoidDataType.dataType;
                case "char": return CharDataType.dataType;
                case "uchar": return UnsignedCharDataType.dataType;
                case "short": return ShortDataType.dataType;
                case "ushort": return UnsignedShortDataType.dataType;
                case "int": return IntegerDataType.dataType;
                case "uint": return UnsignedIntegerDataType.dataType;
                case "long": return LongDataType.dataType;
                case "ulong": return UnsignedLongDataType.dataType;
                case "longlong": return LongLongDataType.dataType;
                case "ulonglong": return UnsignedLongLongDataType.dataType;
                case "float": return FloatDataType.dataType;
                case "double":
                case "longdouble": return DoubleDataType.dataType;
                case "bool": return BooleanDataType.dataType;
                case "wchar": return WideChar16DataType.dataType;
                default: return Undefined4DataType.dataType;
            }
        }
        JsonObject o = e.getAsJsonObject();
        if (o.has("ptr")) {
            return new PointerDataType(convert(o.get("ptr")), 4, dtm);
        }
        if (o.has("arr")) {
            DataType elem = convert(o.get("arr"));
            int n = o.get("n").getAsInt();
            if (n <= 0 || elem.getLength() <= 0) {
                return VoidDataType.dataType;
            }
            return new ArrayDataType(elem, n, elem.getLength(), dtm);
        }
        if (o.has("ref")) {
            DataType dt = named.get(o.get("ref").getAsString());
            return dt != null ? dt : Undefined4DataType.dataType;
        }
        if (o.has("raw")) {
            return Undefined.getUndefinedDataType(o.get("raw").getAsInt());
        }
        String key = o.get("fn").toString();
        DataType known = functionTypes.get(key);
        if (known != null) {
            return known;
        }
        JsonObject sig = o.getAsJsonObject("fn");
        FunctionDefinitionDataType def = new FunctionDefinitionDataType(
            new CategoryPath("/ByteTactics/fn"), "fn_" + Integer.toHexString(key.hashCode()), dtm);
        def.setReturnType(convert(sig.get("ret")));
        List<ParameterDefinition> args = new ArrayList<>();
        for (JsonElement p : sig.getAsJsonArray("params")) {
            args.add(new ParameterDefinitionImpl(null, convert(p), null));
        }
        def.setArguments(args.toArray(new ParameterDefinition[0]));
        def.setVarArgs(sig.get("varargs").getAsBoolean());
        try {
            def.setCallingConvention(sig.get("conv").getAsString());
        }
        catch (Exception ex) {
            // Unknown convention name: leave the default.
        }
        DataType added = dtm.addDataType(def, DataTypeConflictHandler.REPLACE_HANDLER);
        functionTypes.put(key, added);
        return added;
    }

    private void applySignature(Function fn, JsonObject f) throws Exception {
        String conv = f.get("conv").getAsString();
        List<String> parts = splitScope(f.get("name").getAsString());
        String leaf = parts.remove(parts.size() - 1);
        if (!parts.isEmpty()) {
            fn.setParentNamespace(namespace(parts, conv.equals("__thiscall")));
        }
        String expected = String.format("FUN_%08x", fn.getEntryPoint().getOffset());
        // A FUN_ name for another address is a shared body; the address's own name is clearer.
        if (!leaf.startsWith("FUN_") || leaf.equalsIgnoreCase(expected)) {
            fn.setName(clean(leaf), SourceType.IMPORTED);
        }

        JsonArray types = f.getAsJsonArray("params");
        JsonArray names = f.getAsJsonArray("param_names");
        List<Variable> params = new ArrayList<>();
        Set<String> used = new HashSet<>();
        for (int i = 0; i < types.size(); i++) {
            String pname = i < names.size() ? clean(names.get(i).getAsString()) : "param_" + (i + 1);
            if (!used.add(pname) || pname.equals("this")) {
                pname = "param_" + (i + 1);
            }
            params.add(new ParameterImpl(pname, convert(types.get(i)), currentProgram));
        }
        fn.updateFunction(conv, new ReturnParameterImpl(convert(f.get("ret")), currentProgram),
            params, FunctionUpdateType.DYNAMIC_STORAGE_FORMAL_PARAMS, true, SourceType.IMPORTED);
        fn.setVarArgs(f.get("varargs").getAsBoolean());
    }

    // Places a type where Ghidra looks for a class's struct when it types `this`:
    // the category path matching the class's enclosing namespaces.
    private CategoryPath category(String name) {
        List<String> parts = splitScope(name);
        CategoryPath cat = CategoryPath.ROOT;
        for (String p : parts.subList(0, parts.size() - 1)) {
            cat = new CategoryPath(cat, clean(p));
        }
        return cat;
    }

    private String leafName(String name) {
        List<String> parts = splitScope(name);
        return parts.get(parts.size() - 1);
    }

    private Namespace namespace(List<String> parts, boolean isClass) throws Exception {
        SymbolTable st = currentProgram.getSymbolTable();
        Namespace ns = currentProgram.getGlobalNamespace();
        for (int i = 0; i < parts.size(); i++) {
            String n = clean(parts.get(i));
            Namespace next = st.getNamespace(n, ns);
            boolean last = i == parts.size() - 1;
            if (next == null) {
                next = last && isClass ? st.createClass(ns, n, SourceType.IMPORTED)
                        : st.createNameSpace(ns, n, SourceType.IMPORTED);
            }
            else if (last && isClass && !(next instanceof GhidraClass)) {
                next = ghidra.app.util.NamespaceUtils.convertNamespaceToClass(next);
            }
            ns = next;
        }
        return ns;
    }

    // Splits on :: outside template arguments.
    private static List<String> splitScope(String name) {
        List<String> parts = new ArrayList<>();
        int depth = 0, start = 0;
        for (int i = 0; i < name.length(); i++) {
            char c = name.charAt(i);
            if (c == '<') {
                depth++;
            }
            else if (c == '>') {
                depth--;
            }
            else if (depth == 0 && c == ':' && i + 1 < name.length() && name.charAt(i + 1) == ':') {
                parts.add(name.substring(start, i));
                start = i + 2;
                i++;
            }
        }
        parts.add(name.substring(start));
        return parts;
    }

    // Ghidra symbol names may not contain whitespace.
    private static String clean(String name) {
        return name.replaceAll("(?<=\\w)\\s+(?=\\w)", "_").replaceAll("\\s+", "");
    }

    private DataType globalType(DataTypeParser parser, String type, int size) {
        String t = type.replace("const ", "").trim();
        boolean open = t.endsWith("[]");
        if (open) {
            t = t.substring(0, t.length() - 2);
        }
        DataType dt;
        try {
            dt = parser.parse(t);
        }
        catch (Exception ex) {
            DataType byName = named.get(t);
            if (byName == null) {
                return null;
            }
            dt = byName;
        }
        if (open) {
            if (dt.getLength() <= 0 || size <= 0 || size % dt.getLength() != 0) {
                return null;
            }
            return new ArrayDataType(dt, size / dt.getLength(), dt.getLength(), dtm);
        }
        return dt.getLength() == size ? dt : null;
    }
}
