// Decompiled by Opus. Names are provisional.
// Loads the two game fonts (COMIX and smlfont) into g_game; the loader is
// 0x4292e0, inlined twice.

#pragma pack(push, 1)
struct Game_0042a320 {
    char unknown_0[0x391f9];
    void* field_391f9;                 // +0x391f9
    void* field_391fd;                 // +0x391fd
};
#pragma pack(pop)

extern Game_0042a320* g_game;

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_004bbe50(char* path, int flags);
void __stdcall FUN_004b6290(char* path);

static inline void* LoadFont(const char* name)
{
    char path[256];
    FUN_004290f0(path, "fonts", name, "FNT");
    void* font = FUN_004bbe50(path, 0);
    if (font == 0) {
        FUN_004b6290(path);
    }
    return font;
}

// FUNCTION: 0x42a320
void FUN_0042a320()
{
    g_game->field_391f9 = LoadFont("COMIX");
    g_game->field_391fd = LoadFont("smlfont");
}
