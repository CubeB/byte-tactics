// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x148d7];
    void* logos;                    // +0x148d7
    void* logos32;                  // +0x148db
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadGaf(char* path);
void* __stdcall FindGafEntry(void* gaf, const char* name);

// FUNCTION: 0x4318c0
void FUN_004318c0()
{
    char path[256];
    FUN_004290f0(path, "textures", "logos", "GAF");
    g_game->logos = LoadGaf(path);
    g_game->logos32 = FindGafEntry(g_game->logos, "32xlogos");
}
