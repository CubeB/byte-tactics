// Decompiled by Claude Opus 5.5. Names are provisional.
// The online.dll glue (0x45b250, 0x45b490, 0x45b670 share these inline
// helpers).
#include <windows.h>
#include <string.h>

extern int DAT_00512c80[0x54];      // the online configuration, 0x150 bytes
extern char g_onlineDllPath[MAX_PATH + 1];
extern int g_onlineConfigLoaded;
extern HMODULE g_onlineDll;

typedef unsigned int (__stdcall* OnlGetVersion)(void);
typedef int (__stdcall* OnlLoadConfigFile)(char* file, void* config, int size);

// Builds the path of online.dll beside the executable and loads it.
static BOOL LoadOnline(void)
{
    if (g_onlineDllPath[0] == 0) {
        int len = GetModuleFileNameA(0, g_onlineDllPath, MAX_PATH);
        g_onlineDllPath[MAX_PATH] = 0;
        char* p = g_onlineDllPath;
        if (len > 0) {
            for (p = &g_onlineDllPath[len - 1]; len > 0; p--, len--) {
                if (strchr("/\\", *p))
                    break;
            }
        }
        // The `& 1` is in the original (`and edx, 1` after the setg).
        strcpy(p + ((len > 0) & 1), "online.dll");
    }
    if (g_onlineDll == 0) {
        g_onlineDll = LoadLibraryA(g_onlineDllPath);
        if (g_onlineDll == 0)
            return FALSE;
    }
    return TRUE;
}

static BOOL CheckVersion(void)
{
    BOOL ok = FALSE;
    if (g_onlineDll) {
        OnlGetVersion f = (OnlGetVersion)GetProcAddress(g_onlineDll, "ONLGetVersion");
        if (f)
            ok = f() >= 3;
    }
    return ok;
}

static int LoadConfigFile(char* file, void* config, int size)
{
    int result = 0;
    if (g_onlineDll) {
        OnlLoadConfigFile f = (OnlLoadConfigFile)GetProcAddress(g_onlineDll, "ONLLoadConfigFile");
        if (f)
            result = f(file, config, size);
    }
    return result;
}

// FUNCTION: 0x45b670
int __stdcall FUN_0045b670(char* file)
{
    g_onlineConfigLoaded = 0;
    memset(DAT_00512c80, 0, sizeof(DAT_00512c80));
    try {
        if (LoadOnline() && CheckVersion())
            g_onlineConfigLoaded = LoadConfigFile(file, DAT_00512c80, sizeof(DAT_00512c80));
    } catch (...) {
        g_onlineConfigLoaded = 0;
    }
    return g_onlineConfigLoaded;
}
