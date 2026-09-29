// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Not matched yet, 84.7% (354 bytes, same size as the original). The only
// difference left is register allocation in the prologue: the original keeps
// the x difference in ecx (b.x is loaded into ecx before the register pushes,
// so `sub ecx, ebx`, the abs block runs in esi, and n ends up in esi), while
// here the difference lands in esi and the abs temporary and n use ecx.
//
// What made the jump from 75.3%: subtract in place on the by-value parameter
// (b.y -= a.y; b.z -= a.z; b.x -= a.x) and then copy it whole into the step
// vector (`Vec3 d = b;`). The struct copy is what keeps the original's dead
// store of the undivided x difference (`mov [esp+0x10], ecx`) and drops the z
// one; field-by-field copies, a local per difference, a Diff() helper, a
// `d = b` before the subtractions and an aggregate initialiser all score lower
// (71 to 82%). The y step is the undivided difference, as the original has it.
// Scored without changing 84.7%: all six subtraction orders (yzx and yxz are
// best), the three spellings of the abs maximum, `/=` vs `= x / n`, taking abs
// and the divisions from b or from d, n/i/c/best declared up front in every
// order, spelling each subtraction five ways, and 1500 random mixes of in-place
// and local differences.
//
// deepseek-v4.1-flash additions (all still 84.7% or lower): all 128 header
// sets from tools/headers.py (best 84.7% with <stdlib.h>), a Sub/MaxAbs
// static inline helper in every field order, separate `int` difference
// locals built into d, an explicit max local, __max, swapped abs order and
// swapped division order, `Vec3 p = a` copies, and six-arg layouts. The
// original loads b.x into ecx before the callee-saved pushes (a.y already
// owns esi there, and esi is reused for abs(dx)/n), while ours reuses esi for
// b.x and gives ecx to abs(dx)/n. Nothing tried moves that one choice.
//
// Second pass (deepseek-v4.1-flash), still 84.7%: the whole hunk is the one
// register pair dx=ecx/abs=esi (original) vs dx=esi/abs=ecx (ours). Tried
// again and ruled out: `Vec3_004851c0 d = b - a` with an inline operator-
// whose body order is xyz, yxz, yzx, and by-value or const-ref parameters
// (80.6 to 81.5%); `Vec3 d = b; d -= a;` (82.3%), and the copy before and
// after the subtractions, `d(b)`, default-construct then assign, d declared
// at function scope, and field-by-field copies (74.5 to 84.7%): every form
// that keeps the copy after the in-place subtraction reproduces the byte
// sequence and the dead `mov [esp+0x10], dx`, so the source shape is right.
// Also tried: all six subtraction orders, six raw int component locals
// initialised in the original load order (b.y, b.x, a.x, a.y, a.z, b.z) then
// subtracted, n computed from the parameter fields before the copy, and a
// reference alias of b. tools/headers.py confirms no header set beats 84.7%.
// The original's b.x load is hoisted above the push ebx and above a.y's
// load, which is a scheduler/allocator decision this source shape does not
// reach; the instruction sequence is otherwise identical.
#include <stdlib.h>

#pragma pack(push, 1)
struct Cell_004851c0 {
    unsigned short unit;               // +0x0
    char unknown_2[0x4 - 0x2];
    unsigned char height;              // +0x4
    char unknown_5[0x8 - 0x5];
    unsigned short field_8;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct Type_004851c0 {
    char unknown_0[0x16e];
    int field_16e;                     // +0x16e
};

struct Unit_004851c0 {
    char unknown_0[0x6e];
    int field_6e;                      // +0x6e
    char unknown_72[0x92 - 0x72];
    Type_004851c0* type;               // +0x92
    char unknown_96[0x118 - 0x96];
};

struct Game_004851c0 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    unsigned char* mapping;            // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell_004851c0* cells;              // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    Unit_004851c0* units;              // +0x14357
};
#pragma pack(pop)

extern Game_004851c0* g_game;

struct Vec3_004851c0 {
    int x;
    int y;
    int z;
};

static inline Cell_004851c0* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// FUNCTION: 0x4851c0
int __stdcall FUN_004851c0(Vec3_004851c0 a, Vec3_004851c0 b)
{
    b.y -= a.y;
    b.z -= a.z;
    b.x -= a.x;
    Vec3_004851c0 d = b;
    int n = (abs(d.x) < abs(d.z) ? abs(d.z) : abs(d.x)) / 0x100000 + 1;
    d.x /= n;
    d.z /= n;
    short best = 0;
    for (int i = 0; i <= n; i++) {
        Cell_004851c0* c = GetCell(a.x / 0x100000, a.z / 0x100000);
        if (c) {
            short v = g_game->mapping[c->field_8 * 256 + 0xfa] + c->height;
            if (best < v) best = v;
            if (c->unit) {
                short w = (g_game->units[c->unit].type->field_16e + g_game->units[c->unit].field_6e) >> 16;
                if (best < w) best = w;
            }
        }
        a.x += d.x; a.y += d.y; a.z += d.z;
    }
    return best;
}
