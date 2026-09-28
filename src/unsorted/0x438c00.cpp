// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
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
// What is already right: every expression and every call argument and order
// (verified against the disassembly), the call to FUN_004be950, the byte
// colours chosen from g_game+0xdcc/0xdce and +0xdd4/0xdd5 by bit 4 of the
// owner's flags at +0x110, the 16.16 hi-word extraction, and the frame. The
// world-space corners must be a 16-byte-stride Vec3 of Fixed (see Vec3q): that
// is what puts lo at frame +0x14/+0x18/+0x1c and hi at +0x24 (hi.x) and +0x2c
// (hi.z), matching the original exactly. `#include <memory.h>` is not used by
// the code; it only shifts MSVC's declaration counter into the best state found
// (see below).
//
// PARTIAL, 44.7 percent. What still differs, in order of size:
//  1. Register rotation. The original loads `order` into edx and keeps pos.x,
//     pos.z in ebp,edi across the whole projection; its p1..p5 temporary is
//     edx; lo.y lands in ebx and the h1/h4 results in ebp,ecx; the view pointer
//     stays in ecx. Ours loads `order` into ecx, keeps pos.z in edi (matches)
//     but pos.x in esi, the p temporary in edx (matches), lo.y in ebp, h1/h4 in
//     ebx/ebp, and reloads the view pointer from the arg3 slot. This is a
//     register rotation, not a source-shape error.
//  2. The clamp. The original computes __max(delta,0) twice: once for the
//     `clamped < 10` compare and again in that arm, and spills `level` into the
//     dead arg2 slot. Ours computes it once and spills into the dead arg4 slot.
//     Both come from `__min(__max(delta, 0), 10)`, which is proven to give the
//     double evaluation in the matched 0x409dc0; a small probe of every static
//     spelling tested (if/else both ways, ?:, split into two statements) gives
//     a single evaluation, so this looks like optimizer state too.
//  3. Declaration-count sensitivity (compiler state). Same source, only the
//     includes changed: none 40.7, <stdio.h> 40.7, <windows.h> 42.1,
//     <string.h> 41.2, <math.h> 41.2, <memory.h> 44.7. Adding N unused
//     `extern int` before the function: N=1..7 drop to 41.6, N=8,16 return to
//     44.7, so the state cycles with period 8 in the dummy count and a wider
//     sweep does not open a new state. <windows.h>+<memory.h> (the set the
//     matched sibling 0x4399f0 uses) is 42.1 here.
#include <stdlib.h>
#include <memory.h>

#pragma pack(push, 1)
union Fixed_00438c00 {
    int value;
    struct { unsigned short frac; short whole; };
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
struct Boxq_00438c00 {
    Vec3q_00438c00 lo, hi;
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

// FUNCTION: 0x438c00
void __stdcall FUN_00438c00(void* surface, View_00438c00* view, Order_00438c00* order,
                            Vec3f_00438c00* out, int unused)
{
    unsigned short index = order->type;
    if (index == 0)
        return;

    UnitType_00438c00* def = g_game->types + index;

    int px = order->pos.x.value;
    int py = order->pos.y.value;
    int pz = order->pos.z.value;

    Boxq_00438c00 world;
    world.lo.x.value = px + def->bounds.lo.x.value;
    world.lo.y.value = py + def->bounds.lo.y.value;
    world.lo.z.value = pz + def->bounds.lo.z.value;
    world.hi.x.value = px + def->bounds.hi.x.value;
    world.hi.z.value = pz + def->bounds.hi.z.value;

    int sx = view->scroll_x;
    int sy = view->scroll_y;
    int half = world.lo.y.whole >> 1;
    int ax = world.lo.x.whole - sx + 0x80;
    int az = world.lo.z.whole - half - sy + 0x20;
    int bx = world.hi.x.whole - sx + 0x80;
    int bz = world.hi.z.whole - half - sy + 0x20;

    unsigned int delta = g_game->ticks - order->timestamp;
    int level = __min(__max(delta, 0), 10);
    int dx = ((bx - ax) * level) / 10;
    int dz = ((bz - az) * level) / 10;

    unsigned char color1;
    unsigned char color2;
    if (order->owner->flags.bits.b4) {
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
