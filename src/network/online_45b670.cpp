// Decompiled by Claude Opus 5.5. Names are provisional.
// The online.dll glue (0x45b250, 0x45b490, 0x45b670 share these inline
// helpers). Built without /GX like the rest of the game: MSVC 5 still builds
// the C++ exception frame for try/catch (warning C4530) and then keeps every
// local of the function, those of inlined callees too, in a stack slot of its
// own, storing each one there whenever it changes inside the try block.
#include <windows.h>
#include <string.h>

extern int DAT_00512c80[0x54];      // the online configuration, 0x150 bytes
extern char DAT_00512dd0[MAX_PATH + 1];
extern int DAT_00512ee8;
extern HMODULE DAT_00512eec;

typedef unsigned int (__stdcall* OnlGetVersion)(void);
typedef int (__stdcall* OnlLoadConfigFile)(char* file, void* config, int size);

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
        // The `& 1` is in the original (`and edx, 1` after the setg).
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

static int LoadConfigFile(char* file, void* config, int size)
{
    int result = 0;
    if (DAT_00512eec) {
        OnlLoadConfigFile f = (OnlLoadConfigFile)GetProcAddress(DAT_00512eec, "ONLLoadConfigFile");
        if (f)
            result = f(file, config, size);
    }
    return result;
}

// FUNCTION: 0x45b670
int __stdcall FUN_0045b670(char* file)
{
    DAT_00512ee8 = 0;
    memset(DAT_00512c80, 0, sizeof(DAT_00512c80));
    try {
        if (LoadOnline() && CheckVersion())
            DAT_00512ee8 = LoadConfigFile(file, DAT_00512c80, sizeof(DAT_00512c80));
    } catch (...) {
        DAT_00512ee8 = 0;
    }
    return DAT_00512ee8;
}
