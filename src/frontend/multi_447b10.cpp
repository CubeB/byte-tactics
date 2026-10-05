// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by space-bunny-free, rewritten by claude-opus-5-5, finished by claude-opus-5-5, finished by GPT-6, finished by claude-opus-5-5. Names are provisional.
// Battle room button handler: per player slot LOGO, PLAYER, SIDE, ALLY,
// TEAMICONS, RES and READY, then PREVMENU, MESSAGE, COMMANDER, LOSTYPE,
// WATCHING, CHEATING, FIXEDLOC, MAPPING, START, GAMEOPEN, RESTRICTIONS and
// MAP/MAPNAME, and finally FUN_004ab0a0 on the gadget.
//
// What the match needed, in the order it was found:
// - <windows.h> and real Player/Game structs: the loop head builds the player
//   pointer from the spilled i*331 the way the original does.
// - Functions of this file with no callers, defined above unannotated and
//   inlined: 0x440c10 (free slot search, in PLAYER; it needs `inline`, MSVC
//   does not inline its two loops on its own), 0x440cd0 (map check, in READY)
//   and 0x446f50 (colour cycle, in TEAMICONS; `player` declared before
//   `colour`, which still matches 0x446f50 out of line). START's team test is
//   the CountAlliance helper of 0x446a50 with the flag byte read once, as in
//   0x4478b0.
// - The text buffer is 249 or 250 bytes (rounded to 252 in the frame): with
//   the inlined 0x440c10's used[10] that puts used[] above the text, as in the
//   original frame (frame order is references per byte).
// - g_game+0x2bee is one 1-bit unsigned short bitfield (`dirty`). In the tail
//   MSVC hoists the constant 1 into ebp, which gives the original's
//   `or word ptr [..],bp` next to the plain `or byte ptr [..],1` ones.
// - The slot loop as `while (1) { ...; i++; if (i >= 10) break; }`: the plain
//   for loop gives the tail's registers to the wrong values.
// - The map check's version test is `if (major >= 2) check = 1; else if
//   (major == 1 && minor >= 2) check = 1;`, as 0x448c70 needs too.
// - `goto done;` on START's "no map selected" path (93.8% to 99.0%). It emits
//   nothing, but with it the gadget is kept in esi only for the chain of
//   button tests and the final FUN_004ab0a0 reloads it from the stack.
// - The MAP/MAPNAME test written into a local (`hit = MAP; if (!hit) hit =
//   MAPNAME; if (hit)`), which took it from 99.2% to MATCH (#5533). Every
//   spelling of the test as one condition (`||`, `!A && !B` with a goto,
//   `?:`, a do/while around the body) left the body in the constant 1's ebp
//   region (`push ebp`, or a second `mov ebp,1` at 99.0%), and a local
//   assigned once from the `||` keeps a test of the materialised value
//   (99.0%). The earlier passes' C2 traces of that region (FUN_00438f79) are
//   in the git history of this file.
// - SetType is Class_00463c60's method in data/symbols.csv, so it is
//   called through a cast of the player pointer, as 0x445450 does.
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct PlayerInfo_00447b10 {
    char unknown_0[0x94];
    char kind;                          // +0x94
    unsigned char side;                 // +0x95
    unsigned char slot;                 // +0x96
    unsigned char f97_0 : 1;            // +0x97
    unsigned char f97_rest : 7;
    char unknown_98[0x9b - 0x98];
    union {
        unsigned char flags_9b;         // +0x9b
        struct {
            unsigned short low : 4;
            unsigned short started : 1;
            unsigned short ready : 1;
            unsigned short bit6 : 1;
            unsigned short watching : 1;
            unsigned short mapping : 1;
            unsigned short los : 1;
            unsigned short losType : 1;
            unsigned short commander : 2;
            unsigned short cheating : 1;
            unsigned short fixedloc : 1;
            unsigned short closed : 1;
        } b;
    };
    unsigned char f9d_0 : 2;            // +0x9d
    unsigned char f9d_2 : 1;
    unsigned char f9d_rest : 5;
    char unknown_9e[0xa7 - 0x9e];
    unsigned char versionMajor;         // +0xa7
    unsigned char versionMinor;         // +0xa8
    unsigned int mapCrc;                // +0xa9
};

