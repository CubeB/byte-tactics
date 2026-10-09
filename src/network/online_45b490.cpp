// Decompiled by Claude Opus 5.5. Names are provisional.
// Built without /GX; see 0x45b670.cpp for the online.dll helpers.
#include <windows.h>
#include <string.h>

// GLOBAL: 0x512dd0
extern char g_onlineDllPath[MAX_PATH + 1];
extern HMODULE g_onlineDll;

char* __stdcall Translate(char* text);

#include "link_info.h"

typedef unsigned int (__stdcall* OnlGetVersion)(void);
typedef unsigned int (__stdcall* OnlGetLinkInfo)(LinkInfo* links);

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

static unsigned int GetLinkInfo(LinkInfo* links)
{
    unsigned int count = 0;
    if (g_onlineDll) {
        OnlGetLinkInfo f = (OnlGetLinkInfo)GetProcAddress(g_onlineDll, "ONLGetLinkInfo");
        if (f)
            count = f(links);
    }
    return count;
}

// FUNCTION: 0x45b490
unsigned int __stdcall OnlineGetLinkInfo(LinkInfo* links)
{
    unsigned int count = 0;
    try {
        if (LoadOnline() && CheckVersion()) {
            count = GetLinkInfo(links);
            for (unsigned int i = 0; i < count; i++) {
                if (links[i].id != -1) {
                    char* name = Translate(links[i].name);
                    if (name != links[i].name) {
                        strncpy(links[i].name, name, sizeof(links[i].name));
                        links[i].name[sizeof(links[i].name) - 1] = 0;
                    }
                }
            }
        }
    } catch (...) {
        count = 0;
    }
    return count;
}
