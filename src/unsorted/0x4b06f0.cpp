// Decompiled by Sonnet. Names are provisional.

void FUN_004d85a0(int* param_1);

class Class_004b06f0 {
public:
    char unknown_4[0x10 - 4];
    void* ptr10;    // +0x10
    void* ptr14;    // +0x14

    virtual void FUN_00() = 0; virtual void FUN_01() = 0; virtual void FUN_02() = 0;
    virtual void FUN_03() = 0; virtual void FUN_04() = 0; virtual void FUN_05() = 0;
    virtual void FUN_06() = 0; virtual void FUN_07() = 0; virtual void FUN_08() = 0;
    virtual void FUN_09() = 0; virtual void FUN_10() = 0; virtual void FUN_11() = 0;
    virtual void FUN_12() = 0; virtual void FUN_13() = 0; virtual void FUN_14() = 0;
    virtual void FUN_15() = 0; virtual void FUN_16() = 0; virtual void FUN_17() = 0;
    virtual void FUN_18() = 0; virtual void FUN_19() = 0; virtual void FUN_20() = 0;

    ~Class_004b06f0();
};

// FUNCTION: 0x4b06f0
Class_004b06f0::~Class_004b06f0()
{
    if (ptr14) {
        FUN_004d85a0((int*)ptr14);
    }
    if (ptr10) {
        FUN_004d85a0((int*)ptr10);
    }
}
