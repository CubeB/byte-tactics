// Decompiled by DeepSeek V4.1 Flash, then Claude Opus 5.5, finished by
// This file of the original was built with /Gz, so the function is __stdcall;
// found by the orchestrator's calling-convention sweep of every partial.
// space-bunny-free. Names are provisional.
// Sends the average of the six load-stage percentages (g_game+0x38d6f..74)
// as a two-byte packet (type 0x2a) to every active player of type 1 or 2,
// and stores it in that player's +0x20.
//
// PARTIAL (98.6%). One difference is left, a two-instruction order at the
// loop latch. After the call to FUN_00451df0 the original reloads the two
// values the call may have clobbered in this order
//
//     mov ecx, dword ptr [g_game]     ; for the next iteration's active test
//     mov dl, byte ptr [esp+0xb]      ; packet.progress
//
// while this file reloads them the other way round. Both are independent
// loads hoisted to the same point, so only the scheduler's choice differs.
// Ruled out (all scored, none reached 100%): taking the packet address into
// a local, a per-iteration `Player*` for the fields (both the all-pointer
// and the mixed `p->` / `g_game->players[i].` spellings), reading
// packet.progress into a local before the loop (83.3%) or at the top of each
// iteration, reading packet.progress a second time in the body, an
// `unsigned char` loop counter (74.6%), `i != 10`, do/while, a `switch` on
// the type (94.4%), the type test before the active test (91.5%), hoisting
// PlayerId into a local (38.4%), and marking the packet's progress, the
// player's progress, the player's active field or g_game itself volatile
// (94.4% and 41.6% for the two that hurt). The `volatile` on `stages` is
// still needed for the rest; see below.
//
// The big win, 73.2% to 98.6%, was declaring the six stage bytes volatile,
// plus writing the sum as an accumulating chain rather than one expression.
//
// - `volatile unsigned char stages[6]` is what produces the original's
//   widening of each byte, `mov al, [m]; and eax, 0xff`. As a plain
//   `unsigned char` field MSVC always folds the widening into the load and
//   emits `xor eax, eax; mov al, [m]`, and the `and` never appears. The
//   volatile load forces a real byte load whose upper bits are unknown, so
//   the conversion to int needs the explicit mask. This is the same shape as
//   the network flags field at g_game+0x38d75 that AGENTS.md already records
//   as volatile, and the evidence is the same: the mask cannot be produced
//   any other way. Flagged in the pull request for the orchestrator to
//   decide. Earlier passes ruled out every non-volatile spelling:
//   char / bool / (unsigned char) / `& 0xff`, 8-bit bitfields in char, short
//   and int containers, a `short` accumulator, plain and accumulating sums,
//   `t += v` in a loop, a pointer to the byte array, and inline
//   `unsigned char` helpers of every shape (plain, ternary, switch, loop,
//   with and without an `if`, taking the value or an int accumulator). The
//   `if` and `switch` forms do keep all six `and`s, in the original's load
//   order, but park the six bytes in stack slots and score 58 to 71%.
// - The sum is `stages[0]` then `+= stages[5]`, `+= stages[4]`, `+= stages[3]`,
//   `+= stages[2]`, `+= stages[1]`. As one expression MSVC emits the loads in
//   descending address order no matter how they are written (all six
//   permutations score the same), which is not the original's order; as a
//   chain of `+=` in this order the first term is loaded into the
//   accumulator's own low byte (`mov dl, [ecx+0x38d6f]`), as the original
//   does, and the rest follow in descending order.
// - `packet.type = 0x2a` has to be the first statement of the function, so
//   the store lands next to the g_game load before the `push ebx` / `push esi`
//   prologue stores. Written after the sum it scores 88.7% instead of 90.1%.
// - The sum needs its own `int total` statement, and `<stdio.h>` is needed for
//   the `[ecx + esi]` base/index order (without it the base and index swap).
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
    volatile unsigned char stages[6];   // +0x38d6f
};

struct Packet_00456de0 {
    unsigned char type;                 // +0x0
    unsigned char progress;             // +0x1
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_00451df0(int player, void* data, int size);

static inline int PlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

// FUNCTION: 0x456de0
void __stdcall FUN_00456de0()
{
    Packet_00456de0 packet;
    packet.type = 0x2a;
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
            FUN_00451df0(PlayerId(i), &packet, 2);
        }
    }
}
