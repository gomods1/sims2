// German: the game's language 3. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Barrierefreiheit für Die Sims 2 geladen"},
    {Str::UnsupportedBuild, "Barrierefreiheit für Die Sims 2: nicht unterstützte Spielversion, Mod deaktiviert"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Pfeiltasten bewegen innerhalb einer Gruppe, Tab und Umschalt+Tab wechseln die Gruppe, Pos1 und Ende "
     "springen zum Anfang und zum Ende, Eingabe aktiviert, R wiederholt. Strg+Pfeil nach oben und Strg+Pfeil "
     "nach unten lesen die Details des Gesagten, Strg+Pfeil nach links und Strg+Pfeil nach rechts wechseln "
     "zwischen Details und Ereignissen."},
    // Control state
    {Str::Checked, "aktiviert"},
    {Str::NotChecked, "nicht aktiviert"},
    {Str::Selected, "ausgewählt"},
    {Str::TabSelected, "ausgewählt"},
    {Str::Unavailable, "nicht verfügbar"},
    {Str::UnlabeledButton, "Schalter ohne Beschriftung"},
    {Str::Unnamed, "unbenannt"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "Schalter"},
    {Str::RoleTab, "Registerkarte"},
    {Str::RoleCheckBox, "Kontrollkästchen"},
    {Str::RoleSlider, "Schieberegler"},
    {Str::RolePicker, "Auswahlfeld"},
    {Str::NothingHere, "hier gibt es nichts zu durchlaufen"},
    // Review buffers
    {Str::BufferDetails, "Details"},
    {Str::BufferEvents, "Ereignisse"},
    {Str::BufferEmpty, "%s, leer"},
    {Str::PositionInGroup, "%d von %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d Elemente"},
    {Str::GroupSummaryOne, "%s, 1 Element"},
    {Str::GroupButtons, "Schalter"},
    {Str::GroupText, "Text"},
    {Str::GroupControls, "Steuerung"},
    // Text fields
    {Str::TextField, "%s %s, Eingabefeld"},
    {Str::Blank, "leer"},
    {Str::Space, "Leerzeichen"},
    // Values
    {Str::ValueOfMax, "%d von %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "nächste Seite"},
    {Str::PreviousPage, "vorige Seite"},
    // Interaction menu
    {Str::PieMenu, "Interaktionen"},
    {Str::NoInteractions, "keine Interaktionen"},
    {Str::Cancelled, "abgebrochen"},
    // Cheat console
    {Str::CheatConsole, "Cheat-Konsole"},
    {Str::CheatRan, "Cheat: %s"},
    {Str::CheatEmpty, "gib zuerst einen Cheat ein oder wähle einen aus"},
    // Money
    {Str::FundsNow, "Vermögen %s, %s"},
    {Str::FundsChanged, "%1$s, übrig %2$s"},
};

}  // namespace

extern const Translations kGerman = {kEntries, std::size(kEntries)};

}  // namespace lang
