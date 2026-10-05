// Decompiled by space-bunny-free. Names are provisional.
// Steps the colour of the current player one step round the colour wheel (a
// non-zero argument walks backwards, the signed modulo can produce -1 which is
// then wrapped to the last colour) and keeps stepping while another active
// player still wears that colour, then refreshes the matching "Color<n>"
// gadget and marks the menu for redraw.

#include <windows.h>

#pragma pack(push, 1)
struct Entry_0047acd0 {                // GUI entry, 0x15b bytes
    char unknown_0[0xbe];
    void* field_be;                    // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6;           // +0xc6
    char unknown_c8[0x15b - 0xc8];
};

struct Player_0047acd0 {               // 0x18 bytes
    int active;                        // +0x00
    int shade;                         // +0x04
    int type;                          // +0x08
    char unknown_c[0x14 - 0xc];
    int color;                         // +0x14
};

struct Table_0047acd0 {
    Player_0047acd0 players[22];       // +0x00
    char unknown_210[0x224 - 0x210];
    int current;                       // +0x224
};

struct Holder_0047acd0 {
    int unknown_0;
    Entry_0047acd0* entries;           // +0x04
};

struct Menu_0047acd0 {
    char unknown_0[1];
};

struct Game {
    char unknown_0[0x519];
    Menu_0047acd0 menu;                // +0x519
    char unknown_51a[0x531 - 0x51a];
    Holder_0047acd0* holder;           // +0x531
    char unknown_535[0x29a0 - 0x535];
    Table_0047acd0* table;             // +0x29a0
    char unknown_29a4[0x148db - 0x29a4];
    unsigned short* colorCount;        // +0x148db
    char unknown_148df[0x38d81 - 0x148df];
    int playerCount;                   // +0x38d81
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FindGadgetIndex(Entry_0047acd0* entries, const char* name, int flag);
void __stdcall FUN_0049fa90(Menu_0047acd0* menu);

// The loop's test: an inlined helper keeps its two results in eax, which is
// what the original's `xor eax,eax` / `mov eax,1` merge shows.
static inline int color_taken(int me)
{
    int j;
    for (j = 0; j < g_game->playerCount; j++) {
        if (g_game->table->players[j].color == g_game->table->players[me].color &&
            g_game->table->players[j].active != 0 && j != me)
            return 1;
    }
    return 0;
}

// FUNCTION: 0x47acd0
void __stdcall CyclePlayerColor(int param_1)
{
    char buf[0x40];
    Entry_0047acd0* entries = g_game->holder->entries;
    int current = g_game->table->current;

    do {
        g_game->table->players[current].color =
            (g_game->table->players[current].color + (param_1 ? -1 : 1)) % *g_game->colorCount;
        if (g_game->table->players[current].color == -1)
            g_game->table->players[current].color = *g_game->colorCount - 1;
    } while (color_taken(current));

    wsprintfA(buf, "Color%d", current);
    int index = FindGadgetIndex(entries, buf, 6);
    if (index != -1) {
        Entry_0047acd0* gadget = &entries[index];
        if (gadget != 0) {
            gadget->field_be = g_game->colorCount;
            gadget->field_c6 = (unsigned short)g_game->table->players[current].color;
        }
    }
    FUN_0049fa90(&g_game->menu);
}
