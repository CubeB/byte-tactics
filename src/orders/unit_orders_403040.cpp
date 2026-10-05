// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Sub_403040 {
    char unknown_0[0x245];
    unsigned int unused_bits : 2;
    unsigned int flag : 1;      // +0x245, bit 2
    unsigned int unused_bits2 : 29;
};

struct Obj_403040 {
    char unknown_0[0x92];
    Sub_403040* sub;            // +0x92
};
#pragma pack(pop)

class Class_0048b090 {
public:
    void SetStateBits(int param_1, int param_2);
};

// FUNCTION: 0x403040
int __stdcall FUN_00403040(Obj_403040* param_1, int unused1, int unused2)
{
    if (param_1->sub->flag) {
        ((Class_0048b090*)param_1)->SetStateBits(1, 0);
    }
    return 5;
}
