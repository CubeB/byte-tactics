// Decompiled by space-bunny-free. Names are provisional.
// A received 0x3a-byte team packet (the same shape 0x4573d0 sends, the four
// angles as shorts in the packet dwords, the six doubles as floats) updates one
// player record, and then, when the packet's flag byte is set, tells the other
// local players about that player's team.
//
// The first pass looks for a local player (active, type 1 or 2) that already
// has the target player's team in its +0x129 byte table. If there is none, the
// packet's fields are copied into the player; the +0x146 byte is that player's
// team number, the two tables at +0x11e and +0x129 are eleven bytes each, one
// entry per team.
//
// Two things are load bearing for the byte match:
//
// 1. The third byte of the 3-byte packet built in the second pass must be
//    written as an if/else pair of constant stores,
//        if (p->t0[team] != 0) team.flag2 = 1; else team.flag2 = 0;
//    and not as the single expression `team.flag2 = (p->t0[team] != 0);`.
//    Both give the same `test bl,bl; setne dl; mov [esp+N],dl`, but the
//    expression form makes MSVC 5 materialise the bool as a 32 bit temporary
//    and prepend a `xor edx,edx` to define it (hoisted to the top of the
//    block). That costs 2 bytes, and because it also pushes the loop body's
//    end past 127 bytes it turns the loop head's `je` from a short jump into a
//    near one, another 4: 457 bytes against the original's 451. The if/else
//    form if-converts to the setcc with a byte sized result and needs no
//    zeroing. (The same trick with `? 1 : 0` also works; an `unsigned char`
//    or `(unsigned char)` cast of the expression does not.)
// 2. `#include <stdlib.h>` is load bearing although nothing here calls a
//    library function. With `<string.h>` instead (the header the matched
//    siblings 0x4572a0 and 0x4573d0 need), the first loop's last read comes out
//    as `add ecx,edx; add ecx,eax; cmp byte [ecx+ebx+0x1c8c],0` where the
//    original has `add ecx,ebx; add ecx,edx; cmp byte [ecx+eax+0x1c8c],0`:
//    the same address, but with the loop pointer and g_game swapped between
//    the base and the added-in register: 96.7% (same length, wrong operand).
//    tools/headers.py says <windows.h>, <stdlib.h>, <math.h> and <ddraw.h> all
//    flip it, so this is the first thing to try if the SIB order ever breaks.
#include <stdlib.h>

struct PlayerData_00457540 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

class MissionConditions {
public:
    int CheckVictory();
};

#pragma pack(push, 1)
struct Player_00457540 {               // 0x14b bytes
    int active;                        // +0x00
    int dpid;                          // +0x04
    char unknown_8[0x22 - 0x8];
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerData_00457540* data;         // +0x27
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
    char unknown_108[0x11e - 0x108];
    unsigned char t0[11];              // +0x11e
    unsigned char t1[11];              // +0x129
    char unknown_134[0x144 - 0x134];
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00457540 players[10];       // +0x1b63
    char unknown_2851[0x391ed - 0x2851];
    MissionConditions* conditions;     // +0x391ed
};

struct Packet_00457540 {              // 0x3a bytes
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    short field_2;                     // +0x2
    char unknown_4[0x6 - 0x4];
    short field_6;                     // +0x6
    char unknown_8[0xa - 0x8];
    short field_a;                     // +0xa
    char unknown_c[0xe - 0xc];
    short field_e;                     // +0xe
    char unknown_10[0x12 - 0x10];
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

struct TeamPacket_00457540 {           // 3 bytes
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    unsigned char flag2;               // +0x2
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
void __stdcall SendPlayerEconomy(Player_00457540* player, Player_00457540* target,
                            unsigned char flag);

// FUNCTION: 0x457540
void __stdcall HandlePlayerEconomy(Packet_00457540* packet, Player_00457540* player)
{
    if (player == 0)
        return;
    if (player->field_22 != 0)
        return;

    int found = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)
            && g_game->players[i].t1[player->field_146] != 0)
            found = 1;
    }

    if (found == 0) {
        player->field_fc = packet->field_2;
        player->field_fe = packet->field_6;
        player->field_104 = packet->field_a;
        player->field_106 = packet->field_e;
        player->field_98 = packet->field_12;
        player->field_8c = packet->field_16;
        player->field_a8 = packet->field_1a;
        player->field_a4 = packet->field_1e;
        player->field_ac = packet->field_22;
        player->field_bc = packet->field_26;
        player->field_cc = packet->field_2a;
        player->field_b4 = packet->field_2e;
        player->field_c4 = packet->field_32;
        player->field_d4 = packet->field_36;
    }

    if (packet->flag == 0)
        return;

    TeamPacket_00457540 team;
    team.type = 0x29;
    team.flag = 1;

    {
    for (int i = 0; i < 10; i++) {
        Player_00457540* p = &g_game->players[i];
        if (p->active == 0)
            continue;
        if (p->type != 1 && p->type != 2)
            continue;
        if (p->field_22 != 0)
            continue;
        p->t1[player->field_146] = 1;
        if (p->t0[player->field_146] != 0)
            team.flag2 = 1;
        else
            team.flag2 = 0;
        SendPacketToPlayer(p->dpid, player->dpid, &team, 3);
        if (g_game->conditions->CheckVictory() != 0)
            continue;
        if (p->t0[player->field_146] != 0)
            continue;
        SendPlayerEconomy(p, player, 1);
    }
    }
}
