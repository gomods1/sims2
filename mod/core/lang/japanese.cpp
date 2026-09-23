// Japanese: the game's language 14. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "ザ・シムズ2 アクセシビリティを読み込みました"},
    {Str::UnsupportedBuild, "ザ・シムズ2 アクセシビリティ: 対応していないゲームのバージョンです。モッドを無効にしました"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "矢印キーでグループ内を移動、タブとシフト+タブでグループを切り替え、ホームとエンドで端へ移動、エンターで押す、Rで繰り返します。コントロール+上矢印とコントロール+下矢印で、今読んだものの詳細を読み、コントロール+"
     "左矢印とコントロール+右矢印で、詳細とイベントを切り替えます。"},
    // Control state
    {Str::Checked, "チェック"},
    {Str::NotChecked, "チェックなし"},
    {Str::Selected, "選択中"},
    {Str::TabSelected, "選択中"},
    {Str::Unavailable, "利用不可"},
    {Str::UnlabeledButton, "名前のないボタン"},
    {Str::Unnamed, "名前なし"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "ボタン"},
    {Str::RoleTab, "タブ"},
    {Str::RoleCheckBox, "チェックボックス"},
    {Str::RoleSlider, "スライダー"},
    {Str::RolePicker, "セレクター"},
    {Str::NothingHere, "ここには移動できる項目がありません"},
    // Review buffers
    {Str::BufferDetails, "詳細"},
    {Str::BufferEvents, "イベント"},
    {Str::BufferEmpty, "%s、項目なし"},
    {Str::PositionInGroup, "%2$d件中%1$d件目、%3$s"},
    // Groups
    {Str::GroupSummary, "%s、%d項目"},
    {Str::GroupSummaryOne, "%s、1項目"},
    {Str::GroupButtons, "ボタン"},
    {Str::GroupText, "テキスト"},
    {Str::GroupControls, "コントロールパネル"},
    // Text fields
    {Str::TextField, "%s %s、入力欄"},
    {Str::Blank, "空欄"},
    {Str::Space, "スペース"},
    // Values
    {Str::ValueOfMax, "%2$d段階中%1$d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "次のページ"},
    {Str::PreviousPage, "前のページ"},
    // Interaction menu
    {Str::PieMenu, "アクション"},
    {Str::NoInteractions, "アクションなし"},
    {Str::Cancelled, "キャンセルしました"},
    // Cheat console
    {Str::CheatConsole, "チートコンソール"},
    {Str::CheatRan, "チート: %s"},
    {Str::CheatEmpty, "先にチートを入力するか選んでください"},
    // Money
    {Str::FundsNow, "予算 %s、%s"},
    {Str::FundsChanged, "%1$s、残り%2$s"},
};

}  // namespace

extern const Translations kJapanese = {kEntries, std::size(kEntries)};

}  // namespace lang
