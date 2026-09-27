// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

// FUNCTION: 0x47a8e0
int __cdecl FUN_0047a8e0()
{
    int* esi;
    int* eax;
    int ecx;
    int result;
    int divisor;

    esi = (int*)DAT_00511de8;
    eax = *(int**)((int)esi + 0x29a0);
    ecx = *(int*)((char*)eax + 0x224);
    ecx += ecx * 2;
    result = *(int*)((char*)eax + ecx * 8 + 4);
    result++;
    divisor = *(int*)((int)esi + 0x37f39);
    *(int*)((char*)eax + ecx * 8 + 4) = result % divisor;
    return result / divisor;
}
