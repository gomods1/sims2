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
std::string FundsText();
// "cámara: vista media, 3 de 6, mirando al noreste": the control panel's item (empty: no camera yet).
std::string CameraLine();
// Ctrl+Shift+F: the camera follows the selected Sim (the game's Return, when the Sim has moved away:
// the game's sounds come from what is on screen), or stays where the player left it.
void ToggleFollow();
// Ctrl+N: the narrator speaks everything, warnings only (urgent needs, visitors), or nothing.
void CycleNarrator();
// Whose conversations are spoken (talk balloons and the relationship headlines socials bring):
// everyone on the lot; anyone talking with the family (the default); the family's own; nobody
// (kept in the events buffer). Ctrl+Shift+N cycles it ([mod] social=3/2/1/0).
enum SocialScopeLevel { kSocialNobody = 0, kSocialFamily = 1, kSocialWithFamily = 2, kSocialEveryone = 3 };
void CycleSocial();
int RunningActionWith();  // the object the selected Sim's running action is done with (its id), 0 none
int SocialScope();
// Ctrl+T: every new hour of the game's clock spoken ("son las 14:00"), or not ([mod] hours=1/0).
void ToggleHours();
// A narrator line: spoken (and kept in the events buffer) as the verbosity allows (warnings are
// spoken unless the narrator is off), else only kept.
void Narrate(const std::string& line, bool warning);  // the control panel's funds as shown ("83.787 simoleones"), empty if none

}  // namespace nav::live
