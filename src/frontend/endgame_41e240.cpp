// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x3905f];
    unsigned int nextTime;             // +0x3905f
};
#pragma pack(pop)

extern Game* g_game;

unsigned int __cdecl FUN_004b6340();

// FUNCTION: 0x41e240
void FUN_0041e240(void)
{
    g_game->nextTime = FUN_004b6340() + 1;
}
