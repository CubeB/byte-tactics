// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry in #3190: seven checker invocations, best remains 88.8%; no MATCH. Helper forms scored 86.9%, 71.9%, and 86.5%; the local type alias tied at 88.8%. The bit-19 arithmetic register allocation and one-byte size difference remain.
// #2988 retry by GPT-6.1-sol: five checks retained 88.8%; a pointer local
// scored lower. Early-exit targets and bit-19 register allocation still differ.
// GPT-6.1-sol retry (#2420): two checker runs kept the existing 88.8% best;
// one Windows invocation failed before the checker. A register-int split
// variation emitted identical code; the prior bit-19 register mismatch remains.
// deepseek-v4.1 pass (#2008): 88.8%, 262 bytes, one byte over. New best shape for
// the bit 19 block: `int h; int* p = &h; *p = unit->type->draft * 0xffff;
// unit->pos.y = (*p + g_game->seaLevel) << 16;`. Taking h's address is the only
// construct found that stops MSVC folding `(x * 0xffff + y) << 16` into
// `(y - x) << 16`, and with the fold blocked the block's last three instructions
// match the original (`add ecx,eax / shl ecx,0x10 / mov [esi+0x6e],ecx`). What
// still differs, all inside that block: the multiply lands in eax (original: ecx),
// so `shl eax,0x10 / sub eax,ecx` replaces `shl ecx,0x10 / sub ecx,eax`; the
// g_game load is hoisted to the top of the block and goes to edx (the original
// loads it after the multiply into eax, `mov eax,[0x511de8]` being the 5 byte A1
// form against our 6 byte `mov edx,...`, which is exactly the one byte of size);
// and the sea byte loads into ecx (original: edx, whose `xor edx,edx` is hoisted
// above the draft load). The same 262 byte shape and the same eax/ecx/edx rotation
// came out of every spelling tried this pass, all screened with `check.py --sym`:
// `(*p + sea)` and `(sea + *p)`, `*p += sea`, a seeded `*p = 0`, `unsigned char`
// and `unsigned int` locals for the sea, the explicit `*p = draft << 16;
// *p -= draft`, and a Pos* pointing at unit->pos (88.3%, 265 bytes). Plain locals,
// struct and union members and int[1] all fold back to 255 bytes at 86.9%. The
// immediate predecessor of this form, `*p += g_game->seaLevel;`, is byte identical
// except it accumulates the sum into eax (86.5%). This is the same allocator wall
// the sibling 0x4589c0 reports at 0x458abf; no source spelling controls which
// register receives the multiply result.
// GPT-6.1-sol retry (#1616): still 86.9%; explicit shift/subtract tied the existing best.
// Claude Sonnet 5.5 pass (#755): still 86.9% and 255 bytes, code unchanged. Re-checked
// on top of the list below, none of it moved the bit 19 fold: the declaration-count
// sweep (0 to 400 in steps of 8, flat at 255 bytes) and all 128 header sets of
// headers.py (86.9% at best); the outer shift written as a multiply (`* 0x10000`,
// `* 65536`, `0x10000 *`, an unsigned cast around the sum, a local `h * 0x10000`);
// `unsigned char` locals for draft and seaLevel in both orders; `(int)(d * 0xffff)`
// inside the sum; `(d << 16) - d + sea` written out (one expression, a local, or with
// the parts on separate statements); and accumulating in `unit->pos.y` itself
// (`pos.y = d * 0xffff; pos.y += sea; pos.y <<= 16;`), which is the only form that
// keeps the original's eleven instructions without a pointer trick, but adds the
// intermediate `mov [esi+0x6e], eax` store (265 bytes, 88.3%, the highest score seen but
// bigger than the original), and a clamp on the product (`if (h < 0) h = 0`, 266
// bytes, 87.8%). The original has no such store: its product only ever lives in ecx.
// Places a unit's height (pos.y, 16.16 fixed point) on the ground, at the sea
// level or on the water surface, depending on the flags in the unit's type
// (+0x241): bit 12 floats, bit 19 floats on water, bit 20 can leave the water.
// Only runs for a unit that belongs to somebody and whose type can be off the
// ground; the flag at +0x110 bit 16 asks for this to be redone.
//
// NOT MATCHED: the bit 19 branch (0x48a93f to 0x48a966). The original keeps
// `draft * 0xffff + seaLevel` as a 32 bit value and shifts that sum afterwards
// (`shl ecx,0x10; sub ecx,eax; ... add ecx,edx; shl ecx,0x10`), while MSVC 5
// folds `(x * 0xffff + y) << 16` into `(y - x) << 16`. That fold is exact (both
// give the same 32 bits), so it always wins the cost comparison, and it fired
// in every plain form tried: 0xffff / 65535 / 0xffffL, the operands swapped,
// a local or a long local for the sum, a static inline helper returning it, a
// static inline helper taking a pointer, unsigned casts, a 64 bit cast of the
// sum and a 64 bit local (the only thing that stopped the fold was routing the
// product through a pointer to a local, `int h; int* p = &h; *p =
// draft * 0xffff; *p += g_game->seaLevel; unit->pos.y = *p << 16;`, which then
// gives the original's eleven instructions in the original's order, but MSVC
// hoists the `g_game` load to the top of the block, keeps the product in eax
// instead of ecx, and loads g_game with `mov edx,[0x511de8]` (6 bytes) rather
// than `mov eax,[0x511de8]` (5 bytes), so the function comes out one byte long,
// 262 against 261, and every `je` in it lands one byte past the original's).
// The orchestrator confirmed that diagnosis independently and could not move the
// allocation either: routing the draft through an `unsigned int` local first
// (`unsigned int d = 0; d = type->draft;` before the pointer) also blocks the
// fold and gives the eleven instructions, but still 262 bytes with the product
// in eax; putting the constant on the left (`*p = 0xffff * d`) changes nothing;
// fetching seaLevel into a `unsigned char` local first is also 262; and giving
// each term its own pointer (`*p = d * 0xffff; *q = g_game->seaLevel; *p += *q;`)
// is worse at 71.9% and 264 bytes. The one-byte gap is exactly the `A1` short
// form: the original has eax holding the dead draft value at the moment it
// loads g_game, so the load reuses eax and encodes in 5 bytes, while every
// variant that blocks the fold needs eax for the live product. Nothing tried
// from the source controls which of the two MSVC picks.
// deepseek-v4.1-flash retried this: a dead second use of the sum
// (`int h = ...; if (h == h) {}`) does block the fold with no extra code and
// gives the eleven instructions, but MSVC always lowers it as product in eax,
// g_game in edx (6-byte load) and seaLevel in ecx, i.e. 262 bytes again; it is
// 86.5%. Only a memory barrier (a volatile read/write of the sum) reproduces
// the original's product-in-ecx / g_game-in-eax / seaLevel-in-edx schedule, and
// it adds a stack store plus reload, so it cannot match. Every spelling of the
// multiply (0xffff, 65535, (x<<16)-x, x*0x10000-x, a 16.16 bitfield
// MakeFixed().value, an __int64 sum, long/unsigned/short locals, a static
// inline helper, three term splits) folds; headers.py (plain and --cpp) and an
// unused-declaration sweep to 3000 prototypes all leave the fold in place.
// The three early exits in the original jump to 0x48a96f, the shared epilogue at
// the very end, and the last branch (the FUN_0048a490 call) to 0x48a969 just
// before it; those targets follow from the size of this block, so they move
// with it.
//
// deepseek-v4.1-flash pass (#1283): wall confirmed, code unchanged (86.9%, 255
// bytes). headers.py with no header and with --cpp (all 128 and 768 sets) never
// beats 86.9%. A byte scan of the exe for the original's non-folded product
// `8B C8 C1 E1 10 2B C8` (mov ecx,eax; shl ecx,0x10; sub ecx,eax) and for the
// whole bit 19 block `33 C0 33 D2 8A 81 2C 02 00 00 8B C8 C1 E1 10 2B C8` finds
// exactly one hit each (0x48a94b), so there is no sibling copy to learn the
// source from. The second hit of the product form is 0x458abf inside 0x4589c0,
// whose own notes record the identical unresolved fold. The pointer-to-local
// variant was rerun here: 262 bytes, 86.5%, product in eax, g_game hoisted to
// edx, seaLevel in ecx, i.e. the whole 11-instruction block is a rotation of
// the original's ecx/eax/edx schedule, matching docs/field-notes.md item 2
// ("callee-saved register rotation wall"). Nothing in the source spelling
// controls which register the allocator picks for the multiply result, so this
// is left as a wall.

