// Decompiled by Haiku. Names are provisional.

extern void* DAT_004fd428;
extern void __cdecl operator delete(void*);

class Class_00490840 {
public:
    void* vtable_field;

    void* FUN_00490840(unsigned char param_1);
};

// FUNCTION: 0x490840
void* Class_00490840::FUN_00490840(unsigned char param_1) {
    vtable_field = DAT_004fd428;
    if (param_1 & 1) {
        operator delete(this);
    }
    return this;
}
