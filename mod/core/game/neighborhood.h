// The neighborhood view: its lots and the lot info dialog (what a click on a house opens).
#pragma once

#include <string>
#include <vector>

#include "memory.h"

namespace game {

enum class LotType { Residential = 0, Community = 1, ApartmentBuilding = 8, ApartmentUnit = 9 };

struct LotEntry {
    int id = 0;
    int type = 0;           // LotType values; others exist (dorms, vacation lots...)
    std::string name;       // UTF-8
    int x = 0, y = 0;       // on the neighborhood grid
    int width = 0, depth = 0;
    uint32_t flags = 0;
};

struct FamilyEntry {
    int id = 0;
    int lot = 0;            // 0: not moved in
    std::string name, description;
    int funds = 0;
    int members = 0;
};

// The current neighborhood's lots, by id.
std::vector<LotEntry> Lots();
// Every family the game knows in this neighborhood, townies included.
std::vector<FamilyEntry> Families();

// The dialog's window proc (cTSWinProcLotInfoDialog), found through its window (id 0xcdc02de3).
Ptr LotInfoProc();
// Shows the lot info dialog for a lot, as a click on the house does (Show, interface slot 4).
bool ShowLotInfo(int lotId, int arg2, int mode);

// With a family chosen in the family bin (the move-in tool), a click on a lot as the mouse makes
// it: the game then asks what it asks (buy the lot, move in). False when no family is chosen.
bool ClickLotWithTool(int lotId);
void InitNeighborhoodHooks();  // from InitHooks

}  // namespace game
