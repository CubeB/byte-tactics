// Decompiled by Sonnet. Names are provisional.

class Class_00408810 {
public:
    virtual void FUN_00408100();
    virtual void FUN_00408600();
    virtual void FUN_004086d0();
    virtual void* FUN_00408810(unsigned char flag);

    int field_4;
    int field_8;
    int field_c;
    unsigned int field_10;

    Class_00408810(int param_1, int param_2);
};

// FUNCTION: 0x4085d0
Class_00408810::Class_00408810(int param_1, int param_2)
    : field_4(param_1), field_8(param_2), field_c(0), field_10(*(unsigned char*)(param_1 + 4))
{
}
