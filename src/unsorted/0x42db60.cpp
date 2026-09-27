// Decompiled by Haiku. Names are provisional.

int _strcmpi(const char* str1, const char* str2);

// FUNCTION: 0x42db60
int __stdcall FUN_0042db60(const char* param_1, const char* param_2)
{
    const char* str1 = param_1 + 0x20;
    const char* str2 = param_2 + 0x20;
    int result = _strcmpi(str1, str2);
    int edx = 0;
    if (result < 0) {
        edx = 1;
    }
    return edx;
}
