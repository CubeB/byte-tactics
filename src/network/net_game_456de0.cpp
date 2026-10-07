// Decompiled by DeepSeek V4.1 Flash, then Claude Opus 5.5, finished by
// space-bunny-free. Names are provisional.
// Sends the average of the six load-stage percentages (g_game+0x38d6f..74)
// as a two-byte packet (type 0x2a) to every active player of type 1 or 2,
// and stores it in that player's +0x20.
// <stdio.h> must stay: it gives the [ecx + esi] base/index order.
#include <stdio.h>

#pragma pack(push, 1)
struct Player_00456de0 {
    int active;                         // +0x0
    int id;                             // +0x4
    char unknown_8[0x20 - 8];
    unsigned char progress;             // +0x20
    char unknown_21[0x73 - 0x21];
    char type;                          // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00456de0 players[10];        // +0x1b63
    char unknown_2851[0x38d6f - 0x2851];
    // volatile: the byte loads need the explicit widening mask.
    volatile unsigned char stages[6];   // +0x38d6f
};

struct Packet_00456de0 {
    unsigned char type;                 // +0x0
    unsigned char progress;             // +0x1
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall BroadcastPacket(int player, void* data, int size);

static inline int PlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

// FUNCTION: 0x456de0
void __stdcall SendLoadProgress()
{
    Packet_00456de0 packet;
    // The type store must be the first statement, ahead of the prologue stores.
    packet.type = 0x2a;
    // Own int total, summed as a += chain in this order: sets the load order.
    int total = g_game->stages[0];
    total += g_game->stages[5];
    total += g_game->stages[4];
    total += g_game->stages[3];
    total += g_game->stages[2];
    total += g_game->stages[1];

    packet.progress = total / 6;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)) {
            g_game->players[i].progress = packet.progress;
            BroadcastPacket(PlayerId(i), &packet, 2);
        }
    }
}
