// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
struct Obj_4d7c20
{
    char unknown_0[0x24];
    void (__stdcall* fn_at_0x24)(int, int);  // +0x24
    int value_at_0x28;  // +0x28
};
#pragma pack(pop)

// FUNCTION: 0x4d7c20
void __stdcall FUN_004d7c20(int param1, Obj_4d7c20* param2)
{
    param2->fn_at_0x24(param2->value_at_0x28, param1);
}
