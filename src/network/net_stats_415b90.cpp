// Decompiled by Haiku. Names are provisional.

extern void __cdecl operator delete(void*);

struct BitWriter {
    char unknown_0[0xc];
    void* ptr;          // +0xc
    char unknown_10[4];

    void FreeBuffer();
};

// FUNCTION: 0x415b90
void BitWriter::FreeBuffer()
{
    if (ptr != (void*)((char*)this + 0x10)) {
        operator delete(ptr);
    }
}
