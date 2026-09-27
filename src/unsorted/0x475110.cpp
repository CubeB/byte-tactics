// Decompiled by Sonnet, rewritten without volatile by Opus. Names are provisional.
// Sibling of 0x474d10.cpp. Scalar deleting destructor (vtable slot 0 at
// 0x4fd638) of another class derived from Class_00471cc0, with a std::vector
// of 32-byte records at +0x0c (0x4751c0 divides its size by 32). The derived
// destructor is implicit: it destroys the vector (the inlined ~vector leaves
// the dead store of _First in the `push ecx` slot) and calls the base
// destructor. The base destructor is
// already named as a method, so both steps are written out explicitly.
#include <vector>

struct Record_00475110 {
    int unknown[8];
};

class Class_00471d00 {
public:
    void FUN_00471d00();
};

extern void __stdcall FUN_00471d50(void* obj);

class Class_00475110 {
public:
    void* vtable;                           // +0x00
    char unknown_4[8];
    std::vector<Record_00475110> records;   // +0x0c (_First +0x10, _Last +0x14, _End +0x18)

    void* FUN_00475110(unsigned char flag);
};

// FUNCTION: 0x475110
void* Class_00475110::FUN_00475110(unsigned char flag)
{
    records.~vector();
    ((Class_00471d00*)this)->FUN_00471d00();
    if (flag & 1) {
        FUN_00471d50(this);
    }
    return this;
}
