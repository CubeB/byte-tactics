// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

struct AnimEntry_00429700 {
    char name[0x40];         // +0x0
    void* gaf;               // +0x40
};

#pragma pack(push, 1)
struct Game_00429700 {
    char unknown_0[0x147ab];
    int animCount;           // +0x147ab
    AnimEntry_00429700* anims;  // +0x147af
};
#pragma pack(pop)

extern Game_00429700* g_game;

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_004b8c60(char* path);
void __stdcall FUN_004b6290(char* path);
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);

// FUNCTION: 0x429700
void* __stdcall FUN_00429700(char* name)
{
    char path[256];
    int i;
    for (i = 0; i < g_game->animCount; i++) {
        if (_strcmpi(g_game->anims[i].name, name) == 0) {
            return g_game->anims[i].gaf;
        }
    }
    FUN_004290f0(path, "anims", name, "GAF");
    void* gaf = FUN_004b8c60(path);
    if (gaf == 0) {
        FUN_004b6290(path);
    }
    g_game->anims = (AnimEntry_00429700*)FUN_004d84a0(g_game->anims, "Animation Files",
                                                      (g_game->animCount + 1) * 0x44);
    strcpy(g_game->anims[g_game->animCount].name, name);
    g_game->anims[g_game->animCount].gaf = gaf;
    g_game->animCount++;
    return gaf;
}
