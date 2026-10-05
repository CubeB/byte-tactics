// Decompiled by Haiku. Names are provisional.

extern char* __cdecl strrchr(const char*, int);

// FUNCTION: 0x4da590
char* __cdecl GetFileNameFromPath(char* param_1)
{
    char* result = strrchr(param_1, 0x5c);
    if (result != 0) {
        return result + 1;
    }
    return param_1;
}
