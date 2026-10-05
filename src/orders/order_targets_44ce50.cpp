// Decompiled by Sonnet. Names are provisional.

extern void* DAT_004fd2f8[];

extern void __cdecl operator delete(void* p);

class Class_0044ce50 {
public:
    void** vtable;

    void* FUN_0044ce50(unsigned char flag);
};

// FUNCTION: 0x44ce50
void* Class_0044ce50::FUN_0044ce50(unsigned char flag)
{
    vtable = DAT_004fd2f8;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}
