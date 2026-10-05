// Decompiled by deepseek-v4.1-flash, finished by GPT-6, checked by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by claude-opus-5-5, edited by Space Bunny Free, finished by Space Bunny Free, rewritten by claude-opus-5-5, finished by DeepSeek V4.1 Flash, finished by claude-opus-5-5. Names are provisional.
// Refreshes the multiplayer battle room every frame: scrolls the OUTPUT chat
// list, the MAPNAME/MAP state, the ready bits of the local players, then the
// ten player rows (CD, PLAYER, LOGO, SIDE, ALLY, TEAMICONS, RES, PING, MEM,
// READY), and finally the lowest ping limit.
//
// MATCH. What it took, largest first:
// - Two functions of this file that have no callers are inlined here and are
//   defined above, unannotated: the map check 0x440cd0 (CheckMapCrc) and the
//   SIDE%d update 0x448bf0 (UpdateSideGadget). Both still compile to their own
//   original bytes out of line.
// - <windows.h> (the file's other functions use it too): it gives the
//   original's base/index order and the loop-head shape.
// - The mapname text and the PING text share one `char* str` (one frame slot);
//   the row loop counter is a plain unsigned char `n` with no int copy.
// - MEM writes its whole tail (the FUN_00435920 compare, colour and visible
//   stores) in each arm of the n/a / %d test. MSVC cross-jumps the two copies
//   back down to the shared call, but the duplicated tail is what gives `p`
//   ebp and the int value of `n` ebx through the whole row loop (86.2% to
//   99.7% in one change). One call per arm with the compare after the if/else
//   was 84.5%, and the compare and colour per arm with `visible = 1` after it
//   86.9%, both with the two registers still swapped.
// - TEAMICONS lays out the 1 before the 0, the layout MSVC gives the last
//   term of an `||` chain (as in ALLY); the plain IsWatching test gives the 0
//   first. `|| 0` emits no code and gives that layout; it is probably a term
//   that the release build compiled to 0.
// - In CheckMapCrc the version test is `if (major >= 2) check = 1; else if
//   (major == 1 && minor >= 2) check = 1;`. Same bytes out of line, but inlined
//   it gives the original's edi/edx for g_game/check.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Class_004a1080;

#pragma pack(push, 1)
struct PlayerInfo_00448c70 {
    char unknown_0[0x8b];
    unsigned short width;               // +0x8b
    unsigned short height;              // +0x8d
    char unknown_8f[0x94 - 0x8f];
    char kind;                          // +0x94
    unsigned char side;                 // +0x95
    unsigned char field_96;             // +0x96
    unsigned char f97_0 : 1;            // +0x97
    unsigned char f97_rest : 7;
    char unknown_98;
    unsigned short memory;              // +0x99
    union {
        unsigned char flags_9b;         // +0x9b
        unsigned short flags;
        struct {
            unsigned short low : 5;
            unsigned short ready : 1;
            unsigned short bit6 : 1;
            unsigned short watching : 1;
            unsigned short b8 : 3;
            unsigned short commander : 2;
            unsigned short b13 : 3;
        } b;
    };
    unsigned char f9d_0 : 2;            // +0x9d
    unsigned char f9d_2 : 1;
    unsigned char f9d_rest : 5;
    char unknown_9e;
    unsigned short pingLimit;           // +0x9f
    char unknown_a1[0xa7 - 0xa1];
    unsigned char versionMajor;         // +0xa7
    unsigned char versionMinor;         // +0xa8
    unsigned int mapCrc;                // +0xa9
};

