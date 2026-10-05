// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-opus-5-5. Names are provisional.
// Refreshes the ally screen: for every playing slot it fills the next
// consecutive PLAYER/LOGO/ALLY/TEAMICONS entries and the LIVEPLYR/LIVEALLY
// markers. The slot test is two inline helpers, !IsWatching(p) && IsActive(p),
// the same pair 0x448c70 uses: that split is what gives the original's
// register rotation for the second sprintf group (earlier attempts that
// wrote it as one helper stayed at 93.5%, or 95.1% with lstrcpynA moved).
#include <stdio.h>
#include <string.h>
#include <windows.h>

#pragma pack(push, 1)
struct PlayerInfo_00447380 {
    char unknown_0[0x94];
    unsigned char field_94;             // +0x94
    char unknown_95[0x96 - 0x95];
    unsigned char field_96;             // +0x96
    char unknown_97[0x9b - 0x97];
    unsigned char flags_9b;             // +0x9b
};

struct Player_00447380 {                // 0x14b bytes
    int active;                         // +0x00
    int field_4;                        // +0x04
    char unknown_8[0x27 - 0x8];
    PlayerInfo_00447380* info;          // +0x27
    char name[0x73 - 0x2b];             // +0x2b
    unsigned char type;                 // +0x73
    char unknown_74[0x13f - 0x74];
    unsigned char alliance;             // +0x13f
    int field_140;                      // +0x140
    short field_144;                    // +0x144
    unsigned char field_146;            // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Entry_00447380 {                 // 0x15b bytes
    unsigned char field_0;              // +0x00
    char unknown_1[0x1b - 1];
    int field_1b;                       // +0x1b
    char unknown_1f[0x29 - 0x1f];
    unsigned char field_29;             // +0x29
    char unknown_2a[0xbe - 0x2a];
    int field_be;                       // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6;            // +0xc6
    unsigned int field_c8;              // +0xc8
    char unknown_cc[0x15b - 0xcc];
};

struct Layer_00447380 {
    int unknown_0;
    Entry_00447380* entries;            // +0x4
};

struct Game_00447380 {
    char unknown_0[0x519];
    char gui[0x531 - 0x519];            // +0x519
    Layer_00447380* table;              // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_00447380 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    unsigned char flags_2a44;           // +0x2a44
    char unknown_2a45[0x148db - 0x2a45];
    int field_148db;                    // +0x148db
};
#pragma pack(pop)

extern Game_00447380* g_game;

int __stdcall FUN_0049fdf0(Entry_00447380* entries, char* name, int type);
Entry_00447380* __stdcall FUN_0049ff10(Entry_00447380* entries, char* name);
Entry_00447380* __stdcall FUN_004a0280(Entry_00447380* entries, char* name);
void __stdcall FUN_004a0570(void* obj, char* name, int value);
void __stdcall FUN_004a0bf0(void* obj, char* name, char* text, int size);
void __stdcall FUN_004a1450(void* obj, char* name, int value);
void __stdcall FUN_0049f930(void* obj, char* name, char* text);

static inline int IsType_00447380(Player_00447380* p)
{
    return p->type == 1 || p->type == 2 || p->type == 3;
}

static inline int IsCounted_00447380(Player_00447380* p)
{
    if (!IsType_00447380(p))
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    return 1;
}

static inline int IsWatching_00447380(Player_00447380* p)
{
    return p->active != 0 && (p->info->flags_9b & 0x40);
}

static inline int IsActive_00447380(Player_00447380* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

// FUNCTION: 0x447380
void __stdcall FUN_00447380(int param_1)
{
    Entry_00447380* entries = g_game->table->entries;
    int i;
    int n;
    Player_00447380* local = &g_game->players[g_game->localPlayer];
    char player[20];
    char teamicons[20];
    char ally[20];
    char live[20];
    char logo[20];
    char name[0x80];

    for (i = 0, n = 0; i < 10; i++) {
        sprintf(player, "PLAYER%d", i);
        sprintf(logo, "LOGO%d", i);
        sprintf(ally, "ALLY%d", i);
        sprintf(teamicons, "TEAMICONS%d", i);
        FUN_004a0570((char*)g_game + 0x519, player, 0);
        FUN_004a0570((char*)g_game + 0x519, logo, 0);
        FUN_004a0570((char*)g_game + 0x519, ally, 0);
        FUN_004a0570((char*)g_game + 0x519, teamicons, 0);

        Player_00447380* p = &g_game->players[i];
        if (!IsWatching_00447380(p) && IsActive_00447380(p)
            && (i != g_game->localPlayer || param_1 == 0)
            && (!(g_game->flags_2a44 & 4) || IsCounted_00447380(p))
            && p->info->field_96 != 0xff) {
            sprintf(player, "PLAYER%d", n);
            sprintf(logo, "LOGO%d", n);
            sprintf(ally, "ALLY%d", n);
            sprintf(teamicons, "TEAMICONS%d", n);
            lstrcpynA(name, p->name, 0x80);

            int idx = FUN_0049fdf0(entries, player, 0xe);
            if (entries[idx].field_0 == 1) {
                Entry_00447380* e = FUN_0049ff10(entries, player);
                if (e != 0 && (e->field_1b & 0x4000)) {
                    strcat(name, "|");
                    strcat(name, p->name);
                }
            }

            FUN_004a0bf0((char*)g_game + 0x519, player, name, 0x80);
            FUN_004a0570((char*)g_game + 0x519, player, 1);
            sprintf(live, "LIVEPLYR%d", i);
            FUN_0049f930((char*)g_game + 0x519, player, live);

            if (p->active != 0
                && IsType_00447380(p)
                && p->field_146 != 10
                && (p->field_144 != 0 || p->field_140 == 0)
                && p->type != 1
                && p->type != 2
                && !(p->type == 3 && p->info->field_94 == 2)) {
                Player_00447380* q = &g_game->players[g_game->localPlayer];
                if (q->active != 0
                    && IsType_00447380(q)
                    && q->field_146 != 10
                    && (q->field_144 != 0 || q->field_140 == 0)) {
                    FUN_004a0570((char*)g_game + 0x519, ally, 1);
                }
            }

            sprintf(live, "LIVEALLY%d", i);
            FUN_0049f930((char*)g_game + 0x519, ally, live);

            if (p->alliance == local->alliance && p->alliance != 5) {
                FUN_004a1450((char*)g_game + 0x519, live, 1);
            }

            FUN_004a0570((char*)g_game + 0x519, teamicons, 1);

            int value;
            if (p->active != 0 && (p->type == 1 || p->type == 2)
                && !(g_game->flags_2a44 & 4)) {
                value = 0;
            } else {
                value = 1;
            }
            FUN_004a1450((char*)g_game + 0x519, teamicons, value);

            Entry_00447380* e2 = FUN_004a0280(entries, logo);
            if (e2 != 0) {
                e2->field_29 = 1;
                e2->field_be = g_game->field_148db;
                e2->field_c6 = p->info->field_96;
                e2->field_c8 &= ~1;
            }

            n++;
        }
    }
}