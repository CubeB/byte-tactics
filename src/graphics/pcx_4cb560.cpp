// Decompiled by Haiku. Names are provisional.

extern void* __stdcall FUN_004bbe50(void*, int);
extern void* __stdcall FUN_004cb4c0(void*, void*);

// FUNCTION: 0x4cb560
void* __stdcall FUN_004cb560(char* param)
{
    void* result = FUN_004bbe50(param, 0);
    if (result == 0) {
        return 0;
    }
    FUN_004cb4c0(result, result);
    return result;
}
