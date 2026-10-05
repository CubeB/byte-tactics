// Decompiled by Opus. Names are provisional.

struct Struct_004ba6b0 {
    char unknown_0[0xcc];
    void* gray_table;                  // +0xcc
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// FUNCTION: 0x4ba6b0
int __stdcall AllocGrayTable(Struct_004ba6b0* obj)
{
    obj->gray_table = FUN_004d83b0("GRAY TABLE", 0x100);
    return 1;
}
