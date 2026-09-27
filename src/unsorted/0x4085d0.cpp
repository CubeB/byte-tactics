// Decompiled by Sonnet, class corrected by Opus. Names are provisional.
// Its vtable at 0x4fc9a8 has two slots: another class's vtable starts at
// 0x4fc9b0 (stored by 0x4087e0), right after it.

class Class_004085d0 {
public:
    virtual void FUN_00408100();
    virtual void FUN_00408600();

    int field_4;
    int field_8;
    int field_c;
    unsigned int field_10;

    Class_004085d0(int param_1, int param_2);
};

// FUNCTION: 0x4085d0
Class_004085d0::Class_004085d0(int param_1, int param_2)
    : field_4(param_1), field_8(param_2), field_c(0), field_10(*(unsigned char*)(param_1 + 4))
{
}
