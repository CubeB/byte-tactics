// Decompiled by Claude Opus 5.5. Names are provisional.
// Starts a palette fade: keeps copies of the target and current palettes,
// shows the current one and works out a signed step per channel (at least
// 1 towards the target) that 0x41e270 then adds on each tick.
// Needs a header set such as <windows.h> + <ddraw.h> (or <memory.h> alone)
// for the loop pointers to be advanced in the original order.
#include <windows.h>
#include <ddraw.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x39063];
    int done;                          // +0x39063
    char unknown_39067[0x39083 - 0x39067];
    unsigned char* current;            // +0x39083
    unsigned char* target;             // +0x39087
    char* delta;                       // +0x3908b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);

#define FADE_STEP(k)                                                        \
    if (current[k] > target[k]) {                                           \
        g_game->delta[k] = min(-1, (target[k] - current[k]) / steps);       \
    } else if (current[k] == target[k]) {                                   \
        g_game->delta[k] = 0;                                               \
    } else {                                                                \
        g_game->delta[k] = max(1, (target[k] - current[k]) / steps);        \
    }

// FUNCTION: 0x41dfc0
void __stdcall StartPaletteFade(unsigned char* target, unsigned char* current, int steps)
{
    memcpy(g_game->target, target, 0x400);
    memcpy(g_game->current, current, 0x400);
    SetPaletteColors(current, 0, 0x100);
    g_game->done = 0;
    for (int i = 0; i < 0x100; i++) {
        FADE_STEP(i * 4)
        FADE_STEP(i * 4 + 1)
        FADE_STEP(i * 4 + 2)
        FADE_STEP(i * 4 + 3)
    }
}
