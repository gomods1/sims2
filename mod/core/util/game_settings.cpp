#include "game_settings.h"

#include <windows.h>

namespace util {

int ReadGameLanguageNumber() {
    DWORD value = 0;
    DWORD size = sizeof value;
    if (RegGetValueA(HKEY_CURRENT_USER, "Software\\Electronic Arts\\The Sims 2 Ultimate Collection 25\\1.0",
                     "language", RRF_RT_REG_DWORD, nullptr, &value, &size) != ERROR_SUCCESS)
        return 0;
    return (int)value;
}

}  // namespace util