struct Player_00448c70 {                // 0x14b bytes
    int active;                         // +0x00
    int id;                             // +0x04
    char unknown_8[0x14 - 0x8];
    unsigned int ping;                  // +0x14
    char unknown_18[0x27 - 0x18];
    PlayerInfo_00448c70* info;          // +0x27
    char name[0x73 - 0x2b];             // +0x2b
    char type;                          // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char field_108[0xb];       // +0x108
    unsigned char field_113[0xb];       // +0x113
    char unknown_11e[0x146 - 0x11e];
    unsigned char field_146;            // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Gadget_00448c70 {
    char unknown_0[0x19];
    short height;                       // +0x19
    char unknown_1b[0x23 - 0x1b];
    int colour;                         // +0x23
    char unknown_27[0x29 - 0x27];
    unsigned char visible;              // +0x29
    char unknown_2a[0xb6 - 0x2a];
    char text[0xbe - 0xb6];             // +0xb6
    union {
        int field_be;                   // +0xbe
        struct {
            short unknown_be;
            short count;                // +0xc0
        } list;
    };
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6;            // +0xc6
    unsigned int c8_0 : 1;              // +0xc8
    unsigned int c8_rest : 31;
    char unknown_cc[0x138 - 0xcc];
    unsigned short field_138;           // +0x138
    char unknown_13a[0x13c - 0x13a];
    unsigned short b13c_0 : 1;          // +0x13c
    unsigned short b13c_rest : 15;
};
#pragma pack(pop)

class Class_004358f0 {
public:
    int FUN_004358f0();
};
class Class_00435920 {
public:
    int FUN_00435920();
};
class Class_00435a20 {
public:
    int LoadMissionByName(char* map);
};
class Class_00435c20 {
public:
    char* FUN_00435c20();
};
class Class_00435c30 {
public:
    char* FUN_00435c30();
};
class Class_00435c40 {
public:
    bool FUN_00435c40();
};
class Class_004373a0 {
public:
    unsigned int ComputeMapChecksum();
};
class UnitSync {
public:
    int IsPlayerSynced(int id);
};

struct Layer_00448c70 {
    int unknown_0;
    char* entries;                      // +0x4
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    char gui[0x531 - 0x519];            // +0x519
    Layer_00448c70* table;              // +0x531
    char unknown_535[0x12ef - 0x535];
    char messages[30][0x48];            // +0x12ef
    char unknown_1b5f[0x1b63 - 0x1b5f];
    Player_00448c70 players[10];        // +0x1b63
    char unknown_2851[0x2a30 - 0x2851];
    UnitSync* net;                      // +0x2a30
    char unknown_2a34[0x2a3e - 0x2a34];
    unsigned short scrollEnd;           // +0x2a3e
    unsigned short scrollStart;         // +0x2a40
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2a9b - 0x2a43];
    int list;                           // +0x2a9b
    char unknown_2a9f[0x148db - 0x2a9f];
    int field_148db;                    // +0x148db
    char unknown_148df[0x391e9 - 0x148df];
    Class_004358f0* map;                // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

int GetFontLineHeight();
char* __stdcall FUN_004b6af0(int list, int index);
void UpdateBattleRoomFlags();
void ShowSelectedMapInfo();
void RefreshTeamIcons();
void FUN_00446c70();
void BroadcastPlayerInfo();
void UpdateNetGameInfo();
unsigned char FindHostSlot();
int IsHostLocal();
int GetTicks();
int __stdcall GetSlotDpid(unsigned char player);
void __stdcall RejectPlayer(int id, unsigned char msg);
void __stdcall SendChatMessage(Player_00448c70* p, char* text, int a, int b);
char* __stdcall FUN_004c5740(char* text);
Gadget_00448c70* __stdcall FindGadgetChecked(char* entries, char* name);
Gadget_00448c70* __stdcall FindGadgetOrNull(char* entries, char* name);
Gadget_00448c70* __stdcall FUN_004a0180(char* entries, char* name);
Gadget_00448c70* __stdcall FUN_004a0280(char* entries, char* name);
int __stdcall FindGadgetIndex(char* entries, char* name, int type);
int __stdcall IsScreenNamed(char* gui, char* name);
void __stdcall FUN_0049fa90(char* gui);
void __stdcall FUN_004a0570(char* gui, char* name, int value);
void __stdcall FUN_004a0bf0(char* gui, char* name, char* text, int size);
int __stdcall SetButtonStageByName(Class_004a1080* gui, char* name, int value);
void __stdcall SetGadgetStatusByName(char* gui, char* name, int value);
void __stdcall FUN_004a1450(char* gui, char* name, int value);
void __stdcall FUN_004a5d50(char* gui, int index);

