// Decompiled by Sonnet. Names are provisional.

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

// FUNCTION: 0x42db60
int __stdcall FUN_0042db60(const char* param_1, const char* param_2)
{
    return _strcmpi(param_1 + 0x20, param_2 + 0x20) < 0;
}
