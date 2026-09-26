// Seeds a freshly imported TotalA.exe with the function map from
// data/functions.csv (FPO starts, runtime library names) before auto-analysis.
// @category ByteTactics

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;

public class BtImportFunctions extends GhidraScript {
    @Override
    public void run() throws Exception {
        List<String> lines = Files.readAllLines(Paths.get(getScriptArgs()[0]));
        int created = 0, named = 0;
        for (String line : lines.subList(1, lines.size())) {
            String[] f = line.split(",", -1);
            if (f[2].equals("gap")) {
                continue; // let analysis decide what these are
            }
            Address addr = toAddr(Long.decode(f[0]));
            disassemble(addr);
            Function fn = getFunctionAt(addr);
            if (fn == null) {
                fn = createFunction(addr, null);
                if (fn != null) {
                    created++;
                }
            }
            if (fn != null && !f[3].isEmpty()) {
                fn.setName(f[3], SourceType.IMPORTED);
                named++;
            }
        }
        println("ByteTactics: created " + created + " functions, named " + named);
    }
}
