// Decompiled by Haiku. Names are provisional.

extern int __cdecl _chdrive(int drive);

// FUNCTION: 0x4bc300
int __stdcall FUN_004bc300(char* param_1)
{
    if (param_1 == 0) {
        return -1;
    }
    return _chdrive(*param_1 - 0x40);
}
