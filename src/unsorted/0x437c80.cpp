// Decompiled by Opus. Names are provisional.
// Hands the whole arena (its length minus the 8-byte record header) to the
// allocator 0x437a30 as one block, storing the handle at +0xc.

class Class_00437a30 {
public:
    int FUN_00437a30(void** handle, int size);
};

class Class_00437c80 {
public:
    int length;                        // +0x0
    char unknown_4[8];
    void* handle;                      // +0xc

    void FUN_00437c80();
};

// FUNCTION: 0x437c80
void Class_00437c80::FUN_00437c80()
{
    ((Class_00437a30*)this)->FUN_00437a30(&handle, length - 8);
}
