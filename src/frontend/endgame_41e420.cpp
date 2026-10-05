// Decompiled by Claude Opus 5.5. Names are provisional.
// Fills the end-of-game statistics screen: for each of the 10 player slots
// in use, adds a "PlayerColor<n>" button (given the logo image and the
// player's colour), the player's name as a TEXT gadget, and seven bars
// (Kills, Losses, EProduced, MProduced, EWasted, MWasted, Score) scaled
// against the per-stat values at +0x3918f, one row every 0x14 pixels.
// The setup statements before the loop must stay in this order: moving the
// button's flag bit or the four bar fields changes the register choice.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x96];
    unsigned char color;               // +0x96
};

struct PlayerEntry_0041e420 {          // 0x14b bytes
    Unit* unit;                        // +0x0
    char unknown_4[0x14b - 4];
};

struct Slot_0041e420 {                 // 0x3a bytes
    char name[0x1e];                   // +0x0
    int stats[7];                      // +0x1e
};

struct Entry_0041e420 {                // 0x15b bytes
    char unknown_0[0x17];
    short width;                       // +0x17
    char unknown_19[2];
    int attr;                          // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6 (entry 0 only)
    char unknown_b8[0xbe - 0xb8];
    void* image;                       // +0xbe
    char unknown_c2[4];
    unsigned short color;              // +0xc6
    char unknown_c8[0x15b - 0xc8];
};

struct Holder_0041e420 {
    char unknown_0[4];
    Entry_0041e420* entries;           // +0x4
};

struct Menu_0041e420 {
    char unknown_0[8];
    void* font_8;                      // +0x8
    void* font_c;                      // +0xc
    char unknown_10[4];
    void* font;                        // +0x14 (current font)
    Holder_0041e420* holder;           // +0x18
};

struct Header_0041e420 {
    unsigned char type;                // +0x0
    char unknown_1;
    char name[0x11];                   // +0x2
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    int attr;                          // +0x1b
    int color;                         // +0x1f
    int color2;                        // +0x23
    char unknown_27[2];
    unsigned char flag;                // +0x29
    char unknown_2a[0xb6 - 0x2a];
};

struct Bar_0041e420 {                  // 0xd6 bytes, passed to FUN_004ab3a0
    Header_0041e420 h;
    int max;                           // +0xb6
    int field_ba;                      // +0xba
    int value;                         // +0xbe
    int field_c2;                      // +0xc2
    int field_c6;                      // +0xc6
    float scale;                       // +0xca
    int field_ce;                      // +0xce
    int field_d2;                      // +0xd2
};

struct Button_0041e420 {               // 0xcc bytes, passed to FUN_004ab310
    Header_0041e420 h;
    char unknown_b6[0xc8 - 0xb6];
    int flags;                         // +0xc8
};

struct Game_0041e420 {
    char unknown_0[0x519];
    Menu_0041e420 menu;                // +0x519
    char unknown_535[0xdcf - 0x535];
    unsigned char color1;              // +0xdcf
    char unknown_dd0[3];
    unsigned char color2;              // +0xdd3
    char unknown_dd4[0x1b8a - 0xdd4];
    PlayerEntry_0041e420 players[10];  // +0x1b8a
    char unknown_2878[0x148db - 0x2878];
    void* logos32;                     // +0x148db
    char unknown_148df[0x38dd9 - 0x148df];
    Slot_0041e420 slots[10];           // +0x38dd9
    char unknown_3901d[0x3906b - 0x3901d];
    int field_3906b;                   // +0x3906b
    char unknown_3906f[0x3918f - 0x3906f];
    int maxStats[7];                   // +0x3918f
};
#pragma pack(pop)

extern Game_0041e420* g_game;

int __stdcall FUN_0049fdf0(Entry_0041e420* entries, char* name, int type);
int FUN_004a50b0();
int __stdcall FUN_004ab1b0(Holder_0041e420* holder, char* type, char* text, int x, int y,
                           int width, int attr);
int __stdcall FUN_004ab310(Menu_0041e420* menu, Button_0041e420* record);
int __stdcall FUN_004ab3a0(Menu_0041e420* menu, Bar_0041e420* record);

#define STAT_BAR(n, px, fmt)                                                \
    bar.h.x = px;                                                           \
    bar.value = g_game->slots[i].stats[n];                                  \
    bar.max = g_game->maxStats[n];                                          \
    bar.scale = max(bar.value * 0.06666667f, 1.0f);                         \
    wsprintfA(bar.h.name, fmt, i);                                          \
    FUN_004ab3a0(&g_game->menu, &bar);

// FUNCTION: 0x41e420
void FUN_0041e420(void)
{
    Bar_0041e420 bar;
    Button_0041e420 button;
    char name[64];
    Menu_0041e420* menu = &g_game->menu;
    Entry_0041e420* entries = menu->holder->entries;
    memset(&bar, 0, sizeof(bar));
    bar.h.flag = 0;
    bar.h.y = 0x5d;
    bar.h.width = 0x43;
    bar.h.height = 0x12;
    bar.h.color = g_game->color1;
    bar.h.color2 = g_game->color2;
    bar.field_d2 = 1;
    bar.field_ce = 1;
    bar.field_ba = 0;
    bar.field_c2 = 1;
    memset(&button, 0, sizeof(button));
    button.h.flag = 1;
    button.h.attr = 0x400;
    button.flags |= 1;
    g_game->field_3906b = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->slots[i].name[0]) {
            wsprintfA(name, "PlayerColor%d", i);
            strcpy(button.h.name, name);
            button.h.x = 0x10;
            button.h.y = bar.h.y;
            button.h.width = 0x5b;
            button.h.height = 0x15;
            FUN_004ab310(&g_game->menu, &button);
            int idx = FUN_0049fdf0(entries, name, 6);
            if (idx != -1) {
                Entry_0041e420* e = &entries[idx];
                if (e) {
                    e->image = g_game->logos32;
                    e->color = g_game->players[i].unit->color;
                }
            }
            menu->font = menu->font_c;
            int h = FUN_004a50b0();
            FUN_004ab1b0(g_game->menu.holder, "TEXT", g_game->slots[i].name, 0x10,
                         (0x14 - h) / 2 + bar.h.y, -1, 2);
            entries[entries->count].attr = 2;
            entries[entries->count].width = 0x5a;
            menu->font = menu->font_8;
            STAT_BAR(0, 0x70, "Kills%d")
            STAT_BAR(1, 0xba, "Losses%d")
            STAT_BAR(2, 0x104, "EProduced%d")
            STAT_BAR(3, 0x14e, "MProduced%d")
            STAT_BAR(4, 0x198, "EWasted%d")
            STAT_BAR(5, 0x1e2, "MWasted%d")
            STAT_BAR(6, 0x22c, "Score%d")
            bar.h.y += 0x14;
        }
    }
}
