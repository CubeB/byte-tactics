// Decompiled by Haiku. Names are provisional.

extern void* __cdecl operator new(unsigned int size);

// FUNCTION: 0x4b3410
void __stdcall FUN_004b3410(int param_1, int param_2) {
    void* ptr = operator new(0x18);
    *(int*)((char*)ptr + 0x4) = param_1;
    *(int*)((char*)ptr + 0x14) = param_2;
}
