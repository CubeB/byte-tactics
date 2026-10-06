// Decompiled by Opus, Haiku and Sonnet. Names are provisional.

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall HAPI_FileLengthByName(char* path);
void* __stdcall HAPI_LoadFile(char* path, int flags);
void __stdcall FatalError(char* path);
void __stdcall SetAlphaTable(unsigned int* param_1);
void __stdcall SetShadeTable(unsigned int* param_1);
void __stdcall SetLightTable(unsigned int* param_1);
void __cdecl FUN_004d85a0(void* p);
unsigned int* __stdcall BuildAlphaTable(void* palette);
unsigned int* __stdcall BuildShadeTable(void* palette);
unsigned int* __stdcall BuildLightTable(void* palette);
int __stdcall WriteBufferToFile(char* filename, void* data, int size);
void __stdcall BuildGrayTable(unsigned int param_1);
void __stdcall BuildBlueTable(unsigned int param_1);

// Loads palettes\PALETTE.ALP (a 0x10000-byte table) when it exists;
// otherwise builds it from the palette with BuildAlphaTable and saves it.
// Returns the table (freed again in the load case). Same shape as 0x42e1d0.
// FUNCTION: 0x42e140
unsigned int* __stdcall LoadAlphaTable(void* palette)
{
    char path[256];
    BuildDataPath(path, "palettes", "PALETTE", "ALP");
    if (HAPI_FileLengthByName(path)) {
        unsigned int* table = (unsigned int*)HAPI_LoadFile(path, 0);
        if (table == 0) {
            FatalError(path);
        }
        SetAlphaTable(table);
        FUN_004d85a0(table);
        return table;
    }
    unsigned int* table = BuildAlphaTable(palette);
    WriteBufferToFile(path, table, 0x10000);
    return table;
}

// Loads palettes\PALETTE.SHD (a 0x2000-byte shade table) when it exists;
// otherwise builds it from the palette with BuildShadeTable and saves it.
// Returns the table (freed again in the load case).
// FUNCTION: 0x42e1d0
unsigned int* __stdcall LoadShadeTable(void* palette)
{
    char path[256];
    BuildDataPath(path, "palettes", "PALETTE", "SHD");
    if (HAPI_FileLengthByName(path)) {
        unsigned int* table = (unsigned int*)HAPI_LoadFile(path, 0);
        if (table == 0) {
            FatalError(path);
        }
        SetShadeTable(table);
        FUN_004d85a0(table);
        return table;
    }
    unsigned int* table = BuildShadeTable(palette);
    WriteBufferToFile(path, table, 0x2000);
    return table;
}

// Loads palettes\PALETTE.LHT (a 0x2000-byte lighting table) when it exists;
// otherwise builds it from the palette with BuildLightTable and saves it.
// Returns the table (freed again in the load case). Twin of 0x42e1d0 (SHD).
// FUNCTION: 0x42e260
unsigned int* __stdcall LoadLightTable(void* palette)
{
    char path[256];
    BuildDataPath(path, "palettes", "PALETTE", "LHT");
    if (HAPI_FileLengthByName(path)) {
        unsigned int* table = (unsigned int*)HAPI_LoadFile(path, 0);
        if (table == 0) {
            FatalError(path);
        }
        SetLightTable(table);
        FUN_004d85a0(table);
        return table;
    }
    unsigned int* table = BuildLightTable(palette);
    WriteBufferToFile(path, table, 0x2000);
    return table;
}

// FUNCTION: 0x42e2f0
void __stdcall MakeGrayTable(unsigned int param_1)
{
    BuildGrayTable(param_1);
}

// FUNCTION: 0x42e300
void __stdcall MakeBlueTable(unsigned int param_1)
{
    BuildBlueTable(param_1);
}
