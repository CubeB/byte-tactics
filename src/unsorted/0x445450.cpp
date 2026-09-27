// Decompiled by Space Bunny Free. Names are provisional.
// Partial (91.5%, 341 of 341 bytes, every instruction present). The one
// remaining difference is the SIB base/index order in the renumbering loop:
// the original loads g_game into ecx and forms the slot address as
// [ecx + eax + 0x1b63] (g_game is the SIB base, the induction variable the
// index), while this source produces the identical instructions with the two
// registers the other way round, [eax + ecx + 0x1b63] (the induction variable
// is the SIB base). That is one byte in each of five addresses, so the byte
// counts agree and only the operand order differs.
//
// Two things were needed to get this far, and both matter for the next
// attempt:
//  - reading the global into a local (`Game_00445450* g = g_game;`) as the
//    first statement of the loop body moves the reloaded global from edx into
//    ecx, which is the register the original uses;
//  - taking the field through a byte pointer (`unsigned char* f =
//    &g->players[i].field_146;` then `*f`) stops MSVC emitting an extra
//    `lea ecx, [eax + ecx + 0x1ca9]` and reloading through it, so the store
//    is written straight back to the same address as the compare.
//
// What did not flip the SIB order: a plain `g_game->players[i].field` in every
// position; a local for the global pointer (before or inside the loop); a
// local for the array base; a reference to the global; walking the array with
// a `char*`; a `static inline` getter returning either the field or the slot;
// an int, unsigned, short or char loop index; two induction variables instead
// of MSVC narrowing the index to ebx; hoisting the type or the field into a
// local; a named constant for 10; nested if/else, a negated condition, a
// ternary and a local for the stored value. The worker also tried about ninety
// further spellings. This looks like the guide's rare "operand order that
// nothing changes" case, decided by the term order MSVC builds the address
// from rather than by anything in the source.

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

struct Game_00445450 {
    char unknown_0[0x1b63];
    Player_00445450 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
};
#pragma pack(pop)

extern Game_00445450* g_game;

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
               || (p->type == 4 && p < end)) {
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
            Game_00445450* g = g_game;
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
