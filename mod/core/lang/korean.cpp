// Korean: the game's language 15. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "심즈 2 접근성 기능을 불러왔습니다"},
    {Str::UnsupportedBuild, "심즈 2 접근성: 지원하지 않는 게임 버전이므로 모드를 껐습니다"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "화살표로 그룹 안을 이동하고, 탭과 시프트+탭으로 그룹을 바꾸고, 홈과 엔드로 처음과 끝으로 가고, 엔터로 누르고, R로 다시 읽습니다. 컨트롤+위쪽 화살표와 컨트롤+아래쪽 화살표로 방금 "
     "읽은 것의 세부 정보를 읽고, 컨트롤+왼쪽 화살표와 컨트롤+오른쪽 화살표로 세부 정보와 이벤트를 전환합니다."},
    // Control state
    {Str::Checked, "체크됨"},
    {Str::NotChecked, "체크 안 됨"},
    {Str::Selected, "선택됨"},
    {Str::TabSelected, "선택됨"},
    {Str::Unavailable, "사용 불가"},
    {Str::UnlabeledButton, "이름 없는 버튼"},
    {Str::Unnamed, "이름 없음"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "버튼"},
    {Str::RoleTab, "탭"},
    {Str::RoleCheckBox, "체크 박스"},
    {Str::RoleSlider, "슬라이더"},
    {Str::RolePicker, "선택기"},
    {Str::NothingHere, "여기에는 이동할 항목이 없습니다"},
    // Review buffers
    {Str::BufferDetails, "세부 정보"},
    {Str::BufferEvents, "이벤트"},
    {Str::BufferEmpty, "%s, 비어 있음"},
    {Str::PositionInGroup, "%2$d개 중 %1$d번째, %3$s"},
    // Groups
    {Str::GroupSummary, "%s, 항목 %d개"},
    {Str::GroupSummaryOne, "%s, 항목 1개"},
    {Str::GroupButtons, "버튼"},
    {Str::GroupText, "텍스트"},
    {Str::GroupControlPanel, "제어판"},
    {Str::GroupLivePanel, "심 패널"},
    {Str::GroupFamily, "가족"},
    {Str::GroupWantsFears, "소망과 두려움"},
    {Str::GroupNeeds, "욕구"},
    {Str::GroupControls, "제어판"},
    {Str::GroupBuyPanel, "구매 모드"},
    {Str::GroupCatalog, "카탈로그"},
    {Str::GroupStarSign, "별자리"},
    {Str::GroupAspiration, "열망"},
    {Str::GroupSteps, "단계"},
    {Str::GroupAge, "나이"},
    {Str::GroupSex, "성별"},
    // Text fields
    {Str::TextField, "%s %s, 편집창"},
    {Str::Blank, "빈칸"},
    {Str::Space, "스페이스"},
    // Values
    {Str::ValueOfMax, "%2$d 중 %1$d"},
    {Str::SliderRange, "%1$s에서 %2$s"},
    {Str::PointsLeft, "남은 포인트 %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "다음 페이지"},
    {Str::PreviousPage, "이전 페이지"},
    // Interaction menu
    {Str::PieMenu, "상호작용"},
    {Str::NoInteractions, "상호작용 없음"},
    {Str::Cancelled, "취소됨"},
    // Cheat console
    {Str::CheatConsole, "치트 콘솔"},
    {Str::CheatRan, "치트: %s"},
    {Str::CheatEmpty, "먼저 치트를 입력하거나 고르세요"},
    // Neighborhood: lots and families
    {Str::GroupLots, "부지"},
    {Str::LotFamily, "%s 가족"},
    {Str::LotForSale, "매물"},
    {Str::LotCommunity, "공공 부지"},
    {Str::LotApartmentsEmpty, "아파트 건물, 입주자 없음"},
    {Str::LotApartmentsWith, "아파트 건물: %s"},
    {Str::FamilySummary, "%1$s: 구성원 %2$d명, 자금 %3$s"},
    // Money
    {Str::Simoleons, "시몰레온"},
    {Str::FundsNow, "자금 %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "심즈"},
    {Str::CategoryOther, "다른 물건"},
    {Str::NoObjects, "여기에는 물건이 없습니다"},
    {Str::FloorNumber, "%d층"},
    {Str::NotOnLot, "부지에 있지 않습니다"},
    {Str::HereNow, "바로 여기"},
    {Str::TilesAway, "%2$s으로 %1$d칸"},
    {Str::OneTileAway, "%s으로 1칸"},
    {Str::DirNorth, "북쪽"},
    {Str::DirNorthEast, "북동쪽"},
    {Str::DirEast, "동쪽"},
    {Str::DirSouthEast, "남동쪽"},
    {Str::DirSouth, "남쪽"},
    {Str::DirSouthWest, "남서쪽"},
    {Str::DirWest, "서쪽"},
    {Str::DirNorthWest, "북서쪽"},
    {Str::OtherFloor, "다른 층"},
    {Str::SelectedSimSuffix, "선택됨"},
    {Str::NoInteractionsAtAll, "상호작용 없음: 이 물건으로는 아무것도 할 수 없습니다"},
    {Str::WalkingThere, "그곳으로 가는 중"},
    {Str::CannotWalk, "그곳으로 갈 수 없습니다"},
    {Str::NoSimSelected, "선택된 심 없음"},
    // Needs
    {Str::MotiveValue, "%s, %d퍼센트"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "행동 대기열"},
    {Str::StatusIdle, "아무것도 안 함"},
    {Str::StatusNeedsLow, "낮은 욕구: %s"},
    {Str::StatusNeedsFine, "욕구 양호"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s, 끝냄: %2$s"},
    {Str::NeedLow, "%1$s, 낮은 욕구: %2$s"},
    {Str::NeedUrgent, "%1$s, 급한 욕구: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, 선택됨"},
    {Str::NoSuchSim, "%d번째 심 없음"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s 도착"},
    {Str::VisitorLeft, "%s 떠남"},
    // Lot: rooms
    {Str::RoomObjects, "%s, 물건 %d개"},
    {Str::RoomObjectsOne, "%s, 물건 1개"},
    {Str::FundsChanged, "%1$s, 남은 돈 %2$s"},
    // Buy mode
    {Str::PlaceBegin, "%s: Page Down으로 놓을 수 있는 자리 찾기, 화살표로 이동, Enter로 놓기, Escape로 취소"},
    {Str::PlaceTile, "칸 %1$d, %2$d"},
    {Str::PlaceOffLot, "부지 밖"},
    {Str::PlaceFits, "놓을 수 있음"},
    {Str::PlaceBlocked, "여기에는 놓을 수 없음"},
    {Str::PlacePlaced, "%s 놓음"},
    {Str::PlaceFacing, "방향: %s"},
    {Str::PlaceNoSpot, "이 방향으로는 부지 어디에도 놓을 수 없음"},
    {Str::PlaceRoomSpots, "%1$s, 자리 %2$d개"},
    {Str::PlaceMoving, "%s 옮기기: Page Down으로 자리 찾기, Enter로 놓기, Delete로 팔기, Escape로 제자리에"},
    {Str::PlaceCannotPick, "%s 들 수 없음"},
    {Str::PlaceSold, "%s 팔았음"},
};

}  // namespace

extern const Translations kKorean = {kEntries, std::size(kEntries)};

}  // namespace lang
