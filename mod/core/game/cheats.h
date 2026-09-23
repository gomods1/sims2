// The game's cheat system (cTSCheatSystem), the one behind the cheat console (Ctrl+Shift+C):
// the mod runs command lines through it (loadLot, setHour, ...).
#pragma once

#include <string>

namespace game {

// Dev command !screen: the navigator logs its whole model on its next tick.
extern bool g_screenDumpRequested;
extern bool g_shutInCheckRequested;  // !shutin: nav/build.cpp runs its shut-in check

// Runs one cheat console line (UTF-8); false when the cheat system is missing or refuses it.
// Output goes to the console's history, like a typed line.
bool RunCheat(const std::string& line);

// Development: runs the line in Sims2Access\cheat_request.txt (tools/game.py cheat) once.
void PollCheatRequest(const wchar_t* modDir);

}  // namespace game
