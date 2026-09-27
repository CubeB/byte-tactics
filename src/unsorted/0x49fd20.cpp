// Decompiled by Sonnet. Names are provisional.

struct Vec6_49fd20 {
    int x;          // +0x0
    int y;          // +0x4
    char pad[16];   // +0x8 .. +0x17 (unused by this function)
};

struct Obj1_49fd20 {
    char unknown_0[0x3c];
    Vec6_49fd20 data;   // +0x3c
};

#pragma pack(push, 1)
struct Obj2_49fd20 {
    char unknown_0[0x13];
    short val_13;   // +0x13
    short val_15;   // +0x15
};
#pragma pack(pop)

// FUNCTION: 0x49fd20
void __stdcall FUN_0049fd20(Obj1_49fd20* param_1, Obj2_49fd20* param_2, Vec6_49fd20* param_3)
{
    *param_3 = param_1->data;
    param_3->x -= param_2->val_13;
    param_3->y -= param_2->val_15;
}
