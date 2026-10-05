// Decompiled by Haiku. Names are provisional.

extern int GetDisplay();

// FUNCTION: 0x4c1420
void __stdcall SetFont(int param_1)
{
    if (param_1 != 0) {
        int eax = GetDisplay();
        *(int*)(eax + 0x204) = param_1;
    }
}
