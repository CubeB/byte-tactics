// Decompiled by Sonnet. Names are provisional.

void __stdcall FUN_004864b0(void* unit, int param_2);

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0xa6];
    short field_a6;                     // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

struct Game_00486ed0 {
    char unknown_0[0x14357];
    Unit* units;                        // +0x14357
    Unit* units_end;                    // +0x1435b
};
#pragma pack(pop)

extern Game_00486ed0* g_game;

// FUNCTION: 0x486ed0
void FUN_00486ed0(void)
{
    Unit* u = g_game->units;
    Unit* end = g_game->units_end;
    if (u != 0) {
        for (; u <= end; u = (Unit*)((char*)u + 0x118)) {
            if (u->field_a6 != 0) {
                FUN_004864b0(u, 8);
            }
        }
    }
}