struct Player_00447b10 {                // 0x14b bytes
    int active;                         // +0x00
    int id;                             // +0x04
    unsigned int time;                  // +0x08
    char unknown_c[0x27 - 0xc];
    PlayerInfo_00447b10* info;          // +0x27
    char name[0x73 - 0x2b];             // +0x2b
    char type;                          // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char ally[0xb];            // +0x108
    unsigned char ally2[0xb];           // +0x113
    char unknown_11e[0x13f - 0x11e];
    unsigned char colour;               // +0x13f
    int field_140;                      // +0x140
    short field_144;                    // +0x144
    unsigned char field_146;            // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Entry_00447b10 {
    char unknown_0[0xb6];
    char text[0xcc - 0xb6];             // +0xb6
    char label[0x15b - 0xcc];           // +0xcc
};

struct Layer_00447b10 {
    int unknown_0;
    Entry_00447b10* entries;            // +0x4
    char unknown_8[0x20 - 0x8];
    int field_20;                       // +0x20
};

struct Gadget_00447b10 {
    char unknown_0[0x8];
    void (__stdcall* handler)(void*);   // +0x8
    char unknown_c[0x18 - 0xc];
    Layer_00447b10* table;              // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                       // +0x60
};

struct Options_00447b10 {
    char unknown_0[0x118];
    int fixedloc;                       // +0x118
};
#pragma pack(pop)

class Class_00463c60 {
public:
    void SetType(int state);
};
class Class_004358f0 {
public:
    int FUN_004358f0();
};
class Class_00435c40 {
public:
    bool FUN_00435c40();
};
class Class_004373a0 {
public:
    unsigned int FUN_004373a0();
};
class UnitSync {
public:
    char* GetSyncStatusText();
};
class PacketManager {
public:
    int SendAllQueued(int value);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x499];
    int field_499;                      // +0x499
    char unknown_49d[0x519 - 0x49d];
    char gui[0x531 - 0x519];            // +0x519
    Layer_00447b10* table;              // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_00447b10 players[10];        // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Options_00447b10* options;          // +0x29a0
    char unknown_29a4[0x2a30 - 0x29a4];
    UnitSync* net;                      // +0x2a30
    char unknown_2a34[0x2a3c - 0x2a34];
    unsigned short field_2a3c;          // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43;
    unsigned char flags_2a44;           // +0x2a44
    char unknown_2a45[0x2a9b - 0x2a45];
    void* list;                         // +0x2a9b
    char unknown_2a9f[0x2bc0 - 0x2a9f];
    char state;                         // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    unsigned short dirty : 1;           // +0x2bee
    unsigned short dirty_rest : 15;
    char unknown_2bf0[0x37eee - 0x2bf0];
    int field_37eee;                    // +0x37eee
    char unknown_37ef2[0x37f39 - 0x37ef2];
    int sides;                          // +0x37f39
    char unknown_37f3d[0x391e9 - 0x37f3d];
    Class_004358f0* map;                // +0x391e9
    char unknown_391ed[0x39229 - 0x391ed];
    int commander;                      // +0x39229
    int mapping;                        // +0x3922d
    int los;                            // +0x39231
    int losType;                        // +0x39235
};
#pragma pack(pop)

extern Game* g_game;
extern int g_usePacketManager;
extern int DAT_00512994;
extern PacketManager g_packetManager;

