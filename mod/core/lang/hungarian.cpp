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
    {Str::KeysLot,
     "Telek: Page Down és Page Up a telek tárgyai, a legközelebbi elöl; Shifttel a kategóriák, Ctrl-lal a "
     "szobák. Az Enter megnyitja a tárgy cselekvéseit, vásárlás módban felveszi; a Ctrl+Enter odaküldi a "
     "kiválasztott Simet. Ctrl+E a Sim állapota, Ctrl+1–9 Sim kiválasztása, Tab a panelek, Ctrl+fel és le nyíl a "
     "részletek."},
    {Str::KeysPlacement,
     "Elhelyezés: Page Down és Page Up a helyek, ahol elfér, Ctrl-lal a szobák, nyilak egy mező, Home vissza a "
     "Simhez, vessző és pont forgat, Enter leteszi, Delete eladja, Escape befejezi."},
    {Str::KeysBuild,
     "Építés: a nyilak mozgatják a sarkot, Shifttel ötöt, Page Up és Page Down a szint, szóköz jelöli a "
     "kezdetet, Enter épít a jeltől, Ctrl+Enter bont, Home vissza a Simhez, R ismétel, Escape kilép."},
    {Str::KeysNeighborhood,
     "Szomszédság: először a telkek listája, az Enter megnyitja a telek paneljét. Család beköltöztetéséhez "
     "válaszd ki a családtárolóban, majd nyomj Entert egy telken. Tab a panelek."},
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
    {Str::GroupBuyPanel, "vásárlás mód"},
    {Str::GroupBuildPanel, "építés mód"},
    {Str::GroupCatalog, "katalógus"},
    {Str::GroupStarSign, "csillagjegy"},
    {Str::GroupAspiration, "törekvés"},
    {Str::GroupSteps, "lépések"},
    {Str::GroupAge, "életkor"},
    {Str::GroupSex, "nem"},
    {Str::GroupTraits, "tulajdonságok"},
    // Text fields
    {Str::TextField, "%s %s, szövegmező"},
    {Str::Blank, "üres"},
    {Str::Space, "szóköz"},
    // Values
    {Str::ValueOfMax, "%d / %d"},
    {Str::SliderRange, "%1$s – %2$s"},
    {Str::RadioBetween, "%1$s és %2$s között"},
    {Str::PointsLeft, "%d pont maradt"},
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
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, kiválasztva"},
    {Str::NoSuchSim, "nincs %d. Sim"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s megérkezett"},
    {Str::VisitorLeft, "%s elment"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d tárgy"},
    {Str::RoomObjectsOne, "%s, 1 tárgy"},
    {Str::FundsChanged, "%1$s, maradt %2$s"},
    // Camera
    {Str::CameraFree, "szabad kamera"},
    {Str::CameraFollowing, "követi: %s"},
    // Narrator
    {Str::NarratorAll, "narrátor: minden"},
    {Str::NarratorWarnings, "narrátor: csak figyelmeztetések"},
    {Str::NarratorOff, "narrátor ki"},
    // Relationships
    {Str::RelationshipValues, "%1$s, napi %2$s, életre szóló %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "itt nincs szoba"},
    {Str::LayoutSummary, "%1$s, %2$d. szint, %3$d mező, %4$d, %5$d – %6$d, %7$d"},
    {Str::LayoutOpenings, "%1$d ajtó, %2$d ablak"},
    {Str::LayoutDoor, "ajtó %1$s, fal: %2$s, ide vezet: %3$s, mező %4$d, %5$d"},
    {Str::LayoutWindow, "ablak %1$s, fal: %2$s, kilátás: %3$s, mező %4$d, %5$d"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: a Page Down helyet keres, ahol elfér, a nyilak mozgatják, az Enter leteszi, az Escape megszakítja"},
    {Str::PlaceTile, "mező %1$d, %2$d"},
    {Str::PlaceOffLot, "a telken kívül"},
    {Str::PlaceFits, "elfér"},
    {Str::PlaceBlocked, "itt nem fér el"},
    {Str::PlacePlaced, "%s letéve"},
    {Str::PlaceFacing, "irány: %s"},
    {Str::PlaceNoSpot, "így forgatva sehol sem fér el a telken"},
    {Str::PlaceRoomSpots, "%1$s, %2$d hely"},
    {Str::PlaceWallSide, "fal: %s"},
    {Str::PlaceLeadsTo, "ide vezet: %s"},
    {Str::PlaceLeadsToAnother, "másik helyiségbe vezet: %s"},
    {Str::PlaceMoving,
     "áthelyezés: %s. A Page Down helyet keres, az Enter leteszi, a Delete eladja, az Escape visszateszi"},
    {Str::PlaceCannotPick, "%s nem vehető fel"},
    {Str::PlaceSold, "%s eladva"},
    // Build mode
    {Str::BuildBegin,
     "%s: a nyilak mozgatják a sarkot, a szóköz jelöli a kezdetet, az Enter épít, a Ctrl+Enter bont, az Escape "
     "kilép"},
    {Str::BuildCorner, "sarok %1$d, %2$d"},
    {Str::BuildMarked, "kezdet megjelölve: %1$d, %2$d"},
    {Str::BuildNoMark, "előbb jelöld meg a kezdetet szóközzel"},
    {Str::BuildDone, "megépítve %1$s áron, maradt %2$s"},
    {Str::BuildRemoved, "lebontva, %1$s vissza, %2$s"},
    {Str::BuildNothing, "nem változott semmi"},
};

}  // namespace

extern const Translations kHungarian = {kEntries, std::size(kEntries)};

}  // namespace lang
