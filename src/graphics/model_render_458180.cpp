// Decompiled by Opus. Names are provisional.
// Initialises the memory cache base (0x4379b0), then allocates the 600x600
// "CompositeBuffer" bitmap at +0x10.

class CMemoryCache {
public:
    int InitCache(unsigned int size);
};

void* __stdcall FUN_004b8e00(const char* name, int width, int height);

class Class_00458180 {
public:
    char unknown_0[0x10];
    void* buffer;                      // +0x10

    int FUN_00458180(unsigned int size);
};

// FUNCTION: 0x458180
int Class_00458180::FUN_00458180(unsigned int size)
{
    if (!((CMemoryCache*)this)->InitCache(size))
        return 0;
    buffer = FUN_004b8e00("CompositeBuffer", 600, 600);
    return buffer != 0;
}
