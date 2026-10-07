// Decompiled by Sonnet. Names are provisional.

extern "C" void __cdecl FUN_004d85a0(void* p);

class CMemoryCache {
public:
    void FreeBuffer();
};

class Class_004581c0 {
public:
    char unknown_0[0x10];
    void* ptr;              // +0x10

    void Destroy();
};

// FUNCTION: 0x4581c0
void Class_004581c0::Destroy()
{
    if (ptr) {
        FUN_004d85a0(ptr);
    }
    ((CMemoryCache*)this)->FreeBuffer();
}
