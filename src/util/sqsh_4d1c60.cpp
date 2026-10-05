// Decompiled by Haiku. Names are provisional.

extern void* g_squashErrorNames[];

// FUNCTION: 0x4d1c60
void* __stdcall SquashErrorString(int param_1)
{
    if (param_1 < 7) {
        return g_squashErrorNames[param_1];
    }
    return 0;
}
