// Decompiled by Space Bunny Free. Names are provisional.

void __stdcall FUN_004864b0(void* unit, int param_2);
void __cdecl FUN_004d85a0(void* p);

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0xa6];
    short field_a6;                     // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                        // +0x14357
    Unit* units_end;                    // +0x1435b
    void* list;                         // +0x1435f (unit id list)
    void* buffer;                       // +0x14363
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x485980
void FUN_00485980(void)
{
    Unit* u = g_game->units;
    Unit* end = g_game->units_end;
    if (u != 0) {
        for (; u <= end; u = (Unit*)((char*)u + 0x118)) {
            if (u->field_a6 != 0)
                FUN_004864b0(u, 8);
        }
    }
    if (g_game->buffer != 0)
        FUN_004d85a0(g_game->buffer);
    g_game->buffer = 0;
    if (g_game->list != 0)
        FUN_004d85a0(g_game->list);
    g_game->list = 0;
    if (g_game->units != 0)
        FUN_004d85a0(g_game->units);
    g_game->units = 0;
}
