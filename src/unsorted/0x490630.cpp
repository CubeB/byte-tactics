// Decompiled by Haiku. Names are provisional.

extern void (operator delete)(void*);

struct Class_00490630 {
    void* vtable_ptr;

    void* FUN_00490630(int param_1);
};

extern void* DAT_004fd428;

// FUNCTION: 0x490630
void* Class_00490630::FUN_00490630(int param_1)
{
    vtable_ptr = &DAT_004fd428;
    if ((param_1 & 1) != 0) {
        operator delete(this);
    }
    return this;
}
