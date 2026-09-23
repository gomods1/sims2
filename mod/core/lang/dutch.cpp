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
    {Str::GroupControlPanel, "bedieningspaneel"},
    {Str::GroupLivePanel, "Sim-paneel"},
    {Str::GroupFamily, "gezin"},
    {Str::GroupWantsFears, "wensen en angsten"},
    {Str::GroupNeeds, "behoeften"},
    {Str::GroupControls, "besturing"},
    {Str::GroupBuyPanel, "koopmodus"},
    {Str::GroupCatalog, "catalogus"},
    // Text fields
    {Str::TextField, "%s %s, tekstveld"},
    {Str::Blank, "leeg"},
    {Str::Space, "spatie"},
    // Values
    {Str::ValueOfMax, "%d van %d"},
    {Str::SliderRange, "%1$s tot %2$s"},
    {Str::PointsLeft, "nog %d punten"},
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
    // Neighborhood: lots and families
    {Str::GroupLots, "kavels"},
    {Str::LotFamily, "familie %s"},
    {Str::LotForSale, "te koop"},
    {Str::LotCommunity, "gemeenschapskavel"},
    {Str::LotApartmentsEmpty, "appartementengebouw, zonder huurders"},
    {Str::LotApartmentsWith, "appartementengebouw: %s"},
    {Str::FamilySummary, "%1$s: %2$d leden, geld %3$s"},
    // Money
    {Str::Simoleons, "simoleons"},
    {Str::FundsNow, "geld %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Sims"},
    {Str::CategoryOther, "overige voorwerpen"},
    {Str::NoObjects, "hier zijn geen voorwerpen"},
    {Str::FloorNumber, "niveau %d"},
    {Str::NotOnLot, "niet op een kavel"},
    {Str::HereNow, "hier"},
    {Str::TilesAway, "%d tegels naar het %s"},
    {Str::OneTileAway, "1 tegel naar het %s"},
    {Str::DirNorth, "noorden"},
    {Str::DirNorthEast, "noordoosten"},
    {Str::DirEast, "oosten"},
    {Str::DirSouthEast, "zuidoosten"},
    {Str::DirSouth, "zuiden"},
    {Str::DirSouthWest, "zuidwesten"},
    {Str::DirWest, "westen"},
    {Str::DirNorthWest, "noordwesten"},
    {Str::OtherFloor, "op een andere verdieping"},
    {Str::SelectedSimSuffix, "geselecteerd"},
    {Str::NoInteractionsAtAll, "geen interacties: met dit voorwerp kun je niets doen"},
    {Str::WalkingThere, "loopt erheen"},
    {Str::CannotWalk, "kan er niet heen"},
    {Str::NoSimSelected, "geen Sim geselecteerd"},
    // Needs
    {Str::MotiveValue, "%s, %d procent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "wachtrij"},
    {Str::StatusIdle, "doet niets"},
    {Str::StatusNeedsLow, "lage behoeften: %s"},
    {Str::StatusNeedsFine, "behoeften in orde"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s is klaar met: %2$s"},
    {Str::NeedLow, "%1$s, lage behoefte: %2$s"},
    {Str::NeedUrgent, "%1$s, dringende behoefte: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, geselecteerd"},
    {Str::NoSuchSim, "geen Sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s is aangekomen"},
    {Str::VisitorLeft, "%s is vertrokken"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d voorwerpen"},
    {Str::RoomObjectsOne, "%s, 1 voorwerp"},
    {Str::FundsChanged, "%1$s, nog %2$s"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Page Down zoekt plekken waar het past, de pijltjes verplaatsen het, Enter zet het neer, Escape "
     "annuleert"},
    {Str::PlaceTile, "vak %1$d, %2$d"},
    {Str::PlaceOffLot, "buiten de kavel"},
    {Str::PlaceFits, "past"},
    {Str::PlaceBlocked, "past hier niet"},
    {Str::PlacePlaced, "%s neergezet"},
    {Str::PlaceFacing, "richting %s"},
    {Str::PlaceNoSpot, "zo gedraaid past het nergens op deze kavel"},
    {Str::PlaceRoomSpots, "%1$s, %2$d plekken"},
    {Str::PlaceMoving,
     "%s verplaatsen: Page Down zoekt plekken, Enter zet het neer, Delete verkoopt het, Escape zet het terug"},
    {Str::PlaceCannotPick, "%s kan niet opgepakt worden"},
    {Str::PlaceSold, "%s verkocht"},
};

}  // namespace

extern const Translations kDutch = {kEntries, std::size(kEntries)};

}  // namespace lang
