// Dutch: the game's language 8. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Toegankelijkheid voor De Sims 2 geladen"},
    {Str::UnsupportedBuild, "Toegankelijkheid voor De Sims 2: niet-ondersteunde spelversie, mod uitgeschakeld"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Pijltoetsen bewegen binnen een groep, Tab en Shift+Tab wisselen van groep, Home en End gaan naar het begin "
     "en het einde, Enter activeert, R herhaalt. Control+pijl omhoog en Control+pijl omlaag lezen de details van "
     "wat er gezegd is, Control+pijl links en Control+pijl rechts wisselen tussen details en gebeurtenissen."},
    // Control state
    {Str::Checked, "aangevinkt"},
    {Str::NotChecked, "niet aangevinkt"},
    {Str::Selected, "geselecteerd"},
    {Str::TabSelected, "geselecteerd"},
    {Str::Unavailable, "niet beschikbaar"},
    {Str::UnlabeledButton, "knop zonder naam"},
    {Str::Unnamed, "naamloos"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "knop"},
    {Str::RoleTab, "tabblad"},
    {Str::RoleCheckBox, "selectievakje"},
    {Str::RoleSlider, "schuifregelaar"},
    {Str::RolePicker, "waardekiezer"},
    {Str::NothingHere, "hier is niets om te doorlopen"},
    // Review buffers
    {Str::BufferDetails, "details"},
    {Str::BufferEvents, "gebeurtenissen"},
    {Str::BufferEmpty, "%s, leeg"},
    {Str::PositionInGroup, "%d van %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d items"},
    {Str::GroupSummaryOne, "%s, 1 item"},
    {Str::GroupButtons, "knoppen"},
    {Str::GroupText, "tekst"},
    {Str::GroupControls, "besturing"},
    // Text fields
    {Str::TextField, "%s %s, tekstveld"},
    {Str::Blank, "leeg"},
    {Str::Space, "spatie"},
    // Values
    {Str::ValueOfMax, "%d van %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "volgende pagina"},
    {Str::PreviousPage, "vorige pagina"},
    // Interaction menu
    {Str::PieMenu, "interacties"},
    {Str::NoInteractions, "geen interacties"},
    {Str::Cancelled, "geannuleerd"},
    // Cheat console
    {Str::CheatConsole, "cheatconsole"},
    {Str::CheatRan, "cheat: %s"},
    {Str::CheatEmpty, "typ of kies eerst een cheat"},
    // Money
    {Str::FundsNow, "geld %s, %s"},
    {Str::FundsChanged, "%1$s, nog %2$s"},
};

}  // namespace

extern const Translations kDutch = {kEntries, std::size(kEntries)};

}  // namespace lang
