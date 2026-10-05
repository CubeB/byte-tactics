// Decompiled by Haiku. Names are provisional.

extern unsigned int __cdecl _beginthread(void* start, unsigned int stack, void* param);

// FUNCTION: 0x4b6b20
int __stdcall FUN_004b6b20(void* param_1, unsigned int param_2, void* param_3)
{
    unsigned int result = _beginthread(param_1, param_2, param_3);
    if (result != (unsigned int)-1) {
        return 1;
    }
    return 0;
}
