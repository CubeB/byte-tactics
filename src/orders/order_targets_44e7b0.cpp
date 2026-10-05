// Decompiled by Haiku. Names are provisional.

extern void __cdecl operator delete(void* ptr);
extern void* DAT_004fd2f8;

class Class_0044e7b0 {
public:
    void* vtable;

    void* FUN_0044e7b0(int param_1);
};

// FUNCTION: 0x44e7b0
void* Class_0044e7b0::FUN_0044e7b0(int param_1)
{
    vtable = &DAT_004fd2f8;
    if ((param_1 & 1) != 0) {
        operator delete(this);
    }
    return this;
}
