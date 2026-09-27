// Decompiled by Haiku. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391cf];
    char field_391cf[24];
    char field_391e7;
    char field_391e8;
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x41da30
void FUN_0041da30(void)
{
    memset(&g_game->field_391cf[0], 0x55, 25);
    g_game->field_391e8 = 0;
}