void __cdecl FUN_004d85a0(void* data);
void FUN_00430f00();
void FUN_00444a20();
void __stdcall FUN_00444ba0(void* gadget);
void FUN_00444ea0();
void __stdcall FUN_00446080(int index);
void FUN_00446310();
void FUN_00446a50();
void FUN_00446c70();
void __stdcall FUN_00446e90(Player_00447b10* player);
void FUN_0044c7e0();
void BroadcastPlayerInfo();
void UpdateNetGameInfo();
int __stdcall CreateLocalPlayer(unsigned char player, int state);
void __stdcall RequestPlayerColor(int slot);
void __stdcall SetAlliance(int a, int b, unsigned char allied, int d);
void __stdcall FUN_00452bd0(Player_00447b10* player);
void __stdcall RejectPlayer(int id, unsigned char msg);
unsigned char FindHostSlot();
int IsHostLocal();
int CountHumanPlayers();
int CountComputerPlayers();
int CountLocalComputerPlayers();
unsigned int GetTicks();
void __stdcall AddMessage(char* text, int a, int b, int c);
void __stdcall SendChatMessage(Player_00447b10* p, char* text, int a, int b);
void __stdcall ReportGameEvent(int sound);
void __stdcall FUN_0047f1a0(char* sound, int b);
int __stdcall FUN_004288d0(char* name, int a, int b, int c);
void __stdcall FUN_0049fb10(char* gui, int value);
int __stdcall FUN_0049fd60(Gadget_00447b10* gadget, char* name);
int __stdcall FUN_0049fdf0(Entry_00447b10* entries, char* name, int type);
Entry_00447b10* __stdcall FUN_004a0010(Entry_00447b10* entries, char* name);
int __stdcall FUN_004a0f30(char* gui, int index);
int __stdcall FUN_004a0f60(Gadget_00447b10* gadget, char* name);
int __stdcall FUN_004a1080(Gadget_00447b10* gadget, char* name, int value);
void __stdcall FUN_004a1110(char* gui, char* name, int value);
void __stdcall FUN_004a5f40(Gadget_00447b10* gadget, int value);
void __stdcall FUN_004a7190(char* gui, int index);
void __stdcall FUN_004a81e0(char* gui, int value);
Gadget_00447b10* __stdcall FUN_004aa8f0(char* gui, char* name, int flags);
void __stdcall FUN_004ab0a0(void* gadget);
void __stdcall FUN_004abd90(void* gadget, char* text, int a, int b, int c);
char* __stdcall FUN_004c5740(char* text);

