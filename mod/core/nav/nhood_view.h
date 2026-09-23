// The neighborhood view: its lots as a list (the houses are 3D, not windows). Enter on a lot
// opens the game's own lot info panel, as a click on the house does.
#pragma once

#include <vector>

#include "model.h"

namespace nav::neighborhood {

bool ViewActive();              // the neighborhood's control panel is on screen
Group LotsGroup();              // kLotsGroup: every lot, homes of families first
std::vector<std::string> LotDetails(const Item& it);
void OpenLot(const Item& it);   // shows the lot info panel for it

}  // namespace nav::neighborhood
