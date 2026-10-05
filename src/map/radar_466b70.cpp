// Decompiled by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// Without a header in front, MSVC 5 puts each short in a scratch register and
// the int in eax (`movsx edx, ...; mov eax, ...; imul eax, edx`, 75%); the
// original's `movsx eax, ...; imul eax, [mem]` comes from the compiler state
// after <windows.h>, which tools/headers.py found. The source is unchanged.
#include <windows.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1422b];
    int divX;                          // +0x1422b
    int divY;                          // +0x1422f
    char unknown_14233[0x1423b - 0x14233];
    int zoomX;                         // +0x1423b
    int zoomY;                         // +0x1423f
    char unknown_14243[0x142e7 - 0x14243];
    short originX;                     // +0x142e7
    short originY;                     // +0x142e9
    short sizeX;                       // +0x142eb
    short sizeY;                       // +0x142ed
    char unknown_142ef[0x1431f - 0x142ef];

    int scaleX;                        // +0x1431f
    int scaleY;                        // +0x14323
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x466b70
void __stdcall FUN_00466b70(int* param_1)
{
    param_1[0] = g_game->sizeX * g_game->scaleX / g_game->divX + g_game->originX;
    param_1[1] = g_game->sizeY * g_game->scaleY / g_game->divY + g_game->originY;
    param_1[2] = g_game->sizeX * g_game->zoomX * 16 / g_game->divX + param_1[0] - 1;
    param_1[3] = g_game->sizeY * g_game->zoomY * 16 / g_game->divY + param_1[1] - 1;
}
