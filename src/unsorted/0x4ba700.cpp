// Decompiled by Opus. Names are provisional.

struct Struct_004ba700 {
    char unknown_0[0xd0];
    void* blue_table;                  // +0xd0
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// FUNCTION: 0x4ba700
int __stdcall FUN_004ba700(Struct_004ba700* obj)
{
    obj->blue_table = FUN_004d83b0("BLUE TABLE", 0x100);
    return 1;
}
