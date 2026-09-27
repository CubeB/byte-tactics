// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x142ef];
    short blinkTimer;               // +0x142ef
    unsigned short blinkOn : 1;     // +0x142f1, bit 0
    unsigned short rest : 15;
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x466580
void FUN_00466580()
{
    if (g_game->blinkTimer > 0) {
        g_game->blinkTimer--;
        return;
    }
    g_game->blinkTimer = 7;
    g_game->blinkOn = !g_game->blinkOn;
}
