// Decompiled by Opus. Names are provisional.
// Loads palettes\PALETTE.SHD (a 0x2000-byte shade table) when it exists;
// otherwise builds it from the palette with FUN_004badf0 and saves it.
// Returns the table (freed again in the load case).

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bbc40(char* path);
void* __stdcall FUN_004bbe50(char* path, int flags);
void __stdcall FUN_004b6290(char* path);
void __stdcall FUN_004bab00(unsigned int* param_1);
void FUN_004d85a0(void* p);
unsigned int* __stdcall FUN_004badf0(void* palette);
int __stdcall FUN_004bc290(char* filename, void* data, int size);

// FUNCTION: 0x42e1d0
unsigned int* __stdcall FUN_0042e1d0(void* palette)
{
    char path[256];
    FUN_004290f0(path, "palettes", "PALETTE", "SHD");
    if (FUN_004bbc40(path)) {
        unsigned int* table = (unsigned int*)FUN_004bbe50(path, 0);
        if (table == 0) {
            FUN_004b6290(path);
        }
        FUN_004bab00(table);
        FUN_004d85a0(table);
        return table;
    }
    unsigned int* table = FUN_004badf0(palette);
    FUN_004bc290(path, table, 0x2000);
    return table;
}
