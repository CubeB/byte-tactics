// Decompiled by Opus. Names are provisional.

struct Struct_004ba660 {
    char unknown_0[0xc8];
    void* light_table;                 // +0xc8
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// FUNCTION: 0x4ba660
int __stdcall AllocLightTable(Struct_004ba660* obj)
{
    obj->light_table = FUN_004d83b0("LIGHT TABLE", 0x2000);
    return 1;
}
