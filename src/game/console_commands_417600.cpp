// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>
#include <string.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1422b];
    int mapWidth;                      // +0x1422b
    int mapHeight;                     // +0x1422f
    char unknown_14233[0x1423b - 0x14233];
    int screenTilesX;                  // +0x1423b
    int screenTilesY;                  // +0x1423f
    char unknown_14243[0x1431f - 0x14243];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x38a37 - 0x14327];
    int lastShotTime;                  // +0x38a37
    char unknown_38a3b[0x38a53 - 0x38a3b];
    char installPath[1];               // +0x38a53
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

// Command-line arguments, as used by FUN_004b73c0 and FUN_004b73e0.
class Class_004b73c0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    char* FUN_004b73c0(int index, char* fallback);
};

class Class_004b73e0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int FUN_004b73e0(int index, int fallback);
};

void __stdcall FUN_004bcf00(char* path);
void __stdcall WriteScreenshot(char* name, char* description, int x, int y, int w, int h);
unsigned int GetTicks();

// FUNCTION: 0x417600
void __stdcall CmdMakePoster(Class_004b73c0* args)
{
    int w = 0xc80;
    int h = 0x960;
    if (args->count > 1)
        w = ((Class_004b73e0*)args)->FUN_004b73e0(1, 0);
    if (args->count > 2)
        h = ((Class_004b73e0*)args)->FUN_004b73e0(2, 0);
    if (_strcmpi(args->FUN_004b73c0(1, DAT_005119b8), "all") == 0) {
        w = g_game->mapWidth;
        h = g_game->mapHeight;
    }
    int sx = g_game->screenTilesX;
    w = max(w, sx * 16);
    w = min(w, g_game->mapWidth);
    int sy = g_game->screenTilesY;
    h = max(h, sy * 16);
    h = min(h, g_game->mapHeight);
    int x = g_game->scrollX - w / 2 + sx * 8;
    int y = g_game->scrollY - h / 2 + sy * 8;
    x = max(x, 0);
    x = min(x, g_game->mapWidth - w);
    y = max(y, 0);
    y = min(y, g_game->mapHeight - h);
    char buf[256];
    sprintf(buf, "%s\\screenshots", g_game->installPath);
    FUN_004bcf00(buf);
    WriteScreenshot(buf, "BIGSHOT", x, y, w, h);
    g_game->lastShotTime = GetTicks();
}
