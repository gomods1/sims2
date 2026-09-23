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
    {Str::KeysLot,
     "Pozemek: Page Down a Page Up, předměty na pozemku, nejbližší první; se Shiftem kategorie, s Ctrl "
     "místnosti. Enter otevře interakce předmětu, v režimu nákupu ho zvedne; Ctrl+Enter pošle vybraného Sima "
     "tam. Ctrl+E stav Sima, Ctrl+1 až 9 vyberou Sima, Tab panely, Ctrl+šipka nahoru a dolů podrobnosti."},
    {Str::KeysPlacement,
     "Umísťování: Page Down a Page Up místa, kam se vejde, s Ctrl místnosti, šipky o jedno pole, Home zpět k "
     "Simovi, čárka a tečka otáčejí, Enter položí, Delete prodá, Escape ukončí."},
    {Str::KeysBuild,
     "Stavba: šipky posouvají roh, se Shiftem o pět, Page Up a Page Down podlaží, mezerník označí začátek, Enter "
     "staví od značky, Ctrl+Enter odstraní, Home zpět k Simovi, R zopakuje, Escape ukončí."},
    {Str::KeysNeighborhood,
     "Sousedství: nejprve seznam pozemků, Enter otevře panel pozemku. Chcete-li nastěhovat rodinu, vyberte ji v "
     "zásobníku rodin a stiskněte Enter na pozemku. Tab panely."},
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
    {Str::GroupBuyPanel, "režim nákupu"},
    {Str::GroupBuildPanel, "režim stavby"},
    {Str::GroupCatalog, "katalog"},
    {Str::GroupStarSign, "znamení zvěrokruhu"},
    {Str::GroupAspiration, "aspirace"},
    {Str::GroupSteps, "kroky"},
    {Str::GroupAge, "věk"},
    {Str::GroupSex, "pohlaví"},
    {Str::GroupTraits, "vlastnosti"},
    // Text fields
    {Str::TextField, "%s %s, textové pole"},
    {Str::Blank, "prázdné"},
    {Str::Space, "mezera"},
    // Values
    {Str::ValueOfMax, "%d z %d"},
    {Str::SliderRange, "od %1$s po %2$s"},
    {Str::RadioBetween, "mezi: %1$s a %2$s"},
    {Str::PointsLeft, "zbývá bodů: %d"},
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
    {Str::SimSelectedNow, "%s, vybrán"},
    {Str::NoSuchSim, "žádný Sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s přišel"},
    {Str::VisitorLeft, "%s odešel"},
    // Lot: rooms
    {Str::RoomObjects, "%s, objektů: %d"},
    {Str::RoomObjectsOne, "%s, 1 objekt"},
    {Str::FundsChanged, "%1$s, zbývá %2$s"},
    // Camera
    {Str::CameraFree, "kamera volná"},
    {Str::CameraFollowing, "sleduje: %s"},
    // Narrator
    {Str::NarratorAll, "vypravěč: vše"},
    {Str::NarratorWarnings, "vypravěč: jen varování"},
    {Str::NarratorOff, "vypravěč vypnut"},
    // Relationships
    {Str::RelationshipValues, "%1$s, denní %2$s, celoživotní %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "tady není místnost"},
    {Str::LayoutSummary, "%1$s, podlaží %2$d, polí: %3$d, od %4$d, %5$d do %6$d, %7$d"},
    {Str::LayoutOpenings, "dveří: %1$d, oken: %2$d"},
    {Str::LayoutDoor, "dveře %1$s, stěna: %2$s, vedou do: %3$s, pole %4$d, %5$d"},
    {Str::LayoutWindow, "okno %1$s, stěna: %2$s, výhled na: %3$s, pole %4$d, %5$d"},
    // Buy mode
    {Str::PlaceBegin, "%s: Page Down hledá místa, kam se vejde, šipky ho posouvají, Enter ho položí, Escape ruší"},
    {Str::PlaceTile, "pole %1$d, %2$d"},
    {Str::PlaceOffLot, "mimo pozemek"},
    {Str::PlaceFits, "vejde se"},
    {Str::PlaceBlocked, "sem se nevejde"},
    {Str::PlacePlaced, "%s položeno"},
    {Str::PlaceFacing, "natočeno na %s"},
    {Str::PlaceNoSpot, "takto natočené se nikam na pozemku nevejde"},
    {Str::PlaceRoomSpots, "%1$s, míst: %2$d"},
    {Str::PlaceWallSide, "stěna: %s"},
    {Str::PlaceLeadsTo, "vede do: %s"},
    {Str::PlaceLeadsToAnother, "vede do jiné místnosti: %s"},
    {Str::PlaceMoving, "přesun: %s. Page Down hledá místa, Enter položí, Delete prodá, Escape vrátí na místo"},
    {Str::PlaceCannotPick, "%s nelze zvednout"},
    {Str::PlaceSold, "%s prodáno"},
    // Build mode
    {Str::BuildBegin,
     "%s: šipky posouvají roh, mezerník označí začátek, Enter staví, Ctrl+Enter odstraní, Escape ukončí"},
    {Str::BuildCorner, "roh %1$d, %2$d"},
    {Str::BuildMarked, "začátek označen na %1$d, %2$d"},
    {Str::BuildNoMark, "nejprve označ začátek mezerníkem"},
    {Str::BuildDone, "postaveno za %1$s, zbývá %2$s"},
    {Str::BuildRemoved, "odstraněno, vráceno %1$s, %2$s"},
    {Str::BuildNothing, "nic se nezměnilo"},
    {Str::BalloonThinks, "%1$s myslí: %2$s"},
    {Str::BalloonDreams, "%1$s sní: %2$s"},
    {Str::BalloonSays, "%1$s říká: %2$s"},
    {Str::BalloonScreams, "%1$s křičí: %2$s"},
    {Str::BalloonScreamsNothing, "%s křičí"},
    {Str::BalloonCrossed, "ne %s"},
    {Str::BalloonRelationUp, "%s: vztah se zlepšuje"},
    {Str::BalloonRelationUpMuch, "%s: vztah se hodně zlepšuje"},
    {Str::BalloonRelationDown, "%s: vztah se zhoršuje"},
    {Str::BalloonRelationDownMuch, "%s: vztah se hodně zhoršuje"},
    {Str::BalloonRomance, "%s: romance"},
    {Str::BalloonRomanceBroken, "%s: konec romance"},
    {Str::BalloonCrush, "%s: zamilovanost"},
    {Str::BalloonCrushBroken, "%s: konec zamilovanosti"},
    {Str::BalloonMomentBad, "%s: špatná chvíle"},
    {Str::BalloonMomentGood, "%s: skvělá chvíle"},
    {Str::BalloonFirstKiss, "%s: první polibek"},
    {Str::BalloonFirstLove, "%s: první láska"},
    {Str::BalloonInLove, "%s: zamilovaný"},
    {Str::BalloonLoveLost, "%s: ztracená láska"},
    {Str::BalloonFriend, "%s: nový přítel"},
    {Str::BalloonFriendLost, "%s: ztracený přítel"},
    {Str::BalloonBestFriend, "%s: nový nejlepší přítel"},
    {Str::BalloonBestFriendLost, "%s: ztracený nejlepší přítel"},
    {Str::BalloonEnemy, "%s: nový nepřítel"},
    {Str::BalloonEnemyLost, "%s: už nejsou nepřátelé"},
    {Str::BalloonMemoryGood, "%s: dobrá vzpomínka"},
    {Str::BalloonMemoryBad, "%s: špatná vzpomínka"},
    {Str::BalloonTopicLiked, "%s: téma se líbí"},
    {Str::BalloonTopicDisliked, "%s: téma se nelíbí"},
    {Str::BalloonPersonalityGood, "%s: povahy se shodují"},
    {Str::BalloonPersonalityBad, "%s: povahy se střetávají"},
};

}  // namespace

extern const Translations kCzech = {kEntries, std::size(kEntries)};

}  // namespace lang
