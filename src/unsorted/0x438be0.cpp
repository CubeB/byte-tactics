// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x438be0
int __stdcall FUN_00438be0(void* param)
{
    if (!param) return 0;
    void* ptr1 = *(void**)((char*)param + 0x5c);
    if (!ptr1) return 0;
    return *(int*)((char*)ptr1 + 0x42);
}
