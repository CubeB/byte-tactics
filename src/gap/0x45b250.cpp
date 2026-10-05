// Decompiled by Claude Opus 5.5. Names are provisional.
// Built without /GX; see 0x45b670.cpp for the online.dll helpers and what the
// try block does to the locals. Every local has a slot, ordered by a hash of
// its name (16 buckets, the later declaration first within a bucket), which is
// why `text` is declared before `format` here.
#include <windows.h>
#include <stdio.h>
#include <string.h>

extern char DAT_00512dd0[MAX_PATH + 1];
extern HMODULE DAT_00512eec;

char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_0045b480(char* text);

typedef unsigned int (__stdcall* OnlGetVersion)(void);
typedef int (__stdcall* OnlProcessButtonCommand)(int button, char* message, unsigned int size,
                                                 void (__stdcall* translate)(char* text));

// Builds the path of online.dll beside the executable and loads it.
static BOOL LoadOnline(void)
{
    if (DAT_00512dd0[0] == 0) {
        int len = GetModuleFileNameA(0, DAT_00512dd0, MAX_PATH);
        DAT_00512dd0[MAX_PATH] = 0;
        char* p = DAT_00512dd0;
        if (len > 0) {
            for (p = &DAT_00512dd0[len - 1]; len > 0; p--, len--) {
                if (strchr("/\\", *p))
                    break;
            }
        }
        strcpy(p + ((len > 0) & 1), "online.dll");
    }
    if (DAT_00512eec == 0) {
        DAT_00512eec = LoadLibraryA(DAT_00512dd0);
        if (DAT_00512eec == 0)
            return FALSE;
    }
    return TRUE;
}

static BOOL CheckVersion(void)
{
    BOOL ok = FALSE;
    if (DAT_00512eec) {
        OnlGetVersion f = (OnlGetVersion)GetProcAddress(DAT_00512eec, "ONLGetVersion");
        if (f)
            ok = f() >= 3;
    }
    return ok;
}

static int ProcessButtonCommand(int button, char* message, unsigned int size)
{
    int result = 2;
    if (DAT_00512eec) {
        OnlProcessButtonCommand f =
            (OnlProcessButtonCommand)GetProcAddress(DAT_00512eec, "ONLProcessButtonCommand");
        if (f)
            result = f(button, message, size, FUN_0045b480);
    }
    return result;
}

// FUNCTION: 0x45b250
int __stdcall FUN_0045b250(int button, char* message, unsigned int size)
{
    int result = 2;
    try {
        if (button != -1 && LoadOnline() && CheckVersion())
            result = ProcessButtonCommand(button, message, size);
    } catch (...) {
        if (message && size > 0) {
            *message = 0;
            strncat(message, FUN_004c5740("Fault while trying to execute button action."), size - 1);
        }
        result = 2;
    }
    if (result == 1) {
        char text[0x78];
        char* format = FUN_004c5740("Please insert the Installation CD (Disc 1) and select%s\"%s\" again.");
        if (_snprintf(text, sizeof(text), format, "\n\n", message) < 0)
            text[sizeof(text) - 1] = 0;
        *message = 0;
        strncat(message, text, size - 1);
    }
    return result;
}
