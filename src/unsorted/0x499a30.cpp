// Decompiled by Opus. Names are provisional.
// Allocates and clears the 0x7d64-byte weapon array, then resets its count.
#include <string.h>

#pragma pack(push, 1)
struct Game_00499a30 {
    char unknown_0[0x141f3];
    int field_141f3;                   // +0x141f3
    void* field_141f7;                 // +0x141f7
};
#pragma pack(pop)

extern Game_00499a30* g_game;

void* FUN_004d83b0(char* name, unsigned int size);

// FUNCTION: 0x499a30
void FUN_00499a30(void)
{
    g_game->field_141f7 = FUN_004d83b0("WEAPON ARRAY", 0x7d64);
    memset(g_game->field_141f7, 0, 0x7d64);
    g_game->field_141f3 = 0;
}
