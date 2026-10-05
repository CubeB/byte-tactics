// Decompiled by Space Bunny Free. Names are provisional.
// Finds or adds a sound in the game's 32-byte sound tables: looks it up by
// logical name (in the +0x33e13 table) when one is given, otherwise by file
// name (in the +0x35e13 table), and loads the file through FUN_0047efe0 when
// the sound is new. Returns the index, or 0 when the table is full.
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x33a0f];
    int soundCount;                     // +0x33a0f
    void* sounds[0x100];                // +0x33a13
    char soundNames[0x100][0x20];       // +0x33e13
    char soundFiles[0x100][0x20];       // +0x35e13
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

void* __stdcall FUN_0047efe0(const char* name);

// FUNCTION: 0x429470
int __stdcall FUN_00429470(char* name, const char* file)
{
    int i;
    for (i = 0; i < g_game->soundCount; i++) {
        if (name != 0) {
            if (g_game->soundNames[i][0] != 0 &&
                _strnicmp(g_game->soundNames[i], name, 0x20) == 0)
                return i;
        } else {
            if (_strnicmp(g_game->soundFiles[i], file, 0x20) == 0)
                return i;
        }
    }
    if (g_game->soundCount + 1 >= 0x100)
        return 0;
    g_game->sounds[g_game->soundCount] = FUN_0047efe0(file);
    strncpy(g_game->soundFiles[g_game->soundCount], file, 0x20);
    if (name != 0)
        strncpy(g_game->soundNames[g_game->soundCount], name, 0x20);
    else
        strcpy(g_game->soundNames[g_game->soundCount], DAT_005119b8);
    g_game->soundCount++;
    return i;
}
