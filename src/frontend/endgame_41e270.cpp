// Decompiled by Claude Opus 5.5. Names are provisional.
// One tick of the palette fade set up by 0x41dfc0: once the next tick is
// due, moves every channel of the current palette by its step without
// overshooting the target, marks the fade done when the palettes are equal,
// shows the palette and schedules the next tick.
// <string.h> alone (or <windows.h>) gives the original register order;
// adding <ddraw.h> swaps the delta and current loads.
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x3905f];
    unsigned int nextTime;             // +0x3905f
    int done;                          // +0x39063
    char unknown_39067[0x39083 - 0x39067];
    unsigned char* current;            // +0x39083
    unsigned char* target;             // +0x39087
    char* delta;                       // +0x3908b
};
#pragma pack(pop)

extern Game* g_game;

unsigned int __cdecl FUN_004b6340();
void __stdcall FUN_004ba200(unsigned char* palette, int first, int count);

#define FADE_TICK(k)                                                        \
    {                                                                       \
        int v = g_game->current[k] + g_game->delta[k];                      \
        g_game->current[k] = g_game->delta[k] < 0                           \
            ? (v < g_game->target[k] ? g_game->target[k] : v)               \
            : (v > g_game->target[k] ? g_game->target[k] : v);              \
    }

// FUNCTION: 0x41e270
void FUN_0041e270(void)
{
    if (g_game->nextTime <= FUN_004b6340()) {
        for (int i = 0; i < 0x100; i++) {
            FADE_TICK(i * 4)
            FADE_TICK(i * 4 + 1)
            FADE_TICK(i * 4 + 2)
            FADE_TICK(i * 4 + 3)
        }
        if (memcmp(g_game->current, g_game->target, 0x400) == 0)
            g_game->done = 1;
        FUN_004ba200(g_game->current, 0, 0x100);
        g_game->nextTime = FUN_004b6340() + 1;
    }
}
