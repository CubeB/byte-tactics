// Decompiled by Space Bunny Free. Names are provisional.
// Opens the ALLIES.GUI dialog (with HandleAlliesClick as its handler), renames the
// "ALLY%d" and "TEAMICONS%d" entries of the ally table to consecutive numbers,
// refreshes the menus, then sets the VICTORY entry from the local player:
// visible when the local player's team has more than one member on it, or
// when bit 6 of its info byte at +0x9b is set. The second loop in
// CountAlliance_004478b0 repeats the type test that IsPlaying already made,
// which is dead: the type is 1, 2 or 3 there too. Kept as the compiler has it.
#include <stdio.h>

struct Class_004a1080;
struct Class_004a1450;
struct Dialog;
struct Class_0049fb10;

#pragma pack(push, 1)
struct PlayerInfo_004478b0 {
    char unknown_0[0x9b];
    unsigned char flags_9b;             // +0x9b
    char unknown_9c[0x9d - 0x9c];
    unsigned short flags_9d;            // +0x9d
};

struct Player_004478b0 {
    int active;                         // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_004478b0* info;          // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                 // +0x73
    char unknown_74[0x13f - 0x74];
    unsigned char alliance;             // +0x13f
    int field_140;                      // +0x140
    short field_144;                    // +0x144
    unsigned char field_146;            // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Layer_004478b0 {
    int unknown_0;
    char* entries;                      // +0x4
};

struct Gadget_004478b0 {
    char unknown_0[0x8];
    void (__stdcall* handler)(void*);   // +0x8
    int field_c;                        // +0xc
};

struct Game {
    char unknown_0[0x519];
    char gui[0x531 - 0x519];            // +0x519
    Layer_004478b0* table;              // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_004478b0 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    unsigned char flags_2a44;          // +0x2a44, bit 2 is the "count extra" test
    char unknown_2a45[0x37ebe - 0x2a45];
    // A short, not a byte: only the short form gets the single-instruction
    // "or byte ptr [eax+0x37ebe], 0x20" the original uses.
    unsigned short flags_37ebe;         // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

Gadget_004478b0* __stdcall LoadGuiLayer(char* sub, const char* name, int flags);
void __stdcall HandleAlliesClick(void* gadget);
int __stdcall FindGadgetIndex(char* entries, const char* name, int flag);
int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, int value);
void __stdcall FUN_004a1450(Class_004a1450* obj, char* name, int value);
void __stdcall FUN_0049fb10(Class_0049fb10* obj, int value);
void __stdcall RenderLayer(Dialog* obj, int value);
void __stdcall RefreshAlliesScreen(int value);
void RebuildAllyList();
void RefreshTeamIcons();

static inline int IsPlaying_004478b0(Player_004478b0* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted_004478b0(Player_004478b0* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

static inline int CountAlliance_004478b0(int alliance)
{
    int count = 0;
    unsigned char f = (g_game->flags_2a44 >> 2) & 1;
    for (int j = 0; j < 10; j++) {
        Player_004478b0* q = &g_game->players[j];
        if (f) {
            if (q->alliance == alliance && IsPlaying_004478b0(q)
                && IsCounted_004478b0(q))
                count++;
        } else {
            if (q->alliance == alliance && IsPlaying_004478b0(q))
                count++;
        }
    }
    return count;
}

// FUNCTION: 0x4478b0
void OpenAlliesDialog()
{
    Gadget_004478b0* gadget = LoadGuiLayer(g_game->gui, "ALLIES.GUI", 0x800);
    gadget->handler = HandleAlliesClick;
    gadget->field_c = (int)g_game;
    g_game->flags_37ebe |= 0x20;
    char* entries = g_game->table->entries;
    int i, j;
    for (i = 0; (j = FindGadgetIndex(entries, "ALLYx", 0xe)) != -1; i++)
        sprintf(entries + j * 0x15b + 2, "ALLY%d", i);
    for (i = 0; (j = FindGadgetIndex(entries, "TEAMICONSx", 0xe)) != -1; i++)
        sprintf(entries + j * 0x15b + 2, "TEAMICONS%d", i);
    RefreshAlliesScreen(0);
    RebuildAllyList();
    RefreshTeamIcons();
    Player_004478b0* local = &g_game->players[g_game->localPlayer];
    int old = (local->info->flags_9d >> 1) & 1;
    unsigned char win = (local->info->flags_9b >> 6) & 1;
    SetButtonStageByName((Class_004a1080*)g_game->gui, "VICTORY", old);
    int alliance = local->alliance;
    int count;
    if (alliance == 5)
        count = 0;
    else
        count = CountAlliance_004478b0(alliance);
    FUN_004a1450((Class_004a1450*)g_game->gui, "VICTORY",
                 (count > 1 || win) ? 1 : 0);
    FUN_0049fb10((Class_0049fb10*)g_game->gui, 1);
    RenderLayer((Dialog*)g_game->gui, 0x40);
}
