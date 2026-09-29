// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 83.6% (627 of 644 bytes; up from 82.6%). The signature was never
// wrong: `bool __stdcall FUN_0048d9a0(int, int)`, and everything below the
// description is settled. What moved the file this round was ONE change, the
// second scan being written out inline in the match block instead of being an
// out-of-line `static inline` helper assigned to `found`:
//
//   int found;                                  // NOT initialised
//   ...
//   if (... && TestBit(setB, u->type)) {
//       Player_0048d9a0* q = &g_game->players[g_game->localPlayer];
//       for (v = (Unit32*)q->unitsBegin; v <= (Unit32*)q->unitsEnd; v++) {
//           if (...) { found = 1; break; }
//       }
//       break;
//   }
//
// `found` must NOT be initialised at its declaration. With `int found = 0;`
// the same body scores 82.2%, and with a helper doing `found = AnyFlag32(q,
// id)` it scores 82.6%: the initialiser makes MSVC 5 store 0 to the slot BOTH
// before the second FUN_00488c50 call and again right after it (one redundant
// `mov [esp+0x14], ebx` the original does not have), and the helper form also
// reorders the merge block so the `found = 0` store lands before the
// `mov ebx, [esp+0x10]` reload instead of after it. Leaving `found`
// uninitialised is correct here because every path into the third loop either
// stores 0 (0x48db0e) or stores 1 (0x48dc17), so the value is always defined.
//
// NEGATIVE RESULTS, all measured with tools/check.py's own metric on compiled
// objects (no check.py run spent):
//   - Declaration order of {setA, cnt, player, found} is a DEAD axis on this
//     function. Ten orderings, including `found` before and after `player`,
//     before and after `cnt`, and before and after the three loop cursors, all
//     produced byte-identical objects at 627 bytes and 83.63%. This is the
//     lever that fixed 0x48c390; here it does nothing.
//   - Passing the second scan's two bounds as arguments
//     (`AnyFlag32BE(begin, end, id)`) is much worse: 68.2% with `found`
//     initialised, 68.0% without the block-local `q`.
//   - Using the function-scope `player` as the second scan's pointer instead of
//     a fresh `&g_game->players[g_game->localPlayer]` is catastrophic: 54%.
//   - An `int r` local inside the second scan assigned to `found` afterwards
//     scores 73.3%; `found = 0;` written before the inline scan 71.1%; a
//     `do`-style scan with an explicit end pointer 74.1%; spelling the two
//     bounds differently to defeat common-subexpression elimination 73.6%.
//   - Casting the second scan's player address through `void*` and reordering
//     the declarations are both exactly neutral (83.63%, same 627 bytes).
//   - Declaring the second scan's player pointer as a FUNCTION-SCOPE local
//     instead of a block-local `q` is also exactly neutral (83.63%, 627 bytes).
//   - REASSIGNING setA is a total loss: 35.5%. Writing
//     `setA = (Class_00488d30*)&g_game->players[g_game->localPlayer];` before
//     the second call, reading the second scan's bounds through setA, and
//     leaving the third loop's `TestBit(setA, ...)` on the reassigned setA is
//     the ONLY model that explains gap 2 below (one variable, so the second
//     scan's address store provably lands in setA's own [esp+0x1c] slot, and
//     the third loop's `TestBit` provably reads the player address), but it
//     costs 48 points: MSVC then keeps the player address live in the setA
//     chain across all three loops and the first loop's two bounds stop folding
//     into [ecx+eax*2+0x1bca] / [ecx+eax*2+0x1bce]. Do not retry it as written;
//     the real source had a weaker form of it that MSVC 5 then over-optimised.
//
// WHAT IS STILL LEFT, three gaps, all in the second scan and all allocation:
//   1. The materialised player address. The original does
//      `lea esi, [ecx + eax*2 + 0x1b63]` at 0x48da14 and then loads
//      `[esi+0x67]` and `[esi+0x6b]`; every variant here folds the constant
//      and does `lea esi, [ecx + eax*2]` with `[esi+0x1bca]` and
//      `[esi+0x1bce]`. The DEAD store of the same address at 0x48d9e1 DOES get
//      the 0x1b63 in the `lea` in every variant, so MSVC 5 can form the element
//      address; it declines to when the address feeds two field loads. Tried:
//      char* casts, array decay, const, inlined getters, the function-level
//      `player`, and passing begin/end as two arguments. Still the axis to
//      attack.
//   2. Two frame slots are swapped. The original keeps `found` at [esp+0x14]
//      and the third loop's player pointer at [esp+0x18], and its dead store of
//      the player address at 0x48d9e1 lands on [esp+0x1c], sharing setA's
//      slot. This file puts the third loop's player pointer at [esp+0x14] and
//      `found` at [esp+0x18], and the dead store also lands on [esp+0x18].
//      Since declaration order does not move these, the slot order follows
//      MSVC 5's own order of first definition, and the only way to move
//      `found` up to +0x14 that was found is to initialise it, which costs
//      more elsewhere (see above). Note that the original NEVER STORES
//      [esp+0x18] either: the third loop's `mov edi, [esp+0x18]` at 0x48db16
//      reads a local that was never written, exactly as `mov ebx, [esp+0x10]`
//      reads a never-written counter. So the third loop's player pointer is an
//      uninitialised local in the original, and that, not the register
//      allocation, is what MSVC 5 fell into when it reused setA's slot.
//   3. The out-of-line `found = 1` block. The original's 0x48dc13 is
//      `mov ebx, [esp+0x10]` / `mov [esp+0x14], 1` / `jmp 0x48db16`: the
//      `mov ebx, [esp+0x10]` counter reload is duplicated at the second
//      scan's two exits rather than shared in one merge block, which is the
//      last 17 bytes of code this file is missing.
//
// ON THE ORIGINAL. [esp+0x10], [esp+0x14], [esp+0x18] and [esp+0x1c] are the
// four pushed registers (ebx, ebp, esi, edi), and the function uses all four
// push slots for its own variables. Two consequences are visible and both
// look like MSVC 5 frame-allocation artefacts rather than Cavedog mistakes:
//   - `mov ebx, [esp+0x10]` reloads the counter from the saved-ebx slot, which
//     nothing ever wrote, so on any path through the first scan's failed bit
//     test the counter starts from the CALLER's ebx, and the third loop's
//     `inc ebx` counts from there.
//   - the store at 0x48d9e1 puts the player address into [esp+0x1c], the slot
//     that held setA from 0x48d9b9, so the third loop's `TestBit(setA, ...)`
//     at 0x48db9c reads the player address as the bit set.
// Re-derive either before believing it; the evidence is the operand lists
// above. The float-comparison inversion noted below is likewise an MSVC 5
// idiom, not a Cavedog bug, and the same inversion appears in the matched
// 0x48be00 and 0x48c9b0.
//
// Three scans over the local player's unit list. The first looks for a unit
// that passes the common test and whose type index (+0xa6) is in the second
// CTRL_F set; the second re-scans the same list for one of those that also
// carries bit 31 of its flags word; the third tags or clears bit 4 of the
// flags word of every unit that matches the given id, counts the tagged ones,
// issues the STOP order, drops the selection and sets order flag 0x10 at
// +0x37ebe, and returns whether anything was tagged. The unit flags at +0x110
// are read as a byte in the first scan and as a dword in the other two, so the
// two unit struct views are separate types (see 0x48dc30, which is the same
// first scan in miniature and matches).
// Suspected original bug: MSVC 5 inverts the sense of the float comparison
// against zero, so `== 0.0f` here tags units whose +0x104 value is NOT 0.0f
// (same inversion as 0x48be00 and 0x48c9b0, which match with the same code).

