// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Shared-resource tick: every 60 game ticks, if the local player is over its
// energy (or metal) reserve and has the matching flag, give up to a third (a
// half for metal) of the surplus to the neediest ally that shows up in the
// per-player array. Every 450 ticks it also sends the subtype-3 share packet
// (the same one 0x4571c0 sends) to every allied player.
//
// Three things were load bearing, all verified by check.py:
//  - The min() must be a ternary assigned to a fresh local
//    (`float result = amount < limit ? amount : limit;`), not an
//    `if (limit < amount) amount = limit;`. The ternary keeps amount and the
//    clamp on the x87 stack the way the original does; the if-form spills and
//    emits `fcomp [esp+0x10]` instead of `fcomp st(1)`.
//  - The bit at info+0x97 bit 5 is a standalone `if (flags.b5)` over an
//    `unsigned short` bitfield. As `(flags & 0x20)` or `(flags >> 5) & 1`
//    MSVC folds it to `test byte ptr [m], 0x20`; the other two tests (bits 1
//    and 2) sit inside `&&` chains and stay byte tests.
//  - `#include <windows.h>` is what flips the SIB base/index order of the
//    third loop's `[esi + edi + 0x1b63]` (g_game base, index), matching
//    0x445450's note. Without it the same instructions use `[edi + esi + ...]`.

#include <windows.h>

#pragma pack(push, 1)
struct FlagBits_00457d30 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;             // +0x97 bit 1
    unsigned short b2 : 1;             // +0x97 bit 2
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short b5 : 1;             // +0x97 bit 5
    unsigned short b6 : 1;
    unsigned short b7 : 1;
    unsigned short b8 : 1;
    unsigned short b9 : 1;
    unsigned short b10 : 1;
    unsigned short b11 : 1;
    unsigned short b12 : 1;
    unsigned short b13 : 1;
    unsigned short b14 : 1;
    unsigned short b15 : 1;
};

struct Info_00457d30 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
    char unknown_95[0x97 - 0x95];
    FlagBits_00457d30 flags;           // +0x97
};

struct Player_00457d30 {
    int active;                        // +0x00
    int dpid;                          // +0x04
    char unknown_8[0x27 - 0x8];
    Info_00457d30* info;               // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x8c - 0x74];
    float metal;                       // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy;                      // +0x98
    char unknown_9c[0xa4 - 0x9c];
    float field_a4;                    // +0xa4
    float field_a8;                    // +0xa8
    char unknown_ac[0xe4 - 0xac];
    float field_e4;                    // +0xe4
    float field_e8;                    // +0xe8
    char unknown_ec[0x108 - 0xec];
    unsigned char field_108[0x140 - 0x108];
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00457d30 players[10];       // +0x1b63
    char unknown_2851[0x2a44 - 0x2851];
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x38a47 - 0x2a45];
    unsigned int ticks;                // +0x38a47
};

struct Packet_00457d30 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int zero;                          // +0xd
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_00464c60(unsigned char from, unsigned char to, float amount, int flag);
void __stdcall FUN_00464b30(unsigned char from, unsigned char to, float amount, int flag);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);

static inline int PlayerDpid_00457d30(unsigned char i)
{
    if (g_game->players[i].type != 0)
        return g_game->players[i].dpid;
    return -1;
}

// FUNCTION: 0x457d30
void __stdcall UpdateResourceSharing(Player_00457d30* player)
{
    if ((g_game->flags_2a44 & 1) == 0)
        return;

    if (g_game->ticks % 60 == 0) {
        Player_00457d30* found = player;
        if (player->info->flags.b1 && player->energy > player->field_e4) {
            for (int i = 0; i < 10; i++) {
                Player_00457d30* p = &g_game->players[i];
                if (p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && (p->field_144 != 0 || p->field_140 == 0)
                    && p->type == 3
                    && p->info->field_94 == 1
                    && player->field_108[i] != 0
                    && p->energy < player->energy)
                    found = p;
            }
        }
        if (found != player && player->energy > player->field_e4) {
            float amount = (player->energy - player->field_e4) * 0.33333334f;
            float limit = found->field_a8 - found->energy;
            float result = amount < limit ? amount : limit;
            amount = result;
            FUN_00464c60(player->field_146, found->field_146, amount, 1);
        }

        found = player;
        if (player->info->flags.b2 && player->metal > player->field_e8) {
            for (int i = 0; i < 10; i++) {
                Player_00457d30* p = &g_game->players[i];
                if (p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && (p->field_144 != 0 || p->field_140 == 0)
                    && p->type == 3
                    && p->info->field_94 == 1
                    && player->field_108[i] != 0
                    && p->metal < player->metal)
                    found = p;
            }
        }
        if (found != player && player->metal > player->field_e8) {
            float amount = (player->metal - player->field_e8) * 0.5f;
            float limit = found->field_a4 - found->metal;
            float result = amount < limit ? amount : limit;
            amount = result;
            FUN_00464b30(player->field_146, found->field_146, amount, 1);
        }
    }

    if (g_game->ticks % 450 == 0) {
        if (player->info->flags.b5) {
        for (int i = 0; i < 10; i++) {
            Player_00457d30* p = &g_game->players[i];
            if (p->active != 0
                && (p->type == 1 || p->type == 2 || p->type == 3)
                && p->field_146 != 10
                && (p->type == 1 || p->type == 2 || p->type == 3)
                && (p->field_144 != 0 || p->field_140 == 0)
                && p->type == 3
                && p->info->field_94 == 1
                && player->field_108[i] != 0) {
                unsigned char a = player->field_146;
                unsigned char b = p->field_146;
                if (a != 10 && b != 10) {
                    Packet_00457d30 packet;
                    packet.type = 0x16;
                    packet.subtype = 3;
                    packet.from = PlayerDpid_00457d30(a);
                    packet.to = PlayerDpid_00457d30(b);
                    packet.zero = 0;
                    SendPacketToPlayer(PlayerDpid_00457d30(a), PlayerDpid_00457d30(b),
                                 &packet, sizeof(packet));
                }
            }
        }
    }
    }
}
