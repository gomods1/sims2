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
    // Money
    {Str::FundsNow, "资金%s，%s"},
    {Str::FundsChanged, "%1$s，剩余%2$s"},
};

}  // namespace

extern const Translations kSimplifiedChinese = {kEntries, std::size(kEntries)};

}  // namespace lang
