// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

struct DPNAME {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

extern char* g_game;

int __stdcall FUN_004ca7c0(void* net, unsigned long id, void* data, unsigned long* size);

// FUNCTION: 0x450140
int __stdcall FUN_00450140(int dpid, char* shortName, char* longName)
{
    int r;
    if (dpid != -1) {
        char buf[0x400];
        unsigned long size = 0x400;
        r = FUN_004ca7c0(g_game + 0x14, dpid, buf, &size);
        if (r == 0) {
            strcpy(shortName, ((DPNAME*)buf)->lpszShortNameA);
            strcpy(longName, ((DPNAME*)buf)->lpszLongNameA);
        }
    } else {
        strcpy(shortName, "COMPUTER");
        strcpy(longName, "COMPUTER");
        r = 0;
    }
    return r == 0;
}
