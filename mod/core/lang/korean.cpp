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
    {Str::KeysLot,
     "부지: Page Down과 Page Up으로 부지의 물건, 가까운 것부터. Shift와 함께 카테고리, Ctrl과 함께 방. Enter로 물건의 행동을 열고, 구매 모드에서는 집어 듭니다. "
     "Ctrl+Enter로 선택한 심을 그곳으로 보냅니다. Ctrl+E 심의 상태, Ctrl+1~9 심 선택, Tab 패널, Ctrl+위아래 화살표 세부 정보."},
    {Str::KeysPlacement,
     "배치: Page Down과 Page Up으로 놓을 수 있는 자리, Ctrl과 함께 방, 화살표로 한 칸, Home으로 심에게, 쉼표와 마침표로 회전, Enter로 놓기, Delete로 팔기, "
     "Escape로 종료."},
    {Str::KeysBuild,
     "건축: 화살표로 모서리 이동, Shift와 함께 다섯 칸, Page Up과 Page Down으로 층, 스페이스로 시작점, Enter로 시작점부터 건설, Ctrl+Enter로 철거, "
     "Home으로 심에게, R 반복, Escape 종료."},
    {Str::KeysNeighborhood,
     "동네: 먼저 부지 목록, Enter로 부지 패널을 엽니다. 가족을 이사시키려면 가족 보관함에서 가족을 고르고 부지에서 Enter를 누르세요. Tab 패널."},
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
    {Str::GroupBuildPanel, "건축 모드"},
    {Str::GroupCatalog, "카탈로그"},
    {Str::GroupStarSign, "별자리"},
    {Str::GroupAspiration, "열망"},
    {Str::GroupSteps, "단계"},
    {Str::GroupAge, "나이"},
    {Str::GroupSex, "성별"},
    {Str::GroupTraits, "특성"},
    // Text fields
    {Str::TextField, "%s %s, 편집창"},
    {Str::Blank, "빈칸"},
    {Str::Space, "스페이스"},
    // Values
    {Str::ValueOfMax, "%2$d 중 %1$d"},
    {Str::SliderRange, "%1$s에서 %2$s"},
    {Str::RadioBetween, "%1$s와 %2$s 사이"},
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
    // Camera
    {Str::CameraFree, "카메라 자유"},
    {Str::CameraFollowing, "%s 따라가기"},
    // Narrator
    {Str::NarratorAll, "해설: 모두"},
    {Str::NarratorWarnings, "해설: 경고만"},
    {Str::NarratorOff, "해설 끔"},
    // Relationships
    {Str::RelationshipValues, "%1$s, 일일 %2$s, 평생 %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "여기에는 방이 없음"},
    {Str::LayoutSummary, "%1$s, %2$d층, %3$d칸, %4$d, %5$d부터 %6$d, %7$d까지"},
    {Str::LayoutOpenings, "문 %1$d개, 창문 %2$d개"},
    {Str::LayoutDoor, "문 %1$s, %2$s 벽, %3$s(으)로 통함, 칸 %4$d, %5$d"},
    {Str::LayoutWindow, "창문 %1$s, %2$s 벽, %3$s 쪽, 칸 %4$d, %5$d"},
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
    {Str::PlaceWallSide, "%s 벽"},
    {Str::PlaceLeadsTo, "%s(으)로 통함"},
    {Str::PlaceLeadsToAnother, "다른 %s(으)로 통함"},
    {Str::PlaceMoving, "%s 옮기기: Page Down으로 자리 찾기, Enter로 놓기, Delete로 팔기, Escape로 제자리에"},
    {Str::PlaceCannotPick, "%s 들 수 없음"},
    {Str::PlaceSold, "%s 팔았음"},
    // Build mode
    {Str::BuildBegin, "%s: 화살표로 모서리 이동, 스페이스로 시작점 표시, Enter로 건설, Ctrl+Enter로 철거, Escape로 종료"},
    {Str::BuildCorner, "모서리 %1$d, %2$d"},
    {Str::BuildMarked, "시작점 %1$d, %2$d 표시됨"},
    {Str::BuildNoMark, "먼저 스페이스로 시작점을 표시하세요"},
    {Str::BuildDone, "%1$s에 건설, 남은 돈 %2$s"},
    {Str::BuildRemoved, "철거함, %1$s 돌려받음, %2$s"},
    {Str::BuildNothing, "바뀐 것 없음"},
    {Str::BalloonThinks, "%1$s의 생각: %2$s"},
    {Str::BalloonDreams, "%1$s의 꿈: %2$s"},
    {Str::BalloonSays, "%1$s의 말: %2$s"},
    {Str::BalloonScreams, "%1$s의 외침: %2$s"},
    {Str::BalloonScreamsNothing, "%s 외침"},
    {Str::BalloonCrossed, "%s 싫음"},
    {Str::BalloonRelationUp, "%s: 관계 좋아짐"},
    {Str::BalloonRelationUpMuch, "%s: 관계 크게 좋아짐"},
    {Str::BalloonRelationDown, "%s: 관계 나빠짐"},
    {Str::BalloonRelationDownMuch, "%s: 관계 크게 나빠짐"},
    {Str::BalloonRomance, "%s: 로맨스"},
    {Str::BalloonRomanceBroken, "%s: 로맨스 끝"},
    {Str::BalloonCrush, "%s: 짝사랑"},
    {Str::BalloonCrushBroken, "%s: 짝사랑 끝"},
    {Str::BalloonMomentBad, "%s: 나쁜 순간"},
    {Str::BalloonMomentGood, "%s: 멋진 순간"},
    {Str::BalloonFirstKiss, "%s: 첫 키스"},
    {Str::BalloonFirstLove, "%s: 첫사랑"},
    {Str::BalloonInLove, "%s: 사랑에 빠짐"},
    {Str::BalloonLoveLost, "%s: 사랑을 잃음"},
    {Str::BalloonFriend, "%s: 새 친구"},
    {Str::BalloonFriendLost, "%s: 친구를 잃음"},
    {Str::BalloonBestFriend, "%s: 새 절친"},
    {Str::BalloonBestFriendLost, "%s: 절친을 잃음"},
    {Str::BalloonEnemy, "%s: 새 적"},
    {Str::BalloonEnemyLost, "%s: 더 이상 적이 아님"},
    {Str::BalloonMemoryGood, "%s: 좋은 추억"},
    {Str::BalloonMemoryBad, "%s: 나쁜 추억"},
    {Str::BalloonTopicLiked, "%s: 이 주제를 좋아함"},
    {Str::BalloonTopicDisliked, "%s: 이 주제를 싫어함"},
    {Str::BalloonPersonalityGood, "%s: 성격이 잘 맞음"},
    {Str::BalloonPersonalityBad, "%s: 성격이 충돌함"},
    {Str::BalloonRouteFailed, "%s 갈 수 없음"},
    {Str::IconAmbulance, "구급차"},
    {Str::IconPhone, "전화"},
    {Str::IconPoolLadder, "수영장 사다리"},
    {Str::IconQuestion, "물음표"},
    {Str::IconFingerprint, "지문"},
    {Str::IconBaby, "아기"},
    {Str::IconDoor, "문"},
    {Str::IconBabyQuestion, "아기?"},
    {Str::FamilyTreePicked, "%s 선택됨: 다른 심에서 Enter로 관계 연결, 다시 Enter로 내려놓기"},
};

}  // namespace

extern const Translations kKorean = {kEntries, std::size(kEntries)};

}  // namespace lang