static inline int IsPlaying_00447b10(Player_00447b10* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted_00447b10(Player_00447b10* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

static inline int IsLocalHuman_00447b10(Player_00447b10* p)
{
    return p->active != 0 && p->type == 1;
}

static inline int IsRemoteHuman_00447b10(Player_00447b10* p)
{
    return p->active != 0 && p->type == 3 && p->info->kind == 1;
}

static inline int IsLocal_00447b10(Player_00447b10* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2);
}

static inline int CountAlliance_00447b10(int alliance)
{
    if (alliance == 5)
        return 0;
    int count = 0;
    unsigned char f = (g_game->flags_2a44 >> 2) & 1;
    for (int j = 0; j < 10; j++) {
        Player_00447b10* q = &g_game->players[j];
        if (f) {
            if (q->colour == alliance && IsPlaying_00447b10(q) && IsCounted_00447b10(q))
                count++;
        } else {
            if (q->colour == alliance && IsPlaying_00447b10(q))
                count++;
        }
    }
    return count;
}

// The free slot search at 0x440c10, which has no callers. MSVC inlines it only
// when it is declared inline (it has two loops).
inline int FindUnusedLogo()
{
    int used[10];
    memset(used, 0, sizeof(used));
    for (int i = 0; i < 10; i++) {
        Player_00447b10* p = &g_game->players[i];
        if (p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->field_146 != 10)
            used[p->info->slot < 9 ? p->info->slot : 9] = 1;
    }
    int result = 0;
    for (int j = 0; j < 10; j++) {
        if (!used[j]) {
            result = j;
            break;
        }
    }
    return result;
}

// The map check at 0x440cd0, which has no callers: /Ob2 inlined it.
int CheckMapCrc()
{
    if (!g_game->map->FUN_004358f0()) {
        return 0;
    }
    unsigned char me = FindHostSlot();
    PlayerInfo_00447b10* data = 0;
    int check = 0;
    if (me != 10) {
        data = g_game->players[me].info;
        if (data->versionMajor >= 2)
            check = 1;
        else if (data->versionMajor == 1 && data->versionMinor >= 2)
            check = 1;
    }
    if (!check) {
        return 1;
    }
    if (((Class_004373a0*)g_game->map)->FUN_004373a0() != data->mapCrc)
        return 0;
    return 1;
}

// The colour cycle at 0x446f50, which has no callers: /Ob2 inlined it.
void __stdcall FUN_00446f50(int index)
{
    Player_00447b10* player = &g_game->players[index];
    int colour = g_game->players[index].colour;
    FUN_00446e90(player);
    player->colour = (colour + 1) % 6;
    FUN_00452bd0(player);
    FUN_00446c70();
    FUN_00446a50();
}

// FUNCTION: 0x447b10
void __stdcall FUN_00447b10(Gadget_00447b10* gadget)
{
    char text[250];
    Entry_00447b10* entries = gadget->table->entries;

    if (gadget->field_60 == -1) {
        FUN_004d85a0(g_game->list);
        g_game->list = 0;
        DAT_00512994 = 0;
        FUN_00446c70();
        return;
    }

    int lp = g_game->localPlayer;
    Player_00447b10* me = &g_game->players[lp];
    int canAdd = IsHostLocal();
    int i = 0;
    while (1) {
        Player_00447b10* p = &g_game->players[i];

        sprintf(text, "LOGO%d", i);
        if (FUN_0049fd60(gadget, text) && IsLocal_00447b10(p)) {
            FUN_0047f1a0("Multi", 0);
            RequestPlayerColor(p->info->slot + 1);
            g_game->dirty = 1;
            BroadcastPlayerInfo();
        }

        sprintf(text, "PLAYER%d", i);
        if (FUN_0049fd60(gadget, text) && i != lp) {
            FUN_0047f1a0("Multi", 0);
            char type = p->type;
            if (type == 0 && canAdd) {
                ((Class_00463c60*)p)->SetType(4);
                p->id = -1;
                g_game->field_499--;
            } else if (type != 4 && type != 0) {
                if (p->active != 0 && type == 2 && GetTicks() - p->time > 30) {
                    RejectPlayer(p->id, 1);
                    ((Class_00463c60*)p)->SetType(0);
                } else if (canAdd && p->active != 0 && p->type == 3) {
                    FUN_00446080(i);
                }
            } else {
                if (type == 4) {
                    ((Class_00463c60*)p)->SetType(0);
                    g_game->field_499++;
                    UpdateNetGameInfo();
                }
                if (g_game->players[FindHostSlot()].info->b.closed) {
                    FUN_004abd90(g_game->gui, FUN_004c5740("Can't add another player when game is closed."), 500, 1, 1);
                    ((Class_00463c60*)p)->SetType(0);
                    g_game->dirty = 1;
                    break;
                }
                if (g_game->players[FindHostSlot()].info->b.commander != 2 && !CountLocalComputerPlayers()) {
                    CreateLocalPlayer(i, 2);
                    p->info->slot = FindUnusedLogo();
                }
            }
            g_game->dirty = 1;
            UpdateNetGameInfo();
            BroadcastPlayerInfo();
        }

        sprintf(text, "SIDE%d", i);
        if (FUN_0049fd60(gadget, text)) {
            FUN_0047f1a0("Multi", 0);
            if (p->active != 0 && p->info->b.bit6) {
                p->info->b.bit6 = 0;
                p->info->side = 0;
            } else {
                p->info->side++;
                if (p->info->side >= g_game->sides) {
                    p->info->side = 0;
                    if (g_game->players[FindHostSlot()].info->b.watching
                        && p->active != 0 && p->type == 1) {
                        p->info->b.bit6 = 1;
                    } else {
                        FUN_004a1080(gadget, text, 0);
                        FUN_004a5f40(gadget, gadget->field_60);
                    }
                }
            }
            g_game->dirty = 1;
            ReportGameEvent(4);
            BroadcastPlayerInfo();
        }

        sprintf(text, "ALLY%d", i);
        if (FUN_0049fd60(gadget, text)) {
            me->ally[i] ^= 1;
            SetAlliance(me->id, p->id, me->ally[i], 0);
            char same;
            if (me->colour == 5)
                same = 0;
            else
                same = me->colour == p->colour;
            if (same) {
                FUN_00446e90(me);
                me->colour = 5;
                FUN_00452bd0(me);
            }
            // Original bug (docs/bugs.md): `<<` binds tighter than `==` and
            // `==` tighter than `|`, so this is ((ally2 << 1) == 3) | ally,
            // and the left side is never true.
            if (me->ally2[i] << 1 == 3 | me->ally[i])
                FUN_0047f1a0("Ally", 0);
            else
                FUN_0047f1a0("Multi", 0);
            sprintf(text, " %s %s",
                    FUN_004c5740(me->ally[i] ? "allied with" : "broke alliance with"),
                    g_game->players[i].name);
            SendChatMessage(me, text, 4, 0);
            g_game->dirty = 1;
            BroadcastPlayerInfo();
        }

        sprintf(text, "TEAMICONS%d", i);
        if (FUN_0049fd60(gadget, text)) {
            FUN_0047f1a0("Ally", 0);
            FUN_00446f50(i);
            FUN_00452bd0(p);
        }

        sprintf(text, "RES%d", i);
        if (FUN_0049fd60(gadget, text) && IsLocalHuman_00447b10(p)) {
            FUN_0047f1a0("Multi", 0);
            FUN_00446310();
            FUN_004ab0a0(gadget);
            g_game->dirty = 1;
            return;
        }

        sprintf(text, "READY%d", i);
        if (FUN_0049fd60(gadget, text) && IsLocalHuman_00447b10(p)) {
            FUN_0047f1a0("Multi", 0);
            if (CheckMapCrc()) {
                p->info->b.ready = FUN_004a0f30(g_game->gui, FUN_0049fdf0(entries, text, 1));
                if (p->info->f97_0) {
                    strcpy(entries->label, "START");
                    g_game->table->field_20 = FUN_0049fdf0(entries, "START", 1);
                }
                for (int j = 0; j < 10; j++) {
                    Player_00447b10* q = &g_game->players[j];
                    if (IsLocal_00447b10(q))
                        q->info->b.ready = g_game->players[g_game->localPlayer].info->b.ready;
                }
                g_game->dirty = 1;
                BroadcastPlayerInfo();
            } else {
                FUN_004a1110(g_game->gui, text, 0);
            }
        }
        i++;
        if (i >= 10)
            break;
    }
    if (i != g_game->field_2a3c)
        g_game->dirty = 1;

    if (FUN_0049fd60(gadget, "PREVMENU")) {
        FUN_0047f1a0("Previous", 0);
        for (int j = 0; j < 10; j++) {
            Player_00447b10* q = &g_game->players[j];
            if (IsLocal_00447b10(q))
                RejectPlayer(q->id, 2);
        }
        g_game->state = 3;
        return;
    }
    if (FUN_0049fd60(gadget, "MESSAGE")) {
        Entry_00447b10* box = FUN_004a0010(entries, "MESSAGE");
        char* msg = box->text;
        if (strlen(msg) != 0) {
            if (_strcmpi(msg, "+syncerr") == 0) {
                char* s = g_game->net->GetSyncStatusText();
                if (s)
                    AddMessage(s, 4, 0, 10);
            } else {
                SendChatMessage(me, msg, 4, 0);
                if (g_usePacketManager)
                    g_packetManager.SendAllQueued(1);
            }
            g_game->dirty = 1;
            strcpy(msg, "");
        }
        FUN_004a7190(g_game->gui, FUN_0049fdf0(g_game->table->entries, "MESSAGE", 3));
    } else if (FUN_0049fd60(gadget, "COMMANDER")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.commander++;
        if (me->info->b.commander > 2)
            me->info->b.commander = 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "LOSTYPE")) {
        FUN_0047f1a0("Multi", 0);
        if (!me->info->b.los) {
            me->info->b.los = 1;
            me->info->b.losType = 1;
        } else if (me->info->b.losType == 1) {
            me->info->b.losType = 0;
        } else {
            me->info->b.los = 0;
        }
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "WATCHING")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.watching = !me->info->b.watching;
        if (!me->info->b.watching && me->active != 0 && me->info->b.bit6)
            me->info->b.bit6 = 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "CHEATING")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.cheating = !me->info->b.cheating;
        BroadcastPlayerInfo();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "FIXEDLOC")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.fixedloc = !me->info->b.fixedloc;
        BroadcastPlayerInfo();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "MAPPING")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.mapping = FUN_004a0f60(gadget, "MAPPING") == 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "START")) {
        int count = 0;
        FUN_0047f1a0("BigButton", 0);
        for (int j = 0; j < 10; j++) {
            Player_00447b10* q = &g_game->players[j];
            if ((IsLocalHuman_00447b10(q) || IsRemoteHuman_00447b10(q)) && q->info->f9d_2)
                count++;
        }
        if (count < 1 || (count < 2 && CountHumanPlayers() > 3) || (count < 3 && CountHumanPlayers() > 6)) {
            FUN_004ab0a0(g_game->gui);
            FUN_004abd90(gadget, FUN_004c5740("There are not enough game CDs present to play"), 200, 1, 1);
            return;
        }
        int total = CountComputerPlayers() + CountHumanPlayers();
        for (int t = 0; t < 5; t++) {
            if (CountAlliance_00447b10(t) == total) {
                FUN_004ab0a0(g_game->gui);
                FUN_004abd90(gadget, FUN_004c5740("Can not start game with all players on the same team."), 200, 1, 1);
                return;
            }
        }
        if (!((Class_00435c40*)g_game->map)->FUN_00435c40()) {
            FUN_0047f1a0("Multi", 0);
            FUN_00444ea0();
            goto done;
        }
        if (!me->info->b.watching) {
            for (int j = 0; j < 10; j++) {
                Player_00447b10* q = &g_game->players[j];
                if (q->active != 0 && q->type == 3 && (q->info->flags_9b & 0x40))
                    RejectPlayer(q->id, 9);
            }
        }
        g_game->state = 0x11;
        me->info->b.started = 1;
        UpdateNetGameInfo();
        g_game->los = me->info->b.los;
        g_game->losType = me->info->b.losType;
        g_game->commander = me->info->b.commander;
        g_game->options->fixedloc = me->info->b.fixedloc;
        g_game->mapping = me->info->b.mapping;
        FUN_00430f00();
        g_game->field_37eee = 2;
        return;
    } else if (FUN_0049fd60(gadget, "GAMEOPEN")) {
        FUN_0047f1a0("Multi", 0);
        me->info->b.closed = FUN_004a0f60(gadget, "GAMEOPEN") == 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (FUN_0049fd60(gadget, "RESTRICTIONS")) {
        FUN_0047f1a0("Options", 0);
        FUN_0044c7e0();
        FUN_004ab0a0(gadget);
    } else {
        // MAP and MAPNAME through a local, not `MAP || MAPNAME` in the
        // else-if: with the `||` the MAP body joins the region where C2 keeps
        // the constant 1 in ebp, and FUN_0049fb10 gets `push ebp` (99.2%).
        int hit = FUN_0049fd60(gadget, "MAP");
        if (!hit)
            hit = FUN_0049fd60(gadget, "MAPNAME");
        if (hit) {
            FUN_0047f1a0("Multi", 0);
            if (me->info->f97_0) {
                FUN_00444ea0();
            } else {
                Gadget_00447b10* view = FUN_004aa8f0(g_game->gui, "VIEWMAP.GUI", 0x900);
                view->handler = FUN_00444ba0;
                FUN_004288d0("DVIEWMAP", 0, 0, 0);
                FUN_00444a20();
                FUN_0049fb10(g_game->gui, 1);
                FUN_004a81e0(g_game->gui, 0x40);
            }
        }
    }
done:
    FUN_004ab0a0(gadget);
}
