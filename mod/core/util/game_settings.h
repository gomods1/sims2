// The game's settings in the registry (HKCU\Software\Electronic Arts\The Sims 2 Ultimate
// Collection 25), which Steam's install script writes: the language the player chose in Steam.
#pragma once

namespace util {

// The "language" value of the 1.0 key: Steam's language number for the game (5 Spanish...);
// 0 when missing.
int ReadGameLanguageNumber();

}  // namespace util
