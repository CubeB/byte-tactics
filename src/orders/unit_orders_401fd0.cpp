// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Obj {
    char unknown_0[5];
    unsigned char state;   // +5
    int field_6;            // +6 (unaligned)
    char unknown_a[0x16 - 0xa];
    int flag_16;             // +0x16 (unaligned)
};
#pragma pack(pop)

// FUNCTION: 0x401fd0
int __stdcall FUN_00401fd0(int unused1, Obj* obj, int unused3)
{
    if (obj->flag_16 == 0) {
        return 5;
    }
    unsigned int s = 0;
    s = obj->state;
    switch (s) {
    case 0:
        obj->field_6 = 0x18;
        return 1;
    case 1:
        return 5;
    default:
        return 7;
    }
}
