// Decompiled by Opus. Names are provisional.
// Loads bitmaps\<name>.PCX; reports the path when loading fails.

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadPcx(char* path, int param_2);
void __stdcall FatalError(char* path);

// FUNCTION: 0x429290
void* __stdcall LoadBitmapByName(const char* name, int param_2)
{
    char path[256];
    BuildDataPath(path, "bitmaps", name, "PCX");
    void* bitmap = LoadPcx(path, param_2);
    if (bitmap == 0) {
        FatalError(path);
    }
    return bitmap;
}
