// Decompiled by Opus. Names are provisional.
// Starts the fade handled by 0x41df20: ten steps, the first one due on the
// next tick.

#pragma pack(push, 1)
struct Game_0041dee0 {
    char unknown_0[0x3905f];
    unsigned int nextTime;             // +0x3905f
    int done;                          // +0x39063
    int steps;                         // +0x39067
};
#pragma pack(pop)

extern Game_0041dee0* g_game;

unsigned int __cdecl FUN_004b6340();

// FUNCTION: 0x41dee0
void FUN_0041dee0(void)
{
    g_game->steps = 10;
    g_game->nextTime = FUN_004b6340() + 1;
    g_game->done = 0;
}
