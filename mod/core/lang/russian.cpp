// Russian: the game's language 16. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Доступность для The Sims 2 загружена"},
    {Str::UnsupportedBuild, "Доступность для The Sims 2: неподдерживаемая версия игры, мод отключён"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Стрелки перемещают внутри группы, Tab и Shift+Tab меняют группу, Home и End переходят к краям, Enter "
     "нажимает, R повторяет. Ctrl+стрелка вверх и Ctrl+стрелка вниз читают подробности сказанного, Ctrl+стрелка "
     "влево и Ctrl+стрелка вправо переключают между подробностями и событиями."},
    {Str::KeysLot,
     "Участок: Page Down и Page Up — предметы участка, ближайшие первыми; с Shift — категории, с Ctrl — комнаты. "
     "Enter открывает действия предмета или поднимает его в режиме покупки; Ctrl+Enter отправляет туда "
     "выбранного сима. Ctrl+E состояние сима, Ctrl+1–9 выбор сима, Tab панели, Ctrl+стрелки вверх и вниз "
     "подробности."},
    {Str::KeysPlacement,
     "Размещение: Page Down и Page Up — места, куда он встаёт, с Ctrl — комнаты, стрелки на клетку, Home — к "
     "симу, запятая и точка поворачивают, Enter ставит, Delete продаёт, Escape завершает."},
    {Str::KeysBuild,
     "Строительство: стрелки двигают угол, с Shift на пять, Page Up и Page Down — этаж, пробел отмечает начало, "
     "Enter строит от метки, Ctrl+Enter сносит, Home — к симу, R повторяет, Escape выход."},
    {Str::KeysNeighborhood,
     "Район: сначала список участков, Enter открывает панель участка. Чтобы заселить семью, выберите её в "
     "корзине семей и нажмите Enter на участке. Tab — панели."},
    // Control state
    {Str::Checked, "отмечено"},
    {Str::NotChecked, "не отмечено"},
    {Str::Selected, "выбрано"},
    {Str::TabSelected, "выбрана"},
    {Str::Unavailable, "недоступно"},
    {Str::UnlabeledButton, "кнопка без названия"},
    {Str::Unnamed, "без имени"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "кнопка"},
    {Str::RoleTab, "вкладка"},
    {Str::RoleCheckBox, "флажок"},
    {Str::RoleSlider, "ползунок"},
    {Str::RolePicker, "выбор значения"},
    {Str::NothingHere, "здесь нечего просматривать"},
    // Review buffers
    {Str::BufferDetails, "подробности"},
    {Str::BufferEvents, "события"},
    {Str::BufferEmpty, "%s, пусто"},
    {Str::PositionInGroup, "%d из %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, элементов: %d"},
    {Str::GroupSummaryOne, "%s, 1 элемент"},
    {Str::GroupButtons, "кнопки"},
    {Str::GroupText, "текст"},
    {Str::GroupControlPanel, "панель управления"},
    {Str::GroupLivePanel, "панель персонажа"},
    {Str::GroupFamily, "семья"},
    {Str::GroupWantsFears, "желания и страхи"},
    {Str::GroupNeeds, "потребности"},
    {Str::GroupControls, "элементы управления"},
    {Str::GroupBuyPanel, "режим покупки"},
    {Str::GroupBuildPanel, "режим строительства"},
    {Str::GroupCatalog, "каталог"},
    {Str::GroupStarSign, "знак зодиака"},
    {Str::GroupAspiration, "стремление"},
    {Str::GroupSteps, "шаги"},
    {Str::GroupAge, "возраст"},
    {Str::GroupSex, "пол"},
    {Str::GroupTraits, "черты"},
    // Text fields
    {Str::TextField, "%s %s, текстовое поле"},
    {Str::Blank, "пусто"},
    {Str::Space, "пробел"},
    // Values
    {Str::ValueOfMax, "%d из %d"},
    {Str::SliderRange, "от %1$s до %2$s"},
    {Str::RadioBetween, "между: %1$s и %2$s"},
    {Str::PointsLeft, "осталось очков: %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "следующая страница"},
    {Str::PreviousPage, "предыдущая страница"},
    // Interaction menu
    {Str::PieMenu, "действия"},
    {Str::NoInteractions, "нет действий"},
    {Str::Cancelled, "отменено"},
    // Cheat console
    {Str::CheatConsole, "консоль кодов"},
    {Str::CheatRan, "код: %s"},
    {Str::CheatEmpty, "сначала введите или выберите код"},
    // Neighborhood: lots and families
    {Str::GroupLots, "участки"},
    {Str::LotFamily, "семья %s"},
    {Str::LotForSale, "продаётся"},
    {Str::LotCommunity, "общественный участок"},
    {Str::LotApartmentsEmpty, "многоквартирный дом, без жильцов"},
    {Str::LotApartmentsWith, "многоквартирный дом: %s"},
    {Str::FamilySummary, "%1$s: членов семьи: %2$d, деньги %3$s"},
    // Money
    {Str::Simoleons, "симолеонов"},
    {Str::FundsNow, "деньги %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "персонажи"},
    {Str::CategoryOther, "другие предметы"},
    {Str::NoObjects, "здесь нет предметов"},
    {Str::FloorNumber, "этаж %d"},
    {Str::NotOnLot, "не на участке"},
    {Str::HereNow, "прямо здесь"},
    {Str::TilesAway, "%d клеток на %s"},
    {Str::OneTileAway, "1 клетка на %s"},
    {Str::DirNorth, "север"},
    {Str::DirNorthEast, "северо-восток"},
    {Str::DirEast, "восток"},
    {Str::DirSouthEast, "юго-восток"},
    {Str::DirSouth, "юг"},
    {Str::DirSouthWest, "юго-запад"},
    {Str::DirWest, "запад"},
    {Str::DirNorthWest, "северо-запад"},
    {Str::OtherFloor, "на другом этаже"},
    {Str::SelectedSimSuffix, "выбран"},
    {Str::NoInteractionsAtAll, "нет действий: с этим предметом ничего нельзя сделать"},
    {Str::WalkingThere, "идёт туда"},
    {Str::CannotWalk, "туда не пройти"},
    {Str::NoSimSelected, "персонаж не выбран"},
    // Needs
    {Str::MotiveValue, "%s, %d процентов"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "очередь действий"},
    {Str::StatusIdle, "ничего не делает"},
    {Str::StatusNeedsLow, "низкие потребности: %s"},
    {Str::StatusNeedsFine, "потребности в норме"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s закончил: %2$s"},
    {Str::NeedLow, "%1$s, низкая потребность: %2$s"},
    {Str::NeedUrgent, "%1$s, срочная потребность: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, выбран"},
    {Str::NoSuchSim, "нет сима %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s пришёл"},
    {Str::VisitorLeft, "%s ушёл"},
    // Lot: rooms
    {Str::RoomObjects, "%s, объектов: %d"},
    {Str::RoomObjectsOne, "%s, 1 объект"},
    {Str::FundsChanged, "%1$s, осталось %2$s"},
    // Camera
    {Str::CameraFree, "камера свободна"},
    {Str::CameraFollowing, "следит за: %s"},
    // Narrator
    {Str::NarratorAll, "рассказчик: всё"},
    {Str::NarratorWarnings, "рассказчик: только предупреждения"},
    {Str::NarratorOff, "рассказчик выключен"},
    // Relationships
    {Str::RelationshipValues, "%1$s, за день %2$s, за всю жизнь %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "здесь нет комнаты"},
    {Str::LayoutSummary, "%1$s, этаж %2$d, клеток: %3$d, от %4$d, %5$d до %6$d, %7$d"},
    {Str::LayoutOpenings, "дверей: %1$d, окон: %2$d"},
    {Str::LayoutDoor, "дверь %1$s, стена: %2$s, ведёт в: %3$s, клетка %4$d, %5$d"},
    {Str::LayoutWindow, "окно %1$s, стена: %2$s, выходит на: %3$s, клетка %4$d, %5$d"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Page Down ищет места, куда он встанет, стрелки двигают его, Enter ставит, Escape отменяет"},
    {Str::PlaceTile, "клетка %1$d, %2$d"},
    {Str::PlaceOffLot, "за пределами участка"},
    {Str::PlaceFits, "встаёт"},
    {Str::PlaceBlocked, "сюда не встаёт"},
    {Str::PlacePlaced, "%s поставлен"},
    {Str::PlaceFacing, "направление: %s"},
    {Str::PlaceNoSpot, "так повёрнутый он никуда на участке не встаёт"},
    {Str::PlaceRoomSpots, "%1$s, мест: %2$d"},
    {Str::PlaceWallSide, "стена: %s"},
    {Str::PlaceLeadsTo, "ведёт в: %s"},
    {Str::PlaceLeadsToAnother, "ведёт в другую комнату: %s"},
    {Str::PlaceMoving,
     "перемещение: %s. Page Down ищет места, Enter ставит, Delete продаёт, Escape возвращает на место"},
    {Str::PlaceCannotPick, "%s нельзя поднять"},
    {Str::PlaceSold, "%s продан"},
    // Build mode
    {Str::BuildBegin,
     "%s: стрелки двигают угол, пробел отмечает начало, Enter строит, Ctrl+Enter сносит, Escape выход"},
    {Str::BuildCorner, "угол %1$d, %2$d"},
    {Str::BuildMarked, "начало отмечено в %1$d, %2$d"},
    {Str::BuildNoMark, "сначала отметьте начало пробелом"},
    {Str::BuildDone, "построено за %1$s, осталось %2$s"},
    {Str::BuildRemoved, "снесено, возвращено %1$s, %2$s"},
    {Str::BuildNothing, "ничего не изменилось"},
    {Str::BalloonThinks, "%1$s думает: %2$s"},
    {Str::BalloonDreams, "%1$s видит сон: %2$s"},
    {Str::BalloonSays, "%1$s говорит: %2$s"},
    {Str::BalloonScreams, "%1$s кричит: %2$s"},
    {Str::BalloonScreamsNothing, "%s кричит"},
    {Str::BalloonCrossed, "не %s"},
    {Str::BalloonRelationUp, "%s: отношения улучшаются"},
    {Str::BalloonRelationUpMuch, "%s: отношения сильно улучшаются"},
    {Str::BalloonRelationDown, "%s: отношения ухудшаются"},
    {Str::BalloonRelationDownMuch, "%s: отношения сильно ухудшаются"},
    {Str::BalloonRomance, "%s: роман"},
    {Str::BalloonRomanceBroken, "%s: роман окончен"},
    {Str::BalloonCrush, "%s: влюблённость"},
    {Str::BalloonCrushBroken, "%s: влюблённость прошла"},
    {Str::BalloonMomentBad, "%s: плохой момент"},
    {Str::BalloonMomentGood, "%s: отличный момент"},
    {Str::BalloonFirstKiss, "%s: первый поцелуй"},
    {Str::BalloonFirstLove, "%s: первая любовь"},
    {Str::BalloonInLove, "%s: влюблён"},
    {Str::BalloonLoveLost, "%s: любовь потеряна"},
    {Str::BalloonFriend, "%s: новый друг"},
    {Str::BalloonFriendLost, "%s: друг потерян"},
    {Str::BalloonBestFriend, "%s: новый лучший друг"},
    {Str::BalloonBestFriendLost, "%s: лучший друг потерян"},
    {Str::BalloonEnemy, "%s: новый враг"},
    {Str::BalloonEnemyLost, "%s: больше не враги"},
    {Str::BalloonMemoryGood, "%s: хорошее воспоминание"},
    {Str::BalloonMemoryBad, "%s: плохое воспоминание"},
    {Str::BalloonTopicLiked, "%s: нравится тема"},
    {Str::BalloonTopicDisliked, "%s: не нравится тема"},
    {Str::BalloonPersonalityGood, "%s: характеры совпадают"},
    {Str::BalloonPersonalityBad, "%s: характеры не совпадают"},
};

}  // namespace

extern const Translations kRussian = {kEntries, std::size(kEntries)};

}  // namespace lang
