// Decompiled by Opus. Names are provisional.

void __stdcall FUN_004864b0(void* unit, int param_2);

#pragma pack(push, 1)
struct Unit_00486e80 {
    char unknown_0[0xa6];
    short field_a6;                     // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

struct Game_00486e80 {
    char unknown_0[0x14357];
    Unit_00486e80* units;               // +0x14357
    Unit_00486e80* units_end;           // +0x1435b
};
#pragma pack(pop)

extern Game_00486e80* g_game;

// FUNCTION: 0x486e80
void __stdcall FUN_00486e80(short id)
{
    Unit_00486e80* u = g_game->units;
    Unit_00486e80* end = g_game->units_end;
    if (id != 0 && u != 0) {
        for (; u <= end; u = (Unit_00486e80*)((char*)u + 0x118)) {
            if (u->field_a6 == id) {
                FUN_004864b0(u, 8);
            }
        }
    }
}
