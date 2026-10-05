// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free, GPT-6.1-sol and Space Bunny Free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// MATCH. Draws the footprint box of the unit type an order is about to build
// (order->type indexes g_game->types), as two nested "#" shapes whose inner
// lines slide in by level/10 of the box size, level being the frames since
// order->timestamp clamped to 0..10. The outer shape is drawn one pixel bigger
// in color1, the inner one in color2, and the order's position is copied to
// *out like the other order-drawing callbacks (0x439740, 0x4399f0). The box is
// flat: both screen corners use the low corner's height.
//
// Rebuilt by Claude Opus 5.5 (#4994, 58.3 -> MATCH). The old body (a 28-byte
// box with a pad field, `char* const` arithmetic for pos.y, the timestamp and
// the owner, an unsigned cast in the clamp) was replaced by the shapes the
// sibling callbacks use. Each step, measured with check.py:
//  * the two corners through a helper, one call per corner (58.3 -> 64.0).
//    This gives the original's 0x30 frame without a pad field, and it spills
//    x2 rather than keeping it in a callee-saved register, which frees ebx
//    for `surface` across the eight calls.
//  * ScreenX/ScreenY, the projection of 0x439740 and 0x4399f0, called in the
//    order x1, y1, x2, y2 (-> 76.2). `order` then stops being a register
//    variable and is reloaded for the timestamp and the owner, as in the
//    original.
//  * the owner flag as a 1-bit bitfield (+2.3): `shr ecx,4; test cl,1` is the
//    bitfield form, while the mask folds to `test byte ptr [..], 0x10`.
//  * the height passed to ScreenY separately (+2.2), and the corner returned
//    by value (-> 86.8 once the operands were ordered).
//  * the four screen coordinates in a Rect struct (-> 92.5). This settled the
//    frame. The original keeps the spilled x2 in the dead lo.z dword and has
//    an unused dword between the corners. MSVC 5 never packs a plain local
//    into a dead struct, but it lays the Rect (an inline-promoted aggregate:
//    x1, y1 and y2 stay in registers, only x2 lives in memory at r+8) over
//    the dead `lo` temporary. The 16-byte Rect over the 12-byte corner leaves
//    the 4-byte hole.
//  * `operator+` as a const member of Pos taking a const reference, the 0x403a20
//    Vec3 idiom (-> MATCH). A free operator+ or a free AddPos returning Pos
//    left the corner loads in the wrong registers.
#include <stdlib.h>

#pragma pack(push, 1)

union Fixed_00438c00 {
    int value;                        // 16.16
    struct {
        unsigned short frac;
        short whole;
    };
};

struct Pos_00438c00 {
    Fixed_00438c00 x, y, z;

    Pos_00438c00 operator+(const Pos_00438c00& other) const
    {
        Pos_00438c00 r;
        r.x.value = x.value + other.x.value;
        r.y.value = y.value + other.y.value;
        r.z.value = z.value + other.z.value;
        return r;
    }
};

struct UnitType_00438c00 {
    char unknown_0[0x15e];
    Pos_00438c00 lo;                  // +0x15e
    Pos_00438c00 hi;                  // +0x16a
    char unknown_176[0x249 - 0x176];
};

struct Unit {
    char unknown_0[0x110];
    unsigned int flag0 : 1;           // +0x110
    unsigned int flag1 : 1;
    unsigned int flag2 : 1;
    unsigned int flag3 : 1;
    unsigned int flag4 : 1;
    unsigned int flags_rest : 27;
};

struct Order {
    char unknown_0[0xe];
    Unit* owner;                      // +0xe
    char unknown_12[0x22 - 0x12];
    Pos_00438c00 pos;                 // +0x22
    char unknown_2e[0x36 - 0x2e];
    unsigned short type;              // +0x36
    char unknown_38[0x46 - 0x38];
    unsigned int timestamp;           // +0x46
};

struct View_00438c00 {
    char unknown_0[0x2c];
    int cx;                           // +0x2c
    int cy;                           // +0x30
};

struct Game {
    char unknown_0[0xdcc];
    unsigned char field_dcc;          // +0xdcc
    char unknown_dcd[0xdce - 0xdcd];
    unsigned char field_dce;          // +0xdce
    char unknown_dcf[0xdd4 - 0xdcf];
    unsigned char field_dd4;          // +0xdd4
    unsigned char field_dd5;          // +0xdd5
    char unknown_dd6[0x1439b - 0xdd6];
    UnitType_00438c00* types;         // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    unsigned int frame;               // +0x38a47
};

#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);

struct Rect_00438c00 {
    int x1, y1, x2, y2;
};

static int ScreenX(View_00438c00* v, int x)
{
    return x - v->cx + 0x80;
}

static int ScreenY(View_00438c00* v, int z, int y)
{
    return z - (y >> 1) - v->cy + 0x20;
}

// FUNCTION: 0x438c00
void __stdcall FUN_00438c00(void* surface, View_00438c00* view, Order* order,
                            Pos_00438c00* out, int unused)
{
    if (order->type == 0)
        return;
    UnitType_00438c00* type = &g_game->types[order->type];
    Pos_00438c00 lo = order->pos + type->lo;
    Pos_00438c00 hi = order->pos + type->hi;
    Rect_00438c00 r;
    r.x1 = ScreenX(view, lo.x.whole);
    r.y1 = ScreenY(view, lo.z.whole, lo.y.whole);
    r.x2 = ScreenX(view, hi.x.whole);
    r.y2 = ScreenY(view, hi.z.whole, lo.y.whole);
    int level = __min(__max(g_game->frame - order->timestamp, 0), 10);
    int dx = (r.x2 - r.x1) * level / 10;
    int dy = (r.y2 - r.y1) * level / 10;
    unsigned char outer;
    unsigned char inner;
    if (order->owner->flag4) {
        outer = g_game->field_dce;
        inner = g_game->field_dd5;
    } else {
        outer = g_game->field_dcc;
        inner = g_game->field_dd4;
    }
    FUN_004be950(surface, r.x1 + dx - 1, r.y1 - 1, r.x1 + dx - 1, r.y2 + 1, outer);
    FUN_004be950(surface, r.x2 - dx + 1, r.y1 - 1, r.x2 - dx + 1, r.y2 + 1, outer);
    FUN_004be950(surface, r.x1 - 1, r.y1 + dy - 1, r.x2 + 1, r.y1 + dy - 1, outer);
    FUN_004be950(surface, r.x1 - 1, r.y2 - dy + 1, r.x2 + 1, r.y2 - dy + 1, outer);
    FUN_004be950(surface, r.x1 + dx, r.y1, r.x1 + dx, r.y2, inner);
    FUN_004be950(surface, r.x2 - dx, r.y1, r.x2 - dx, r.y2, inner);
    FUN_004be950(surface, r.x1, r.y1 + dy, r.x2, r.y1 + dy, inner);
    FUN_004be950(surface, r.x1, r.y2 - dy, r.x2, r.y2 - dy, inner);
    *out = order->pos;
}
