// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry: verified the saved best at 84.9%; the remaining mismatch
// is the local stack-slot/register permutation documented below.
// GPT-6 retry: remains 84.9%. Byte-index classes and inheritance, and a
// three-component average position with varied scope/representation, did
// not improve the saved frame slots. The existing implementation is retained.
// Partial (84.9%), both sides exactly 742 bytes. Deepseek-v4.1-flash got here
// from space-bunny-free's 63.4% by three changes, all on the same shape:
//   1. Pass flag_a, not `range`, as FUN_0043afc0's second argument. The
//      original loads the flag_a slot both times (0x48d1c6 and 0x48d1e6, the
//      latter with one push outstanding so [esp+0x1c] is the +0x18 slot).
//      `use_flag = range` was only steering the allocator.
//   2. Write the FUN_0043afc0 call in BOTH arms (`continue` in the inner one)
//      instead of one call after a shared `use_pos`. That restores the
//      original's two argument setups, tail-merged into the single call at
//      0x48d1f4. Note this is what rotates the first loop's count/sum registers
//      unless the compiler state is right, which is why it used to score 54 to
//      58%.
//   3. Declare "Standing_FireOrder" as a plain local (no placement new), so
//      there is no null test and no `entry` kept in edi.
// `#include <stdio.h>` is load bearing compiler state: without it this shape
// is 59.5%, with it 84.9%. tools/headers.py reports several sets give 84.9%
// (`<stdio.h>` alone, `<stdio.h>`+`<stdlib.h>`, `<string.h>`+`<math.h>`);
// sweeping N unused `extern int dummyN;` declarations reaches the same 84.9%
// in two windows, N = 282..309 and 346..373, so the shape is right and only
// the state differs. Do not commit such declarations; a real header set works.
// WHAT STILL DIFFERS (all one cause, the frame slot permutation):
//   original: move 0x12, buf 0x13, except 0x14, flag_a 0x18, range 0x1c,
//   p 0x20, avg_x 0x2c, avg_z 0x34, here 0x38.
//   ours: except 0x10, avg_x 0x14, avg_z 0x18, flag_a 0x1c, fire 0x20,
//   move 0x24, range 0x28, p 0x2c, here 0x38, and the one-byte buf gets the
//   dead argument-0 slot (0x48) where the original puts the fire object.
// Because avg_x and avg_z sit in the two slots the original gives flag_a and
// range, the distance block also swaps their registers (ours ebx=avg_x,
// edi=avg_z; original edi=avg_x, ebx=avg_z). Everything else in the diff,
// including every branch target, is just the same permutation. Reordering the
// declarations did not move it; fixed-byte locals and parameter-slot reuse are
// the two suspects left.
// Known-good detail from space-bunny-free: the fifth argument of FUN_0043f0e0
// is the ADDRESS of g_game->field_2caa (`add ecx, 0x2caa`), and the
// field_2cba test is written `if (!x) except = 0; else ...` so the zero store
// is the `jne` fall-through.
//
// Additional attempts by deepseek-v4.1-flash, all no-ops on the permutation:
// moving avg_x/avg_z (or range, or dx/dz) to function scope as bare
// declarations assigned later, and making `buf` function scope. All leave
// every slot where it was (and moving buf drops the score to 83.2% by
// disturbing the fild block). `tools/headers.py` tried all 128 sets: best is
// 84.9% (`<stdio.h>` and several others), none match. This agrees with the
// note above that a flat dummy-declaration sweep only ever reached 84.9%, so
// the remaining frame permutation is translation-unit compiler state, not
// source shape; it should resolve when this file is regrouped into its
// original translation unit in address order.
// deepseek-v4.1 (this session): the frame permutation is invariant to every
// source shape tried. Locals renamed, declaration order changed, avg_x/avg_z/
// range/here/buf moved to function scope, and `int avg[2]` instead of two
// scalars all compile to the same 752-byte body, byte for byte, so the
// permutation is not source shape. It IS translation-unit symbol state: N
// inert `extern int dummy%d;` declarations inserted at the end of the comment
// header move the allocator through a small set of layouts. Measured with a
// per-instruction diff of the compiled body against the original (232
// instructions on both sides, addresses masked, so it counts only real
// differences):
//   N = 0 (and N = 20, 50, 300, 450, 500, 700): 41 instructions differ
//   N = 250 (and 240..260): 35 instructions differ, check.py 87.5%
//   N = 20, 100..200, 265..300, 350..400, 550..600, 800: 102 differ
// N = 250 fixed only the distance block's load
// order (mov ecx,[esi+0x6a] / mov ebp,avg_x / mov ebx,[esi+0x72] /
// mov edi,avg_z, one step closer to the original's eax/edi/ecx/ebx order) and
// leaves the frame permutation: ours puts except 0x10, avg_x 0x14, avg_z 0x18,
// flag_a 0x1c, fire 0x20, move 0x24, range 0x28, p 0x2c, here 0x38, buf 0x48,
// the original puts move 0x12, buf 0x13, except 0x14, flag_a 0x18, range 0x1c,
// p 0x20, avg_x 0x2c, avg_z 0x34, here 0x38, fire 0x48. The two one-byte
// objects (move, buf) end up in dword slots instead of the original's byte
// pool at 0x12/0x13, and `fire` never takes the dead entry-argument slot.
// The dummy-declaration probe is NOT shipped: this file is the clean, probe-free
// version, which is the same 84.9% as origin/main. The probe lives in
// build/scratch/0x48cf30/ and exists only to show that the frame permutation is
// translation-unit symbol state; the real fix is for this file to sit in its
// original translation unit, where the preceding functions and declarations
// supply that state legitimately.
// All 768 header sets from tools/headers.py --cpp were tried in this session
// (best 84.9%, the N = 0 layout), so headers alone cannot supply the state.
// deepseek-v4.1 (second session): a fifteen-variant statement-shape sweep was
// run against this same file. Buf scoped inside the if (mode) block, explicit
// (int) casts on the averages, split declarations with later assignment,
// unsigned or grouped average declarations, function-scope dx/dz, and
// avg_x/avg_z at function scope all reproduce the 84.9% body byte for byte.
// The rest only regress: range declared before the averages 82.8%, buf hoisted
// above the loop 83.2%, p declared after the sums 70.3%, unsigned count 64.4%,
// fire/move built as temporaries 62.1%, and an extra math.h include 75.2%.
// Statement shape therefore does not move the frame permutation; the best
// version stays this 84.9% one.
#include <stdio.h>

