// Decompiled by Haiku. Names are provisional.

extern void* FUN_004b6220();
extern void FUN_004ba200(void*, int, int);

// FUNCTION: 0x4ba590
void FUN_004ba590(int param_1) {
    void* ptr = FUN_004b6220();
    *(int*)((char*)ptr + 0x614) = param_1;
    FUN_004ba200((char*)ptr + 0x214, 0, 0x100);
}
