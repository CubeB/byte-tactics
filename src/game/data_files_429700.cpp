// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

struct AnimEntry_00429700 {
    char name[0x40];         // +0x0
    void* gaf;               // +0x40
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x147ab];
    int animCount;           // +0x147ab
    AnimEntry_00429700* anims;  // +0x147af
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadGaf(char* path);
void __stdcall FatalError(char* path);
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);

// FUNCTION: 0x429700
void* __stdcall LoadAnimGaf(char* name)
{
    char path[256];
    int i;
    for (i = 0; i < g_game->animCount; i++) {
        if (_strcmpi(g_game->anims[i].name, name) == 0) {
            return g_game->anims[i].gaf;
        }
    }
    BuildDataPath(path, "anims", name, "GAF");
    void* gaf = LoadGaf(path);
    if (gaf == 0) {
        FatalError(path);
    }
    g_game->anims = (AnimEntry_00429700*)FUN_004d84a0(g_game->anims, "Animation Files",
                                                      (g_game->animCount + 1) * 0x44);
    strcpy(g_game->anims[g_game->animCount].name, name);
    g_game->anims[g_game->animCount].gaf = gaf;
    g_game->animCount++;
    return gaf;
}
