// Decompiled by Haiku. Names are provisional.

extern void* DAT_005119c0;

// FUNCTION: 0x40c230
int __stdcall FUN_0040c230(int param_1)
{
    char* base = (char*)&DAT_005119c0;
    void* ptr = *(void**)(base + param_1 * 4);
    return *(int*)((char*)ptr + 0x75);
}
