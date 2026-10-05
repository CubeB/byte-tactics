// Decompiled by Opus. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Cell_00424840 {
    char unknown_0[0x8];
    unsigned short owner;               // +0x8
    char unknown_a[0xd - 0xa];
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00424840* cells;               // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall RemoveFeature(void* target, int flag);

// FUNCTION: 0x424840
void RemoveAllFeatures(void)
{
    int n = g_game->width * g_game->height;
    for (int i = 0; i < n; i++) {
        Cell_00424840* c = &g_game->cells[i];
        if (c->owner < 0xfffb || c->owner == 0xfffe) {
            RemoveFeature(c, 1);
        }
    }
}
