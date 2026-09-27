// Decompiled by Haiku. Names are provisional.

extern void operator delete(void* ptr);

// FUNCTION: 0x434400
void __stdcall FUN_00434400(int param_1)
{
    int esi = param_1;
    int eax = *(int*)((unsigned char*)esi + 4);
    operator delete((void*)eax);
    *(int*)((unsigned char*)esi + 4) = 0;
    *(int*)((unsigned char*)esi + 8) = 0;
    *(int*)((unsigned char*)esi + 0xc) = 0;
}
