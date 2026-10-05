// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Sub_403010 {
    char unknown_0[0x245];
    unsigned int unused_bits : 2;
    unsigned int flag : 1;      // +0x245, bit 2
    unsigned int unused_bits2 : 29;
};

struct Obj_403010 {
    char unknown_0[0x92];
    Sub_403010* sub;            // +0x92
};
#pragma pack(pop)

class Class_0048b090 {
public:
    void SetStateBits(int param_1, int param_2);
};

// FUNCTION: 0x403010
int __stdcall FUN_00403010(Obj_403010* param_1, int unused1, int unused2)
{
    if (param_1->sub->flag) {
        ((Class_0048b090*)param_1)->SetStateBits(1, 1);
    }
    return 5;
}
