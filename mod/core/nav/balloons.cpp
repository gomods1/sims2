#include "balloons.h"

#include <windows.h>

#include <algorithm>
#include <cctype>
#include <map>
#include <string>
#include <vector>

#include "../game/game_strings.h"
#include "../game/objects.h"
#include "../game/simantics.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
#include "live.h"

namespace nav::balloons {
namespace {

constexpr uint32_t kGlobalGroup = 0x7fd46cd0;
constexpr unsigned kSheetNames = 0x95;     // STR# of a balloon's icon texture sheets
constexpr unsigned kNeedNames = 130;       // Live.package: the needs panel's names
constexpr unsigned kInterestNames = 0x8c;  // Live.package: the interests (conversation topics)
constexpr unsigned kHobbyNames = 0xf2;     // Live.package: the hobbies (Free Time)
constexpr unsigned kAspirationNames = 0x8b;  // CAS.package: the aspirations

// A sheet's texture name, looked up as the game does: the tree's object file's own STR# 0x95, then
// its semi-global's, then the global one (the tree's group when the hook could not read the files).
// Lower case.
std::string SheetName(const game::Balloon& b) {
    if (b.sheet < 0) return "";
    uint32_t groups[] = {b.sheetGroup ? b.sheetGroup : b.group, b.semiGroup, kGlobalGroup};
    std::string name;
    for (uint32_t g : groups) {
        if (!g) continue;
        name = game::ObjectString(g, kSheetNames, b.sheet, true);
        if (!name.empty() && name != "-") break;
    }
    for (char& c : name)
        if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
    return name;
}

// A route failure: the misc sheet's crossed-out footsteps (the global "Route Failure Feedback").
bool IsRouteFailure(const std::string& sheet, int cell) {
    return cell == 8 && sheet.find("misc01") != std::string::npos;
}

// The global effects (STR# 0x8f of the global group, by index) the narrator knows.
enum class Kind { Thought, Scream, Talk, Headline };
struct Effect {
    int index;
    Kind kind;
    bool crossed;  // the icon is crossed out (the "_x" effects)
    Str headline;  // Kind::Headline: its line
};
const Effect kEffects[] = {
    {0x00, Kind::Headline, false, Str::BalloonRomance},
    {0x01, Kind::Headline, false, Str::BalloonRomanceBroken},
    {0x02, Kind::Headline, false, Str::BalloonCrush},
    {0x03, Kind::Headline, false, Str::BalloonCrushBroken},
    {0x04, Kind::Headline, false, Str::BalloonRelationUp},
    {0x05, Kind::Headline, false, Str::BalloonRelationUpMuch},
    {0x06, Kind::Headline, false, Str::BalloonRelationDown},
    {0x07, Kind::Headline, false, Str::BalloonRelationDownMuch},
    {0x08, Kind::Headline, false, Str::BalloonMomentBad},
    {0x09, Kind::Headline, false, Str::BalloonFirstKiss},
    {0x0a, Kind::Headline, false, Str::BalloonMomentGood},
    {0x0b, Kind::Headline, false, Str::BalloonFirstLove},
    {0x10, Kind::Headline, false, Str::BalloonInLove},
    {0x11, Kind::Headline, false, Str::BalloonLoveLost},
    {0x12, Kind::Headline, false, Str::BalloonCrush},
    {0x13, Kind::Headline, false, Str::BalloonCrushBroken},
    {0x14, Kind::Headline, false, Str::BalloonFriend},
    {0x15, Kind::Headline, false, Str::BalloonFriendLost},
    {0x16, Kind::Headline, false, Str::BalloonBestFriend},
    {0x17, Kind::Headline, false, Str::BalloonBestFriendLost},
    {0x18, Kind::Headline, false, Str::BalloonEnemy},
    {0x19, Kind::Headline, false, Str::BalloonEnemyLost},
    {0x27, Kind::Thought, false, Str::BalloonThinks},
    {0x28, Kind::Thought, true, Str::BalloonThinks},
    {0x2b, Kind::Talk, false, Str::BalloonSays},
    {0x2c, Kind::Talk, false, Str::BalloonSays},
    {0x2d, Kind::Talk, true, Str::BalloonSays},
    {0x2e, Kind::Talk, true, Str::BalloonSays},
    {0xc3, Kind::Headline, false, Str::BalloonMemoryGood},
    {0xc4, Kind::Headline, false, Str::BalloonMemoryGood},
    {0xc5, Kind::Headline, false, Str::BalloonMemoryBad},
    {0xc6, Kind::Headline, false, Str::BalloonMemoryBad},
    {0xc9, Kind::Headline, false, Str::BalloonTopicLiked},
    {0xca, Kind::Headline, false, Str::BalloonTopicLiked},
    {0xcb, Kind::Headline, false, Str::BalloonTopicDisliked},
    {0xcc, Kind::Headline, false, Str::BalloonTopicDisliked},
    {0xd2, Kind::Scream, false, Str::BalloonScreams},
    {0xd3, Kind::Scream, true, Str::BalloonScreams},
    {0xd8, Kind::Talk, false, Str::BalloonSays},
    {0xd9, Kind::Talk, false, Str::BalloonSays},
    {0xda, Kind::Talk, true, Str::BalloonSays},
    {0xdb, Kind::Talk, true, Str::BalloonSays},
    {0xdc, Kind::Thought, false, Str::BalloonThinks},
    {0xde, Kind::Thought, true, Str::BalloonThinks},
    {0xe0, Kind::Scream, false, Str::BalloonScreams},
    {0xe2, Kind::Scream, true, Str::BalloonScreams},
    {0xe3, Kind::Thought, false, Str::BalloonDreams},
    {0xe4, Kind::Thought, true, Str::BalloonDreams},
    {0xe5, Kind::Thought, false, Str::BalloonDreams},
    {0xe6, Kind::Thought, true, Str::BalloonDreams},
    {0xe7, Kind::Headline, false, Str::BalloonPersonalityGood},
    {0xe8, Kind::Headline, false, Str::BalloonPersonalityGood},
    {0xe9, Kind::Headline, false, Str::BalloonPersonalityBad},
    {0xea, Kind::Headline, false, Str::BalloonPersonalityBad},
};

const Effect* FindEffect(int index) {
    for (const Effect& e : kEffects)
        if (e.index == index) return &e;
    return nullptr;
}

// The needs sheets (uieffects-headlines-motives-thought / -scream): cell -> Live.package STR# 130
// entry (1 energy, 2 bladder, 3 hygiene, 4 environment, 5 fun, 6 comfort, 7 hunger, 8 social,
// 9 a baby bottle: hunger).
// (STR# 130: 1 hunger, 3 energy, 5 comfort, 7 fun, 9 hygiene, 11 social, 13 bladder, 15 environment.)
const int kNeedCells[] = {-1, 3, 13, 9, 15, 7, 5, 1, 11, 1};

// What a sheet's cell pictures, in the game's words; empty when the mod does not know it.
// name: the sheet's texture name (SheetName).
std::string IconName(const std::string& name, int cell) {
    // The food objects' sheet (their semi-global's): what a meal lacks, with an exclamation mark
    // (a Sim told to cook without a counter thinks of one).
    if (name.find("foodchainicons") != std::string::npos) {
        static const Str kFood[] = {Str::IconNeedOven, Str::IconNeedStove, Str::IconNeedGrill,
                                    Str::IconNeedCounter, Str::IconNeedToasterOven, Str::IconNeedMicrowave};
        return cell >= 0 && cell < 6 ? S(kFood[cell]) : "";
    }
    if (name.find("motives-thought") != std::string::npos || name.find("motives-scream") != std::string::npos) {
        if (cell >= 1 && cell < (int)(sizeof kNeedCells / sizeof kNeedCells[0]))
            return game::GameString("Live.package", kNeedNames, kNeedCells[cell]);
        return "";
    }
    // The six shared conversation sheets hold the 18 interests, five icons each, in the order of
    // the interests panel (Live.package STR# 0x8c).
    size_t at = name.find("conversationshared0");
    if (at != std::string::npos && at + 19 < name.size()) {
        int n = name[at + 19] - '1';
        if (n >= 0 && n < 6 && cell >= 0 && cell < 15)
            return util::CleanForSpeech(game::GameString("Live.package", kInterestNames, n * 3 + cell / 5));
    }
    // Free Time's hobby sheets (the game's misspelled "converstaion-<hobby>"; some objects have
    // their own): the whole sheet is one hobby, named as the hobby panel does (STR# 0xf2).
    static const struct {
        const char* word;
        int hobby;
    } kHobbies[] = {{"food", 0},       {"artandcrafts", 1}, {"filmandliterature", 2}, {"literature", 2},
                    {"sports", 3},     {"games", 4},        {"nature", 5},            {"birdwatching", 5},
                    {"engineering", 6}, {"fitness", 7},     {"science", 8},           {"stars", 8},
                    {"planets", 8},    {"consteallations", 8}, {"music", 9}};
    // The aspirations sheet: cells 0-6 blue, 8-14 the same in red; named as Create a Sim does
    // (CAS.package STR# 0x8b).
    if (name.find("conversation-aspirations01") != std::string::npos) {
        static const int kAspirations[] = {5, -1, 2, 4, 6, 7, 3, -1};
        int a = cell >= 0 && cell < 16 ? kAspirations[cell % 8] : -1;
        return a < 0 ? "" : util::CleanForSpeech(game::GameString("CAS.package", kAspirationNames, a));
    }
    // Nightlife's sheets: the star signs (CAS.package STR# 0x8a, in order), the turn-ons (UIText
    // STR# 0xe1: sheet 01 is entries 0-13, 02 the hair colors 14-19, 03 robots, PlantSims and
    // zombies), money ($, $$, $$$: the Money interest).
    if (name.find("conversation-zodiac01") != std::string::npos)
        return cell >= 0 && cell < 12 ? util::CleanForSpeech(game::GameString("CAS.package", 0x8a, cell)) : "";
    if (name.find("conversation-turnonoff0") != std::string::npos) {
        char n = name[name.find("turnonoff0") + 10];
        int t = n == '1' ? (cell < 14 ? cell : -1) : n == '2' ? (cell < 6 ? 14 + cell : -1)
              : n == '3' ? (cell == 0 ? 30 : cell == 1 ? 31 : cell == 2 ? 29 : -1) : -1;
        return t < 0 || cell < 0 ? "" : util::CleanForSpeech(game::GameString("UIText.package", 0xe1, t));
    }
    if (name.find("conversation-wealth01") != std::string::npos)
        return util::CleanForSpeech(game::GameString("Live.package", kInterestNames, 1));
    // Pets' sheets: the pet interests (Live.package STR# 0x8c, 21-29), five icons each: 01 toys,
    // weather, animals; 02 outside, food, playtime; 03 scary things, pets, sleeping.
    size_t pets = name.find("petsconversation0");
    if (pets != std::string::npos && cell >= 0 && cell < 15) {
        static const int kPetTopics[3][3] = {{28, 27, 29}, {24, 23, 26}, {22, 21, 25}};
        int n = name[pets + 17] - '1';
        if (n >= 0 && n < 3) return util::CleanForSpeech(game::GameString("Live.package", kInterestNames, kPetTopics[n][cell / 5]));
    }
    // The misc sheet: pictures the mod names itself (8, footsteps crossed out, is a route failure,
    // said on its own line).
    if (name.find("conversation-misc01") != std::string::npos) {
        static const Str kMisc[] = {Str::IconAmbulance, Str::IconPhone,   Str::IconPoolLadder, Str::IconQuestion,
                                    Str::IconFingerprint, Str::IconBaby, Str::IconDoor,       Str::IconBabyQuestion};
        return cell >= 0 && cell < 8 ? S(kMisc[cell]) : "";
    }
    // University's professor sheet: cells 0-11 the majors, in the order of MajorGlobals' BCON
    // 0x2001 (each Major's attribute 0, which "Major - Get Major" returns for the topic), named by
    // each Major object's catalog name; 15 a brain, a zombie's thought (found 2026-11-10 in the
    // BHAVs, STRUCTS.md "SimAntics"; not yet seen live).
    if (name.find("conversation-professor01") != std::string::npos) {
        static const uint32_t kMajors[] = {0x8e97bf1d, 0x2e9cf007, 0x4e9cf02b, 0x4e9cf04d, 0xee9cf044, 0x2e9cf074,
                                           0xce9cf085, 0xee9cf08d, 0x2e9cf057, 0xae9cf063, 0x4e9cf06d, 0xce9cf07c};
        if (cell >= 0 && cell < 12) return util::CleanForSpeech(game::CatalogName(kMajors[cell]));
        return cell == 15 ? S(Str::IconBrain) : "";
    }
    // Nightlife's job sheet: only cell 15 is ever used ("Social - Whats Your Job", for a Sim without
    // a job), the Sim panel's "Unemployed" (Live.package STR# 0x88 7); a Sim with a job shows the
    // job's own thumbnail, named by its GUID.
    if (name.find("conversation-job01") != std::string::npos)
        return cell == 15 ? util::CleanForSpeech(game::GameString("Live.package", 0x88, 7)) : "";
    // The second aspirations sheet: Pleasure and Grilled Cheese (+8 red), Live.package STR# 0x91.
    if (name.find("aspirations02") != std::string::npos) {
        int a = cell % 8;
        return cell >= 0 && cell < 16 && a < 2 ? util::CleanForSpeech(game::GameString("Live.package", 0x91, 39 + a)) : "";
    }
    at = name.find("converstaion-");
    if (at != std::string::npos) {
        std::string word = name.substr(at + 13);
        for (const auto& h : kHobbies)
            if (word == h.word) return util::CleanForSpeech(game::GameString("Live.package", kHobbyNames, h.hobby));
    }
    return "";
}

// A memory's or a want's name keeps the game's tokens ("$Subject se casa"): the balloon does not say
// who, so "$Subject" is "someone" and other tokens go.
std::string FillTokens(std::string s) {
    for (size_t at = s.find('$'); at != std::string::npos; at = s.find('$', at)) {
        size_t end = at + 1;
        while (end < s.size() && (isalnum((unsigned char)s[end]) || s[end] == ':' || s[end] == '_')) end++;
        std::string token = s.substr(at + 1, end - at - 1);
        std::string with = token.rfind("Subject", 0) == 0 ? S(Str::BalloonSomeone) : "";
        s.replace(at, end - at, with);
        at += with.size();
    }
    std::string out;
    for (char c : s)
        if (c != ' ' || (!out.empty() && out.back() != ' ')) out += c;
    while (!out.empty() && out.back() == ' ') out.pop_back();
    return out;
}

std::map<std::string, ULONGLONG> g_said;  // "object|line" -> when it was said

bool InFamily(const std::vector<int>& family, int id) {
    return id && std::find(family.begin(), family.end(), id) != family.end();
}

// Whether a Sim from outside the family is busy with one of the family: the tree's runner or its
// stack object (a social runs on one Sim with the other as its stack object), or the selected
// Sim's running action done with that Sim.
bool WithFamily(const game::Balloon& b, const std::vector<int>& family) {
    if (InFamily(family, b.runner) || InFamily(family, b.stackObject)) return true;
    return game::SelectedSimId() && InFamily(family, game::SelectedSimId()) &&
           live::RunningActionWith() == b.objectId;
}

// Whose balloons are spoken (Ctrl+Shift+N chooses for conversations): the family's thoughts and
// screams, anyone's screams, every thought with the scope "everyone"; conversations (talk
// balloons, relationship headlines) by the scope. A route failure is always news. The others are
// only kept in the events buffer.
bool Speak(Kind kind, bool family, bool withFamily, bool routeFailure) {
    if (routeFailure && family) return true;
    int scope = live::SocialScope();
    if (kind == Kind::Thought || kind == Kind::Scream)
        return family || kind == Kind::Scream || scope == live::kSocialEveryone;
    switch (scope) {
    case live::kSocialEveryone: return true;
    case live::kSocialWithFamily: return family || withFamily;
    case live::kSocialFamily: return family;
    }
    return false;
}

}  // namespace

void Tick() {
    game::Balloon got[32];
    int n = game::TakeBalloons(got, 32);
    if (!n) return;
    ULONGLONG now = GetTickCount64();
    std::vector<int> family = game::FamilySimIds();
    for (int i = 0; i < n; i++) {
        const game::Balloon& b = got[i];
        if (b.mode != 0 && b.mode != 0xf) continue;  // starts only: 2-4 stop effects, 1 and 3 refresh them
        LogVerbose("balloon: over %d, effect 0x%x, flags %04x, group %08x (sheets %08x, %08x), sheet %d, icon %d%s, "
                   "thing %d, guid %08x",
                   b.objectId, b.effect, b.flags, b.group, b.sheetGroup, b.semiGroup, b.sheet, b.iconValue,
                   b.iconObject ? " (a Sim)" : "", b.iconThing, b.iconGuid);
        const Effect* e = FindEffect(b.effect);
        if (!e) continue;
        std::string who, last;
        if (!game::SimName(b.objectId, &who, &last)) continue;  // over an object
        std::string icon, sheet = SheetName(b);
        if (b.iconObject) {
            game::NeighborName(b.iconValue, &icon, nullptr);
        } else if (b.sheet >= 0) {
            icon = IconName(sheet, b.iconValue);
        } else if (b.iconGuid) {
            icon = game::CatalogName(b.iconGuid);
        } else if (b.iconThing > 0) {
            std::string first, last;
            game::LotObject o;
            if (game::SimName(b.iconThing, &first, &last)) icon = first;
            else if (game::ReadLotObject(b.iconThing, &o)) icon = !o.catalogName.empty() ? o.catalogName : o.name;
            if (!icon.empty() && icon[0] == '*') icon.clear();  // an internal part ("*Head Slots Holder")
        }
        if (icon.find('$') != std::string::npos) icon = FillTokens(icon);
        if (!icon.empty() && icon == who) continue;  // the Sim's own face ("Carmen dice: Carmen") says nothing
        std::string line;
        bool routeFailure = IsRouteFailure(sheet, b.iconValue);
        if (routeFailure) {
            line = util::Format(S(Str::BalloonRouteFailed), who.c_str());
        } else if (e->kind == Kind::Headline) {
            line = util::Format(S(e->headline), who.c_str());
        } else if (!icon.empty()) {
            // A conversation's balloons are its topics, not its words (a tester: "habla de Dinero",
            // not "dice: Dinero"); a crossed-out topic is a complaint about it.
            Str form = e->headline;
            if (e->kind == Kind::Talk && e->crossed) form = Str::BalloonTalksAgainst;
            else if (e->crossed) icon = util::Format(S(Str::BalloonCrossed), icon.c_str());
            line = util::Format(S(form), who.c_str(), icon.c_str());
        } else if (e->kind == Kind::Scream) {
            line = util::Format(S(Str::BalloonScreamsNothing), who.c_str());
        } else {
            LogVerbose("balloon: no name for sheet %d cell %d (%s)", b.sheet, b.iconValue, sheet.c_str());
            continue;  // a topic the mod cannot name
        }
        LogVerbose("balloon line: %s", line.c_str());
        // The same line over the same Sim once in 15 s in a conversation (it cycles its topics),
        // once a minute for a thought (a Sim keeps thinking of what it wants).
        std::string key = std::to_string(b.objectId) + "|" + line;
        auto said = g_said.find(key);
        ULONGLONG quiet = e->kind == Kind::Talk ? 15000 : 60000;
        if (said != g_said.end() && now - said->second < quiet) continue;
        g_said[key] = now;
        // The small relationship steps each social gives are only kept in the events buffer.
        bool small = b.effect == 0x04 || b.effect == 0x06;
        bool inFamily = InFamily(family, b.objectId);
        bool withFamily = !inFamily && WithFamily(b, family);
        LogVerbose("balloon scope: family %d, with family %d (runner %d, stack object %d)", inFamily, withFamily, b.runner,
                   b.stackObject);
        if (!small && Speak(e->kind, inFamily, withFamily, routeFailure))
            live::Narrate(line, e->kind == Kind::Scream);
        else
            buffers::Record(line);
    }
    if (g_said.size() > 200) g_said.clear();
}

}  // namespace nav::balloons
