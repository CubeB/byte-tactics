// Decompiled by space-bunny-free. Names are provisional.
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
// The three early exits in the original jump to 0x48a96f, the shared epilogue at
// the very end, and the last branch (the FUN_0048a490 call) to 0x48a969 just
// before it; those targets follow from the size of this block, so they move
// with it.

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
                unit->pos.y = (unit->type->draft * 0xffff + g_game->seaLevel) << 16;
            } else {
                FUN_0048a490(unit);
            }
        }
    }
}
