// Live mode state by ear: the selected Sim's needs (from its data, the needs panel need not be
// open), the action queue, the Sim's state on Ctrl+E, and the game speed (pause) as it changes.
#pragma once

#include <string>
#include <vector>

#include "model.h"

namespace nav::live {

// Groups the navigator adds to the lot view.
Group NeedsGroup();        // kNeedsGroup: each need with its level, lowest first
Group QueueGroup();        // kQueueGroup: the selected Sim's queued actions, current first

void CancelQueued(const Item& it);  // Enter on a queued action: the game's cancel (a click)
void SayStatus();                   // Ctrl+E
void Tick();                        // speed and pause changes, the family watch
void SelectMember(int index);       // Ctrl+1..9: the n-th Sim of the family (its portrait)
std::string RatingName(int rating);  // a catalog rating (game::Product::ratings index): the need's name
std::string FundsText();  // the control panel's funds as shown ("83.787 simoleones"), empty if none

}  // namespace nav::live
