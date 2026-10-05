// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Entry_00428730 {
    void* surface;                     // +0x00
    int* data;                         // +0x04
    int unknown_8[8];                  // +0x08
};

struct Owner_00428730 {
    char unknown_0[0x24];
    void* surface;                     // +0x24
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x531];
    Owner_00428730* owner;             // +0x531
    char unknown_535[0x11eb - 0x535];
    void* surface;                     // +0x11eb
};
#pragma pack(pop)

extern Game* g_game;
extern Entry_00428730 DAT_005120b8[10];

void __stdcall FreeSurface(void* param_1);
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x428730
void FUN_00428730()
{
    for (int i = 0; i < 10; i++) {
        if (DAT_005120b8[i].surface != 0) {
            FreeSurface(DAT_005120b8[i].surface);
            FUN_004d85a0(DAT_005120b8[i].data);
            if (g_game->surface == DAT_005120b8[i].surface) {
                g_game->surface = 0;
            }
            if (g_game->owner != 0 && g_game->owner->surface == DAT_005120b8[i].surface) {
                g_game->owner->surface = 0;
            }
            DAT_005120b8[i].surface = 0;
            DAT_005120b8[i].data = 0;
            memset(DAT_005120b8[i].unknown_8, 0, sizeof(DAT_005120b8[i].unknown_8));
        }
    }
}
