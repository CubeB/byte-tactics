// Decompiled by Opus. Names are provisional.
// Loads palettes\PALETTE.LHT (a 0x2000-byte lighting table) when it exists;
// otherwise builds it from the palette with BuildLightTable and saves it.
// Returns the table (freed again in the load case). Twin of 0x42e1d0 (SHD).

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall HAPI_FileLengthByName(char* path);
void* __stdcall HAPI_LoadFile(char* path, int flags);
void __stdcall FatalError(char* path);
void __stdcall SetLightTable(unsigned int* param_1);
void __cdecl FUN_004d85a0(void* p);
unsigned int* __stdcall BuildLightTable(void* palette);
int __stdcall WriteBufferToFile(char* filename, void* data, int size);

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
