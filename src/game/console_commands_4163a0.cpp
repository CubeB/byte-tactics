// Decompiled by Opus. Names are provisional.
// Console command handler (same family as 0x4163d0); the argument is unused.

#pragma pack(push, 1)
struct Game_004163a0 {
    char unknown_0[0x2c8e];
    short x;                         // +0x2c8e
    short y;                         // +0x2c90
    char unknown_2c92[0x2cbc - 0x2c92];
    unsigned short field_2cbc;       // +0x2cbc
};
#pragma pack(pop)

extern Game_004163a0* g_game;

void* __stdcall FUN_00481550(int x, int y);
void __stdcall FUN_004246b0(void* target, int flag);

// FUNCTION: 0x4163a0
void __stdcall FUN_004163a0(void* args)
{
    if (g_game->field_2cbc < 0xfffb) {
        void* target = FUN_00481550(g_game->x, g_game->y);
        FUN_004246b0(target, 1);
    }
}
