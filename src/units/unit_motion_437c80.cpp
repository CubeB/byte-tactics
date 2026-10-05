// Decompiled by Opus. Names are provisional.
// Hands the whole arena (its length minus the 8-byte record header) to the
// allocator 0x437a30 as one block, storing the handle at +0xc.

class CMemoryCache {
public:
    int AllocHandle(void** handle, int size);
};

class Class_00437c80 {
public:
    int length;                        // +0x0
    char unknown_4[8];
    void* handle;                      // +0xc

    void FlushCache();
};

// FUNCTION: 0x437c80
void Class_00437c80::FlushCache()
{
    ((CMemoryCache*)this)->AllocHandle(&handle, length - 8);
}
