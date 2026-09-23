// Czech: the game's language 11. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Zpřístupnění The Sims 2 načteno"},
    {Str::UnsupportedBuild, "Zpřístupnění The Sims 2: nepodporovaná verze hry, mod vypnut"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Šipky se pohybují ve skupině, Tab a Shift+Tab mění skupinu, Home a End jdou na začátek a konec, Enter "
     "stiskne, R zopakuje. Ctrl+šipka nahoru a Ctrl+šipka dolů čtou podrobnosti řečeného, Ctrl+šipka vlevo a "
     "Ctrl+šipka vpravo přepínají mezi podrobnostmi a událostmi."},
    // Control state
    {Str::Checked, "zaškrtnuto"},
    {Str::NotChecked, "nezaškrtnuto"},
    {Str::Selected, "vybráno"},
    {Str::TabSelected, "vybrána"},
    {Str::Unavailable, "nedostupné"},
    {Str::UnlabeledButton, "tlačítko bez popisku"},
    {Str::Unnamed, "bez názvu"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "tlačítko"},
    {Str::RoleTab, "karta"},
    {Str::RoleCheckBox, "zaškrtávací políčko"},
    {Str::RoleSlider, "posuvník"},
    {Str::RolePicker, "výběr hodnoty"},
    {Str::NothingHere, "zde není nic k procházení"},
    // Review buffers
    {Str::BufferDetails, "podrobnosti"},
    {Str::BufferEvents, "události"},
    {Str::BufferEmpty, "%s, prázdné"},
    {Str::PositionInGroup, "%d z %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, položek: %d"},
    {Str::GroupSummaryOne, "%s, 1 položka"},
    {Str::GroupButtons, "tlačítka"},
    {Str::GroupText, "text"},
    {Str::GroupControlPanel, "ovládací panel"},
    {Str::GroupLivePanel, "panel simíka"},
    {Str::GroupFamily, "rodina"},
    {Str::GroupWantsFears, "přání a obavy"},
    {Str::GroupNeeds, "potřeby"},
    {Str::GroupControls, "ovládací prvky"},
    // Text fields
    {Str::TextField, "%s %s, textové pole"},
    {Str::Blank, "prázdné"},
    {Str::Space, "mezera"},
    // Values
    {Str::ValueOfMax, "%d z %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "další stránka"},
    {Str::PreviousPage, "předchozí stránka"},
    // Interaction menu
    {Str::PieMenu, "interakce"},
    {Str::NoInteractions, "žádné interakce"},
    {Str::Cancelled, "zrušeno"},
    // Cheat console
    {Str::CheatConsole, "konzole cheatů"},
    {Str::CheatRan, "cheat: %s"},
    {Str::CheatEmpty, "nejdřív napiš nebo vyber cheat"},
    // Neighborhood: lots and families
    {Str::GroupLots, "parcely"},
    {Str::LotFamily, "rodina %s"},
    {Str::LotForSale, "na prodej"},
    {Str::LotCommunity, "veřejná parcela"},
    {Str::LotApartmentsEmpty, "bytový dům, bez nájemníků"},
    {Str::LotApartmentsWith, "bytový dům: %s"},
    {Str::FamilySummary, "%1$s: členů %2$d, peníze %3$s"},
    // Money
    {Str::Simoleons, "simoleonů"},
    {Str::FundsNow, "peníze %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Simíci"},
    {Str::CategoryOther, "ostatní předměty"},
    {Str::NoObjects, "zde nejsou žádné předměty"},
    {Str::FloorNumber, "podlaží %d"},
    {Str::NotOnLot, "není na parcele"},
    {Str::HereNow, "přímo tady"},
    {Str::TilesAway, "%d polí na %s"},
    {Str::OneTileAway, "1 pole na %s"},
    {Str::DirNorth, "sever"},
    {Str::DirNorthEast, "severovýchod"},
    {Str::DirEast, "východ"},
    {Str::DirSouthEast, "jihovýchod"},
    {Str::DirSouth, "jih"},
    {Str::DirSouthWest, "jihozápad"},
    {Str::DirWest, "západ"},
    {Str::DirNorthWest, "severozápad"},
    {Str::OtherFloor, "v jiném podlaží"},
    {Str::SelectedSimSuffix, "vybraný"},
    {Str::NoInteractionsAtAll, "žádné interakce: s tímto předmětem nelze nic dělat"},
    {Str::WalkingThere, "jde tam"},
    {Str::CannotWalk, "tam nelze dojít"},
    {Str::NoSimSelected, "není vybrán žádný simík"},
    // Needs
    {Str::MotiveValue, "%s, %d procent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "fronta akcí"},
    {Str::StatusIdle, "nic nedělá"},
    {Str::StatusNeedsLow, "nízké potřeby: %s"},
    {Str::StatusNeedsFine, "potřeby v pořádku"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s dokončil %2$s"},
    {Str::NeedLow, "%1$s, nízká potřeba: %2$s"},
    {Str::NeedUrgent, "%1$s, naléhavá potřeba: %2$s"},
    // Lot: selecting Sims
    {Str::FundsChanged, "%1$s, zbývá %2$s"},
};

}  // namespace

extern const Translations kCzech = {kEntries, std::size(kEntries)};

}  // namespace lang
