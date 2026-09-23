// Traditional Chinese: the game's language 18. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "模擬市民2無障礙模組已載入"},
    {Str::UnsupportedBuild, "模擬市民2無障礙模組：不支援的遊戲版本，模組已停用"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "方向鍵在群組內移動，Tab和Shift+Tab切換群組，Home和End移到兩端，Enter執行，R重複朗讀。Ctrl+上方向鍵和Ctrl+下方向鍵朗讀剛才所說內容的詳細資料，Ctrl+左方向鍵和Ctrl+右方向鍵"
     "在詳細資料和事件之間切換。"},
    // Control state
    {Str::Checked, "已核取"},
    {Str::NotChecked, "未核取"},
    {Str::Selected, "已選取"},
    {Str::TabSelected, "已選取"},
    {Str::Unavailable, "無法使用"},
    {Str::UnlabeledButton, "未命名按鈕"},
    {Str::Unnamed, "未命名"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "按鈕"},
    {Str::RoleTab, "頁籤"},
    {Str::RoleCheckBox, "核取方塊"},
    {Str::RoleSlider, "滑桿"},
    {Str::RolePicker, "選擇器"},
    {Str::NothingHere, "這裡沒有可瀏覽的項目"},
    // Review buffers
    {Str::BufferDetails, "詳細資料"},
    {Str::BufferEvents, "事件"},
    {Str::BufferEmpty, "%s，空的"},
    {Str::PositionInGroup, "第%d項，共%d項，%s"},
    // Groups
    {Str::GroupSummary, "%s，%d項"},
    {Str::GroupSummaryOne, "%s，1項"},
    {Str::GroupButtons, "按鈕"},
    {Str::GroupText, "文字"},
    {Str::GroupControlPanel, "控制面板"},
    {Str::GroupLivePanel, "市民面板"},
    {Str::GroupFamily, "家庭"},
    {Str::GroupWantsFears, "願望與恐懼"},
    {Str::GroupNeeds, "需求"},
    {Str::GroupControls, "控制按鈕"},
    {Str::GroupBuyPanel, "購買模式"},
    {Str::GroupBuildPanel, "建造模式"},
    {Str::GroupCatalog, "目錄"},
    {Str::GroupStarSign, "星座"},
    {Str::GroupAspiration, "志向"},
    {Str::GroupSteps, "步驟"},
    {Str::GroupAge, "年齡"},
    {Str::GroupSex, "性別"},
    // Text fields
    {Str::TextField, "%s %s，文字方塊"},
    {Str::Blank, "空白"},
    {Str::Space, "空白"},
    // Values
    {Str::ValueOfMax, "%d，最大%d"},
    {Str::SliderRange, "從%1$s到%2$s"},
    {Str::PointsLeft, "剩餘 %d 點"},
    {Str::LabelValue, "%s：%s"},
    {Str::NextPage, "下一頁"},
    {Str::PreviousPage, "前一頁"},
    // Interaction menu
    {Str::PieMenu, "互動"},
    {Str::NoInteractions, "沒有互動"},
    {Str::Cancelled, "已取消"},
    // Cheat console
    {Str::CheatConsole, "密技控制台"},
    {Str::CheatRan, "密技：%s"},
    {Str::CheatEmpty, "請先輸入或選擇一個密技"},
    // Neighborhood: lots and families
    {Str::GroupLots, "地段"},
    {Str::LotFamily, "%s家"},
    {Str::LotForSale, "待售"},
    {Str::LotCommunity, "公共地段"},
    {Str::LotApartmentsEmpty, "公寓大樓，無住戶"},
    {Str::LotApartmentsWith, "公寓大樓：%s"},
    {Str::FamilySummary, "%1$s：%2$d名成員，資金%3$s"},
    // Money
    {Str::Simoleons, "西莫幣"},
    {Str::FundsNow, "資金%s，%s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "模擬人物"},
    {Str::CategoryOther, "其他物品"},
    {Str::NoObjects, "這裡沒有物品"},
    {Str::FloorNumber, "%d樓"},
    {Str::NotOnLot, "不在區塊上"},
    {Str::HereNow, "就在這裡"},
    {Str::TilesAway, "%2$s方%1$d格"},
    {Str::OneTileAway, "%s方1格"},
    {Str::DirNorth, "北"},
    {Str::DirNorthEast, "東北"},
    {Str::DirEast, "東"},
    {Str::DirSouthEast, "東南"},
    {Str::DirSouth, "南"},
    {Str::DirSouthWest, "西南"},
    {Str::DirWest, "西"},
    {Str::DirNorthWest, "西北"},
    {Str::OtherFloor, "在另一層樓"},
    {Str::SelectedSimSuffix, "已選取"},
    {Str::NoInteractionsAtAll, "沒有互動：這個物品不能做任何事"},
    {Str::WalkingThere, "正在走過去"},
    {Str::CannotWalk, "無法走到那裡"},
    {Str::NoSimSelected, "未選取模擬人物"},
    // Needs
    {Str::MotiveValue, "%s，百分之%d"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "動作佇列"},
    {Str::StatusIdle, "無所事事"},
    {Str::StatusNeedsLow, "需求偏低：%s"},
    {Str::StatusNeedsFine, "需求良好"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s：%2$s"},
    {Str::ActionFinished, "%1$s完成了：%2$s"},
    {Str::NeedLow, "%1$s，需求偏低：%2$s"},
    {Str::NeedUrgent, "%1$s，需求緊急：%2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s，已選取"},
    {Str::NoSuchSim, "沒有第%d個模擬人物"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s來了"},
    {Str::VisitorLeft, "%s走了"},
    // Lot: rooms
    {Str::RoomObjects, "%s，%d件物品"},
    {Str::RoomObjectsOne, "%s，1件物品"},
    {Str::FundsChanged, "%1$s，剩餘%2$s"},
    // Buy mode
    {Str::PlaceBegin, "%s：Page Down 尋找放得下的位置，方向鍵移動，Enter 放下，Escape 取消"},
    {Str::PlaceTile, "格子 %1$d，%2$d"},
    {Str::PlaceOffLot, "在地段外"},
    {Str::PlaceFits, "放得下"},
    {Str::PlaceBlocked, "這裡放不下"},
    {Str::PlacePlaced, "已放置%s"},
    {Str::PlaceFacing, "朝%s"},
    {Str::PlaceNoSpot, "照這個方向，地段上哪裡都放不下"},
    {Str::PlaceRoomSpots, "%1$s，%2$d 個位置"},
    {Str::PlaceMoving, "移動%s：Page Down 尋找位置，Enter 放下，Delete 賣掉，Escape 放回原處"},
    {Str::PlaceCannotPick, "%s無法拿起"},
    {Str::PlaceSold, "已賣掉%s"},
    // Build mode
    {Str::BuildBegin, "%s：方向鍵移動角點，空白鍵標記起點，Enter 建造，Ctrl+Enter 拆除，Escape 離開"},
    {Str::BuildCorner, "角點 %1$d，%2$d"},
    {Str::BuildMarked, "起點標記在 %1$d，%2$d"},
    {Str::BuildNoMark, "請先用空白鍵標記起點"},
    {Str::BuildDone, "花費 %1$s 建成，剩餘 %2$s"},
    {Str::BuildRemoved, "已拆除，退回 %1$s，%2$s"},
    {Str::BuildNothing, "沒有變化"},
};

}  // namespace

extern const Translations kTraditionalChinese = {kEntries, std::size(kEntries)};

}  // namespace lang
