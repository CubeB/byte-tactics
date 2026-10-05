// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
struct Class_4b6220
{
    char unknown_0[0x1d2];
    int field_at_0x1d2;
};
#pragma pack(pop)

extern Class_4b6220* GetDisplay();

// FUNCTION: 0x4c22d0
void __stdcall FUN_004c22d0(int param)
{
    Class_4b6220* obj = GetDisplay();
    obj->field_at_0x1d2 = param;
}
