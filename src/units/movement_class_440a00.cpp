// Decompiled by Sonnet. Names are provisional.

void __cdecl FUN_004d85a0(void* p);

extern char DAT_00512370[];
extern char DAT_00512770[];

static inline void FreeA(int p)
{
    FUN_004d85a0(*(void**)(p - 0x18));
    *(void**)(p - 0x18) = 0;
}

static inline void FreeC(int p)
{
    void* c = *(void**)p;
    *(void**)(p - 8) = 0;
    *(void**)(p - 4) = 0;
    operator delete(c);
    *(void**)p = 0;
}

// FUNCTION: 0x440a00
void FreeMovementClasses(void)
{
    int p = (int)DAT_00512370;
    do {
        FreeA(p);
        FreeC(p);
        p += 0x20;
    } while (p < (int)DAT_00512770);
}
