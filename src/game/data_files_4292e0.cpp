// Decompiled by Opus. Names are provisional.

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_004bbe50(char* path, int flags);
void __stdcall FUN_004b6290(char* path);

// FUNCTION: 0x4292e0
void* __stdcall FUN_004292e0(const char* name)
{
    char path[256];
    FUN_004290f0(path, "fonts", name, "FNT");
    void* font = FUN_004bbe50(path, 0);
    if (font == 0) {
        FUN_004b6290(path);
    }
    return font;
}
