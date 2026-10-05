// Decompiled by Haiku. Names are provisional.

extern void __cdecl operator delete(void*);

struct Class_00415b90 {
    char unknown_0[0xc];
    void* ptr;          // +0xc
    char unknown_10[4];

    void FUN_00415b90();
};

// FUNCTION: 0x415b90
void Class_00415b90::FUN_00415b90()
{
    if (ptr != (void*)((char*)this + 0x10)) {
        operator delete(ptr);
    }
}
