// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x41da10
void __stdcall FUN_0041da10(int param1, int param2, int param3)
{
    char mask;
    if (param3)
        mask = -1;
    else
        mask = 0;
    *(unsigned char*)(param2 + param1) = (unsigned char)((mask & 0xb) + 0x4c);
}
