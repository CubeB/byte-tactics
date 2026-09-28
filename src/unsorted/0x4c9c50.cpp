// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// DPENUMSESSIONSCALLBACK2 for IDirectPlay3::EnumSessions (HAPINET_getgamescallback).
// Copies a session description into the next slot of the context's game array;
// returns FALSE on DPESC_TIMEDOUT (0x1) to stop the enumeration.

#include <string.h>

void FUN_004c9740(int);

struct Guid_4c9c50 {
    int d1;
    int d2;
    int d3;
    int d4;
};

#pragma pack(push, 1)
struct GameRec_4c9c50 {
    int unknown_0;          // +0x00
    int user1;              // +0x04
    int user2;              // +0x08
    int user3;              // +0x0c
    int user4;              // +0x10
    int maxPlayers;         // +0x14
    char name[0x20];        // +0x18
    Guid_4c9c50 guidInstance; // +0x38
    char unknown_48[0xc];   // +0x48
};

struct SessionDesc_4c9c50 {
    int dwSize;             // +0x00
    int dwFlags;            // +0x04
    Guid_4c9c50 guidInstance;    // +0x08
    Guid_4c9c50 guidApplication; // +0x18
    int dwMaxPlayers;       // +0x28
    int dwCurrentPlayers;   // +0x2c
    char* lpszSessionName;  // +0x30
    char* lpszPassword;     // +0x34
    int dwReserved1;        // +0x38
    int dwReserved2;        // +0x3c
    int dwUser1;            // +0x40
    int dwUser2;            // +0x44
    int dwUser3;            // +0x48
    int dwUser4;            // +0x4c
};

struct Net_4c9c50 {
    char unknown_0[0x4bd];
    GameRec_4c9c50* games;  // +0x4bd
    char unknown_4c1[0x28];
    int count;              // +0x4e9
};
#pragma pack(pop)

// FUNCTION: 0x4c9c50
int __stdcall FUN_004c9c50(SessionDesc_4c9c50* lpsd, int* lpdwTimeout, unsigned int dwFlags, Net_4c9c50* ctx)
{
    FUN_004c9740((int)"HAPINET_getgamescallback\n");
    if ((dwFlags & 1) == 0)
    {
        GameRec_4c9c50* rec = &ctx->games[ctx->count];
        strcpy(rec->name, lpsd->lpszSessionName);
        rec->user1 = lpsd->dwUser1;
        rec->user2 = lpsd->dwUser2;
        rec->user3 = lpsd->dwUser3;
        rec->user4 = lpsd->dwUser4;
        rec->guidInstance = lpsd->guidInstance;
        rec->maxPlayers = lpsd->dwMaxPlayers;
        ctx->count++;
        return 1;
    }
    return 0;
}
