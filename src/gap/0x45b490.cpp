// Decompiled by Claude Opus 5.5. Names are provisional.
// Built without /GX; see 0x45b670.cpp for the online.dll helpers.
#include <windows.h>
#include <string.h>

extern char DAT_00512dd0[MAX_PATH + 1];
extern HMODULE DAT_00512eec;

char* __stdcall FUN_004c5740(char* text);

struct LinkInfo {
    int id;             // -1: unused
    char name[32];
};

typedef unsigned int (__stdcall* OnlGetVersion)(void);
typedef unsigned int (__stdcall* OnlGetLinkInfo)(LinkInfo* links);

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

static unsigned int GetLinkInfo(LinkInfo* links)
{
    unsigned int count = 0;
    if (DAT_00512eec) {
        OnlGetLinkInfo f = (OnlGetLinkInfo)GetProcAddress(DAT_00512eec, "ONLGetLinkInfo");
        if (f)
            count = f(links);
    }
    return count;
}

// FUNCTION: 0x45b490
unsigned int __stdcall FUN_0045b490(LinkInfo* links)
{
    unsigned int count = 0;
    try {
        if (LoadOnline() && CheckVersion()) {
            count = GetLinkInfo(links);
            for (unsigned int i = 0; i < count; i++) {
                if (links[i].id != -1) {
                    char* name = FUN_004c5740(links[i].name);
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
