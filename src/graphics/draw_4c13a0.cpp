// Decompiled by Haiku. Names are provisional.

extern void* GetDisplay();

// FUNCTION: 0x4c13a0
void __stdcall SetTextColors(int param_1, int param_2)
{
    void* eax = GetDisplay();
    if (param_1 != -1) {
        *(int*)((unsigned char*)eax + 0x208) = param_1;
    }
    if (param_2 != -1) {
        *(int*)((unsigned char*)eax + 0x20c) = param_2;
    }
}
