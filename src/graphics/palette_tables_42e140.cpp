// Decompiled by Opus. Names are provisional.
// Loads palettes\PALETTE.ALP (a 0x10000-byte table) when it exists;
// otherwise builds it from the palette with BuildAlphaTable and saves it.
// Returns the table (freed again in the load case). Same shape as 0x42e1d0.

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bbc40(char* path);
void* __stdcall FUN_004bbe50(char* path, int flags);
void __stdcall FatalError(char* path);
void __stdcall SetAlphaTable(unsigned int* param_1);
void __cdecl FUN_004d85a0(void* p);
unsigned int* __stdcall BuildAlphaTable(void* palette);
int __stdcall FUN_004bc290(char* filename, void* data, int size);

// FUNCTION: 0x42e140
unsigned int* __stdcall LoadAlphaTable(void* palette)
{
    char path[256];
    BuildDataPath(path, "palettes", "PALETTE", "ALP");
    if (FUN_004bbc40(path)) {
        unsigned int* table = (unsigned int*)FUN_004bbe50(path, 0);
        if (table == 0) {
            FatalError(path);
        }
        SetAlphaTable(table);
        FUN_004d85a0(table);
        return table;
    }
    unsigned int* table = BuildAlphaTable(palette);
    FUN_004bc290(path, table, 0x10000);
    return table;
}
