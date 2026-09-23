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
    {Str::KeysLot,
     "ที่ดิน: Page Down และ Page Up ไล่สิ่งของในที่ดิน ใกล้สุดก่อน กับ Shift คือหมวด กับ Ctrl คือห้อง Enter "
     "เปิดการกระทำของสิ่งของ หรือยกขึ้นในโหมดซื้อ Ctrl+Enter ให้ซิมที่เลือกเดินไปที่นั่น Ctrl+E สถานะซิม Ctrl+1 "
     "ถึง 9 เลือกซิม Tab สลับแผง Ctrl+ลูกศรขึ้นลง รายละเอียด"},
    {Str::KeysPlacement,
     "การวาง: Page Down และ Page Up ที่ที่วางได้ กับ Ctrl คือห้อง ลูกศรเลื่อนหนึ่งช่อง Home กลับไปที่ซิม "
     "จุลภาคและมหัพภาคหมุน Enter วาง Delete ขาย Escape เลิก"},
    {Str::KeysBuild,
     "การก่อสร้าง: ลูกศรเลื่อนมุม กับ Shift ห้าช่อง Page Up และ Page Down เปลี่ยนชั้น เว้นวรรคกำหนดจุดเริ่ม "
     "Enter สร้างจากจุดเริ่ม Ctrl+Enter รื้อ Home กลับไปที่ซิม R พูดซ้ำ Escape ออก"},
    {Str::KeysNeighborhood,
     "ละแวกบ้าน: รายชื่อที่ดินมาก่อน Enter เปิดแผงของที่ดิน หากจะย้ายครอบครัวเข้า "
     "ให้เลือกครอบครัวในถังครอบครัวแล้วกด Enter ที่ที่ดิน Tab สลับแผง"},
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
    {Str::GroupControlPanel, "แผงควบคุม"},
    {Str::GroupLivePanel, "แผงซิม"},
    {Str::GroupFamily, "ครอบครัว"},
    {Str::GroupWantsFears, "ความปรารถนาและความกลัว"},
    {Str::GroupNeeds, "ความต้องการ"},
    {Str::GroupControls, "ปุ่มควบคุม"},
    {Str::GroupBuyPanel, "โหมดซื้อ"},
    {Str::GroupBuildPanel, "โหมดก่อสร้าง"},
    {Str::GroupCatalog, "แคตตาล็อก"},
    {Str::GroupStarSign, "ราศี"},
    {Str::GroupAspiration, "ความปรารถนา"},
    {Str::GroupSteps, "ขั้นตอน"},
    {Str::GroupAge, "อายุ"},
    {Str::GroupSex, "เพศ"},
    // Text fields
    {Str::TextField, "%s %s, ช่องข้อความ"},
    {Str::Blank, "ว่าง"},
    {Str::Space, "เว้นวรรค"},
    // Values
    {Str::ValueOfMax, "%d จาก %d"},
    {Str::SliderRange, "จาก%1$sถึง%2$s"},
    {Str::PointsLeft, "เหลือ %d คะแนน"},
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
    // Neighborhood: lots and families
    {Str::GroupLots, "ที่ดิน"},
    {Str::LotFamily, "ครอบครัว %s"},
    {Str::LotForSale, "ประกาศขาย"},
    {Str::LotCommunity, "ที่ดินสาธารณะ"},
    {Str::LotApartmentsEmpty, "อาคารอพาร์ตเมนต์ ไม่มีผู้เช่า"},
    {Str::LotApartmentsWith, "อาคารอพาร์ตเมนต์: %s"},
    {Str::FamilySummary, "%1$s: สมาชิก %2$d คน, เงิน %3$s"},
    // Money
    {Str::Simoleons, "ซิโมเลียน"},
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
    // Needs
    {Str::MotiveValue, "%s, %d เปอร์เซ็นต์"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "คิวคำสั่ง"},
    {Str::StatusIdle, "ไม่ได้ทำอะไร"},
    {Str::StatusNeedsLow, "ความต้องการต่ำ: %s"},
    {Str::StatusNeedsFine, "ความต้องการปกติ"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s ทำเสร็จแล้ว: %2$s"},
    {Str::NeedLow, "%1$s, ความต้องการต่ำ: %2$s"},
    {Str::NeedUrgent, "%1$s, ความต้องการเร่งด่วน: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, ถูกเลือก"},
    {Str::NoSuchSim, "ไม่มีซิมคนที่ %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s มาถึงแล้ว"},
    {Str::VisitorLeft, "%s กลับไปแล้ว"},
    // Lot: rooms
    {Str::RoomObjects, "%s, วัตถุ %d ชิ้น"},
    {Str::RoomObjectsOne, "%s, วัตถุ 1 ชิ้น"},
    {Str::FundsChanged, "%1$s เหลือ %2$s"},
    // Camera
    {Str::CameraFree, "กล้องอิสระ"},
    {Str::CameraFollowing, "ติดตาม%s"},
    // Narrator
    {Str::NarratorAll, "ผู้บรรยาย: ทั้งหมด"},
    {Str::NarratorWarnings, "ผู้บรรยาย: เฉพาะคำเตือน"},
    {Str::NarratorOff, "ปิดผู้บรรยาย"},
    // Relationships
    {Str::RelationshipValues, "%1$s, รายวัน %2$s, ตลอดชีวิต %3$s"},
    // Buy mode
    {Str::PlaceBegin, "%s: Page Down หาที่ที่วางได้ ลูกศรเลื่อน Enter วาง Escape ยกเลิก"},
    {Str::PlaceTile, "ช่อง %1$d, %2$d"},
    {Str::PlaceOffLot, "นอกที่ดิน"},
    {Str::PlaceFits, "วางได้"},
    {Str::PlaceBlocked, "วางที่นี่ไม่ได้"},
    {Str::PlacePlaced, "วาง%sแล้ว"},
    {Str::PlaceFacing, "หันไปทาง%s"},
    {Str::PlaceNoSpot, "หันแบบนี้วางที่ไหนบนที่ดินไม่ได้เลย"},
    {Str::PlaceRoomSpots, "%1$s, %2$d ที่"},
    {Str::PlaceMoving, "ย้าย%s: Page Down หาที่ Enter วาง Delete ขาย Escape วางคืนที่เดิม"},
    {Str::PlaceCannotPick, "ยก%sไม่ได้"},
    {Str::PlaceSold, "ขาย%sแล้ว"},
    // Build mode
    {Str::BuildBegin, "%s: ลูกศรเลื่อนมุม เว้นวรรคกำหนดจุดเริ่ม Enter สร้าง Ctrl+Enter รื้อ Escape ออก"},
    {Str::BuildCorner, "มุม %1$d, %2$d"},
    {Str::BuildMarked, "กำหนดจุดเริ่มที่ %1$d, %2$d"},
    {Str::BuildNoMark, "กดเว้นวรรคกำหนดจุดเริ่มก่อน"},
    {Str::BuildDone, "สร้างแล้วราคา %1$s เหลือ %2$s"},
    {Str::BuildRemoved, "รื้อแล้ว ได้คืน %1$s, %2$s"},
    {Str::BuildNothing, "ไม่มีอะไรเปลี่ยน"},
};

}  // namespace

extern const Translations kThai = {kEntries, std::size(kEntries)};

}  // namespace lang