#pragma pack(push, 1)
struct UnitType_0048a870 {
    char unknown_0[0x22c];
    unsigned char draft;                // +0x22c
    char unknown_22d[0x241 - 0x22d];
    unsigned int flags_lo : 12;         // +0x241 bits 0..11
    unsigned int floats : 1;            // +0x241 bit 12
    unsigned int unknown_13 : 6;        // +0x241 bits 13..18
    unsigned int on_water : 1;          // +0x241 bit 19
    unsigned int over_water : 1;        // +0x241 bit 20
    unsigned int unknown_21 : 11;       // +0x241 bits 21..31
};

struct Pos_0048a870 {
    int x;                              // +0x0
    int y;                              // +0x4
    int z;                              // +0x8
};

struct Unit_0048a870 {
    int* owner;                         // +0x0
    char unknown_4[0x6a - 4];
    Pos_0048a870 pos;                   // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType_0048a870* type;            // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags;                 // +0x110
};

struct Game_0048a870 {
    char unknown_0[0x1427f];
    unsigned char seaLevel;             // +0x1427f
};
#pragma pack(pop)

extern Game_0048a870* g_game;

int __stdcall FUN_00485070(Pos_0048a870* pos);
void __stdcall FUN_0048a490(Unit_0048a870* unit);

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x48a870
void __stdcall FUN_0048a870(Unit_0048a870* unit)
{
    if ((unit->flags & 0x10000) || unit->type->floats) {
        unit->flags &= ~0x10000;
        if (unit->owner && (unit->flags & 3) == 1) {
            if (unit->type->over_water) {
                if (unit->type->floats) {
                    unit->pos.y = max(FUN_00485070(&unit->pos), g_game->seaLevel - unit->type->draft) << 16;
                } else {
                    unit->pos.y = FUN_00485070(&unit->pos) << 16;
                }
            } else if (unit->type->on_water) {
                int h;
                int* p = &h;
                *p = unit->type->draft * 0xffff;
                unit->pos.y = (*p + g_game->seaLevel) << 16;
            } else {
                FUN_0048a490(unit);
            }
        }
    }
}
