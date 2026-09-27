// Decompiled by Opus. Names are provisional.
// Loads bitmaps\<name>.PCX; reports the path when loading fails.

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_004caf30(char* path, int param_2);
void __stdcall FUN_004b6290(char* path);

// FUNCTION: 0x429290
void* __stdcall FUN_00429290(const char* name, int param_2)
{
    char path[256];
    FUN_004290f0(path, "bitmaps", name, "PCX");
    void* bitmap = FUN_004caf30(path, param_2);
    if (bitmap == 0) {
        FUN_004b6290(path);
    }
    return bitmap;
}
