// Pre-analysis settings for the Sims2EP9.exe dump.
// The MSVC _EH_prolog helper (0xa08248b0: pushes the handler frame, then "ret" into the caller)
// is taken for a non-returning function by the "discovered" analyzer, which then cuts every
// function using it short at the prolog call.
// @category SimsRE

import ghidra.app.script.GhidraScript;

public class PreAnalysis extends GhidraScript {
    @Override
    protected void run() throws Exception {
        setAnalysisOption(currentProgram, "Non-Returning Functions - Discovered", "false");
        println("disabled Non-Returning Functions - Discovered");
    }
}
