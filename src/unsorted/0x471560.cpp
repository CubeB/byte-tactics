// Decompiled by Sonnet. Names are provisional.
// Sibling of 0x474d10.cpp/0x475110.cpp: a scalar deleting destructor (a
// virtual slot at 0x4fd5a8, the same vtable used by 0x471cd0.cpp) of a class
// with a std::vector of trivially-destructible records at +0x0c. The inlined
// ~vector leaves the dead store of _First in the `push ecx` slot; then the
// function stores the vtable and conditionally deregisters via the global
// manager object, exactly like 0x471cd0.cpp.
#include <vector>

class Class_00470ed0 {
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);
};

extern Class_00470ed0 DAT_0051e610;
extern void* DAT_004fd5a8[];

struct Record_00471560 {
    int unknown[8];
};

class Class_00471560 {
public:
    void** vtable;                            // +0x00
    char unknown_4[8];                        // +0x04
    std::vector<Record_00471560> records;     // +0x0c (_First +0x10, _Last +0x14, _End +0x18)

    void* FUN_00471560(unsigned char flag);
};

// FUNCTION: 0x471560
void* Class_00471560::FUN_00471560(unsigned char flag)
{
    records.~vector();
    vtable = DAT_004fd5a8;
    if (flag & 1) {
        DAT_0051e610.FUN_00470ed0(this);
    }
    return this;
}
