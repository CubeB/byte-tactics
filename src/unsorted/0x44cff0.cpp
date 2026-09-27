// Decompiled by Haiku. Names are provisional.

extern void operator delete(void*);
extern void* DAT_004fd2f8[];

class Class_0044cff0 {
public:
    void* vtable;

    void* FUN_0044cff0(unsigned char flag);
};

// FUNCTION: 0x44cff0
void* Class_0044cff0::FUN_0044cff0(unsigned char flag)
{
    vtable = DAT_004fd2f8;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}
