// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a53];
    char path[0x20c];                  // +0x38a53
    int valueSet;                      // +0x38c5f
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

// Command arguments.
class Class_004b73c0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    char* FUN_004b73c0(int index, char* fallback);
};

void SaveSettings();

// FUNCTION: 0x417330
void __stdcall CmdFilm(Class_004b73c0* args)
{
    if (args->count > 1) {
        strcpy(g_game->path, args->FUN_004b73c0(1, DAT_005119b8));
        if (g_game->path[strlen(g_game->path) - 1] == '\\' ||
            g_game->path[strlen(g_game->path) - 1] == '/')
            g_game->path[strlen(g_game->path) - 1] = 0;
        g_game->valueSet = 1;
        SaveSettings();
    }
}