static inline int IsPlaying_00448c70(Player_00448c70* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsWatching_00448c70(Player_00448c70* p)
{
    return p->active != 0 && (p->info->flags_9b & 0x40);
}

static inline int IsLocalHuman_00448c70(Player_00448c70* p)
{
    return p->active != 0 && p->type == 1;
}

static inline int IsLocalAI_00448c70(Player_00448c70* p)
{
    return p->active != 0 && p->type == 2;
}

static inline int IsRemoteHuman_00448c70(Player_00448c70* p)
{
    return p->active != 0 && p->type == 3 && p->info->kind == 1;
}

static inline int IsRemoteAI_00448c70(Player_00448c70* p)
{
    return p->active != 0 && p->type == 3 && p->info->kind == 2;
}

static inline int IsLocal_00448c70(Player_00448c70* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2);
}

// The map check at 0x440cd0, which has no callers: /Ob2 inlined it.
int CheckMapCrc()
{
    if (!g_game->map->FUN_004358f0()) {
        return 0;
    }
    unsigned char me = FindHostSlot();
    PlayerInfo_00448c70* data = 0;
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
    if (((Class_004373a0*)g_game->map)->ComputeMapChecksum() != data->mapCrc)
        return 0;
    return 1;
}

// The SIDE%d update at 0x448bf0, which has no callers: /Ob2 inlined it.
void __stdcall UpdateSideGadget(int side)
{
    char name[20];
    Player_00448c70* p = &g_game->players[side];

    sprintf(name, "SIDE%d", side);
    SetButtonStageByName((Class_004a1080*)g_game->gui, name,
                 (p->active != 0 && (p->info->flags_9b & 0x40)) ? 2 : p->info->side);
}

