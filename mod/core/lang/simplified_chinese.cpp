// Simplified Chinese: the game's language 17. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "模拟人生2无障碍模组已加载"},
    {Str::UnsupportedBuild, "模拟人生2无障碍模组：不支持的游戏版本，模组已停用"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "方向键在组内移动，Tab和Shift+Tab切换组，Home和End跳到两端，回车激活，R重复朗读。Ctrl+上方向键和Ctrl+下方向键朗读刚才所说内容的详情，Ctrl+左方向键和Ctrl+右方向键在详情和事件之"
     "间切换。"},
    // Control state
    {Str::Checked, "已勾选"},
    {Str::NotChecked, "未勾选"},
    {Str::Selected, "已选中"},
    {Str::TabSelected, "已选中"},
    {Str::Unavailable, "不可用"},
    {Str::UnlabeledButton, "未命名按钮"},
    {Str::Unnamed, "未命名"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "按钮"},
    {Str::RoleTab, "选项卡"},
    {Str::RoleCheckBox, "复选框"},
    {Str::RoleSlider, "滑块"},
    {Str::RolePicker, "选择器"},
    {Str::NothingHere, "这里没有可浏览的内容"},
    // Review buffers
    {Str::BufferDetails, "详情"},
    {Str::BufferEvents, "事件"},
    {Str::BufferEmpty, "%s，空"},
    {Str::PositionInGroup, "第%d项，共%d项，%s"},
    // Groups
    {Str::GroupSummary, "%s，%d项"},
    {Str::GroupSummaryOne, "%s，1项"},
    {Str::GroupButtons, "按钮"},
    {Str::GroupText, "文本"},
    {Str::GroupControlPanel, "控制面板"},
    {Str::GroupLivePanel, "市民面板"},
    {Str::GroupFamily, "家庭"},
    {Str::GroupWantsFears, "愿望与恐惧"},
    {Str::GroupNeeds, "需求"},
    {Str::GroupControls, "控制按钮"},
    // Text fields
    {Str::TextField, "%s %s，文本框"},
    {Str::Blank, "空白"},
    {Str::Space, "空格"},
    // Values
    {Str::ValueOfMax, "%d，最大%d"},
    {Str::LabelValue, "%s：%s"},
    {Str::NextPage, "下一页"},
    {Str::PreviousPage, "前一页"},
    // Interaction menu
    {Str::PieMenu, "互动"},
    {Str::NoInteractions, "没有互动"},
    {Str::Cancelled, "已取消"},
    // Cheat console
    {Str::CheatConsole, "秘籍控制台"},
    {Str::CheatRan, "秘籍：%s"},
    {Str::CheatEmpty, "请先输入或选择一个秘籍"},
    // Neighborhood: lots and families
    {Str::GroupLots, "地块"},
    {Str::LotFamily, "%s家"},
    {Str::LotForSale, "待售"},
    {Str::LotCommunity, "公共地块"},
    {Str::LotApartmentsEmpty, "公寓楼，无住户"},
    {Str::LotApartmentsWith, "公寓楼：%s"},
    {Str::FamilySummary, "%1$s：%2$d名成员，资金%3$s"},
    // Money
    {Str::Simoleons, "西莫币"},
    {Str::FundsNow, "资金%s，%s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "模拟市民"},
    {Str::CategoryOther, "其他物品"},
    {Str::NoObjects, "这里没有物品"},
    {Str::FloorNumber, "%d楼"},
    {Str::NotOnLot, "不在地块上"},
    {Str::HereNow, "就在这里"},
    {Str::TilesAway, "%2$s方%1$d格"},
    {Str::OneTileAway, "%s方1格"},
    {Str::DirNorth, "北"},
    {Str::DirNorthEast, "东北"},
    {Str::DirEast, "东"},
    {Str::DirSouthEast, "东南"},
    {Str::DirSouth, "南"},
    {Str::DirSouthWest, "西南"},
    {Str::DirWest, "西"},
    {Str::DirNorthWest, "西北"},
    {Str::OtherFloor, "在另一层楼"},
    {Str::SelectedSimSuffix, "已选中"},
    {Str::NoInteractionsAtAll, "没有互动：这个物品不能做任何事"},
    {Str::WalkingThere, "正在走过去"},
    {Str::CannotWalk, "无法走到那里"},
    {Str::NoSimSelected, "未选中模拟市民"},
    // Needs
    {Str::MotiveValue, "%s，百分之%d"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "动作队列"},
    {Str::StatusIdle, "无所事事"},
    {Str::StatusNeedsLow, "需求偏低：%s"},
    {Str::StatusNeedsFine, "需求良好"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s：%2$s"},
    {Str::ActionFinished, "%1$s完成了：%2$s"},
    {Str::NeedLow, "%1$s，需求偏低：%2$s"},
    {Str::NeedUrgent, "%1$s，需求紧急：%2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s，已选中"},
    {Str::NoSuchSim, "没有第%d个模拟市民"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s来了"},
    {Str::VisitorLeft, "%s走了"},
    // Lot: rooms
    {Str::RoomObjects, "%s，%d件物品"},
    {Str::RoomObjectsOne, "%s，1件物品"},
    {Str::FundsChanged, "%1$s，剩余%2$s"},
};

}  // namespace

extern const Translations kSimplifiedChinese = {kEntries, std::size(kEntries)};

}  // namespace lang
