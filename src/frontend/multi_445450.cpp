// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Matched: two independent fixes were needed on top of the earlier partial.
//
// 1. The scan loop's guard was wrong. The original requires p < end for both
//    ways of stepping over a slot, so the source is
//    `((active && type in 1..3 && field_146 != 10) || type == 4) && p < end`,
//    not a `p < end` attached only to the type-4 clause. The earlier partial
//    produced the same instructions but let the `jne` after the field_146
//    compare jump straight to the `add edx, 0x14b`, skipping the bound test
//    (original: `jne 0x4454a8`, the `cmp edx, ecx`).
//
// 2. The SIB base/index order in the renumbering loop (`[ecx + eax + 0x1b63]`
//    with g_game as the base, not `[eax + ecx + 0x1b63]`) is the guide's
//    header-dependent operand order: with `#include <windows.h>` at the top
//    (and the corrected scan guard) MSVC emits the right SIB. `<windows.h>`
//    on its own fixed the SIB but left the guard difference; the guard fix on
//    its own left the SIB swapped. Neither header set from tools/headers.py
//    matched by itself because the guard still differed.
//
// The earlier writer's two findings still hold and should not be undone:
//  - reading the global into a local (`Game* g = g_game;`) as the
//    first statement of the loop body moves the reloaded global into ecx;
//  - taking the field through a byte pointer (`unsigned char* f =
//    &g->players[i].field_146;` then `*f`) stops MSVC emitting an extra
//    `lea ecx, [eax + ecx + 0x1ca9]` and reloading through it, so the store
//    is written straight back to the same address as the compare.

#include <windows.h>

#pragma pack(push, 1)
// A player slot: the same 0x14b byte record the game keeps in g_game->players
// and the class of the callee below (it writes this->type at +0x73).
class Player_00445450 {
public:
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    int field_27;                      // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

class Class_00463c60 {
public:
    void FUN_00463c60(int param_1);
};

struct Game {
    char unknown_0[0x1b63];
    Player_00445450 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
};
#pragma pack(pop)

extern Game* g_game;

// Compacts the player list: finds the first slot the renumbering pass would
// call dead, moves the next live slot into it, clears the slot it left, then
// renumbers every slot's field_146.
// FUNCTION: 0x445450
void FUN_00445450()
{
    Player_00445450* p = g_game->players;
    Player_00445450* q = g_game->players + 1;
    Player_00445450* end = g_game->players + 10;
    while (1) {
        if (q >= end && p >= end)
            break;
        // Step over slots that are in use, and over type 4 slots.
        while ((p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10)
               || p->type == 4) {
            if (p >= end)
                break;
            p++;
        }
        q = p + 1;
        // Find the next slot that is in use.
        for (; q->active == 0
               || (q->type != 1 && q->type != 2 && q->type != 3)
               || q->field_146 == 10;
             q++) {
            if (q >= end)
                break;
        }
        if (q >= end)
            break;
        if (p >= end)
            break;
        Player_00445450 tmp = *p;
        *p = *q;
        *q = tmp;
        ((Class_00463c60*)q)->FUN_00463c60(0);
        q->active = 0;
        for (int i = 0; i <= 10; i++) {
            Game* g = g_game;
            unsigned char* f = &g->players[i].field_146;
            if (g->players[i].active != 0
                && (g->players[i].type == 1 || g->players[i].type == 2
                    || g->players[i].type == 3) && *f != 10) {
                *f = (unsigned char)i;
            } else {
                *f = 10;
            }
        }
    }
}