// FUNCTION: 0x448c70
void RefreshBattleRoomRows()
{
    char name[20];
    char* str;
    unsigned int minPing = 0xffffffff;
    int count = 0;
    Player_00448c70* me = &g_game->players[g_game->localPlayer];
    int ready = me->info->b.ready;
    Gadget_00448c70* output = FindGadgetChecked(g_game->table->entries, "OUTPUT");

    int end = g_game->scrollEnd;
    int start = g_game->scrollStart;
    if (end < start)
        end += 30;
    if (end - start > output->height / (GetFontLineHeight() + 2)) {
        g_game->scrollStart++;
        if (g_game->scrollStart >= 30)
            g_game->scrollStart = 0;
    }
    if (g_game->scrollEnd != g_game->scrollStart) {
        for (int i = g_game->scrollStart; g_game->scrollEnd != i; ) {
            char* line = FUN_004b6af0(g_game->list, count);
            strcpy(line, g_game->messages[i]);
            count++;
            i++;
            if (i == 30)
                i = 0;
        }
    }
    UpdateBattleRoomFlags();
    output->list.count = count;

    Gadget_00448c70* mapname = FUN_004a0180(g_game->table->entries, "MAPNAME");
    char* map = ((Class_00435c30*)g_game->map)->FUN_00435c30();
    if (!((Class_00435c40*)g_game->map)->FUN_00435c40()) {
        mapname->colour = 0xc;
        FUN_004a0bf0(g_game->gui, "MAPNAME", "NOT SELECTED", 0);
    } else {
        char* cur = ((Class_00435c20*)g_game->map)->FUN_00435c20();
        str = mapname->text;
        int differs = strcmp(str, cur);
        if (differs) {
            if (IsScreenNamed(g_game->gui, "viewmap.gui"))
                ShowSelectedMapInfo();
            else
                ((Class_00435a20*)g_game->map)->LoadMissionByName(map);
        }
        if (!CheckMapCrc()) {
            mapname->colour = (GetTicks() / 30 & 1) ? 0xc : 0;
            if (differs) {
                SendChatMessage(me, FUN_004c5740("does not have this map"), 4, 0);
                me->info->b.ready = 0;
                sprintf(name, "READY%d", g_game->localPlayer);
                SetGadgetStatusByName(g_game->gui, name, 0);
                BroadcastPlayerInfo();
            }
            if (!g_game->players[g_game->localPlayer].info->f97_0)
                FUN_004a1450(g_game->gui, "MAP", 1);
        } else {
            mapname->colour = 0;
            FUN_004a1450(g_game->gui, "MAP", 0);
        }
        strcpy(str, ((Class_00435c20*)g_game->map)->FUN_00435c20());
    }

    int i;
    for (i = 0; i < 10; i++) {
        Player_00448c70* p = &g_game->players[i];
        if (IsLocal_00448c70(p))
            p->info->b.ready = g_game->players[g_game->localPlayer].info->b.ready;
    }
    for (i = 0; i < 10; i++) {
        Player_00448c70* p = &g_game->players[i];
        if (g_game->players[g_game->localPlayer].info->b.commander == 2
            && (IsLocalAI_00448c70(p) || IsRemoteAI_00448c70(p))) {
            RejectPlayer(p->id, 0xb);
            BroadcastPlayerInfo();
        }
        if (FindHostSlot() != 10
            && !g_game->players[FindHostSlot()].info->b.watching
            && p->active != 0) {
            unsigned short flags = p->info->flags;
            if (flags & 0x40) {
                p->info->flags = flags & ~0x40;
                p->info->side = 0;
                BroadcastPlayerInfo();
            }
        }
    }
    FUN_00446c70();
    RefreshTeamIcons();

    char* entries = g_game->table->entries;
    Player_00448c70* local = &g_game->players[g_game->localPlayer];
    for (unsigned char n = 0; n < 10; n++) {
        char text[32];
        char res[52];
        char blocked[52];
        Player_00448c70* p = &g_game->players[n];
        Gadget_00448c70* e;
        if (!IsPlaying_00448c70(p) && !IsWatching_00448c70(p)) {
            sprintf(name, "CD%d", n);
            FUN_004a0570(g_game->gui, name, 0);
            FUN_004a1450(g_game->gui, name, 0);
            sprintf(name, "PLAYER%d", n);
            char* s = "UNUSED";
            if (p->type == 4) {
                sprintf(blocked, "[%s]", FUN_004c5740("BLOCKED"));
                s = blocked;
            }
            strncpy(text, s, 0x1e);
            FUN_004a0bf0(g_game->gui, name, text, 0);
            FUN_004a5d50(g_game->gui, FindGadgetIndex(entries, name, 0xe));
            FUN_004a1450(g_game->gui, name, ready);
            sprintf(name, "LOGO%d", n);
            e = FUN_004a0280(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "SIDE%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e)
                e->visible = 0;
            if (n != g_game->localPlayer) {
                sprintf(name, "ALLY%d", n);
                e = FindGadgetOrNull(entries, name);
                if (e)
                    e->visible = 0;
            }
            sprintf(name, "TEAMICONS%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "RES%d", n);
            e = FUN_004a0180(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "PING%d", n);
            e = FUN_004a0180(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "MEM%d", n);
            e = FUN_004a0180(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "READY%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                e->b13c_0 = 1;
                e->field_138 = 0;
                e->visible = 0;
            }
        } else {
            sprintf(name, "CD%d", n);
            FUN_004a0570(g_game->gui, name,
                         ((IsLocalHuman_00448c70(p) || IsRemoteHuman_00448c70(p))
                          && p->info->f9d_2) ? 1 : 0);
            FUN_004a1450(g_game->gui, name, 0);
            sprintf(name, "LOGO%d", n);
            e = FUN_004a0280(entries, name);
            if (e) {
                e->visible = (p->info->field_96 == 0xff && !ready) ? 0 : 1;
                e->c8_0 = !ready;
                e->field_be = g_game->field_148db;
                e->field_c6 = p->info->field_96;
            }
            sprintf(name, "PLAYER%d", n);
            strncpy(text, p->name, 0x1e);
            FUN_004a0bf0(g_game->gui, name, text, 0);
            FUN_004a5d50(g_game->gui, FindGadgetIndex(entries, name, 0xe));
            FUN_004a1450(g_game->gui, name, ready);
            sprintf(name, "SIDE%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                UpdateSideGadget(n);
                e->visible = 1;
                FUN_004a1450(g_game->gui, name, (IsLocal_00448c70(p) && !ready) ? 0 : 1);
            }
            sprintf(name, "ALLY%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                SetButtonStageByName((Class_004a1080*)g_game->gui, name,
                             me->field_113[n] << 1 | me->field_108[n]);
                e->visible = (IsLocalHuman_00448c70(p) || IsWatching_00448c70(p)
                              || IsLocalAI_00448c70(p) || IsRemoteAI_00448c70(p)
                              || IsWatching_00448c70(local)) ? 0 : 1;
                FUN_004a1450(g_game->gui, name, ready);
            }
            sprintf(name, "TEAMICONS%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                e->visible = (IsWatching_00448c70(p) || 0) ? 0 : 1;  // see the top
                FUN_004a1450(g_game->gui, name, (IsLocal_00448c70(p) && !ready) ? 0 : 1);
            }
            sprintf(name, "RES%d", n);
            if (!IsLocalHuman_00448c70(p) && !IsRemoteHuman_00448c70(p))
                sprintf(res, "%s", "n/a");
            else
                sprintf(res, "%dx%d", p->info->width, p->info->height);
            FUN_004a0bf0(g_game->gui, name, res, 0);
            if (IsLocalHuman_00448c70(p)) {
                FUN_004a1450(g_game->gui, name, ready);
            } else {
                e = FUN_004a0180(entries, name);
                if (e)
                    e->visible = 1;
            }
            sprintf(name, "PING%d", n);
            e = FUN_004a0180(entries, name);
            if (IsRemoteHuman_00448c70(p)) {
                if (e) {
                    str = e->text;
                    _itoa(p->ping, str, 10);
                    if (IsHostLocal())
                        strcat(str, g_game->net->IsPlayerSynced(GetSlotDpid(n)) ? ":s" : "");
                    if (minPing >= p->ping)
                        minPing = p->ping;
                    e->visible = 1;
                }
            } else {
                FUN_004a0bf0(g_game->gui, name, "n/a", 0);
                if (e)
                    e->visible = 1;
            }
            sprintf(name, "MEM%d", n);
            e = FUN_004a0180(entries, name);
            if (!IsLocalHuman_00448c70(p) && !IsRemoteHuman_00448c70(p)) {
                sprintf(e->text, "%s", "n/a");
                e->colour = p->info->memory < ((Class_00435920*)g_game->map)->FUN_00435920() ? 0xc : 0;
                e->visible = 1;
            } else {
                sprintf(e->text, "%d", p->info->memory);
                e->colour = p->info->memory < ((Class_00435920*)g_game->map)->FUN_00435920() ? 0xc : 0;
                e->visible = 1;
            }
            sprintf(name, "READY%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                e->field_138 = g_game->players[n].info->b.ready;
                e->visible = 1;
                e->b13c_0 = !IsLocalHuman_00448c70(p);
            }
        }
    }
    FUN_0049fa90(g_game->gui);
    PlayerInfo_00448c70* info = me->info;
    if (info->f97_0 && minPing < info->pingLimit) {
        info->pingLimit = minPing;
        UpdateNetGameInfo();
    }
}
