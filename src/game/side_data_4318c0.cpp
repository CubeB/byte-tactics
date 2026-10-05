// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_004318c0 {
    char unknown_0[0x148d7];
    void* logos;                    // +0x148d7
    void* logos32;                  // +0x148db
};
#pragma pack(pop)

extern Game_004318c0* g_game;

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_004b8c60(char* path);
void* __stdcall FUN_004b8d40(void* gaf, const char* name);

// FUNCTION: 0x4318c0
void FUN_004318c0()
{
    char path[256];
    FUN_004290f0(path, "textures", "logos", "GAF");
    g_game->logos = FUN_004b8c60(path);
    g_game->logos32 = FUN_004b8d40(g_game->logos, "32xlogos");
}
