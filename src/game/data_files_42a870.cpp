// Decompiled by Opus. Names are provisional.
// The compiler-generated vector deleting destructor of the 12-byte class whose
// constructor is 0x4c2ea0 and destructor 0x4c2eb0 (a global of this class,
// DAT_0051f310, is built by 0x49e610 and destroyed by its atexit handler
// 0x49e630). MSVC emits it for `new Class_004c2ea0[n]`, which the weapon
// loader in the gap at 0x42a8d0 does (DAT_005122a0 = new ...[DAT_005122a8]);
// nothing calls it, so it survives only as an unreferenced COMDAT.
//
// The global below exists only to make the compiler emit the COMDAT here; the
// real `new[]` sits in 0x42a8d0, which is not decompiled yet.

class Class_004c2ea0 {
public:
    void* data;                        // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

// FUNCTION: 0x42a870 ??_EClass_004c2ea0@@QAEPAXI@Z
static Class_004c2ea0* s_array = new Class_004c2ea0[1];
