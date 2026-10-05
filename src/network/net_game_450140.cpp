// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

struct DPNAME {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

extern char* g_game;

int __stdcall HAPINET_getplayername(void* net, unsigned long id, void* data, unsigned long* size);

// FUNCTION: 0x450140
int __stdcall GetPlayerName(int dpid, char* shortName, char* longName)
{
    int r;
    if (dpid != -1) {
        char buf[0x400];
        unsigned long size = 0x400;
        r = HAPINET_getplayername(g_game + 0x14, dpid, buf, &size);
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
