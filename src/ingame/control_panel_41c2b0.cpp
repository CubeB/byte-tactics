// Decompiled by Opus. Names are provisional.
#include <string.h>

extern char* g_game;

// FUNCTION: 0x41c2b0
void FUN_0041c2b0()
{
    char saved = *(g_game + 0x1434d);
    memset(g_game + 0x142f3, 0, 0x5c);
    *(g_game + 0x1434d) = saved;
}
