// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free and GPT-6.1-sol. Names are provisional.
// Best: 51.1% after 3 checks; `(flags & 0x10) != 0` improved 0.1 points. Still differs because MSVC keeps `order` in ecx instead of reloading it from its argument slot, shifting the division and draw-call registers.
// Draws the on-screen bounding box of the object's unit type. `order` is one of
// the per-unit list objects that 0x439b30 walks (type index at +0x36, 16.16
// position at +0x22, owner at +0xe, timestamp at +0x46). The box corners are the
// object position plus the UnitType bounds at +0x15e (0x249-byte entries in the
// array at g_game+0x1439b), projected to screen with (x - scroll_x + 0x80,
// z - (y >> 1) - scroll_y + 0x20). The box is then shrunk towards its centre by
// level/10, where level counts g_game->ticks up to 10 from the object's
// timestamp. Eight lines are drawn: the four sides of the inner rectangle in the
// owner's colour, then the same rectangle offset one pixel outward in the
// alternate colour. Finally the object's position is copied to `out`.
//
// PARTIAL, 51.1 percent (was 44.7). Three fixes, all confirmed with check.py;
// do not re-sweep any of them:
//  1. The world box is 28 BYTES, not 32. `lo` is a 16-byte Vec3q (x, y, z, pad)
//     and `hi` is a 12-byte Vec3f (x, y, z), not another Vec3q: the frame is
//     0x30 and hi.z is the last dword of the locals, so a 32-byte box does not
//     fit and MSVC gives 0x34. This is worth 3.6 points and it also moves the
//     `level` spill from the arg3 slot to the arg2 slot, as in the original.
//  2. The clamp needs the difference written out INLINE and cast to unsigned:
//     `__min(__max((unsigned)(g_game->ticks - order->timestamp), 0), 10)`.
//     With either half of that missing (a `delta` local, or no cast) MSVC
//     value-numbers the two __max subtrees the __min macro expands to and emits
//     a single evaluation plus a conditional store, and the 16 bytes of the
//     original's second `xor/cmp/sbb/and` in the taken arm disappear. The
//     earlier note here ("this looks like optimizer state too") was wrong: it
//     is source shape. unsigned (not int) is required, since the signed form
//     gives `setle` instead of the original's `sbb`. Worth 1.3 points.
//  3. Projection order `sy, sx, half, az, bz, ax, bx` (not sx, sy, half, ax, az,
//     bx, bz) is worth 1.0, and reading the 16.16 values through
//     `*(int*)&x.frac` with a plain frac/whole struct (the 0x438ea0 idiom)
//     rather than a Fixed union with a `value` member is worth 0.4. Naming
//     ix1 = ax + dx, ix2 = bx - dx, iy1 = az + dz, iy2 = bz - dz before the
//     eight calls is worth a further 0.3.
//
// DEAD LEVERS, ALREADY EXHAUSTED (with the shape count, so they are recorded as
// measurements rather than intuitions):
//  * Compiler state. Unlike the matched sibling 0x4399f0, this function has NO
//    declaration-count window: 15 include sets (stdlib alone, with memory.h,
//    math.h, string.h, windows.h, ctype.h, setjmp.h, limits.h, float.h,
//    time.h, assert.h, stdio.h, the three-header set, windows+memory) crossed
//    with 20 dummy `extern int` counts (0 to 320 in steps of 16) is 300
//    variants, and every one of them compiles to the same 50.2 percent. The
//    earlier note in this file about <memory.h> shifting the declaration
//    counter was measured on the wrong 32-byte box and is void.
//  * The clamp spelling, beyond fix 2. 16 static spellings tried (if/else both
//    ways, ?:, two statements, a named temporary, an int/unsigned level, an
//    `age` local, the expression written into both dx and dz, `__max(0, x)`
//    order, `10u`, an extra `level = level;`): all give a single evaluation
//    except the inline-cast form, and the unsigned level silently turns the
//    division unsigned (0xcccccccd, mul, shr 3), so it is wrong as well.
//  * The order of the five box stores: all 120 permutations measured, the best
//    two (lo.x, lo.y, lo.z, hi.x, hi.z and lo.x, hi.x, lo.y, hi.z, lo.z) tie at
//    the level of fix 3 and the worst is 1.5 points below it.
//  * Where the three pos reads and the index test sit relative to each other
//    and to the `def` computation: 10 orderings, all within 3.5 points, best is
//    the plain "index, test, def, px, py, pz".
//  * Statement-order levers for the register rotation (see below): 4 projection
//    orders, 2 with the view scroll read inline, 7 ways of naming the four draw
//    deltas, 3 with a `&order->pos` pointer local, 3 with age/ownerflags hoisted
//    above the box, a single-exit `if (index != 0) { ... }` block, a local copy
//    of `order`, a local copy of `view`, and two `static inline` projection
//    helpers. 24 shapes, none moved the score by more than 0.5.
//
// WHAT STILL DIFFERS: a single register rotation, and everything else follows
// from it. The original does NOT keep `order` in a register: it reloads it from
// its argument slot at 0x438cbd and 0x438d41. That frees ecx, which it spends
// on `bx` and then spills into the (by then dead) world.lo.z slot at 0x438ce2.
// With ecx free, the two divisions use ecx as the multiply scratch while `level`
// stays in edx, and ebx is free to hold `dx` and then `surface`. Ours keeps
// `order` in ecx for the whole function, so ecx is never a scratch: the
// divisions both run in edx, `level` and `dx` are memory-only, and `surface` is
// reloaded from its argument slot for four of the eight calls. The 11 bytes we
// are short are exactly the instructions that fall out of the original's
// version: the `mov [esp+0x2c], ecx` spill of bx, the `mov edx, [esp+0x48]`
// reload of level at the join, the `mov ebx, edx` / `mov ecx, edx` pair in the
// division tails, the `mov [esp+0x14], ebx` spill of dx, and the
// `mov [esp+0x5c], ecx` that writes bx + 1 over the dead surface argument.
// Ours has matching extras: four `mov reg, [esp+0x54]` reloads of surface
// instead of `push ebx`, the `mov edx, ebp` / `sub edx, ebx` pair, and the
// `order->owner` load hoisted into the middle of the first division.
//
// The next thing to try is whatever stops MSVC keeping `order` live in ecx
// across the projection: a source that forces a re-materialisation of the
// pointer, or a use of `order` that a store in between invalidates (the
// static-inline-helper trick that took 0x4a76b0 from 84.9 to 100 percent).
#include <stdlib.h>
#include <memory.h>

