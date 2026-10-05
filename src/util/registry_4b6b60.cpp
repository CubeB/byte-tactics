// Decompiled by Haiku. Names are provisional.

extern void* g_display;

// FUNCTION: 0x4b6b60
void __stdcall FUN_004b6b60(int param_1)
{
    *(int*)((char*)g_display + 0x80) = param_1;
}
