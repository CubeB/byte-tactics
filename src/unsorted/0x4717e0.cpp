// Decompiled by Opus. Names are provisional.
// Scalar deleting destructor (slot 0 of the vtable at 0x4fd5f8) of a class
// derived from Class_00471cc0, with a std::vector of 68-byte records at +0xc
// (see 0x473170). The implicit derived destructor destroys the vector (the
// inlined ~vector leaves the dead store of _First in the `push ecx` slot),
// then the inlined base destructor stores the base vtable; the object is
// returned to the pool DAT_0051e610 as in the base's own ??_G (0x471cd0).
#include <vector>

struct Record_004717e0 {
    int unknown[17];
};

class Class_00470ed0 {
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);
};

extern Class_00470ed0 DAT_0051e610;
extern void* DAT_004fd5a8[];

class Class_004717e0 {
public:
    void** vtable;                          // +0x00
    char unknown_4[8];
    std::vector<Record_004717e0> records;   // +0x0c (_First +0x10, _Last +0x14, _End +0x18)

    void* FUN_004717e0(unsigned char flag);
};

// FUNCTION: 0x4717e0
void* Class_004717e0::FUN_004717e0(unsigned char flag)
{
    records.~vector();
    vtable = DAT_004fd5a8;
    if (flag & 1) {
        DAT_0051e610.FUN_00470ed0(this);
    }
    return this;
}
