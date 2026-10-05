// Decompiled by Haiku. Names are provisional.

extern void __cdecl FUN_004d85a0(void* ptr);

struct Obj_004ba730 {
public:
    char unknown_0[0xd0];
    void* ptr_at_0xd0;
};

// FUNCTION: 0x4ba730
void __stdcall FUN_004ba730(Obj_004ba730* obj)
{
    FUN_004d85a0(obj->ptr_at_0xd0);
}
