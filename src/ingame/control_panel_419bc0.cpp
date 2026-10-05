// Decompiled by Haiku. Names are provisional.

struct Game {
    char unknown_0[0x2cc3];
    unsigned char field_2cc3;
    char unknown_2cc4[2];
    unsigned char field_2cc6;
};

extern Game* g_game;

// FUNCTION: 0x419bc0
void __stdcall FUN_00419bc0(unsigned char param_1)
{
    g_game->field_2cc3 = param_1;
    g_game->field_2cc6 = g_game->field_2cc6 & 0xf7;
}
