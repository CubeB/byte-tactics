// Decompiled by Opus. Names are provisional.
#include <string.h>

// DirectPlay 3 DPNAME (the toolchain's DirectX 3 <dplay.h> lacks it).
struct DPNAME {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

#pragma pack(push, 1)
struct Player_00450090 {
    char unknown_0[4];
    int dpid;                          // +0x04
    char unknown_8[0x2b - 0x8];
    char longName[30];                 // +0x2b
    char shortName[30];                // +0x49
    char unknown_67[0x73 - 0x67];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00450090 {
    char unknown_0[0x14];
    char net[0x1b63 - 0x14];           // +0x14
    Player_00450090 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_00450090* g_game;

int __stdcall FUN_004ca800(void* net, int dpid, DPNAME* name, int flags);

static inline int PlayerDpid(Player_00450090* p)
{
    if (p != 0 && p->type != 0)
        return p->dpid;
    return -1;
}

// FUNCTION: 0x450090
int __stdcall FUN_00450090(unsigned char index, char* shortName, char* longName)
{
    Player_00450090* p = &g_game->players[index];
    strncpy(p->longName, longName, 30);
    strncpy(p->shortName, shortName, 30);
    DPNAME name;
    name.dwSize = sizeof(DPNAME);
    name.dwFlags = 0;
    name.lpszShortNameA = shortName;
    name.lpszLongNameA = longName;
    int r = FUN_004ca800(g_game->net, PlayerDpid(p), &name, 2);
    return r == 0 ? 1 : 0;
}
