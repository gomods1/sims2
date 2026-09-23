// Decompiles the functions containing the given addresses into <outDir>/<va>_<name>.c.
// Args: <outDir> <va> [va ...]   (hex, preferred-base VAs)
// @category SimsRE

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;

import java.io.File;
import java.io.PrintWriter;

public class Decompile extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File out = new File(args[0]);
        out.mkdirs();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (int i = 1; i < args.length; i++) {
            // "+va": a function starts exactly there (Ghidra sometimes merges it into the one
            // before): the containing function is removed and both are made again (in memory
            // only, the project is opened read-only).
            boolean exact = args[i].startsWith("+");
            ghidra.program.model.address.Address at = toAddr(Long.parseLong(args[i].replace("+", "").replace("0x", ""), 16));
            if (exact) {
                Function old = getFunctionContaining(at);
                if (old == null && getFunctionAt(at) == null) {
                    if (getInstructionAt(at) == null) {
                        clearListing(at, at.add(15));
                        disassemble(at);
                    }
                    createFunction(at, null);
                } else if (old != null && !old.getEntryPoint().equals(at)) {
                    ghidra.program.model.address.Address oldEntry = old.getEntryPoint();
                    removeFunction(old);
                    if (getInstructionAt(at) == null) {
                        clearListing(at, at.add(15));
                        disassemble(at);
                    }
                    Function made = createFunction(at, null);
                    if (made == null) {  // flow from the old entry claims it: make it by hand
                        ghidra.app.cmd.function.CreateFunctionCmd cmd =
                            new ghidra.app.cmd.function.CreateFunctionCmd(null, at, null,
                                ghidra.program.model.symbol.SourceType.USER_DEFINED, false, true);
                        println("create at " + at + ": " + cmd.applyTo(currentProgram, monitor) + " " + cmd.getStatusMsg());
                    }
                    println("split " + oldEntry + " / " + at + ": " + (getFunctionAt(at) != null));
                }
            }
            Function f = getFunctionContaining(at);
            if (f == null) {
                println("no function at " + args[i]);
                continue;
            }
            DecompileResults r = di.decompileFunction(f, 120, monitor);
            String name = f.getName(true).replaceAll("[^A-Za-z0-9_]", "_");
            File file = new File(out, String.format("%08x_%s.c", f.getEntryPoint().getOffset(), name));
            try (PrintWriter w = new PrintWriter(file, "UTF-8")) {
                w.println("// " + f.getName(true) + " @ " + f.getEntryPoint() + " (requested " + args[i] + ")");
                w.println(r.decompileCompleted() ? r.getDecompiledFunction().getC() : "// FAILED: " + r.getErrorMessage());
            }
            println("wrote " + file.getName());
        }
        di.dispose();
    }
}
