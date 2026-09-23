// Thai: the game's language 21. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "โหลดม็อดการเข้าถึงเดอะซิมส์ 2 แล้ว"},
    {Str::UnsupportedBuild, "ม็อดการเข้าถึงเดอะซิมส์ 2: ไม่รองรับเกมเวอร์ชันนี้, ปิดม็อดแล้ว"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "ปุ่มลูกศรเลื่อนไปภายในกลุ่ม, Tab และ Shift+Tab เปลี่ยนกลุ่ม, Home และ End ไปที่รายการแรกและรายการสุดท้าย, "
     "Enter กดปุ่ม, R พูดซ้ำ, Control+ลูกศรขึ้น และ Control+ลูกศรลง อ่านรายละเอียดของสิ่งที่เพิ่งพูด, "
     "Control+ลูกศรซ้าย และ Control+ลูกศรขวา สลับระหว่างรายละเอียดกับเหตุการณ์"},
    // Control state
    {Str::Checked, "กาเครื่องหมายแล้ว"},
    {Str::NotChecked, "ไม่ได้กาเครื่องหมาย"},
    {Str::Selected, "ถูกเลือก"},
    {Str::TabSelected, "ถูกเลือก"},
    {Str::Unavailable, "ไม่พร้อมใช้งาน"},
    {Str::UnlabeledButton, "ปุ่มไม่มีชื่อ"},
    {Str::Unnamed, "ไม่มีชื่อ"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "ปุ่ม"},
    {Str::RoleTab, "แท็บ"},
    {Str::RoleCheckBox, "กล่องกาเครื่องหมาย"},
    {Str::RoleSlider, "แถบเลื่อน"},
    {Str::RolePicker, "ตัวเลือกค่า"},
    {Str::NothingHere, "ไม่มีอะไรให้ไล่ดูที่นี่"},
    // Review buffers
    {Str::BufferDetails, "รายละเอียด"},
    {Str::BufferEvents, "เหตุการณ์"},
    {Str::BufferEmpty, "%s, ว่างเปล่า"},
    {Str::PositionInGroup, "%d จาก %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d รายการ"},
    {Str::GroupSummaryOne, "%s, 1 รายการ"},
    {Str::GroupButtons, "ปุ่ม"},
    {Str::GroupText, "ข้อความ"},
    {Str::GroupControls, "ปุ่มควบคุม"},
    // Text fields
    {Str::TextField, "%s %s, ช่องข้อความ"},
    {Str::Blank, "ว่าง"},
    {Str::Space, "เว้นวรรค"},
    // Values
    {Str::ValueOfMax, "%d จาก %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "หน้าถัดไป"},
    {Str::PreviousPage, "หน้าก่อนหน้า"},
    // Interaction menu
    {Str::PieMenu, "การโต้ตอบ"},
    {Str::NoInteractions, "ไม่มีการโต้ตอบ"},
    {Str::Cancelled, "ยกเลิกแล้ว"},
    // Cheat console
    {Str::CheatConsole, "คอนโซลสูตรโกง"},
    {Str::CheatRan, "สูตรโกง: %s"},
    {Str::CheatEmpty, "พิมพ์หรือเลือกสูตรโกงก่อน"},
    // Money
    {Str::FundsNow, "เงินสำรอง %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "ชาวซิม"},
    {Str::CategoryOther, "วัตถุอื่นๆ"},
    {Str::NoObjects, "ไม่มีวัตถุที่นี่"},
    {Str::FloorNumber, "ชั้น %d"},
    {Str::NotOnLot, "ไม่ได้อยู่ในที่ดิน"},
    {Str::HereNow, "อยู่ตรงนี้"},
    {Str::TilesAway, "ห่าง %d ช่อง ทางทิศ%s"},
    {Str::OneTileAway, "ห่าง 1 ช่อง ทางทิศ%s"},
    {Str::DirNorth, "เหนือ"},
    {Str::DirNorthEast, "ตะวันออกเฉียงเหนือ"},
    {Str::DirEast, "ตะวันออก"},
    {Str::DirSouthEast, "ตะวันออกเฉียงใต้"},
    {Str::DirSouth, "ใต้"},
    {Str::DirSouthWest, "ตะวันตกเฉียงใต้"},
    {Str::DirWest, "ตะวันตก"},
    {Str::DirNorthWest, "ตะวันตกเฉียงเหนือ"},
    {Str::OtherFloor, "อยู่คนละชั้น"},
    {Str::SelectedSimSuffix, "ถูกเลือก"},
    {Str::NoInteractionsAtAll, "ไม่มีการกระทำ: วัตถุนี้ทำอะไรไม่ได้"},
    {Str::WalkingThere, "กำลังเดินไปที่นั่น"},
    {Str::CannotWalk, "เดินไปที่นั่นไม่ได้"},
    {Str::NoSimSelected, "ไม่ได้เลือกซิม"},
    {Str::FundsChanged, "%1$s เหลือ %2$s"},
};

}  // namespace

extern const Translations kThai = {kEntries, std::size(kEntries)};

}  // namespace lang
