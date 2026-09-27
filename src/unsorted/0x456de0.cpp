// Decompiled by DeepSeek V4.1 Flash, then Claude Opus 5.5. Names are provisional.
// Sends the average of the six load-stage percentages (g_game+0x38d6f..74)
// as a two-byte packet (type 0x2a) to every active player of type 1 or 2,
// and stores it in that player's +0x20.
//
// PARTIAL (73.2%). What matches: the int loop over the players (the
// original keeps i in ebx for PlayerId and an offset in esi compared with
// 0xcee), the sum's load order (stage 0, then 5, 4, 3, 2, 1, which needs
// the sum in its own `int total` statement before packet.type), and the
// [ecx + esi] operand order (needs <stdio.h>; without it the base and index
// swap). What still differs:
// - The original zero-extends each stage byte as `mov al, [m]; and eax,
//   0xff`; ours gives `xor eax, eax; mov al, [m]`. In scratch tests that
//   form only appears when the byte sits in a register as an unsigned char
//   value before the add: a byte local used twice, byte locals followed by
//   any `if`, or an inline `unsigned char` helper with `if ... return`; the
//   last two spill the six bytes to the stack here.
//   The same form reads these bytes in 0x497f40 and g_game+0x2a42 in
//   0x490080, so it may be the field's declared type or a macro rather than
//   this function's shape. Tried and ruled out: char/unsigned char/bool/
//   bitfield elements, (unsigned char) and & 0xff casts, truncated
//   unsigned short/int reads, sub-struct Average() methods, unrolled locals.
// - After the call the original reloads g_game into ecx before
//   packet.progress into dl; ours reloads dl first.
// The N-declarations sweep (0 to 400 externs, up to 6000 prototypes) and
// every headers.py set leave both differences; the best is 73.2%.
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

struct Game_00456de0 {
    char unknown_0[0x1b63];
    Player_00456de0 players[10];        // +0x1b63
    char unknown_2851[0x38d6f - 0x2851];
    unsigned char stages[6];            // +0x38d6f
};

struct Packet_00456de0 {
    unsigned char type;                 // +0x0
    unsigned char progress;             // +0x1
};
#pragma pack(pop)

extern Game_00456de0* g_game;

int __stdcall FUN_00451df0(int player, void* data, int size);

static inline int PlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

// FUNCTION: 0x456de0
void FUN_00456de0()
{
    Packet_00456de0 packet;
    int total = g_game->stages[0] + g_game->stages[1] + g_game->stages[2]
        + g_game->stages[3] + g_game->stages[4] + g_game->stages[5];
    packet.type = 0x2a;
    packet.progress = total / 6;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)) {
            g_game->players[i].progress = packet.progress;
            FUN_00451df0(PlayerId(i), &packet, 2);
        }
    }
}
