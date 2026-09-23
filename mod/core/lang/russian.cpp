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
    {Str::GroupControls, "элементы управления"},
    // Text fields
    {Str::TextField, "%s %s, текстовое поле"},
    {Str::Blank, "пусто"},
    {Str::Space, "пробел"},
    // Values
    {Str::ValueOfMax, "%d из %d"},
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
    // Money
    {Str::FundsNow, "деньги %s, %s"},
    {Str::FundsChanged, "%1$s, осталось %2$s"},
};

}  // namespace

extern const Translations kRussian = {kEntries, std::size(kEntries)};

}  // namespace lang
