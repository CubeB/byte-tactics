// Decompiled by Opus. Names are provisional.
// Byte-identical sibling of 0x471560.cpp: the scalar deleting destructor
// (slot 0 of the vtable at 0x4fd5d8) of a class with a std::vector at +0xc.
// The inlined ~vector leaves the dead store of _First in the `push ecx`
// slot; then the base vtable is stored and the object is returned to the
// pool DAT_0051e610.

#include <vector>

class Class_00470ed0 {
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);
};

extern Class_00470ed0 DAT_0051e610;
extern void* DAT_004fd5a8[];

struct Record_004716a0 {
    int unknown[8];
};

class Class_004716a0 {
public:
    void** vtable;                            // +0x00
    char unknown_4[8];                        // +0x04
    std::vector<Record_004716a0> records;     // +0x0c (_First +0x10, _Last +0x14, _End +0x18)

    void* FUN_004716a0(unsigned char flag);
};

// FUNCTION: 0x4716a0
void* Class_004716a0::FUN_004716a0(unsigned char flag)
{
    records.~vector();
    vtable = DAT_004fd5a8;
    if (flag & 1) {
        DAT_0051e610.FUN_00470ed0(this);
    }
    return this;
}
