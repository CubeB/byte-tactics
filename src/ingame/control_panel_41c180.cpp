// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x110 - 0xa8];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x37e9c - 0x1435b];
    unsigned short unitIndex;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short b0 : 1;             // +0x37ebe
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;             // 0x10
    unsigned short b5 : 1;
    unsigned short b6 : 1;
    unsigned short b7 : 1;             // 0x80
    unsigned short b8 : 1;             // 0x100
    unsigned short b9 : 1;             // 0x200
    unsigned short b10 : 1;            // 0x400
    unsigned short b11 : 1;
    unsigned short b12 : 1;
    unsigned short b13 : 1;
    unsigned short b14 : 1;
    unsigned short b15 : 1;
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_0041bde0(int param_1);
void __stdcall FUN_0041bf10(int param_1);

// The unit at g_game->unitIndex, or 0 when the index is empty or the slot is
// not live (field_a6 == 0). Inlined at both call sites below.
static Unit* GetSelectedUnit()
{
    unsigned short index = g_game->unitIndex;
    if (index) {
        Unit* unit = &g_game->units[index];
        if (unit->field_a6)
            return unit;
    }
    return 0;
}

// FUNCTION: 0x41c180
void FUN_0041c180()
{
    if (g_game->b7) {
        g_game->b7 = 0;
        FUN_0041bde0(0);
        return;
    }
    if (g_game->b8) {
        g_game->b8 = 0;
        FUN_0041bf10(0);
        return;
    }
    if (g_game->b10) {
        g_game->b10 = 0;
        Unit* unit = GetSelectedUnit();
        if (unit) {
            unit->flags |= 0x400000;
            g_game->b4 = 1;
        }
        return;
    }
    if (g_game->b9) {
        g_game->b9 = 0;
        Unit* unit = GetSelectedUnit();
        if (unit) {
            unit->flags &= ~0x400000;
            g_game->b4 = 1;
        }
    }
}
