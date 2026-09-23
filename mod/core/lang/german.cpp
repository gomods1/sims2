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
    {Str::KeysLot,
     "Grundstück: Bild ab und Bild auf, die Objekte des Grundstücks, die nächsten zuerst; mit Umschalt die "
     "Kategorien, mit Strg die Zimmer. Eingabe öffnet die Interaktionen eines Objekts oder hebt es im Kaufmodus "
     "auf; Strg+Eingabe schickt den gewählten Sim dorthin. Strg+E der Zustand des Sims, Strg+1 bis 9 wählen "
     "einen Sim, Tab die Leisten, Strg+Pfeil hoch und runter die Details."},
    {Str::KeysPlacement,
     "Platzieren: Bild ab und Bild auf die Plätze, an die es passt, mit Strg die Zimmer, Pfeiltasten ein Feld, "
     "Pos1 zurück zum Sim, Komma und Punkt drehen es, Eingabe stellt es hin, Entf verkauft es, Escape beendet."},
    {Str::KeysBuild,
     "Bauen: Pfeiltasten bewegen die Ecke, mit Umschalt fünf, Bild auf und ab die Ebene, Leertaste markiert den "
     "Anfang, Eingabe baut ab der Markierung, Strg+Eingabe entfernt, Pos1 zurück zum Sim, R wiederholt, Escape "
     "beendet."},
    {Str::KeysNeighborhood,
     "Nachbarschaft: zuerst die Liste der Grundstücke, Eingabe öffnet die Leiste eines Grundstücks. Um eine "
     "Familie einziehen zu lassen, wähle sie im Familienbehälter und drücke Eingabe auf einem Grundstück. Tab "
     "die Leisten."},
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
    {Str::GroupControlPanel, "Steuerleiste"},
    {Str::GroupLivePanel, "Sim-Leiste"},
    {Str::GroupFamily, "Familie"},
    {Str::GroupWantsFears, "Wünsche und Ängste"},
    {Str::GroupNeeds, "Bedürfnisse"},
    {Str::GroupControls, "Steuerung"},
    {Str::GroupBuyPanel, "Kaufmodus"},
    {Str::GroupBuildPanel, "Baumodus"},
    {Str::GroupCatalog, "Katalog"},
    {Str::GroupStarSign, "Sternzeichen"},
    {Str::GroupAspiration, "Bestreben"},
    {Str::GroupSteps, "Schritte"},
    {Str::GroupAge, "Alter"},
    {Str::GroupSex, "Geschlecht"},
    {Str::GroupTraits, "Eigenschaften"},
    // Text fields
    {Str::TextField, "%s %s, Eingabefeld"},
    {Str::Blank, "leer"},
    {Str::Space, "Leerzeichen"},
    // Values
    {Str::ValueOfMax, "%d von %d"},
    {Str::SliderRange, "%1$s bis %2$s"},
    {Str::RadioBetween, "zwischen %1$s und %2$s"},
    {Str::PointsLeft, "%d Punkte übrig"},
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
    // Neighborhood: lots and families
    {Str::GroupLots, "Grundstücke"},
    {Str::LotFamily, "Familie %s"},
    {Str::LotForSale, "zu verkaufen"},
    {Str::LotCommunity, "Gemeinschaftsgrundstück"},
    {Str::LotApartmentsEmpty, "Wohnhaus, keine Mieter"},
    {Str::LotApartmentsWith, "Wohnhaus: %s"},
    {Str::FamilySummary, "%1$s: %2$d Mitglieder, Vermögen %3$s"},
    // Money
    {Str::Simoleons, "Simoleons"},
    {Str::FundsNow, "Vermögen %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Sims"},
    {Str::CategoryOther, "Sonstige Objekte"},
    {Str::NoObjects, "hier gibt es keine Objekte"},
    {Str::FloorNumber, "Etage %d"},
    {Str::NotOnLot, "nicht auf einem Grundstück"},
    {Str::HereNow, "genau hier"},
    {Str::TilesAway, "%d Felder Richtung %s"},
    {Str::OneTileAway, "1 Feld Richtung %s"},
    {Str::DirNorth, "Norden"},
    {Str::DirNorthEast, "Nordosten"},
    {Str::DirEast, "Osten"},
    {Str::DirSouthEast, "Südosten"},
    {Str::DirSouth, "Süden"},
    {Str::DirSouthWest, "Südwesten"},
    {Str::DirWest, "Westen"},
    {Str::DirNorthWest, "Nordwesten"},
    {Str::OtherFloor, "auf einer anderen Etage"},
    {Str::SelectedSimSuffix, "ausgewählt"},
    {Str::NoInteractionsAtAll, "keine Interaktionen: mit diesem Objekt kann man nichts tun"},
    {Str::WalkingThere, "geht dorthin"},
    {Str::CannotWalk, "kann nicht dorthin gehen"},
    {Str::NoSimSelected, "kein Sim ausgewählt"},
    // Needs
    {Str::MotiveValue, "%s, %d Prozent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "Warteschlange"},
    {Str::StatusIdle, "tut nichts"},
    {Str::StatusNeedsLow, "niedrige Bedürfnisse: %s"},
    {Str::StatusNeedsFine, "Bedürfnisse in Ordnung"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s ist fertig: %2$s"},
    {Str::NeedLow, "%1$s, niedriges Bedürfnis: %2$s"},
    {Str::NeedUrgent, "%1$s, dringendes Bedürfnis: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, ausgewählt"},
    {Str::NoSuchSim, "kein Sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s ist angekommen"},
    {Str::VisitorLeft, "%s ist gegangen"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d Objekte"},
    {Str::RoomObjectsOne, "%s, 1 Objekt"},
    {Str::FundsChanged, "%1$s, übrig %2$s"},
    // Camera
    {Str::CameraFree, "Kamera frei"},
    {Str::CameraFollowing, "folgt %s"},
    // Narrator
    {Str::NarratorAll, "Erzähler: alles"},
    {Str::NarratorWarnings, "Erzähler: nur Warnungen"},
    {Str::NarratorOff, "Erzähler aus"},
    // Relationships
    {Str::RelationshipValues, "%1$s, täglich %2$s, dauerhaft %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "hier ist kein Zimmer"},
    {Str::LayoutSummary, "%1$s, Etage %2$d, %3$d Felder, von %4$d, %5$d bis %6$d, %7$d"},
    {Str::LayoutOpenings, "%1$d Türen, %2$d Fenster"},
    {Str::LayoutDoor, "Tür %1$s, Wand im %2$s, führt zu %3$s, Feld %4$d, %5$d"},
    {Str::LayoutWindow, "Fenster %1$s, Wand im %2$s, zeigt auf %3$s, Feld %4$d, %5$d"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Bild ab findet Plätze, an die es passt, Pfeiltasten bewegen es, Eingabe stellt es hin, Escape bricht ab"},
    {Str::PlaceTile, "Feld %1$d, %2$d"},
    {Str::PlaceOffLot, "außerhalb des Grundstücks"},
    {Str::PlaceFits, "passt"},
    {Str::PlaceBlocked, "passt hier nicht"},
    {Str::PlacePlaced, "%s hingestellt"},
    {Str::PlaceFacing, "Ausrichtung: %s"},
    {Str::PlaceNoSpot, "passt nirgends auf dieses Grundstück, egal wie gedreht"},
    {Str::PlaceRoomSpots, "%1$s, %2$d Plätze"},
    {Str::PlaceWallSide, "Wand im %s"},
    {Str::PlaceLeadsTo, "führt zu %s"},
    {Str::PlaceLeadsToAnother, "führt zu einem anderen Raum: %s"},
    {Str::PlaceMoving,
     "%s verschieben: Bild ab findet Plätze, Eingabe stellt es hin, Entf verkauft es, Escape stellt es zurück"},
    {Str::PlaceCannotPick, "%s kann nicht aufgehoben werden"},
    {Str::PlaceSold, "%s verkauft"},
    // Build mode
    {Str::BuildBegin,
     "%s: Pfeiltasten bewegen die Ecke, Leertaste markiert den Anfang, Eingabe baut, Strg+Eingabe entfernt, "
     "Escape beendet"},
    {Str::BuildCorner, "Ecke %1$d, %2$d"},
    {Str::BuildMarked, "Anfang markiert bei %1$d, %2$d"},
    {Str::BuildNoMark, "markiere zuerst den Anfang mit der Leertaste"},
    {Str::BuildDone, "gebaut für %1$s, übrig %2$s"},
    {Str::BuildRemoved, "entfernt, %1$s zurück, %2$s"},
    {Str::BuildNothing, "nichts geändert"},
    {Str::BalloonThinks, "%1$s denkt: %2$s"},
    {Str::BalloonDreams, "%1$s träumt: %2$s"},
    {Str::BalloonSays, "%1$s sagt: %2$s"},
    {Str::BalloonScreams, "%1$s schreit: %2$s"},
    {Str::BalloonScreamsNothing, "%s schreit"},
    {Str::BalloonCrossed, "kein %s"},
    {Str::BalloonRelationUp, "%s: Beziehung wird besser"},
    {Str::BalloonRelationUpMuch, "%s: Beziehung wird viel besser"},
    {Str::BalloonRelationDown, "%s: Beziehung wird schlechter"},
    {Str::BalloonRelationDownMuch, "%s: Beziehung wird viel schlechter"},
    {Str::BalloonRomance, "%s: Romanze"},
    {Str::BalloonRomanceBroken, "%s: Romanze zerbrochen"},
    {Str::BalloonCrush, "%s: Schwarm"},
    {Str::BalloonCrushBroken, "%s: Schwarm vorbei"},
    {Str::BalloonMomentBad, "%s: ein schlechter Moment"},
    {Str::BalloonMomentGood, "%s: ein großer Moment"},
    {Str::BalloonFirstKiss, "%s: erster Kuss"},
    {Str::BalloonFirstLove, "%s: erste Liebe"},
    {Str::BalloonInLove, "%s: verliebt"},
    {Str::BalloonLoveLost, "%s: Liebe verloren"},
    {Str::BalloonFriend, "%s: neuer Freund"},
    {Str::BalloonFriendLost, "%s: Freund verloren"},
    {Str::BalloonBestFriend, "%s: neuer bester Freund"},
    {Str::BalloonBestFriendLost, "%s: bester Freund verloren"},
    {Str::BalloonEnemy, "%s: neuer Feind"},
    {Str::BalloonEnemyLost, "%s: keine Feinde mehr"},
    {Str::BalloonMemoryGood, "%s: gute Erinnerung"},
    {Str::BalloonMemoryBad, "%s: schlechte Erinnerung"},
    {Str::BalloonTopicLiked, "%s: mag das Thema"},
    {Str::BalloonTopicDisliked, "%s: mag das Thema nicht"},
    {Str::BalloonPersonalityGood, "%s: Persönlichkeiten passen"},
    {Str::BalloonPersonalityBad, "%s: Persönlichkeiten prallen aufeinander"},
    {Str::BalloonRouteFailed, "%s kommt nicht hin"},
    {Str::IconAmbulance, "Krankenwagen"},
    {Str::IconPhone, "Telefon"},
    {Str::IconPoolLadder, "Poolleiter"},
    {Str::IconQuestion, "Fragezeichen"},
    {Str::IconFingerprint, "Fingerabdruck"},
    {Str::IconBaby, "Baby"},
    {Str::IconDoor, "Tür"},
    {Str::IconBabyQuestion, "ein Baby?"},
    {Str::FamilyTreePicked, "%s aufgenommen: Eingabe auf einem anderen Sim verbindet sie, erneut Eingabe legt ab"},
    {Str::FurnitureGone, "%1$s ist vom Grundstück verschwunden, Wert %2$s"},
};

}  // namespace

extern const Translations kGerman = {kEntries, std::size(kEntries)};

}  // namespace lang
