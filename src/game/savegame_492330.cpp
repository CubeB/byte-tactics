// Decompiled by Sonnet. Names are provisional.

extern "C" const char* __stdcall FUN_004c5740(const char* key);
void __stdcall FUN_004abd90(void* param_1, const char* text, int a3, int a4, int a5);
void __stdcall FUN_004ab0a0(void* param_1);

extern const char DAT_00509310[]; // "Invalid savegame file"

// FUNCTION: 0x492330
void __stdcall FUN_00492330(void* param_1)
{
    FUN_004abd90(param_1, FUN_004c5740(DAT_00509310), 0x140, 1, 1);
    FUN_004ab0a0(param_1);
}
