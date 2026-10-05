// Decompiled by Sonnet. Names are provisional.

extern void FUN_0041c390(void);

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14371];
    unsigned short field_14371;
    unsigned int bit0 : 1;
    unsigned int paused : 1;
    unsigned int rest : 30;
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4174e0
void __stdcall CmdBigBrother(int unused)
{
    if (g_game->paused) {
        g_game->paused = 0;
        FUN_0041c390();
    } else {
        g_game->paused = 1;
        g_game->field_14371 = 1;
    }
}
