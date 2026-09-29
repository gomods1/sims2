// Swedish: the game's language 6. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Tillgänglighet för The Sims 2 laddad"},
    {Str::UnsupportedBuild, "Tillgänglighet för The Sims 2: spelversionen stöds inte, modden är avstängd"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Tabb och Skift+Tabb går från grupp till grupp; Pil upp och ner går genom en grupp, vänster och höger "
     "också, eller ändrar ett reglage; Home och End går till början och slutet; Enter aktiverar, och på en sort "
     "eller ett rum öppnar den föremålen; Backspace går tillbaka; R upprepar. Kontroll+Pil upp och Kontroll+Pil "
     "ned läser detaljerna om det som sades, Kontroll+Pil vänster och Kontroll+Pil höger växlar mellan detaljer "
     "och händelser."},
    {Str::KeysLot,
     "Tomt: Tabb går genom behoven, handlingskön, föremål efter sort, föremål efter rum och spelets paneler. "
     "Under föremål efter sort eller rum öppnar Enter en sort eller ett rum, föremålen kommer de närmaste först, "
     "Backspace går tillbaka. Enter på ett föremål öppnar dess handlingar, eller plockar upp det i köpläge; "
     "Ctrl+Enter skickar den valda simmen dit. Ctrl+E simmens tillstånd, Ctrl+1 till 9 väljer en sim, Ctrl+pil "
     "upp och ner detaljerna."},
    {Str::KeysPlacement,
     "Placering: pilar en ruta, Skift fem, Tabb och Skift+Tabb platserna där den får plats, Page Down och Page "
     "Up rummen, Home tillbaka till simmen, komma och punkt vrider den, Enter ställer ner den, Delete säljer "
     "den, Escape avslutar."},
    {Str::KeysBuild,
     "Bygge: pilarna flyttar hörnet, med Skift fem, Page Up och Page Down nivån, mellanslag markerar början, "
     "Enter bygger från markeringen, Ctrl+Enter tar bort, Home tillbaka till simmen, R upprepar, Escape avslutar."},
    {Str::KeysNeighborhood,
     "Kvarter: först listan med tomter, Enter öppnar en tomts panel. För att flytta in en familj, välj den i "
     "familjelådan och tryck Enter på en tomt. Tabb panelerna."},
    // Control state
    {Str::Checked, "markerad"},
    {Str::NotChecked, "inte markerad"},
    {Str::Selected, "vald"},
    {Str::TabSelected, "vald"},
    {Str::Unavailable, "inte tillgänglig"},
    {Str::UnlabeledButton, "knapp utan namn"},
    {Str::Unnamed, "namnlös"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "knapp"},
    {Str::RoleTab, "flik"},
    {Str::RoleCheckBox, "kryssruta"},
    {Str::RoleSlider, "skjutreglage"},
    {Str::RolePicker, "väljare"},
    {Str::NothingHere, "här finns inget att navigera"},
    // Review buffers
    {Str::BufferDetails, "detaljer"},
    {Str::BufferEvents, "händelser"},
    {Str::BufferEmpty, "%s, tomt"},
    {Str::PositionInGroup, "%d av %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d objekt"},
    {Str::GroupSummaryOne, "%s, 1 objekt"},
    {Str::GroupButtons, "knappar"},
    {Str::GroupText, "text"},
    {Str::GroupControlPanel, "kontrollpanel"},
    {Str::GroupLivePanel, "Sim-panel"},
    {Str::GroupFamily, "familj"},
    {Str::GroupWantsFears, "önskningar och rädslor"},
    {Str::GroupNeeds, "behov"},
    {Str::GroupControls, "kontroller"},
    {Str::GroupBuyPanel, "köpläge"},
    {Str::GroupBuildPanel, "byggläge"},
    {Str::GroupCatalog, "katalog"},
    {Str::GroupStarSign, "stjärntecken"},
    {Str::GroupAspiration, "strävan"},
    {Str::GroupSteps, "steg"},
    {Str::GroupAge, "ålder"},
    {Str::GroupSex, "kön"},
    {Str::GroupTraits, "egenskaper"},
    // Text fields
    {Str::TextField, "%s %s, textfält"},
    {Str::Blank, "tomt"},
    {Str::Space, "mellanslag"},
    // Values
    {Str::ValueOfMax, "%d av %d"},
    {Str::SliderRange, "%1$s till %2$s"},
    {Str::RadioBetween, "mellan %1$s och %2$s"},
    {Str::PointsLeft, "%d poäng kvar"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "nästa sida"},
    {Str::PreviousPage, "föregående sida"},
    // Interaction menu
    {Str::PieMenu, "interaktioner"},
    {Str::NoInteractions, "inga interaktioner"},
    {Str::Cancelled, "avbrutet"},
    // Cheat console
    {Str::CheatConsole, "fuskkonsol"},
    {Str::CheatRan, "fusk: %s"},
    {Str::CheatEmpty, "skriv eller välj ett fusk först"},
    // Neighborhood: lots and families
    {Str::GroupLots, "tomter"},
    {Str::LotFamily, "familjen %s"},
    {Str::LotForSale, "till salu"},
    {Str::LotCommunity, "allmän tomt"},
    {Str::LotApartmentsEmpty, "hyreshus, inga hyresgäster"},
    {Str::LotApartmentsWith, "hyreshus: %s"},
    {Str::FamilySummary, "%1$s: %2$d medlemmar, pengar %3$s"},
    // Money
    {Str::Simoleons, "simoleons"},
    {Str::FundsNow, "tillgångar %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "simmar"},
    {Str::CategoryOther, "andra föremål"},
    {Str::NoObjects, "inga föremål här"},
    {Str::FloorNumber, "våning %d"},
    {Str::NotOnLot, "inte på en tomt"},
    {Str::HereNow, "precis här"},
    {Str::TilesAway, "%d rutor åt %s"},
    {Str::OneTileAway, "1 ruta åt %s"},
    {Str::DirNorth, "norr"},
    {Str::DirNorthEast, "nordost"},
    {Str::DirEast, "öster"},
    {Str::DirSouthEast, "sydost"},
    {Str::DirSouth, "söder"},
    {Str::DirSouthWest, "sydväst"},
    {Str::DirWest, "väster"},
    {Str::DirNorthWest, "nordväst"},
    {Str::OtherFloor, "på en annan våning"},
    {Str::SelectedSimSuffix, "vald"},
    {Str::NoInteractionsAtAll, "inga interaktioner: man kan inte göra något med det här föremålet"},
    {Str::WalkingThere, "går dit"},
    {Str::CannotWalk, "kan inte gå dit"},
    {Str::NoSimSelected, "ingen sim vald"},
    // Needs
    {Str::MotiveValue, "%s, %d procent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "åtgärdskö"},
    {Str::StatusIdle, "gör ingenting"},
    {Str::StatusNeedsLow, "låga behov: %s"},
    {Str::StatusNeedsFine, "behoven är okej"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s är klar med: %2$s"},
    {Str::NeedLow, "%1$s, lågt behov: %2$s"},
    {Str::NeedUrgent, "%1$s, akut behov: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, vald"},
    {Str::NoSuchSim, "ingen sim nummer %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s har kommit"},
    {Str::VisitorLeft, "%s har gått"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d föremål"},
    {Str::RoomObjectsOne, "%s, 1 föremål"},
    {Str::ThingsByKind, "föremål efter sort"},
    {Str::ThingsByRoom, "föremål efter rum"},
    {Str::FundsChanged, "%1$s, kvar %2$s"},
    // Camera
    {Str::CameraFree, "kameran fri"},
    {Str::CameraFollowing, "följer %s"},
    // Narrator
    {Str::NarratorAll, "berättare: allt"},
    {Str::NarratorWarnings, "berättare: bara varningar"},
    {Str::NarratorOff, "berättare av"},
    // Relationships
    {Str::RelationshipValues, "%1$s, daglig %2$s, livslång %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "inget rum här"},
    {Str::LayoutSummary, "%1$s, våning %2$d, %3$d rutor, från %4$d, %5$d till %6$d, %7$d"},
    {Str::LayoutOpenings, "%1$d dörrar, %2$d fönster"},
    {Str::LayoutDoor, "dörr %1$s, vägg mot %2$s, leder till %3$s, ruta %4$d, %5$d"},
    {Str::LayoutWindow, "fönster %1$s, vägg mot %2$s, vetter mot %3$s, ruta %4$d, %5$d"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Page Down hittar platser där den får plats, pilarna flyttar den, Enter ställer ner den, Escape avbryter"},
    {Str::PlaceTile, "ruta %1$d, %2$d"},
    {Str::PlaceOffLot, "utanför tomten"},
    {Str::PlaceFits, "får plats"},
    {Str::PlaceBlocked, "får inte plats här"},
    {Str::PlacePlaced, "%s placerad"},
    {Str::PlaceFacing, "vänd mot %s"},
    {Str::PlaceNoSpot, "får inte plats någonstans på tomten, åt något håll"},
    {Str::PlaceRoomSpots, "%1$s, %2$d platser"},
    {Str::PlaceWallSide, "vägg mot %s"},
    {Str::PlaceLeadsTo, "leder till %s"},
    {Str::PlaceLeadsToAnother, "leder till ett annat rum: %s"},
    {Str::PlaceMoving,
     "flytta %s: Page Down hittar platser, Enter ställer ner den, Delete säljer den, Escape ställer tillbaka den"},
    {Str::PlaceCannotPick, "%s kan inte plockas upp"},
    {Str::PlaceSold, "%s såld"},
    // Build mode
    {Str::BuildBegin,
     "%s: pilarna flyttar hörnet, mellanslag markerar början, Enter bygger, Ctrl+Enter tar bort, Escape avslutar"},
    {Str::BuildCorner, "hörn %1$d, %2$d"},
    {Str::BuildMarked, "början markerad vid %1$d, %2$d"},
    {Str::BuildNoMark, "markera först början med mellanslag"},
    {Str::BuildDone, "byggt för %1$s, %2$s kvar"},
    {Str::BuildRemoved, "borttaget, %1$s tillbaka, %2$s"},
    {Str::BuildNothing, "inget ändrades"},
    {Str::BalloonThinks, "%1$s tänker: %2$s"},
    {Str::BalloonDreams, "%1$s drömmer: %2$s"},
    {Str::BalloonSays, "%1$s säger: %2$s"},
    {Str::BalloonScreams, "%1$s skriker: %2$s"},
    {Str::BalloonScreamsNothing, "%s skriker"},
    {Str::BalloonCrossed, "ingen %s"},
    {Str::BalloonRelationUp, "%s: relationen förbättras"},
    {Str::BalloonRelationUpMuch, "%s: relationen förbättras mycket"},
    {Str::BalloonRelationDown, "%s: relationen försämras"},
    {Str::BalloonRelationDownMuch, "%s: relationen försämras mycket"},
    {Str::BalloonRomance, "%s: romans"},
    {Str::BalloonRomanceBroken, "%s: romansen är slut"},
    {Str::BalloonCrush, "%s: förälskelse"},
    {Str::BalloonCrushBroken, "%s: förälskelsen är över"},
    {Str::BalloonMomentBad, "%s: ett dåligt ögonblick"},
    {Str::BalloonMomentGood, "%s: ett stort ögonblick"},
    {Str::BalloonFirstKiss, "%s: första kyssen"},
    {Str::BalloonFirstLove, "%s: första kärleken"},
    {Str::BalloonInLove, "%s: förälskad"},
    {Str::BalloonLoveLost, "%s: kärlek förlorad"},
    {Str::BalloonFriend, "%s: ny vän"},
    {Str::BalloonFriendLost, "%s: vän förlorad"},
    {Str::BalloonBestFriend, "%s: ny bästa vän"},
    {Str::BalloonBestFriendLost, "%s: bästa vän förlorad"},
    {Str::BalloonEnemy, "%s: ny fiende"},
    {Str::BalloonEnemyLost, "%s: inte längre fiender"},
    {Str::BalloonMemoryGood, "%s: gott minne"},
    {Str::BalloonMemoryBad, "%s: dåligt minne"},
    {Str::BalloonTopicLiked, "%s: gillar ämnet"},
    {Str::BalloonTopicDisliked, "%s: ogillar ämnet"},
    {Str::BalloonPersonalityGood, "%s: personligheterna passar"},
    {Str::BalloonPersonalityBad, "%s: personligheterna krockar"},
    {Str::BalloonRouteFailed, "%s kan inte ta sig dit"},
    {Str::IconAmbulance, "ambulans"},
    {Str::IconPhone, "telefon"},
    {Str::IconPoolLadder, "poolstege"},
    {Str::IconQuestion, "frågetecken"},
    {Str::IconFingerprint, "fingeravtryck"},
    {Str::IconBaby, "bebis"},
    {Str::IconDoor, "dörr"},
    {Str::IconBabyQuestion, "en bebis?"},
    {Str::FamilyTreePicked, "%s upplockad: Enter på en annan Sim kopplar ihop dem, Enter igen släpper"},
    {Str::FurnitureGone, "%1$s har försvunnit från tomten, värde %2$s"},
    {Str::BuildShutIn, "se upp: %2$s har ingen dörr och %1$s är där inne; Ctrl+Z ångrar väggen"},
    {Str::KeysLotMore, "Ctrl+L rummets planlösning, Ctrl+N vad berättaren säger, Ctrl+F om kameran följer simmen."},
    {Str::GroupBusiness, "företag"},
    {Str::GroupSeasons, "årstider"},
    {Str::GroupDate, "dejt"},
};

}  // namespace

extern const Translations kSwedish = {kEntries, std::size(kEntries)};

}  // namespace lang
