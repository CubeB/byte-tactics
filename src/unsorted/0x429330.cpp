// Decompiled by Space Bunny Free. Names are provisional.
// Loads palettes\<name>.PAL; when that is missing, builds it from the
// palettes\PALETTE.PCX file (0x400 bytes) and tries to drop the cached
// ALP/LHT/SHD derivatives so they get rebuilt.

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bbc40(char* path);
void* __stdcall FUN_004bbe50(char* path, int flags);
void __stdcall FUN_004b6290(char* path);
void* __cdecl FUN_004d83b0(const char* name, int size);
int __stdcall FUN_004cb080(char* path, void* data);
int __stdcall FUN_004bc290(char* filename, void* data, int size);
void __stdcall FUN_004bbc30(char* path);

// FUNCTION: 0x429330
void* __stdcall FUN_00429330(char* name)
{
    char path[256];
    char namepath[256];
    void* result;

    FUN_004290f0(namepath, "palettes", name, "PAL");
    if (FUN_004bbc40(namepath) != 0) {
        result = FUN_004bbe50(namepath, 0);
        int bad = (result == 0);
        if (bad) {
            FUN_004b6290(namepath);
            return result;
        }
    }
    else {
        result = FUN_004d83b0("PALETTE", 0x400);
        FUN_004290f0(path, "palettes", name, "PCX");
        if (FUN_004cb080(path, result) == 0) {
            FUN_004b6290(path);
        }
        FUN_004bc290(namepath, result, 0x400);
        FUN_004290f0(path, "palettes", "PALETTE", "ALP");
        FUN_004bbc30(path);
        FUN_004290f0(path, "palettes", "PALETTE", "LHT");
        FUN_004bbc30(path);
        FUN_004290f0(path, "palettes", "PALETTE", "SHD");
        FUN_004bbc30(path);
    }
    return result;
}
