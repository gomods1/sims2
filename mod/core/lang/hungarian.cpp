// Hungarian: the game's language 25. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "The Sims 2 akadálymentesítés betöltve"},
    {Str::UnsupportedBuild, "The Sims 2 akadálymentesítés: nem támogatott játékverzió, a mod kikapcsolva"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "A nyilak egy csoporton belül mozognak, a Tab és a Shift+Tab csoportot vált, a Home és az End a végekre "
     "ugrik, az Enter megnyom, az R megismétel. A Ctrl+Fel és a Ctrl+Le felolvassa az elhangzottak részleteit, a "
     "Ctrl+Balra és a Ctrl+Jobbra a részletek és az események között vált."},
    // Control state
    {Str::Checked, "bejelölve"},
    {Str::NotChecked, "nincs bejelölve"},
    {Str::Selected, "kiválasztva"},
    {Str::TabSelected, "kiválasztva"},
    {Str::Unavailable, "nem elérhető"},
    {Str::UnlabeledButton, "felirat nélküli gomb"},
    {Str::Unnamed, "névtelen"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "gomb"},
    {Str::RoleTab, "fül"},
    {Str::RoleCheckBox, "jelölőnégyzet"},
    {Str::RoleSlider, "csúszka"},
    {Str::RolePicker, "értékválasztó"},
    {Str::NothingHere, "itt nincs mit bejárni"},
    // Review buffers
    {Str::BufferDetails, "részletek"},
    {Str::BufferEvents, "események"},
    {Str::BufferEmpty, "%s, üres"},
    {Str::PositionInGroup, "%d / %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d elem"},
    {Str::GroupSummaryOne, "%s, 1 elem"},
    {Str::GroupButtons, "gombok"},
    {Str::GroupText, "szöveg"},
    {Str::GroupControlPanel, "vezérlőpult"},
    {Str::GroupLivePanel, "Sim-panel"},
    {Str::GroupFamily, "család"},
    {Str::GroupWantsFears, "vágyak és félelmek"},
    {Str::GroupNeeds, "szükségletek"},
    {Str::GroupControls, "vezérlők"},
    // Text fields
    {Str::TextField, "%s %s, szövegmező"},
    {Str::Blank, "üres"},
    {Str::Space, "szóköz"},
    // Values
    {Str::ValueOfMax, "%d / %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "következő oldal"},
    {Str::PreviousPage, "előző oldal"},
    // Interaction menu
    {Str::PieMenu, "interakciók"},
    {Str::NoInteractions, "nincs interakció"},
    {Str::Cancelled, "megszakítva"},
    // Cheat console
    {Str::CheatConsole, "csalókonzol"},
    {Str::CheatRan, "csalás: %s"},
    {Str::CheatEmpty, "előbb írj be vagy válassz egy csalást"},
    // Neighborhood: lots and families
    {Str::GroupLots, "telkek"},
    {Str::LotFamily, "%s család"},
    {Str::LotForSale, "eladó"},
    {Str::LotCommunity, "közösségi telek"},
    {Str::LotApartmentsEmpty, "társasház, lakók nélkül"},
    {Str::LotApartmentsWith, "társasház: %s"},
    {Str::FamilySummary, "%1$s: %2$d tag, pénz %3$s"},
    // Money
    {Str::Simoleons, "simoleon"},
    {Str::FundsNow, "pénz %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Simek"},
    {Str::CategoryOther, "egyéb tárgyak"},
    {Str::NoObjects, "itt nincsenek tárgyak"},
    {Str::FloorNumber, "%d. szint"},
    {Str::NotOnLot, "nincs telken"},
    {Str::HereNow, "pont itt"},
    {Str::TilesAway, "%d mező, %s"},
    {Str::OneTileAway, "1 mező, %s"},
    {Str::DirNorth, "északra"},
    {Str::DirNorthEast, "északkeletre"},
    {Str::DirEast, "keletre"},
    {Str::DirSouthEast, "délkeletre"},
    {Str::DirSouth, "délre"},
    {Str::DirSouthWest, "délnyugatra"},
    {Str::DirWest, "nyugatra"},
    {Str::DirNorthWest, "északnyugatra"},
    {Str::OtherFloor, "másik szinten"},
    {Str::SelectedSimSuffix, "kiválasztva"},
    {Str::NoInteractionsAtAll, "nincs interakció: ezzel a tárggyal semmit sem lehet csinálni"},
    {Str::WalkingThere, "odamegy"},
    {Str::CannotWalk, "oda nem lehet menni"},
    {Str::NoSimSelected, "nincs kiválasztott Sim"},
    // Needs
    {Str::MotiveValue, "%s, %d százalék"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "cselekvési sor"},
    {Str::StatusIdle, "nem csinál semmit"},
    {Str::StatusNeedsLow, "alacsony szükségletek: %s"},
    {Str::StatusNeedsFine, "a szükségletek rendben"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s befejezte: %2$s"},
    {Str::NeedLow, "%1$s, alacsony szükséglet: %2$s"},
    {Str::NeedUrgent, "%1$s, sürgős szükséglet: %2$s"},
    // Narrator: visitors
    {Str::FundsChanged, "%1$s, maradt %2$s"},
};

}  // namespace

extern const Translations kHungarian = {kEntries, std::size(kEntries)};

}  // namespace lang
