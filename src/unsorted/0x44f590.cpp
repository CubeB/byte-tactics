// Decompiled by Haiku. Names are provisional.

typedef void (__cdecl *DeleteFn)(void*);
extern DeleteFn FUN_004b4f20;

extern char DAT_004fd428;

class Class_0044f590 {
public:
    void** vtable;

    Class_0044f590* FUN_0044f590(unsigned char param_1);
};

// FUNCTION: 0x44f590
Class_0044f590* Class_0044f590::FUN_0044f590(unsigned char param_1)
{
    vtable = (void**)&DAT_004fd428;
    if (param_1 & 1) {
        FUN_004b4f20(this);
    }
    return this;
}
