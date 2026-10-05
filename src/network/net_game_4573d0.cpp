// Decompiled by space-bunny-free. Names are provisional.
// Sends a 0x3a-byte packet (type 0x28) carrying a player's four short angles
// (+0xfc, +0xfe, +0x104, +0x106), four dwords (+0x98, +0x8c, +0xa8, +0xa4) and
// six doubles (+0xac, +0xbc, +0xcc, +0xb4, +0xc4, +0xd4) as floats, plus the
// caller's flag byte. It goes to `target` when there is one, otherwise to
// every occupied player slot of type 3 whose data->field_94 is 1.
//
// `#include <string.h>` is load bearing here even though nothing in this file
// calls a string function. Without it the five player-loop reads of
// g_game->players[i] come out as `[esi+eax+0x1b63]` (SIB 0x06) where the
// original has `[eax+esi+0x1b63]` (SIB 0x30, g_game as base and the byte
// offset of the induction variable as index), and the function is 5 bytes out.
// Everything else about the loop already matched, so this was the only
// difference, and it is the same header the matched sibling 0x451b60 carries.
// Worth knowing for the next one: the flip tracks how many memory references
// the loop body has, not how it is phrased. With the call plus three of the
// four tests the accesses come out right, and adding the fourth test flips all
// five, which is what sent the earlier search through every ordering of the
// tests, the address expression, the call's arguments, the static inline
// helpers, tools/headers.py's 128 sets and the N-declarations test without
// finding it. 0x4578f0 needed the same include for the same reason.

#include <string.h>

#pragma pack(push, 1)
struct PlayerData_004573d0 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

struct Player_004573d0 {
    int active;                        // +0x00
    int dpid;                          // +0x04
    char unknown_8[0x22 - 0x8];
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerData_004573d0* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x8c - 0x74];
    int field_8c;                      // +0x8c
    char unknown_90[0x98 - 0x90];
    int field_98;                      // +0x98
    char unknown_9c[0xa4 - 0x9c];
    int field_a4;                      // +0xa4
    int field_a8;                      // +0xa8
    double field_ac;                   // +0xac
    double field_b4;                   // +0xb4
    double field_bc;                   // +0xbc
    double field_c4;                   // +0xc4
    double field_cc;                   // +0xcc
    double field_d4;                   // +0xd4
    char unknown_dc[0xfc - 0xdc];
    short field_fc;                    // +0xfc
    short field_fe;                    // +0xfe
    char unknown_100[0x104 - 0x100];
    short field_104;                   // +0x104
    short field_106;                   // +0x106
    char unknown_108[0x14b - 0x108];
};

struct Game_004573d0 {
    char unknown_0[0x1b63];
    Player_004573d0 players[10];       // +0x1b63
};

struct Packet_004573d0 {              // 0x3a bytes
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int field_2;                       // +0x2
    int field_6;                       // +0x6
    int field_a;                       // +0xa
    int field_e;                       // +0xe
    int field_12;                      // +0x12
    int field_16;                      // +0x16
    int field_1a;                      // +0x1a
    int field_1e;                      // +0x1e
    float field_22;                    // +0x22
    float field_26;                    // +0x26
    float field_2a;                    // +0x2a
    float field_2e;                    // +0x2e
    float field_32;                    // +0x32
    float field_36;                    // +0x36
};
#pragma pack(pop)

extern Game_004573d0* g_game;

int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);

// FUNCTION: 0x4573d0
void __stdcall FUN_004573d0(Player_004573d0* player, Player_004573d0* target,
                            unsigned char flag)
{
    if (player->active == 0)
        return;
    if (player->type != 1 && player->type != 2)
        return;
    if (player->field_22 != 0)
        return;

    Packet_004573d0 packet;
    packet.type = 0x28;
    packet.flag = flag;
    packet.field_2 = player->field_fc;
    packet.field_6 = player->field_fe;
    packet.field_a = player->field_104;
    packet.field_e = player->field_106;
    packet.field_12 = player->field_98;
    packet.field_16 = player->field_8c;
    packet.field_1a = player->field_a8;
    packet.field_1e = player->field_a4;
    packet.field_22 = (float)player->field_ac;
    packet.field_26 = (float)player->field_bc;
    packet.field_2a = (float)player->field_cc;
    packet.field_2e = (float)player->field_b4;
    packet.field_32 = (float)player->field_c4;
    packet.field_36 = (float)player->field_d4;

    if (target != 0) {
        if (target->field_22 == 0)
            FUN_00451bc0(player->dpid, target->dpid, &packet, 0x3a);
        return;
    }

    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active == 0)
            continue;
        if (g_game->players[i].type != 3)
            continue;
        if (g_game->players[i].data->field_94 != 1)
            continue;
        if (g_game->players[i].field_22 != 0)
            continue;
        FUN_00451bc0(player->dpid, g_game->players[i].dpid, &packet, 0x3a);
    }
}
