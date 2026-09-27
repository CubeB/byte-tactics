// Decompiled by Haiku. Names are provisional.

extern void* DAT_004fc980;
extern void __cdecl operator delete(void*);

class Class_00407390 {
public:
    void* vtable_field;

    void* FUN_00407390(unsigned char param_1);
};

// FUNCTION: 0x407390
void* Class_00407390::FUN_00407390(unsigned char param_1) {
    vtable_field = DAT_004fc980;
    if (param_1 & 1) {
        operator delete(this);
    }
    return this;
}
