#include "console.h"

#include <string>

#include "../game/gz.h"
#include "../util/text.h"
#include "buffers.h"

namespace nav::console {
namespace {

size_t g_seen = 0;       // the output's length already read
bool g_open = false;

game::Ptr FindConsole() {
    for (game::Ptr w : game::Children(game::MainWindow()))
        if (game::Visible(w) && game::Caption(w) == "Debug Console") return w;
    return 0;
}

}  // namespace

void Tick() {
    game::Ptr console = FindConsole();
    if (!console) {
        g_open = false;
        return;
    }
    // The output is the tall text field above the one-line input.
    game::Ptr output = 0;
    int tallest = 0;
    for (game::Ptr c : game::Children(console)) {
        if (!game::IsTextEdit(c)) continue;
        RECT r = game::Area(c);
        if (r.bottom - r.top > tallest) {
            tallest = r.bottom - r.top;
            output = c;
        }
    }
    if (!output) return;
    uint32_t length = game::TextEditLength(output);  // the output grows past what ReadStdString takes
    if (!g_open || length < g_seen) {  // opened now, or cleared: start from here
        g_open = true;
        g_seen = length;
        return;
    }
    if (length == g_seen) return;
    std::string added = util::CleanForSpeech(game::TextEditTextFrom(output, (uint32_t)g_seen, 2000));
    g_seen = length;
    if (!added.empty()) buffers::Event(added);
}

}  // namespace nav::console