#pragma pack(push, 1)

// The unit type table entry, 0x19 bytes each.
struct UnitType_0048cf30 {
    unsigned char index;                // +0x00
    char unknown_1[0x8 - 0x1];
    unsigned int flags_a;               // +0x08
    char unknown_c[0x11 - 0xc];
    unsigned int flags;                 // +0x11
};

// A unit type index, passed and returned by value.
class Class_00438830 {
public:
    unsigned char index;
    UnitType_0048cf30* FUN_00438830();
};

// A unit type index built from a name.
class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct UnitDef_0048cf30 {
    char unknown_0[0x245];
    unsigned char flags;                // +0x245
};

struct Unit_0048cf30 {
    char unknown_0[0x6a];
    int x;                              // +0x6a
    char unknown_6e[0x72 - 0x6e];
    int z;                              // +0x72
    char unknown_76[0x92 - 0x76];
    UnitDef_0048cf30* def;              // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned char flags;                // +0x110
    char unknown_111[0x118 - 0x111];
};

struct Player_0048cf30 {
    char unknown_0[0x67];
    Unit_0048cf30* first;               // +0x67
    Unit_0048cf30* last;                // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048cf30 {
    char unknown_0[0x1b63];
    Player_0048cf30 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char field_2a42;           // +0x2a42
    char unknown_2a43[0x2caa - 0x2a43];
    void* field_2caa;                   // +0x2caa
    char unknown_2cae[0x2cba - 0x2cae];
    unsigned short field_2cba;          // +0x2cba
    char unknown_2cbc[0x14357 - 0x2cbc];
    Unit_0048cf30* field_14357;         // +0x14357
};

extern Game_0048cf30* g_game;

int __stdcall FUN_0043e470(unsigned char type);
unsigned char* __stdcall FUN_0043f0e0(unsigned char* buf, unsigned char mode,
                                       Unit_0048cf30* unit, Unit_0048cf30* target,
                                       void* param_5);
void __stdcall FUN_0043afc0(Class_00438830 kind, int flag, Unit_0048cf30* unit,
                            Unit_0048cf30* target, int* pos, int param_5, int param_6);

// FUNCTION: 0x48cf30
void __stdcall FUN_0048cf30(UnitType_0048cf30* entry, unsigned char mode,
                            Class_00438830 kind, int* pos, int param_5, int param_6)
{
    int flag_a = (entry->flags_a >> 2) & 1;
    Unit_0048cf30* except = 0;
    int flag_b;
    if (mode)
        flag_b = FUN_0043e470(mode);
    else
        flag_b = (kind.FUN_00438830()->flags >> 9) & 1;
    if (flag_b) {
        if (!g_game->field_2cba)
            except = 0;
        else
            except = (Unit_0048cf30*)((char*)g_game->field_14357 + 280 * g_game->field_2cba);
    }
    Player_0048cf30* p = &g_game->players[g_game->field_2a42];
    int count = 0;
    int sum_x = 0;
    int sum_z = 0;
    Unit_0048cf30* u;
    for (u = p->first; u <= p->last; u++) {
        if ((u->flags & 0x10) && u != except) {
            count++;
            sum_x += (short)(u->x >> 16);
            sum_z += (short)(u->z >> 16);
        }
    }
    if (!count)
        return;
    int avg_x = (sum_x / count) * 65536.0;
    int avg_z = (sum_z / count) * 65536.0;
    int range = count * 3000;
    for (u = p->first; u <= p->last; u++) {
        if (!(u->flags & 0x10) || u == except)
            continue;
        unsigned char buf;
        if (mode)
            kind.index = *FUN_0043f0e0(&buf, mode, u, except, &g_game->field_2caa);
        if (!kind.index)
            continue;
        Class_00438760 fire("Standing_FireOrder");
        if (kind.index != fire.index || (u->def->flags & 2)) {
            Class_00438760 move("Standing_MoveOrder");
            if (kind.index != move.index || (u->def->flags & 1)) {
                if (pos && (kind.FUN_00438830()->flags & 2)) {
                    int dx = u->x - avg_x;
                    int dz = u->z - avg_z;
                    if ((int)(((__int64)dx * dx) >> 32)
                        + (int)(((__int64)dz * dz) >> 32) <= range) {
                        int here[3];
                        here[0] = pos[0] + u->x - avg_x;
                        here[1] = pos[1];
                        here[2] = pos[2] + u->z - avg_z;
                        FUN_0043afc0(kind, flag_a, u, except, here, param_5, param_6);
                        continue;
                    }
                }
                FUN_0043afc0(kind, flag_a, u, except, pos, param_5, param_6);
            }
        }
    }
}
