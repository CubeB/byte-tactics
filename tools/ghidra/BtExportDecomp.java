// Writes Ghidra's pseudo-C for every game function in data/functions.csv to
// <outdir>/<address>.c, as a starting point for decompiling by hand.
// Args: <functions.csv> <outdir>
// @category ByteTactics

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;

public class BtExportDecomp extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        List<String> lines = Files.readAllLines(Paths.get(args[0]));
        Path out = Paths.get(args[1]);
        Files.createDirectories(out);

        DecompInterface decomp = new DecompInterface();
        decomp.setOptions(new DecompileOptions());
        decomp.openProgram(currentProgram);

        int done = 0, failed = 0;
        for (String line : lines.subList(1, lines.size())) {
            if (monitor.isCancelled()) {
                break;
            }
            String[] f = line.split(",", -1);
            if (!f[2].equals("game")) {
                continue;
            }
            Function fn = getFunctionAt(toAddr(Long.decode(f[0])));
            String text;
            if (fn == null) {
                text = "// no function at " + f[0] + "\n";
                failed++;
            }
            else {
                DecompileResults r = decomp.decompileFunction(fn, 60, monitor);
                if (r.decompileCompleted()) {
                    text = r.getDecompiledFunction().getC();
                }
                else {
                    text = "// decompile failed: " + r.getErrorMessage() + "\n";
                    failed++;
                }
            }
            Files.writeString(out.resolve(f[0] + ".c"), text);
            if (++done % 250 == 0) {
                println("ByteTactics: decompiled " + done);
            }
        }
        decomp.dispose();
        println("ByteTactics: exported " + done + " functions, " + failed + " failed");
    }
}
