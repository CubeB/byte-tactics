// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
struct Struct_4ba5f0
{
    char unknown_0[0xc0];
    void* ptr_at_0xc0;
};
#pragma pack(pop)

extern void __cdecl FUN_004d85a0(void*);

// FUNCTION: 0x4ba5f0
void __stdcall FreeAlphaTable(Struct_4ba5f0* param)
{
    FUN_004d85a0(param->ptr_at_0xc0);
}
