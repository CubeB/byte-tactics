// Decompiled by Opus. Names are provisional.
// Stores a callback and the argument it will be called with.

extern void (__cdecl *DAT_0051fc78)(int);
extern int DAT_0051fc7c;

// FUNCTION: 0x4b4fd0
void __stdcall FUN_004b4fd0(void (__cdecl *callback)(int), int param)
{
    DAT_0051fc78 = callback;
    DAT_0051fc7c = param;
}
