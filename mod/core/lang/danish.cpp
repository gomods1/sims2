// Danish: the game's language 9. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Sims 2-tilgængelighed indlæst"},
    {Str::UnsupportedBuild, "Sims 2-tilgængelighed: spilversionen understøttes ikke, mod deaktiveret"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Piletasterne flytter inden for en gruppe, Tab og Skift+Tab skifter gruppe, Home og End går til enderne, "
     "Enter aktiverer, R gentager. Kontrol+Pil op og Kontrol+Pil ned læser detaljerne om det, der blev sagt, "
     "Kontrol+Venstre pil og Kontrol+Højre pil skifter mellem detaljer og hændelser."},
    {Str::KeysLot,
     "Grund: Page Down og Page Up, grundens genstande, de nærmeste først; med Shift kategorierne, med Ctrl "
     "værelserne. Enter åbner en genstands handlinger, eller samler den op i købetilstand; Ctrl+Enter sender den "
     "valgte Sim derhen. Ctrl+E Simmens tilstand, Ctrl+1 til 9 vælger en Sim, Tab panelerne, Ctrl+pil op og ned "
     "detaljerne."},
    {Str::KeysPlacement,
     "Placering: Page Down og Page Up pladserne hvor den kan stå, med Ctrl værelserne, pile ét felt, Home "
     "tilbage til Simmen, komma og punktum drejer den, Enter stiller den, Delete sælger den, Escape stopper."},
    {Str::KeysBuild,
     "Byggeri: pilene flytter hjørnet, med Shift fem, Page Up og Page Down niveauet, mellemrum markerer starten, "
     "Enter bygger fra mærket, Ctrl+Enter fjerner, Home tilbage til Simmen, R gentager, Escape afslutter."},
    {Str::KeysNeighborhood,
     "Kvarter: først listen over grunde, Enter åbner en grunds panel. For at flytte en familie ind, vælg den i "
     "familiebeholderen og tryk Enter på en grund. Tab panelerne."},
    // Control state
    {Str::Checked, "markeret"},
    {Str::NotChecked, "ikke markeret"},
    {Str::Selected, "valgt"},
    {Str::TabSelected, "valgt"},
    {Str::Unavailable, "ikke tilgængelig"},
    {Str::UnlabeledButton, "knap uden navn"},
    {Str::Unnamed, "uden navn"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "knap"},
    {Str::RoleTab, "fane"},
    {Str::RoleCheckBox, "afkrydsningsfelt"},
    {Str::RoleSlider, "skyder"},
    {Str::RolePicker, "værdivælger"},
    {Str::NothingHere, "her er intet at navigere i"},
    // Review buffers
    {Str::BufferDetails, "detaljer"},
    {Str::BufferEvents, "hændelser"},
    {Str::BufferEmpty, "%s, tom"},
    {Str::PositionInGroup, "%d af %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d elementer"},
    {Str::GroupSummaryOne, "%s, 1 element"},
    {Str::GroupButtons, "knapper"},
    {Str::GroupText, "tekst"},
    {Str::GroupControlPanel, "kontrolpanel"},
    {Str::GroupLivePanel, "Sim-panel"},
    {Str::GroupFamily, "familie"},
    {Str::GroupWantsFears, "ønsker og frygt"},
    {Str::GroupNeeds, "behov"},
    {Str::GroupControls, "kontroller"},
    {Str::GroupBuyPanel, "købetilstand"},
    {Str::GroupBuildPanel, "byggetilstand"},
    {Str::GroupCatalog, "katalog"},
    {Str::GroupStarSign, "stjernetegn"},
    {Str::GroupAspiration, "ambition"},
    {Str::GroupSteps, "trin"},
    {Str::GroupAge, "alder"},
    {Str::GroupSex, "køn"},
    {Str::GroupTraits, "træk"},
    // Text fields
    {Str::TextField, "%s %s, tekstfelt"},
    {Str::Blank, "tomt"},
    {Str::Space, "mellemrum"},
    // Values
    {Str::ValueOfMax, "%d af %d"},
    {Str::SliderRange, "%1$s til %2$s"},
    {Str::RadioBetween, "mellem %1$s og %2$s"},
    {Str::PointsLeft, "%d point tilbage"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "næste side"},
    {Str::PreviousPage, "forrige side"},
    // Interaction menu
    {Str::PieMenu, "handlinger"},
    {Str::NoInteractions, "ingen handlinger"},
    {Str::Cancelled, "annulleret"},
    // Cheat console
    {Str::CheatConsole, "snydekonsol"},
    {Str::CheatRan, "snydekode: %s"},
    {Str::CheatEmpty, "skriv eller vælg en snydekode først"},
    // Neighborhood: lots and families
    {Str::GroupLots, "grunde"},
    {Str::LotFamily, "familien %s"},
    {Str::LotForSale, "til salg"},
    {Str::LotCommunity, "fællesgrund"},
    {Str::LotApartmentsEmpty, "lejlighedsbygning, ingen lejere"},
    {Str::LotApartmentsWith, "lejlighedsbygning: %s"},
    {Str::FamilySummary, "%1$s: %2$d medlemmer, penge %3$s"},
    // Money
    {Str::Simoleons, "simoleons"},
    {Str::FundsNow, "penge %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Simmere"},
    {Str::CategoryOther, "andre objekter"},
    {Str::NoObjects, "ingen objekter her"},
    {Str::FloorNumber, "etage %d"},
    {Str::NotOnLot, "ikke på en grund"},
    {Str::HereNow, "lige her"},
    {Str::TilesAway, "%d fliser mod %s"},
    {Str::OneTileAway, "1 flise mod %s"},
    {Str::DirNorth, "nord"},
    {Str::DirNorthEast, "nordøst"},
    {Str::DirEast, "øst"},
    {Str::DirSouthEast, "sydøst"},
    {Str::DirSouth, "syd"},
    {Str::DirSouthWest, "sydvest"},
    {Str::DirWest, "vest"},
    {Str::DirNorthWest, "nordvest"},
    {Str::OtherFloor, "på en anden etage"},
    {Str::SelectedSimSuffix, "valgt"},
    {Str::NoInteractionsAtAll, "ingen handlinger: man kan ikke gøre noget med denne genstand"},
    {Str::WalkingThere, "går derhen"},
    {Str::CannotWalk, "kan ikke gå derhen"},
    {Str::NoSimSelected, "ingen Sim valgt"},
    // Needs
    {Str::MotiveValue, "%s, %d procent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "handlingskø"},
    {Str::StatusIdle, "laver ingenting"},
    {Str::StatusNeedsLow, "lave behov: %s"},
    {Str::StatusNeedsFine, "behov i orden"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s er færdig med: %2$s"},
    {Str::NeedLow, "%1$s, lavt behov: %2$s"},
    {Str::NeedUrgent, "%1$s, akut behov: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, valgt"},
    {Str::NoSuchSim, "ingen Sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s er kommet"},
    {Str::VisitorLeft, "%s er gået"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d objekter"},
    {Str::RoomObjectsOne, "%s, 1 objekt"},
    {Str::FundsChanged, "%1$s, tilbage %2$s"},
    // Camera
    {Str::CameraFree, "kamera frit"},
    {Str::CameraFollowing, "følger %s"},
    // Narrator
    {Str::NarratorAll, "fortæller: alt"},
    {Str::NarratorWarnings, "fortæller: kun advarsler"},
    {Str::NarratorOff, "fortæller fra"},
    // Relationships
    {Str::RelationshipValues, "%1$s, daglig %2$s, livslang %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "intet rum her"},
    {Str::LayoutSummary, "%1$s, etage %2$d, %3$d felter, fra %4$d, %5$d til %6$d, %7$d"},
    {Str::LayoutOpenings, "%1$d døre, %2$d vinduer"},
    {Str::LayoutDoor, "dør %1$s, væg mod %2$s, fører til %3$s, felt %4$d, %5$d"},
    {Str::LayoutWindow, "vindue %1$s, væg mod %2$s, vender mod %3$s, felt %4$d, %5$d"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Page Down finder pladser, hvor den kan stå, piletasterne flytter den, Enter stiller den, Escape "
     "annullerer"},
    {Str::PlaceTile, "felt %1$d, %2$d"},
    {Str::PlaceOffLot, "uden for grunden"},
    {Str::PlaceFits, "kan stå her"},
    {Str::PlaceBlocked, "kan ikke stå her"},
    {Str::PlacePlaced, "%s stillet"},
    {Str::PlaceFacing, "vendt mod %s"},
    {Str::PlaceNoSpot, "passer ingen steder på grunden, uanset retning"},
    {Str::PlaceRoomSpots, "%1$s, %2$d pladser"},
    {Str::PlaceWallSide, "væg mod %s"},
    {Str::PlaceLeadsTo, "fører til %s"},
    {Str::PlaceLeadsToAnother, "fører til et andet rum: %s"},
    {Str::PlaceMoving,
     "flyt %s: Page Down finder pladser, Enter stiller den, Delete sælger den, Escape stiller den tilbage"},
    {Str::PlaceCannotPick, "%s kan ikke samles op"},
    {Str::PlaceSold, "%s solgt"},
    // Build mode
    {Str::BuildBegin,
     "%s: piletasterne flytter hjørnet, mellemrum markerer starten, Enter bygger, Ctrl+Enter fjerner, Escape "
     "afslutter"},
    {Str::BuildCorner, "hjørne %1$d, %2$d"},
    {Str::BuildMarked, "start markeret ved %1$d, %2$d"},
    {Str::BuildNoMark, "markér først starten med mellemrum"},
    {Str::BuildDone, "bygget for %1$s, %2$s tilbage"},
    {Str::BuildRemoved, "fjernet, %1$s tilbage, %2$s"},
    {Str::BuildNothing, "intet ændret"},
    {Str::BalloonThinks, "%1$s tænker: %2$s"},
    {Str::BalloonDreams, "%1$s drømmer: %2$s"},
    {Str::BalloonSays, "%1$s siger: %2$s"},
    {Str::BalloonScreams, "%1$s skriger: %2$s"},
    {Str::BalloonScreamsNothing, "%s skriger"},
    {Str::BalloonCrossed, "ingen %s"},
    {Str::BalloonRelationUp, "%s: forholdet bliver bedre"},
    {Str::BalloonRelationUpMuch, "%s: forholdet bliver meget bedre"},
    {Str::BalloonRelationDown, "%s: forholdet bliver værre"},
    {Str::BalloonRelationDownMuch, "%s: forholdet bliver meget værre"},
    {Str::BalloonRomance, "%s: romance"},
    {Str::BalloonRomanceBroken, "%s: romancen er slut"},
    {Str::BalloonCrush, "%s: forelskelse"},
    {Str::BalloonCrushBroken, "%s: forelskelsen er slut"},
    {Str::BalloonMomentBad, "%s: et dårligt øjeblik"},
    {Str::BalloonMomentGood, "%s: et stort øjeblik"},
    {Str::BalloonFirstKiss, "%s: første kys"},
    {Str::BalloonFirstLove, "%s: første kærlighed"},
    {Str::BalloonInLove, "%s: forelsket"},
    {Str::BalloonLoveLost, "%s: kærlighed tabt"},
    {Str::BalloonFriend, "%s: ny ven"},
    {Str::BalloonFriendLost, "%s: ven tabt"},
    {Str::BalloonBestFriend, "%s: ny bedste ven"},
    {Str::BalloonBestFriendLost, "%s: bedste ven tabt"},
    {Str::BalloonEnemy, "%s: ny fjende"},
    {Str::BalloonEnemyLost, "%s: ikke længere fjender"},
    {Str::BalloonMemoryGood, "%s: godt minde"},
    {Str::BalloonMemoryBad, "%s: dårligt minde"},
    {Str::BalloonTopicLiked, "%s: kan lide emnet"},
    {Str::BalloonTopicDisliked, "%s: kan ikke lide emnet"},
    {Str::BalloonPersonalityGood, "%s: personligheder passer"},
    {Str::BalloonPersonalityBad, "%s: personligheder støder sammen"},
    {Str::BalloonRouteFailed, "%s kan ikke komme derhen"},
    {Str::IconAmbulance, "ambulance"},
    {Str::IconPhone, "telefon"},
    {Str::IconPoolLadder, "poolstige"},
    {Str::IconQuestion, "spørgsmålstegn"},
    {Str::IconFingerprint, "fingeraftryk"},
    {Str::IconBaby, "baby"},
    {Str::IconDoor, "dør"},
    {Str::IconBabyQuestion, "en baby?"},
    {Str::FamilyTreePicked, "%s taget: Enter på en anden Sim forbinder dem, Enter igen lægger ned"},
    {Str::FurnitureGone, "%1$s er væk fra grunden, værdi %2$s"},
    {Str::BuildShutIn, "pas på: %2$s har ingen dør, og %1$s er derinde; Ctrl+Z fortryder væggen"},
};

}  // namespace

extern const Translations kDanish = {kEntries, std::size(kEntries)};

}  // namespace lang
