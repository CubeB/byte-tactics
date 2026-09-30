// Decompiled by Haiku. Names are provisional.

extern void* DAT_004fd2f8[];

struct Class_0044d450
{
public:
    void** vtable;
    char unknown_4[0x49d];

    Class_0044d450* FUN_0044d450(unsigned char flag);
};

void __cdecl operator delete(void* ptr);

// FUNCTION: 0x44d450
Class_0044d450* Class_0044d450::FUN_0044d450(unsigned char flag)
{
    vtable = DAT_004fd2f8;
    if ((flag & 1) != 0) {
        operator delete(this);
    }
    return this;
}
