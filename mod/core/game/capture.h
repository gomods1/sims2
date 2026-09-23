// Development screenshots of what the game draws (PrintWindow sees nothing of its D3D9 output):
// tools/game.py shot writes the target path to Sims2Access\shot_request.txt, and the next
// IDirect3DDevice9::Present saves the back buffer there as a BMP.
#pragma once

namespace game {

void InitCapture(const wchar_t* modDir);  // after InitHooks (MinHook is initialized there)
void PollCapture();                        // from the tick

}  // namespace game
