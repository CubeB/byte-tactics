// Export the hand-made names of a Ghidra program (functions, globals, structs and
// their fields) to CSV, so a named database can be read without Ghidra.
// Args: <output directory>
// @category ByteTactics

import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class BtExportNames extends GhidraScript {

    private static String csv(String s) {
        if (s == null) return "";
        s = s.replace("\r", " ").replace("\n", " ").trim();
        if (s.contains(",") || s.contains("\"")) return "\"" + s.replace("\"", "\"\"") + "\"";
        return s;
    }

    private static PrintWriter open(File dir, String name, String header) throws IOException {
        PrintWriter w = new PrintWriter(new OutputStreamWriter(
            new FileOutputStream(new File(dir, name)), StandardCharsets.UTF_8));
        w.println(header);
        return w;
    }

    // Ghidra's own placeholders carry no evidence, only the names a person gave.
    private static boolean isDefault(String n) {
        return n.matches("(FUN|DAT|LAB|PTR|s|u|BYTE|WORD|DWORD|switchD|caseD|thunk_FUN|Unwind)_.*")
            || n.matches("(PTR_)?(FUN|DAT|LAB)_[0-9a-fA-F]+.*");
    }

    @Override
    public void run() throws Exception {
        File dir = new File(getScriptArgs()[0]);
        dir.mkdirs();
        Listing listing = currentProgram.getListing();

        try (PrintWriter w = open(dir, "functions.csv", "address,namespace,name,signature,comment")) {
            for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
                if (isDefault(f.getName())) continue;
                Namespace ns = f.getParentNamespace();
                String nsName = ns == null || ns.isGlobal() ? "" : ns.getName(true);
                w.println(String.format("0x%s,%s,%s,%s,%s",
                    f.getEntryPoint().toString().toLowerCase(), csv(nsName), csv(f.getName()),
                    csv(f.getPrototypeString(true, false)), csv(f.getComment())));
            }
        }

        try (PrintWriter w = open(dir, "globals.csv", "address,name,type,comment")) {
            SymbolIterator it = currentProgram.getSymbolTable().getAllSymbols(true);
            while (it.hasNext()) {
                Symbol s = it.next();
                if (s.getSymbolType() != SymbolType.LABEL || s.getSource() == SourceType.DEFAULT) continue;
                if (isDefault(s.getName())) continue;
                if (currentProgram.getMemory().getBlock(s.getAddress()) == null) continue;
                if (currentProgram.getMemory().getBlock(s.getAddress()).isExecute()) continue;
                Data d = listing.getDataAt(s.getAddress());
                String type = d == null ? "" : d.getDataType().getDisplayName();
                String comment = d == null ? null : d.getComment(CodeUnit.EOL_COMMENT);
                w.println(String.format("0x%s,%s,%s,%s",
                    s.getAddress().toString().toLowerCase(), csv(s.getName(true)), csv(type), csv(comment)));
            }
        }

        try (PrintWriter w = open(dir, "structs.csv", "struct,size,offset,field,type,comment")) {
            List<Structure> structs = new ArrayList<>();
            currentProgram.getDataTypeManager().getAllStructures().forEachRemaining(structs::add);
            structs.sort(Comparator.comparing(Structure::getPathName));
            for (Structure st : structs) {
                if (st.getSourceArchive() != null
                        && st.getSourceArchive().getSourceArchiveID()
                           != currentProgram.getDataTypeManager().getLocalSourceArchive().getSourceArchiveID())
                    continue;
                String sn = st.getCategoryPath().isRoot() ? st.getName() : st.getPathName();
                for (DataTypeComponent c : st.getDefinedComponents()) {
                    String fn = c.getFieldName();
                    if (fn == null && c.getComment() == null) continue;
                    w.println(String.format("%s,0x%x,0x%x,%s,%s,%s",
                        csv(sn), st.getLength(), c.getOffset(), csv(fn),
                        csv(c.getDataType().getDisplayName()), csv(c.getComment())));
                }
            }
        }
        println("exported to " + dir);
    }
}
