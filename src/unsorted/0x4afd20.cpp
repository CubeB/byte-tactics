// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Inner_004afd20 {
    char unknown_0[0xa0];
    int field_a0;                      // +0xa0
};

struct Obj_004afd20 {
    char unknown_0[0xa6];
    Inner_004afd20* field_a6;          // +0xa6
};
#pragma pack(pop)

// FUNCTION: 0x4afd20
void __stdcall FUN_004afd20(Obj_004afd20* obj, int value)
{
    obj->field_a6->field_a0 = value;
}
