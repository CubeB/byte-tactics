// Decompiled by Sonnet. Names are provisional.

class Class_00481470 {
public:
    char unknown_0[0x540];
    void* ptr_540;

    unsigned short FUN_00481470();
};

// FUNCTION: 0x481470
unsigned short Class_00481470::FUN_00481470()
{
    void* a = ptr_540;
    void* b = *(void**)((char*)a + 0xc);
    void* c = *(void**)((char*)b + 0x86);
    return c ? *(unsigned short*)((char*)c + 0xa8) : 0;
}
