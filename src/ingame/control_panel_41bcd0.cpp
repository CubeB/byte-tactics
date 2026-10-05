// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 2)
struct Sub_41bcd0 {
    char unknown_0[0x186];
    float divisor;                       // +0x186
    char unknown_18a[0x1ea - 0x186 - 4];
    int field_1ea;                       // +0x1ea
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Obj_41bcd0 {
    char unknown_0[0x92];
    Sub_41bcd0* sub;                     // +0x92
};
#pragma pack(pop)

extern void __stdcall AddBuildProgress(void* param_1, void* param_2, float param_3);

// FUNCTION: 0x41bcd0
void __stdcall FUN_0041bcd0(Obj_41bcd0* param_1, int param_2)
{
    float val = -((float)(param_1->sub->field_1ea * param_2) / param_1->sub->divisor);
    AddBuildProgress(param_1, param_1, val);
}
