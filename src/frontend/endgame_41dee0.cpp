// Decompiled by Opus. Names are provisional.
// Starts the fade handled by 0x41df20: ten steps, the first one due on the
// next tick.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x3905f];
    unsigned int nextTime;             // +0x3905f
    int done;                          // +0x39063
    int steps;                         // +0x39067
};
#pragma pack(pop)

extern Game* g_game;

unsigned int __cdecl GetTicks();

// FUNCTION: 0x41dee0
void StartScreenFade(void)
{
    g_game->steps = 10;
    g_game->nextTime = GetTicks() + 1;
    g_game->done = 0;
}
