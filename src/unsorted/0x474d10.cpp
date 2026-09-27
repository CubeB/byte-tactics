// Decompiled by Sonnet, rewritten without volatile by Opus. Names are provisional.
// Scalar deleting destructor (vtable slot 0 at 0x4fd618) of a class derived
// from Class_00471cc0, with a std::vector of 32-byte records at +0x0c (see
// 0x474f80's size() test, `>> 5`). The derived destructor is implicit: it
// destroys the vector (the inlined ~vector leaves the dead store of _First in
// the `push ecx` slot) and calls the base destructor. The base destructor is
// already named as a method, so both steps are written out explicitly.
#include <vector>

struct Record_00474d10 {
    int unknown[8];
};

class Class_00471d00 {
public:
    void FUN_00471d00();
};

extern void __stdcall FUN_00471d50(void* obj);

class Class_00474d10 {
public:
    void* vtable;                           // +0x00
    char unknown_4[8];
    std::vector<Record_00474d10> records;   // +0x0c (_First +0x10, _Last +0x14, _End +0x18)

    void* FUN_00474d10(unsigned char flag);
};

// FUNCTION: 0x474d10
void* Class_00474d10::FUN_00474d10(unsigned char flag)
{
    records.~vector();
    ((Class_00471d00*)this)->FUN_00471d00();
    if (flag & 1) {
        FUN_00471d50(this);
    }
    return this;
}
