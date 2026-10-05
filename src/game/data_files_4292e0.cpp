// Decompiled by Opus. Names are provisional.

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall HAPI_LoadFile(char* path, int flags);
void __stdcall FatalError(char* path);

// FUNCTION: 0x4292e0
void* __stdcall LoadFontByName(const char* name)
{
    char path[256];
    BuildDataPath(path, "fonts", name, "FNT");
    void* font = HAPI_LoadFile(path, 0);
    if (font == 0) {
        FatalError(path);
    }
    return font;
}
