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
     "Tab en Shift+Tab gaan van groep naar groep; pijl omhoog en omlaag gaan door een groep, links en rechts "
     "ook, of veranderen een schuifregelaar; Home en End gaan naar het begin en het einde; Enter activeert, en "
     "bij een soort of een kamer opent het de objecten; Backspace gaat terug; R herhaalt. Control+pijl omhoog en "
     "Control+pijl omlaag lezen de details van wat er gezegd is, Control+pijl links en Control+pijl rechts "
     "wisselen tussen details en gebeurtenissen."},
    {Str::KeysLot,
     "Kavel: Tab gaat langs de behoeften, de actiewachtrij, de objecten per soort, de objecten per kamer en de "
     "panelen van het spel. Page Down en Page Up gaan door de objecten van het kavel, dichtstbijzijnde eerst; "
     "met Shift veranderen ze de soort object, met Ctrl de kamer. Enter op een object opent de interacties, of "
     "pakt het op in de koopmodus; Ctrl+Enter stuurt de gekozen Sim erheen. Ctrl+E de toestand van de Sim, "
     "Ctrl+1 tot 9 kiezen een Sim, Ctrl+pijl omhoog en omlaag de details."},
    {Str::KeysPlacement,
     "Plaatsen: pijltjes één vak, Shift vijf, Page Down en Page Up de plekken waar het past, met Ctrl de kamers, "
     "met Shift of komma en punt draaien het, Home terug naar de Sim, Enter zet het neer, Delete verkoopt het, "
     "Escape stopt."},
    {Str::KeysBuild,
     "Bouwen: de pijltjes verplaatsen de hoek, met Shift vijf, Page Up en Page Down het niveau, spatie markeert "
     "het begin, Enter bouwt vanaf de markering, Ctrl+Enter verwijdert, Home terug naar de Sim, R herhaalt, "
     "Escape stopt."},
    {Str::KeysNeighborhood,
     "Buurt: eerst de lijst met kavels, Enter opent het paneel van een kavel. Om een gezin te laten verhuizen, "
     "kies het in de gezinnenbak en druk Enter op een kavel. Tab de panelen."},
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
    {Str::GroupBuildPanel, "bouwmodus"},
    {Str::GroupCatalog, "catalogus"},
    {Str::GroupStarSign, "sterrenbeeld"},
    {Str::GroupAspiration, "ambitie"},
    {Str::GroupSteps, "stappen"},
    {Str::GroupAge, "leeftijd"},
    {Str::GroupSex, "geslacht"},
    {Str::GroupTraits, "eigenschappen"},
    // Text fields
    {Str::TextField, "%s %s, tekstveld"},
    {Str::Blank, "leeg"},
    {Str::Space, "spatie"},
    // Values
    {Str::ValueOfMax, "%d van %d"},
    {Str::SliderRange, "%1$s tot %2$s"},
    {Str::RadioBetween, "tussen %1$s en %2$s"},
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
    {Str::ThingsByKind, "objecten per soort"},
    {Str::ThingsByRoom, "objecten per kamer"},
    {Str::FundsChanged, "%1$s, nog %2$s"},
    // Camera
    {Str::CameraFree, "camera vrij"},
    {Str::CameraFollowing, "volgt %s"},
    // Narrator
    {Str::NarratorAll, "verteller: alles"},
    {Str::NarratorWarnings, "verteller: alleen waarschuwingen"},
    {Str::NarratorOff, "verteller uit"},
    // Relationships
    {Str::RelationshipValues, "%1$s, dagelijks %2$s, blijvend %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "hier is geen kamer"},
    {Str::LayoutSummary, "%1$s, verdieping %2$d, %3$d vakken, van %4$d, %5$d tot %6$d, %7$d"},
    {Str::LayoutOpenings, "%1$d deuren, %2$d ramen"},
    {Str::LayoutDoor, "deur %1$s, muur aan de %2$s, komt uit op %3$s, vak %4$d, %5$d"},
    {Str::LayoutWindow, "raam %1$s, muur aan de %2$s, kijkt uit op %3$s, vak %4$d, %5$d"},
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
    {Str::PlaceNoSpot, "past nergens op dit perceel, hoe je het ook draait"},
    {Str::PlaceRoomSpots, "%1$s, %2$d plekken"},
    {Str::PlaceWallSide, "muur aan de %s"},
    {Str::PlaceLeadsTo, "komt uit op %s"},
    {Str::PlaceLeadsToAnother, "komt uit op een andere ruimte: %s"},
    {Str::PlaceMoving,
     "%s verplaatsen: Page Down zoekt plekken, Enter zet het neer, Delete verkoopt het, Escape zet het terug"},
    {Str::PlaceCannotPick, "%s kan niet opgepakt worden"},
    {Str::PlaceSold, "%s verkocht"},
    // Build mode
    {Str::BuildBegin,
     "%s: de pijltjes verplaatsen de hoek, spatie markeert het begin, Enter bouwt, Ctrl+Enter verwijdert, Escape "
     "stopt"},
    {Str::BuildCorner, "hoek %1$d, %2$d"},
    {Str::BuildMarked, "begin gemarkeerd op %1$d, %2$d"},
    {Str::BuildNoMark, "markeer eerst het begin met spatie"},
    {Str::BuildDone, "gebouwd voor %1$s, nog %2$s"},
    {Str::BuildRemoved, "verwijderd, %1$s terug, %2$s"},
    {Str::BuildNothing, "er is niets veranderd"},
    {Str::BalloonThinks, "%1$s denkt: %2$s"},
    {Str::BalloonDreams, "%1$s droomt: %2$s"},
    {Str::BalloonSays, "%1$s zegt: %2$s"},
    {Str::BalloonScreams, "%1$s schreeuwt: %2$s"},
    {Str::BalloonScreamsNothing, "%s schreeuwt"},
    {Str::BalloonCrossed, "geen %s"},
    {Str::BalloonRelationUp, "%s: relatie verbetert"},
    {Str::BalloonRelationUpMuch, "%s: relatie verbetert sterk"},
    {Str::BalloonRelationDown, "%s: relatie verslechtert"},
    {Str::BalloonRelationDownMuch, "%s: relatie verslechtert sterk"},
    {Str::BalloonRomance, "%s: romance"},
    {Str::BalloonRomanceBroken, "%s: romance voorbij"},
    {Str::BalloonCrush, "%s: verliefdheid"},
    {Str::BalloonCrushBroken, "%s: verliefdheid voorbij"},
    {Str::BalloonMomentBad, "%s: een slecht moment"},
    {Str::BalloonMomentGood, "%s: een geweldig moment"},
    {Str::BalloonFirstKiss, "%s: eerste kus"},
    {Str::BalloonFirstLove, "%s: eerste liefde"},
    {Str::BalloonInLove, "%s: verliefd"},
    {Str::BalloonLoveLost, "%s: liefde verloren"},
    {Str::BalloonFriend, "%s: nieuwe vriend"},
    {Str::BalloonFriendLost, "%s: vriend verloren"},
    {Str::BalloonBestFriend, "%s: nieuwe beste vriend"},
    {Str::BalloonBestFriendLost, "%s: beste vriend verloren"},
    {Str::BalloonEnemy, "%s: nieuwe vijand"},
    {Str::BalloonEnemyLost, "%s: geen vijanden meer"},
    {Str::BalloonMemoryGood, "%s: goede herinnering"},
    {Str::BalloonMemoryBad, "%s: slechte herinnering"},
    {Str::BalloonTopicLiked, "%s: vindt het onderwerp leuk"},
    {Str::BalloonTopicDisliked, "%s: vindt het onderwerp niet leuk"},
    {Str::BalloonPersonalityGood, "%s: persoonlijkheden klikken"},
    {Str::BalloonPersonalityBad, "%s: persoonlijkheden botsen"},
    {Str::BalloonRouteFailed, "%s kan er niet komen"},
    {Str::IconAmbulance, "ambulance"},
    {Str::IconPhone, "telefoon"},
    {Str::IconPoolLadder, "zwembadtrap"},
    {Str::IconQuestion, "vraagteken"},
    {Str::IconFingerprint, "vingerafdruk"},
    {Str::IconBaby, "baby"},
    {Str::IconDoor, "deur"},
    {Str::IconBabyQuestion, "een baby?"},
    {Str::FamilyTreePicked,
     "%s opgepakt: Enter op een andere Sim om ze te verbinden, nogmaals Enter om los te laten"},
    {Str::FurnitureGone, "%1$s is van het perceel verdwenen, waarde %2$s"},
    {Str::BuildShutIn, "let op: %2$s heeft geen deur en %1$s is binnen; Ctrl+Z maakt de muur ongedaan"},
    {Str::KeysLotMore,
     "Ctrl+L de indeling van de kamer, Ctrl+N wat de verteller zegt, Ctrl+F of de camera de Sim volgt."},
    {Str::GroupBusiness, "bedrijf"},
    {Str::GroupSeasons, "seizoenen"},
    {Str::GroupDate, "afspraakje"},
    // Groups by letter, the focus on screen (ported from the Sims 1 mod)
    {Str::NoGroupLetter, "geen groep begint met %s"},
    {Str::KeysGroupLetter, "Alt en een letter: de volgende groep waarvan de naam met die letter begint."},
    {Str::KeysShowFocus, "Ctrl Shift V: licht het gekozen element op het scherm op, voor wie helpt."},
    {Str::ShowFocusOn, "selectie zichtbaar op het scherm"},
    {Str::ShowFocusOff, "selectie niet meer zichtbaar op het scherm"},
    // Placing: size, what it runs into and faces, beside the lot's things
    {Str::ObjectSize, "beslaat %1$d vakken: %2$d van west naar oost, %3$d van noord naar zuid"},
    {Str::CollidesWith, "botst met %s"},
    {Str::FacingThing, "naar %s toe"},
    {Str::RoomChange, "verlaat %1$s, gaat %2$s in"},
    {Str::BesideThing, "naast %s"},
    {Str::KeysPlacingThings,
     "Ctrl Shift Page Down en Page Up: zet het naast het volgende object van het kavel, het dichtstbijzijnde "
     "eerst."},
    // Sound cues and the lot map
    {Str::CuesOn, "cursorgeluiden aan"},
    {Str::CuesOff, "cursorgeluiden uit"},
    {Str::SurfaceNone, "geen vloer"},
    {Str::SurfaceGround, "grond"},
    {Str::SurfaceGrass, "gras"},
    {Str::SurfaceWood, "houten vloer"},
    {Str::SurfaceTile, "tegelvloer"},
    {Str::SurfaceCarpet, "vloerbedekking"},
    {Str::SurfacePaving, "bestrating"},
    {Str::SurfaceGravel, "grind"},
    {Str::SurfacePoolRim, "zwembadrand"},
    {Str::SurfaceWater, "water"},
    {Str::EdgeFence, "hek in het %s"},
    {Str::EdgeRailing, "balustrade in het %s"},
    {Str::MapStairs, "trap"},
    {Str::TerrainSloped, "hellend"},
    {Str::EdgeWall, "muur in het %s"},
    {Str::EdgeDoor, "deur in het %s"},
    {Str::EdgeWindow, "raam in het %s"},
    {Str::MapOn, "Plattegrond van het kavel. Ctrl+F1 noemt de toetsen."},
    {Str::MapOff, "plattegrond uit"},
    {Str::MapNoFloor, "op verdieping %d is niets gebouwd"},
    {Str::MapNothingHere, "hier is niets te gebruiken"},
    {Str::MapLotEdge, "rand van het kavel"},
    {Str::KeysMap,
     "Plattegrond: de pijltjes gaan één vak verder, met Shift vijf; elk vak zegt wat er staat en welke kant het "
     "op kijkt, de kamer en de vloer als die veranderen, en de muren, hekken, balustrades, deuren en ramen "
     "eromheen, trappen en hellingen, elk met een geluid. Home gaat terug naar de gekozen Sim. Page Down en Page "
     "Up gaan zoals altijd door de objecten van het kavel, en de plattegrond gaat naar elk ervan. "
     "Ctrl+Shift+Page Up en Page Down wisselen van verdieping. Enter opent het menu van wat er staat; Ctrl+Enter "
     "stuurt de gekozen Sim erheen. R zegt alles, Ctrl+L de indeling van de kamer. Ctrl+Shift+S zet de geluiden "
     "uit en aan. Escape of Ctrl+M sluit de plattegrond."},
    {Str::KeysMapHint, "Ctrl+M: de plattegrond van het kavel, een cursor die het kavel vak voor vak afloopt."},
    {Str::KeysCuesHint, "Ctrl+Shift+S zet de cursorgeluiden uit en aan."},
    // The manual (Control+Shift+F1, the mod help group)
    {Str::ManualHint, "Ctrl+Shift+F1 opent de handleiding; Ctrl+F1 noemt de toetsen van elk scherm."},
    {Str::KeysManual, "Ctrl+Shift+F1 opent de volledige handleiding van de mod in je browser."},
    {Str::ManualOpening, "handleiding wordt geopend in de browser"},
    {Str::ManualMissing, "de handleiding ontbreekt: installeer de mod opnieuw"},
    {Str::ManualButton, "Handleiding Toegankelijkheid voor De Sims 2"},
    {Str::GroupModHelp, "mod-hulp"},
    {Str::KeysButton, "Toetsen voor dit scherm"},
};

}  // namespace

extern const Translations kDutch = {kEntries, std::size(kEntries)};

}  // namespace lang