#pragma pack(push, 1)

struct Team_0048d9a0 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

struct Unit8_0048d9a0 {               // 0x118 bytes, flags read as a byte
    char unknown_0[0x86];
    Team_0048d9a0* owner;              // +0x86
    char unknown_8a[0xa6 - 0x8a];
    unsigned short type;               // +0xa6
    char unknown_a8[0xac - 0xa8];
    int field_ac;                      // +0xac
    char unknown_b0[0xfb - 0xb0];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned char flags;               // +0x110
    char unknown_111[0x118 - 0x111];
};

struct Unit32_0048d9a0 {              // 0x118 bytes, flags read as a dword
    char unknown_0[0x86];
    Team_0048d9a0* owner;              // +0x86
    char unknown_8a[0xa6 - 0x8a];
    unsigned short type;               // +0xa6
    char unknown_a8[0xac - 0xa8];
    int field_ac;                      // +0xac
    char unknown_b0[0xfb - 0xb0];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_0048d9a0 {              // 0x14b bytes
    char unknown_0[0x67];
    Unit8_0048d9a0* unitsBegin;        // +0x67
    Unit8_0048d9a0* unitsEnd;          // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048d9a0 {
    char unknown_0[0x1b63];
    Player_0048d9a0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37e9c - 0x2a43];
    unsigned short field_37e9c;        // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags_0 : 4;        // +0x37ebe
    unsigned short orderFlag : 1;      // +0x37ebe, bit 4
    unsigned short flags_5 : 11;
};
#pragma pack(pop)

