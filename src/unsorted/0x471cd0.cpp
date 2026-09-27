// Decompiled by Sonnet. Names are provisional.

class Class_00470ed0 {
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);
};

extern Class_00470ed0 DAT_0051e610;
extern void* DAT_004fd5a8[];

class Class_00471cd0 {
public:
    void** vtable;

    void* FUN_00471cd0(unsigned char flag);
};

// FUNCTION: 0x471cd0
void* Class_00471cd0::FUN_00471cd0(unsigned char flag)
{
    vtable = DAT_004fd5a8;
    if (flag & 1) {
        DAT_0051e610.FUN_00470ed0(this);
    }
    return this;
}
