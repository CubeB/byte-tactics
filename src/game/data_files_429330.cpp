// Decompiled by Space Bunny Free. Names are provisional.
// Loads palettes\<name>.PAL; when that is missing, builds it from the
// palettes\PALETTE.PCX file (0x400 bytes) and tries to drop the cached
// ALP/LHT/SHD derivatives so they get rebuilt.

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall HAPI_FileLengthByName(char* path);
void* __stdcall HAPI_LoadFile(char* path, int flags);
void __stdcall FatalError(char* path);
void* __cdecl FUN_004d83b0(const char* name, int size);
int __stdcall LoadPcxPalette(char* path, void* data);
int __stdcall WriteBufferToFile(char* filename, void* data, int size);
void __stdcall RemoveFile(char* path);

// FUNCTION: 0x429330
void* __stdcall LoadPaletteByName(char* name)
{
    char path[256];
    char namepath[256];
    void* result;

    BuildDataPath(namepath, "palettes", name, "PAL");
    if (HAPI_FileLengthByName(namepath) != 0) {
        result = HAPI_LoadFile(namepath, 0);
        int bad = (result == 0);
        if (bad) {
            FatalError(namepath);
            return result;
        }
    }
    else {
        result = FUN_004d83b0("PALETTE", 0x400);
        BuildDataPath(path, "palettes", name, "PCX");
        if (LoadPcxPalette(path, result) == 0) {
            FatalError(path);
        }
        WriteBufferToFile(namepath, result, 0x400);
        BuildDataPath(path, "palettes", "PALETTE", "ALP");
        RemoveFile(path);
        BuildDataPath(path, "palettes", "PALETTE", "LHT");
        RemoveFile(path);
        BuildDataPath(path, "palettes", "PALETTE", "SHD");
        RemoveFile(path);
    }
    return result;
}