extern Game_0048d9a0* g_game;

void FUN_00495860(void);

class Class_00488d30 {
public:
    int bits[16];
};

Class_00488d30* __stdcall FUN_00488c50(char* name);

static inline int TestBit(Class_00488d30* set, unsigned short n)
{
    return set->bits[n >> 5] & (1 << (n & 0x1f));
}

// FUNCTION: 0x48d9a0
bool __stdcall FUN_0048d9a0(int id, int param_2)
{
    Class_00488d30* setA = FUN_00488c50("CTRL_F");
    int cnt = 0;
    Player_0048d9a0* player = &g_game->players[g_game->localPlayer];
    Class_00488d30* setB = FUN_00488c50("CTRL_F");

    Unit8_0048d9a0* u;
    Unit32_0048d9a0* v;
    Unit32_0048d9a0* w;
    int found;
    for (u = g_game->players[g_game->localPlayer].unitsBegin; u <= g_game->players[g_game->localPlayer].unitsEnd; u++) {
        if ((u->flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0
            && (u->owner == 0 || (u->owner->flags & 0x40000000))
            && u->field_ac == id && TestBit(setB, u->type)) {
            {
                Player_0048d9a0* q = &g_game->players[g_game->localPlayer];
                for (v = (Unit32_0048d9a0*)q->unitsBegin;
                     v <= (Unit32_0048d9a0*)q->unitsEnd; v++) {
                    if ((v->flags & 0x20) && v->field_104 == 0.0f && v->field_fb == 0
                        && (v->owner == 0 || (v->owner->flags & 0x40000000))
                        && v->field_ac == id && (v->flags & 0x80000000)) {
                        found = 1;
                        break;
                    }
                }
            }
            break;
        }
    }

    for (w = (Unit32_0048d9a0*)player->unitsBegin;
         w <= (Unit32_0048d9a0*)player->unitsEnd; w++) {
        if ((w->flags & 0x20) && w->field_104 == 0.0f && w->field_fb == 0
            && (w->owner == 0 || (w->owner->flags & 0x40000000))) {
            if (w->field_ac == id) {
                if (found) {
                    if (TestBit(setA, w->type)) {
                        w->flags &= ~0x10;
                    } else {
                        w->flags |= 0x10;
                        cnt++;
                    }
                } else {
                    w->flags |= 0x10;
                    cnt++;
                }
            } else if (param_2 == 0) {
                w->flags &= ~0x10;
            }
        }
    }
    FUN_00495860();
    g_game->field_37e9c = 0;
    g_game->orderFlag = 1;
    return cnt > 0;
}
