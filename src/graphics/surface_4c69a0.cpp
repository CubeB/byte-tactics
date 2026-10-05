// Decompiled by Opus. Names are provisional.

int GetDisplay();

// FUNCTION: 0x4c69a0
void __stdcall SetOffscreenSurface(int param_1)
{
    int p = GetDisplay();
    *(int*)(p + 0xdc) = 1;
    *(int*)(p + 0xbc) = param_1;
}
