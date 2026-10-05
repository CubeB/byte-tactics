// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x148df];
    int count;                         // +0x148df
    int* texturePtrs;                  // +0x148e3
    int unknown_148e7;                 // +0x148e7
    int* unknown_148eb;                // +0x148eb
    char unknown_148ef[0x38d6f - 0x148ef];
    char progress;                     // +0x38d6f
};
#pragma pack(pop)

struct FindData_0042a440 {
    char unknown_0[0x14];
    char name[260];                    // +0x14
};

extern Game* g_game;

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bc930(const char* path, int flag);
void* __cdecl FUN_004d83b0(const char* tag, int size);
int __stdcall FUN_004bc4b0(const char* path, FindData_0042a440* fd, int a, int b);
int __stdcall FUN_004bc640(int handle, FindData_0042a440* fd);
void __stdcall FUN_004bc8d0(int handle);
void* __stdcall LoadGaf(char* path);

// FUNCTION: 0x42a440
void LoadTextureGafs()
{
    char path[256];
    FindData_0042a440 fd;

    BuildDataPath(path, "textures", "*", "GAF");
    int count = FUN_004bc930(path, 0);
    g_game->count = count - 1;
    int* texturePtrs = (int*)FUN_004d83b0("TEXTURE PTRS", count * 4);
    g_game->texturePtrs = texturePtrs;
    int handle = FUN_004bc4b0(path, &fd, -1, 1);
    if (handle != -1) {
        int progress = 0;
        int r;
        do {
            if (_strcmpi(fd.name, "logos.GAF") == 0) {
                texturePtrs--;
            } else {
                BuildDataPath(path, "textures", fd.name, "GAF");
                *texturePtrs = (int)LoadGaf(path);
                g_game->progress = (char)(progress / (count - 1));
            }
            r = FUN_004bc640(handle, &fd);
            texturePtrs++;
            progress += 100;
        } while (r == 0);
        g_game->progress = 100;
        FUN_004bc8d0(handle);
    }
    g_game->unknown_148e7 = 0;
    g_game->unknown_148eb = 0;
}
