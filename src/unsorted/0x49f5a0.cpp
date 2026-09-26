// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <stdio.h>

void __stdcall FUN_004bb120(char* path);

// FUNCTION: 0x49f5a0
UINT __stdcall FUN_0049f5a0(char* key, int defaultValue)
{
    char exePath[256];
    char iniPath[256];

    GetModuleFileNameA(NULL, exePath, 256);
    FUN_004bb120(exePath);
    sprintf(iniPath, "%s\\totala.ini", exePath);
    return GetPrivateProfileIntA("Preferences", key, defaultValue, iniPath);
}
