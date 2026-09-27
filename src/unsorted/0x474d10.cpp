// Decompiled by Sonnet. Names are provisional.

class Class_00471d00 {
public:
    void FUN_00471d00();
};

extern void __stdcall FUN_00471d50(void* obj);

class Class_00474d10 {
public:
    char unknown_0[0x10];
    void* buffer;   // +0x10
    void* field_14; // +0x14
    void* field_18; // +0x18

    void* FUN_00474d10(unsigned char flag);
};

// FUNCTION: 0x474d10
void* Class_00474d10::FUN_00474d10(unsigned char flag)
{
    void* temp;
    void* b = buffer;
    *(void* volatile*)&temp = b;
    operator delete(b);
    buffer = 0;
    field_14 = 0;
    field_18 = 0;
    ((Class_00471d00*)this)->FUN_00471d00();
    if (flag & 1) {
        FUN_00471d50(this);
    }
    return this;
}
