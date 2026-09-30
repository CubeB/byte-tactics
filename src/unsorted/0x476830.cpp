// Decompiled by space-bunny-free. Names are provisional.
// Builds the "SideList" string for the menu: allocates count * 30 bytes,
// appends every side name from g_game+0x37f3d (stride 0x232) with a n extra
// terminating nul after each, then walks the finished list once per side
// adding 0x20 to every non-zero character, which lowercases the names (and
// also mangles any character that is not an upper-case letter).
#include <string.h>

#pragma pack(push, 1)
struct Game_476830 {
    char unknown_0[0x37f39];
    int count;                         // +0x37f39
    char names[1][0x232];              // +0x37f3d
};
#pragma pack(pop)

extern Game_476830* g_game;
char* __cdecl FUN_004d83b0(char* name, int size);

// FUNCTION: 0x476830
char* FUN_00476830()
{
    char* buf = FUN_004d83b0("SideList", g_game->count * 30);
    char* p = buf;
    int i;
    int j;

    buf[0] = 0;
    for (i = 0; i < g_game->count; i++) {
        p = strcat(p, g_game->names[i]);
        p = p + strlen(p) + 1;
        p[0] = 0;
    }
    p = buf;
    for (j = 0; j < g_game->count; j++) {
        for (;;) {
            char c = *++p;
            if (c == 0) {
                break;
            }
            p[0] = c + 0x20;
        }
        p++;
    }
    return buf;
}
