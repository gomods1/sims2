// Norwegian: the game's language 22. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Sims 2-tilgjengelighet lastet inn"},
    {Str::UnsupportedBuild, "Sims 2-tilgjengelighet: spillversjonen støttes ikke, modden er deaktivert"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Piltastene flytter innenfor en gruppe, Tab og Skift+Tab bytter gruppe, Home og End går til endene, Enter "
     "aktiverer, R gjentar. Kontroll+Pil opp og Kontroll+Pil ned leser detaljene om det som ble sagt, "
     "Kontroll+Venstre pil og Kontroll+Høyre pil bytter mellom detaljer og hendelser."},
    {Str::KeysLot,
     "Tomt: Page Down og Page Up, tomtens gjenstander, de nærmeste først; med Shift kategoriene, med Ctrl "
     "rommene. Enter åpner en gjenstands handlinger, eller plukker den opp i kjøpemodus; Ctrl+Enter sender den "
     "valgte simen dit. Ctrl+E simens tilstand, Ctrl+1 til 9 velger en sim, Tab panelene, Ctrl+pil opp og ned "
     "detaljene."},
    {Str::KeysPlacement,
     "Plassering: Page Down og Page Up plassene der den får plass, med Ctrl rommene, piler én rute, Home tilbake "
     "til simen, komma og punktum snur den, Enter setter den ned, Delete selger den, Escape avslutter."},
    {Str::KeysBuild,
     "Bygging: pilene flytter hjørnet, med Shift fem, Page Up og Page Down nivået, mellomrom markerer starten, "
     "Enter bygger fra merket, Ctrl+Enter fjerner, Home tilbake til simen, R gjentar, Escape avslutter."},
    {Str::KeysNeighborhood,
     "Nabolag: først listen over tomter, Enter åpner en tomts panel. For å flytte inn en familie, velg den i "
     "familiebeholderen og trykk Enter på en tomt. Tab panelene."},
    // Control state
    {Str::Checked, "avmerket"},
    {Str::NotChecked, "ikke avmerket"},
    {Str::Selected, "valgt"},
    {Str::TabSelected, "valgt"},
    {Str::Unavailable, "utilgjengelig"},
    {Str::UnlabeledButton, "knapp uten navn"},
    {Str::Unnamed, "uten navn"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "knapp"},
    {Str::RoleTab, "fane"},
    {Str::RoleCheckBox, "avmerkingsboks"},
    {Str::RoleSlider, "glidebryter"},
    {Str::RolePicker, "verdivelger"},
    {Str::NothingHere, "ingenting å navigere i her"},
    // Review buffers
    {Str::BufferDetails, "detaljer"},
    {Str::BufferEvents, "hendelser"},
    {Str::BufferEmpty, "%s, tom"},
    {Str::PositionInGroup, "%d av %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d elementer"},
    {Str::GroupSummaryOne, "%s, 1 element"},
    {Str::GroupButtons, "knapper"},
    {Str::GroupText, "tekst"},
    {Str::GroupControlPanel, "kontrollpanel"},
    {Str::GroupLivePanel, "Sim-panel"},
    {Str::GroupFamily, "familie"},
    {Str::GroupWantsFears, "ønsker og frykt"},
    {Str::GroupNeeds, "behov"},
    {Str::GroupControls, "kontroller"},
    {Str::GroupBuyPanel, "kjøpemodus"},
    {Str::GroupBuildPanel, "byggemodus"},
    {Str::GroupCatalog, "katalog"},
    {Str::GroupStarSign, "stjernetegn"},
    {Str::GroupAspiration, "ambisjon"},
    {Str::GroupSteps, "trinn"},
    {Str::GroupAge, "alder"},
    {Str::GroupSex, "kjønn"},
    {Str::GroupTraits, "egenskaper"},
    // Text fields
    {Str::TextField, "%s %s, tekstfelt"},
    {Str::Blank, "tomt"},
    {Str::Space, "mellomrom"},
    // Values
    {Str::ValueOfMax, "%d av %d"},
    {Str::SliderRange, "%1$s til %2$s"},
    {Str::RadioBetween, "mellom %1$s og %2$s"},
    {Str::PointsLeft, "%d poeng igjen"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "neste side"},
    {Str::PreviousPage, "forrige side"},
    // Interaction menu
    {Str::PieMenu, "handlinger"},
    {Str::NoInteractions, "ingen handlinger"},
    {Str::Cancelled, "avbrutt"},
    // Cheat console
    {Str::CheatConsole, "juksekonsoll"},
    {Str::CheatRan, "juksekode: %s"},
    {Str::CheatEmpty, "skriv eller velg en juksekode først"},
    // Neighborhood: lots and families
    {Str::GroupLots, "tomter"},
    {Str::LotFamily, "familien %s"},
    {Str::LotForSale, "til salgs"},
    {Str::LotCommunity, "felles tomt"},
    {Str::LotApartmentsEmpty, "leilighetsbygg, ingen leietakere"},
    {Str::LotApartmentsWith, "leilighetsbygg: %s"},
    {Str::FamilySummary, "%1$s: %2$d medlemmer, penger %3$s"},
    // Money
    {Str::Simoleons, "simoleons"},
    {Str::FundsNow, "midler %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "simmer"},
    {Str::CategoryOther, "andre objekter"},
    {Str::NoObjects, "ingen objekter her"},
    {Str::FloorNumber, "etasje %d"},
    {Str::NotOnLot, "ikke på en tomt"},
    {Str::HereNow, "akkurat her"},
    {Str::TilesAway, "%d fliser mot %s"},
    {Str::OneTileAway, "1 flis mot %s"},
    {Str::DirNorth, "nord"},
    {Str::DirNorthEast, "nordøst"},
    {Str::DirEast, "øst"},
    {Str::DirSouthEast, "sørøst"},
    {Str::DirSouth, "sør"},
    {Str::DirSouthWest, "sørvest"},
    {Str::DirWest, "vest"},
    {Str::DirNorthWest, "nordvest"},
    {Str::OtherFloor, "i en annen etasje"},
    {Str::SelectedSimSuffix, "valgt"},
    {Str::NoInteractionsAtAll, "ingen handlinger: man kan ikke gjøre noe med denne gjenstanden"},
    {Str::WalkingThere, "går dit"},
    {Str::CannotWalk, "kan ikke gå dit"},
    {Str::NoSimSelected, "ingen sim valgt"},
    // Needs
    {Str::MotiveValue, "%s, %d prosent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "handlingskø"},
    {Str::StatusIdle, "gjør ingenting"},
    {Str::StatusNeedsLow, "lave behov: %s"},
    {Str::StatusNeedsFine, "behovene er i orden"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s er ferdig med: %2$s"},
    {Str::NeedLow, "%1$s, lavt behov: %2$s"},
    {Str::NeedUrgent, "%1$s, akutt behov: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, valgt"},
    {Str::NoSuchSim, "ingen sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s har kommet"},
    {Str::VisitorLeft, "%s har gått"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d objekter"},
    {Str::RoomObjectsOne, "%s, 1 objekt"},
    {Str::FundsChanged, "%1$s, igjen %2$s"},
    // Camera
    {Str::CameraFree, "kamera fritt"},
    {Str::CameraFollowing, "følger %s"},
    // Narrator
    {Str::NarratorAll, "forteller: alt"},
    {Str::NarratorWarnings, "forteller: bare advarsler"},
    {Str::NarratorOff, "forteller av"},
    // Relationships
    {Str::RelationshipValues, "%1$s, daglig %2$s, livslang %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "ikke noe rom her"},
    {Str::LayoutSummary, "%1$s, etasje %2$d, %3$d ruter, fra %4$d, %5$d til %6$d, %7$d"},
    {Str::LayoutOpenings, "%1$d dører, %2$d vinduer"},
    {Str::LayoutDoor, "dør %1$s, vegg mot %2$s, fører til %3$s, rute %4$d, %5$d"},
    {Str::LayoutWindow, "vindu %1$s, vegg mot %2$s, vender mot %3$s, rute %4$d, %5$d"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Page Down finner plasser der den får plass, piltastene flytter den, Enter setter den ned, Escape "
     "avbryter"},
    {Str::PlaceTile, "rute %1$d, %2$d"},
    {Str::PlaceOffLot, "utenfor tomten"},
    {Str::PlaceFits, "får plass"},
    {Str::PlaceBlocked, "får ikke plass her"},
    {Str::PlacePlaced, "%s plassert"},
    {Str::PlaceFacing, "vendt mot %s"},
    {Str::PlaceNoSpot, "får ikke plass noe sted på tomten, uansett retning"},
    {Str::PlaceRoomSpots, "%1$s, %2$d plasser"},
    {Str::PlaceWallSide, "vegg mot %s"},
    {Str::PlaceLeadsTo, "fører til %s"},
    {Str::PlaceLeadsToAnother, "fører til et annet rom: %s"},
    {Str::PlaceMoving,
     "flytt %s: Page Down finner plasser, Enter setter den ned, Delete selger den, Escape setter den tilbake"},
    {Str::PlaceCannotPick, "%s kan ikke plukkes opp"},
    {Str::PlaceSold, "%s solgt"},
    // Build mode
    {Str::BuildBegin,
     "%s: piltastene flytter hjørnet, mellomrom markerer starten, Enter bygger, Ctrl+Enter fjerner, Escape "
     "avslutter"},
    {Str::BuildCorner, "hjørne %1$d, %2$d"},
    {Str::BuildMarked, "start markert ved %1$d, %2$d"},
    {Str::BuildNoMark, "marker først starten med mellomrom"},
    {Str::BuildDone, "bygget for %1$s, %2$s igjen"},
    {Str::BuildRemoved, "fjernet, %1$s tilbake, %2$s"},
    {Str::BuildNothing, "ingenting endret"},
    {Str::BalloonThinks, "%1$s tenker: %2$s"},
    {Str::BalloonDreams, "%1$s drømmer: %2$s"},
    {Str::BalloonSays, "%1$s sier: %2$s"},
    {Str::BalloonScreams, "%1$s skriker: %2$s"},
    {Str::BalloonScreamsNothing, "%s skriker"},
    {Str::BalloonCrossed, "ingen %s"},
    {Str::BalloonRelationUp, "%s: forholdet blir bedre"},
    {Str::BalloonRelationUpMuch, "%s: forholdet blir mye bedre"},
    {Str::BalloonRelationDown, "%s: forholdet blir verre"},
    {Str::BalloonRelationDownMuch, "%s: forholdet blir mye verre"},
    {Str::BalloonRomance, "%s: romanse"},
    {Str::BalloonRomanceBroken, "%s: romansen er over"},
    {Str::BalloonCrush, "%s: forelskelse"},
    {Str::BalloonCrushBroken, "%s: forelskelsen er over"},
    {Str::BalloonMomentBad, "%s: et dårlig øyeblikk"},
    {Str::BalloonMomentGood, "%s: et stort øyeblikk"},
    {Str::BalloonFirstKiss, "%s: første kyss"},
    {Str::BalloonFirstLove, "%s: første kjærlighet"},
    {Str::BalloonInLove, "%s: forelsket"},
    {Str::BalloonLoveLost, "%s: kjærlighet tapt"},
    {Str::BalloonFriend, "%s: ny venn"},
    {Str::BalloonFriendLost, "%s: venn tapt"},
    {Str::BalloonBestFriend, "%s: ny bestevenn"},
    {Str::BalloonBestFriendLost, "%s: bestevenn tapt"},
    {Str::BalloonEnemy, "%s: ny fiende"},
    {Str::BalloonEnemyLost, "%s: ikke lenger fiender"},
    {Str::BalloonMemoryGood, "%s: godt minne"},
    {Str::BalloonMemoryBad, "%s: dårlig minne"},
    {Str::BalloonTopicLiked, "%s: liker emnet"},
    {Str::BalloonTopicDisliked, "%s: liker ikke emnet"},
    {Str::BalloonPersonalityGood, "%s: personlighetene passer"},
    {Str::BalloonPersonalityBad, "%s: personlighetene kolliderer"},
    {Str::BalloonRouteFailed, "%s kommer seg ikke dit"},
    {Str::IconAmbulance, "ambulanse"},
    {Str::IconPhone, "telefon"},
    {Str::IconPoolLadder, "bassengstige"},
    {Str::IconQuestion, "spørsmålstegn"},
    {Str::IconFingerprint, "fingeravtrykk"},
    {Str::IconBaby, "baby"},
    {Str::IconDoor, "dør"},
    {Str::IconBabyQuestion, "en baby?"},
    {Str::FamilyTreePicked, "%s tatt opp: Enter på en annen Sim knytter dem sammen, Enter igjen slipper"},
    {Str::FurnitureGone, "%1$s er borte fra tomten, verdi %2$s"},
    {Str::BuildShutIn, "pass på: %1$s er stengt inne, %2$s har ingen dør; Ctrl+Z angrer veggen"},
};

}  // namespace

extern const Translations kNorwegian = {kEntries, std::size(kEntries)};

}  // namespace lang
