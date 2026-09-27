// Decompiled by Sonnet. Names are provisional.
// Sibling of 0x471cd0.cpp (own vtable store, DAT_0051e610.FUN_00470ed0(this)
// pool free) and 0x474d10.cpp (buffer delete + zero at +0x10/+0x14/+0x18).

extern void* DAT_004fd5a8[];

class Class_00470ed0 {
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);
};

extern Class_00470ed0 DAT_0051e610;

class Class_00471430 {
public:
    void** vtable;    // +0x0
    char unknown_4[0x10 - 4];
    void* buffer;     // +0x10
    void* field_14;   // +0x14
    void* field_18;   // +0x18

    void* FUN_00471430(unsigned char flag);
};

// FUNCTION: 0x471430
void* Class_00471430::FUN_00471430(unsigned char flag)
{
    void* temp;
    void* b = buffer;
    *(void* volatile*)&temp = b;
    operator delete(b);
    buffer = 0;
    field_14 = 0;
    field_18 = 0;
    vtable = DAT_004fd5a8;
    if (flag & 1) {
        DAT_0051e610.FUN_00470ed0(this);
    }
    return this;
}
