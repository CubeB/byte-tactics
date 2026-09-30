// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free, GPT-6.1-sol and Space Bunny Free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
//
// PASS 8 (deepseek-v4.1-flash, 2026-09-30): 52.2 percent, unchanged. Two more
// pointer shapes for the pos reads both scored WORSE than the current 52.2:
//   `Vec3f* ppos = &order->pos;` used for all three pos reads   46.7 percent
//     (it also lengthens to 653 bytes; original is 660)
//   px and pz read through `order`, py read through that ppos    50.9 percent
// Neither changes the top-of-body register rotation: MSVC still spends ECX on
// `order`. So the pos-pointer axis is now closed as well, and the residual is
// confirmed to be the single ecx/edx assignment at 0x438c00 (which then makes
// the original spill `order` and keep `view` in ecx, while ours does the
// opposite). No source spelling tried in eight passes has reached that choice.
// Best: 51.1% (unchanged; the pointer experiment below scored 49.9%). The push order in the original IS edi, esi, ebp, ebx and the pop order IS ebx, ebp, esi, edi, exactly as the build emits, so the residual is NOT a callee-saved rotation. The first difference is a single scratch-register swap at the top: the original loads `order` into EDX and puts the type-index copy in ECX, ours loads `order` into ECX and puts the copy in EDX. Everything after (which register holds level, which the two `imul`s scratch in, whether `surface` stays in ebx or is reloaded from its argument slot, and which argument slot each dead local lands in) follows from that one swap.
//
// SLOT MAP, decoded from the original and worth keeping (the earlier passes got
// this wrong in places). The prologue does `sub esp, 0x30` and THEN pushes
// edi/esi/ebp/ebx, so with B = esp after the four pushes the saved registers are
// at [B, B+0x10) and the 0x30 bytes of locals are at [B+0x10, B+0x40), i.e. a
// displacement D in the body is local D-0x10. That makes every stack slot in the
// original legible, and it confirms the struct shapes already used here:
//   local+0x00  colour2 (a byte store, later read as a dword)
//   local+0x04  dx, then reused for ix2 = bx - dx
//   local+0x08  ix1 = ax + dx
//   local+0x0c  iy1 = az + dz
//   local+0x10  &order->pos (the final `*out = *that` reads it)
//   local+0x14 .. local+0x30  the 28-byte box: lo.x, lo.y, lo.z, pad, hi.x,
//               hi.y, hi.z. The whole-part reads are at +0x16, +0x1e, +0x26,
//               +0x1a, +0x2e, which pins lo as a 16-byte Vec3q (x,y,z,pad) and
//               hi as a 12-byte Vec3f (x,y,z) whose y is never read.
// The dead argument slots are reused: the view slot (E+8) holds `level` and then
// dz, the order slot (E+0xc) holds colour1 and then colour2, and the surface
// slot (E+4) holds bx + 1.
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
// A SIXTH PASS checked the "callee-saved rotation" wall and found it void: the
// original pushes edi, esi, ebp, ebx and pops ebx, ebp, esi, edi, which is
// exactly what the build emits, so there is no rotation to reproduce. What is
// left is one scratch-register swap at the top of the body, as described
// above. Two experiments, both scored with `check.py --sym`:
//   `Vec3f* pos = &order->pos;` used for the pos.y read and for `*out = *pos`
//     (49.9 percent, WORSE by 1.2) - this does reproduce the original's
//     `lea ecx, [edx+0x22]; mov [esp+0x20], ecx; ...; mov edx, [ecx+4]` shape,
//     but it moves the &order->pos spill to local+0x0c instead of local+0x10
//     and leaves `order` in ecx anyway, so the register swap survives.
//   128 header sets (`tools/headers.py`): flat at 51.1 percent, none better.
//
// The thing still worth trying is whatever stops MSVC spending ECX on `order`
// at the top: it is the parameter's live range crossing the two divisions, and
// since neither the pointer shape nor any spelling of the clamp nor any
// statement order moves it, the lever is probably the ORDER in which the three
// pos components and the index are first read, or the fact that ours reads
// pos.y through `order` (the original reads it through a pointer to pos).
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
// PASS 7 (deepseek-v4.1, 2026-09-30): 52.2 percent, up from 51.1. The four
// precomputed locals ix1/ix2/iy1/iy2 were WRONG: the original recomputes
// `bx - dx` at each call site (see 0x438daf: mov eax,[esp+0x2c]; mov
// ecx,[esp+0x14]; sub eax,ecx; mov [esp+0x18],eax) and shares one slot for
// the x pair, so the eight FUN_004be950 calls must be written with the
// arithmetic inline (`ax + dx - 1`, `bx - dx + 1`, `az + dz - 1`, ...).
// That is +1.1 points and +1 byte (647 vs 646). Still partial: the first
// divergence is still the top-of-body register swap (original order->edx,
// index->ecx; ours order->ecx, index->edx) which cascades. Also checked and
// neutral (byte-identical to the 51.1 build): `if (order->type == 0)` plus a
// second direct `order->type` read instead of the index local (CSE), and
// `((flags >> 4) & 1)` both inline and through an `unsigned int flags`
// local (MSVC5 folds every spelling to `test byte ptr [..], 0x10`; the
// original's `mov ecx,[eax+0x110]; shr ecx,4; test cl,1` is reached by
// neither, so it needs a shape not yet found).
//
// PASS 9 (deepseek-v4.1-flash, 2026-10-01): 52.2 percent, unchanged. Thirty
// more shapes measured with `check.py --sym`, all 52.2 or below, confirming the
// entry register choice is closed to source shape here:
//   index declared uninitialised at the top (before the type read)  52.2
//   index as an `int`                                                50.4
//   a local `Order* o = order;` used for every order read           52.2
//   `Order& order` reference parameter                              52.2
//   `Order* const order`                                            52.2
//   `&g_game->types[index]` array indexing instead of pointer add    52.2
//   `types` cached in a local first                                 52.2
//   a local `View* v = view;` (top, after test, and at first use)   52.2
//   a local `void* s = surface;` used for all eight calls           52.2
//   `order->owner` and `order->timestamp` hoisted into locals       52.2 / 51.3
//   a `static inline` GetType/GetDef for the top two reads         51.8 (both
//                                                                   52.2 combined)
//   the guard read twice (`if (order->type == 0)` then the index)   52.2 (CSE)
// All produce the same 647-byte body, so the divergence stays the single
// ecx/edx assignment at 0x438c00 described above.
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

    FUN_004be950(surface, ax + dx - 1, az - 1, ax + dx - 1, bz + 1, color1);
    FUN_004be950(surface, bx - dx + 1, az - 1, bx - dx + 1, bz + 1, color1);
    FUN_004be950(surface, ax - 1, az + dz - 1, bx + 1, az + dz - 1, color1);
    FUN_004be950(surface, ax - 1, bz - dz + 1, bx + 1, bz - dz + 1, color1);
    FUN_004be950(surface, ax + dx, az, ax + dx, bz, color2);
    FUN_004be950(surface, bx - dx, az, bx - dx, bz, color2);
    FUN_004be950(surface, ax, az + dz, bx, az + dz, color2);
    FUN_004be950(surface, ax, bz - dz, bx, bz - dz, color2);

    *out = order->pos;
}