#pragma pack(push, 1)
struct Fixed_00438c00 {
    unsigned short frac;
    short whole;
};
struct Vec3f_00438c00 {
    Fixed_00438c00 x, y, z;
};
struct Box_00438c00 {
    Vec3f_00438c00 lo, hi;
};
struct Vec3q_00438c00 {
    Fixed_00438c00 x, y, z, pad;
};
// 28 bytes, not 32: the frame is 0x30 and the original's last field (hi.z) is
// the last dword of the locals, so the `hi` half has no pad dword.
struct Boxq_00438c00 {
    Vec3q_00438c00 lo;
    Vec3f_00438c00 hi;
};
struct UnitType_00438c00 {
    char unknown_0[0x15e];
    Box_00438c00 bounds;               // +0x15e
    char unknown_176[0x249 - 0x176];
};
union Flags_00438c00 {
    unsigned int flags;
    struct {
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int b2 : 1;
        unsigned int b3 : 1;
        unsigned int b4 : 1;
        unsigned int rest : 27;
    } bits;
};
struct Unit_00438c00 {
    char unknown_0[0x110];
    Flags_00438c00 flags;              // +0x110
};
struct Order_00438c00 {
    char unknown_0[0xe];
    Unit_00438c00* owner;              // +0xe
    char unknown_12[0x22 - 0x12];
    Vec3f_00438c00 pos;                // +0x22
    char unknown_2e[0x36 - 0x2e];
    unsigned short type;               // +0x36
    char unknown_38[0x46 - 0x38];
    int timestamp;                     // +0x46
};
struct View_00438c00 {
    char unknown_0[0x2c];
    int scroll_x;                      // +0x2c
    int scroll_y;                      // +0x30
};
struct Game_00438c00 {
    char unknown_0[0xdcc];
    unsigned char color_dcc;           // +0xdcc
    char unknown_dcd[0xdce - 0xdcd];
    unsigned char color_dce;           // +0xdce
    char unknown_dcf[0xdd4 - 0xdcf];
    unsigned char color_dd4;           // +0xdd4
    unsigned char color_dd5;           // +0xdd5
    char unknown_dd6[0x1439b - 0xdd6];
    UnitType_00438c00* types;          // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game_00438c00* g_game;

void __stdcall FUN_004be950(void* surface, int x0, int y0, int x1, int y1, int color);

// A fifth pass closed the one axis the fourth left open. The suggested lever was
// the 0x4a76b0 one, a `static inline` helper taking fresh memory-based
// arguments so a store inside invalidates the pointer, on the theory that it
// would stop MSVC keeping `order` live in ecx. Five shapes, all measured with
// `check.py --sym` after `rm -rf build/obj`, none better than the 51.0% in the
// file:
//
//   BuildBox helper reading order->pos internally    49.7%
//   Clamp helper taking (ticks, order->timestamp)    50.1%
//   both helpers together                            48.8%
//   clamp moved above the box stores                 40.4%
//   order->pos re-read after the box stores          50.6%
//
// So the `static inline` lever, which is worth a lot elsewhere in this project
// (four distinct mechanisms in 0x4a76b0, 0x458dd0, 0x489280 and 0x451220), does
// not reach this particular register rotation. Combined with the fourth pass's
// 300 variants showing compiler state is flat here (15 include sets by 20 dummy
// `extern int` counts, all byte-identical, so unlike 0x4399f0 there is no
// declaration-count window), the `order` lifetime looks settled rather than
// unexplored. It would need something outside the source.
//
// FUNCTION: 0x438c00
void __stdcall FUN_00438c00(void* surface, View_00438c00* view, Order_00438c00* order,
                            Vec3f_00438c00* out, int unused)
{
    unsigned short index = order->type;
    if (index == 0)
        return;

    UnitType_00438c00* def = g_game->types + index;

    int px = *(int*)&order->pos.x.frac;
    int py = *(int*)&order->pos.y.frac;
    int pz = *(int*)&order->pos.z.frac;

    Boxq_00438c00 world;
    *(int*)&world.lo.x.frac = px + *(int*)&def->bounds.lo.x.frac;
    *(int*)&world.lo.y.frac = py + *(int*)&def->bounds.lo.y.frac;
    *(int*)&world.lo.z.frac = pz + *(int*)&def->bounds.lo.z.frac;
    *(int*)&world.hi.x.frac = px + *(int*)&def->bounds.hi.x.frac;
    *(int*)&world.hi.z.frac = pz + *(int*)&def->bounds.hi.z.frac;

    int sy = view->scroll_y;
    int sx = view->scroll_x;
    int half = world.lo.y.whole >> 1;
    int az = world.lo.z.whole - half - sy + 0x20;
    int bz = world.hi.z.whole - half - sy + 0x20;
    int ax = world.lo.x.whole - sx + 0x80;
    int bx = world.hi.x.whole - sx + 0x80;

    // The cast and the lack of a `delta` local are both needed: with either
    // one alone MSVC value-numbers the two __max subtrees of the __min macro
    // and emits a single evaluation, with a conditional store instead of the
    // original's recomputation in the taken arm.
    int level = __min(__max((unsigned)(g_game->ticks - order->timestamp), 0), 10);
    int dx = ((bx - ax) * level) / 10;
    int dz = ((bz - az) * level) / 10;

    unsigned char color1;
    unsigned char color2;
    if ((order->owner->flags.flags & 0x10) != 0) {
        color1 = g_game->color_dce;
        color2 = g_game->color_dd5;
    } else {
        color1 = g_game->color_dcc;
        color2 = g_game->color_dd4;
    }

    int ix1 = ax + dx;
    int ix2 = bx - dx;
    int iy1 = az + dz;
    int iy2 = bz - dz;
    FUN_004be950(surface, ix1 - 1, az - 1, ix1 - 1, bz + 1, color1);
    FUN_004be950(surface, ix2 + 1, az - 1, ix2 + 1, bz + 1, color1);
    FUN_004be950(surface, ax - 1, iy1 - 1, bx + 1, iy1 - 1, color1);
    FUN_004be950(surface, ax - 1, iy2 + 1, bx + 1, iy2 + 1, color1);
    FUN_004be950(surface, ix1, az, ix1, bz, color2);
    FUN_004be950(surface, ix2, az, ix2, bz, color2);
    FUN_004be950(surface, ax, iy1, bx, iy1, color2);
    FUN_004be950(surface, ax, iy2, bx, iy2, color2);

    *out = order->pos;
}
