// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Draws the local player's three light bar frames (one frame table per side)
// onto the blit surface: the first bar twice, the second copy also shifted
// down by the status bar height, then the third bar where it sits.
//
// The include of <stdio.h> is not used by the body. It is the missing piece
// that makes MSVC 5 materialise `side * 4` in edi only after the first table
// load and pick the game pointer as the addressing base; without it the same
// C++ compiles to the shared index built one instruction early and to the
// base/index operands swapped (95.7%).

#include <stdio.h>

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x95];
    unsigned char side;                 // +0x95
};

struct Player_467d70 {
    Unit* unit;                         // +0x00
    char unknown_4[0x14b - 4];
};

struct Game {
    char unknown_0[0x1b8a];
    Player_467d70 players[10];          // +0x1b8a
    char unknown_2878[0x2a43 - 0x2878];
    unsigned char playerIndex;          // +0x2a43
    char unknown_2a44[0x1481f - 0x2a44];
    unsigned short* field_1481f[5];     // +0x1481f
    unsigned short* field_14833[5];     // +0x14833
    unsigned short* field_14847[5];     // +0x14847
    char unknown_1485b[0x37e1b - 0x1485b];
    void* surface;                      // +0x37e1b
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall GetGafFrame(unsigned short* frames, int frame);
void __stdcall DrawFrame(void* surf, short* frame, int x, int y);
int GetScreenHeight();
void __stdcall SetOffscreenSurface(void* surf);
void __stdcall FillSurface(void* surf, int mode);
void FlipScreen();

// FUNCTION: 0x467d70
void FUN_00467d70()
{
    void* surf = g_game->surface;
    SetOffscreenSurface(surf);
    FillSurface(surf, 0);

    int side = g_game->players[g_game->playerIndex].unit->side;

    short* bar = (short*)GetGafFrame(g_game->field_1481f[side], 0);
    DrawFrame(surf, bar, bar[2] + 0x81, bar[3]);

    int dy = GetScreenHeight() - 0x20;
    bar = (short*)GetGafFrame(g_game->field_14833[side], 0);
    DrawFrame(surf, bar, bar[2] + 0x81, bar[3] + dy);

    bar = (short*)GetGafFrame(g_game->field_14847[side], 0);
    DrawFrame(surf, bar, bar[2], bar[3]);

    FlipScreen();
}
