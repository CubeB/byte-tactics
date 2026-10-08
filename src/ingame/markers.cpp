// Decompiled by DeepSeek V4.1 Flash, Space Bunny Free, GPT-6.1-sol, deepseek-v4.1, deepseek-v4.1-flash, mimo-v2.6-pro, Claude Sonnet 5.5 and Claude Opus 5.5. Names are provisional.
// The include set is load-bearing for 0x4399f0: it changes how the compiler reassociates.
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#pragma pack(push, 1)

// A 16.16 fixed-point coordinate.
union Fixed {
    int value;
    struct {
        unsigned short frac;
        short whole;
    };
};

// The 16.16 position, the same layout the sibling helpers (0x4399f0) use.
struct Pos {
    Fixed x;                          // +0x0
    Fixed y;                          // +0x4
    Fixed z;                          // +0x8

    Pos operator+(const Pos& other) const
    {
        Pos r;
        r.x.value = x.value + other.x.value;
        r.y.value = y.value + other.y.value;
        r.z.value = z.value + other.z.value;
        return r;
    }
};

struct Point {
    short x;                          // +0x0
    short y;                          // +0x2
};

struct Vec3 {
    int x, y, z;

    // The three deltas as plain ints, with the length helper of the matched
    // sibling 0x40beb0. Two things there matter and both are visible here: the
    // three components go through their own `double` locals, which is what keeps
    // the three `fild`s in x, y, z order, and the difference is a struct the
    // helper reads through memory, which is what keeps the three deltas in the
    // original's esp+0x28..0x30 slots.
    int Length() const
    {
        double fx = x;
        double fy = y;
        double fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
};

struct View {
    char unknown_0[0x2c];
    int scroll_x;                     // +0x2c
    int scroll_y;                     // +0x30
};

struct Weapon {
    char unknown_0[0xd6];
    unsigned short field_d6;          // +0xd6
    char unknown_d8[0xdc - 0xd8];
    int range;                        // +0xdc
    int field_e0;                     // +0xe0
};

struct Slot {
    Weapon* weapon;                   // +0x0
    char unknown_4[0xf - 4];
    unsigned char flags;              // +0xf
    char unknown_10[0x1c - 0x10];
};

struct UnitType {
    char unknown_0[0x15e];
    Pos lo;                           // +0x15e
    Pos hi;                           // +0x16a
    char unknown_176[0x178 - 0x176];
    short field_178;                  // +0x178
    char unknown_17a[0x202 - 0x17a];
    short sight;                      // +0x202
    short radar;                      // +0x204
    short sonar;                      // +0x206
    short minCloakDistance;           // +0x208
    short radarJam;                   // +0x20a
    short sonarJam;                   // +0x20c
    char unknown_20e[0x212 - 0x20e];
    unsigned short buildDistance;     // +0x212
    unsigned short maneuver;          // +0x214
    unsigned short attackLength;      // +0x216
    unsigned short kamikazeDistance;  // +0x218
    char unknown_21a[0x220 - 0x21a];
    Weapon* weapon_220;               // +0x220
    char unknown_224[0x241 - 0x224];
    unsigned int flags;               // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Anim {
    unsigned short count;             // +0x0
    char unknown_2[0x2c - 2];
    unsigned short field_2c;          // +0x2c
};

struct Player;

struct Unit {
    int field_0;                      // +0x0
    char unknown_4[0x10 - 4];
    Slot slots[3];                    // +0x10
    char unknown_64[0x6a - 0x64];
    Pos pos;                          // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType* def;                    // +0x92
    Player* owner;                    // +0x96
    char unknown_9a[0x10e - 0x9a];
    unsigned char field_10e;          // +0x10e
    char unknown_10f[0x110 - 0x10f];
    unsigned int flag0 : 1;           // +0x110
    unsigned int flag1 : 1;
    unsigned int flag2 : 1;
    unsigned int flag3 : 1;
    unsigned int flag4 : 1;
    unsigned int flags_rest : 27;
};

struct Order {
    char unknown_0[4];
    unsigned char kind;               // +0x4
    char unknown_5[0xe - 5];
    Unit* unit;                       // +0xe
    char unknown_12[0x16 - 0x12];
    Unit* target;                     // +0x16
    char unknown_1a[0x22 - 0x1a];
    Pos pos;                          // +0x22
    Point field_2e;                   // +0x2e
    Point cached;                     // +0x32
    unsigned short type;              // +0x36
    char unknown_38[0x42 - 0x38];
    unsigned int flags;               // +0x42
    unsigned int timestamp;           // +0x46
};

struct Entry {                        // 0x19-byte entries, table at DAT_00512344
    char unknown_0[0x10];
    unsigned char field_10;           // +0x10
    char unknown_11[0x19 - 0x11];
};

struct Game {
    char unknown_0[0xdcc];
    unsigned char field_dcc;          // +0xdcc
    char unknown_dcd[0xdce - 0xdcd];
    unsigned char field_dce;          // +0xdce
    unsigned char field_dcf;          // +0xdcf
    char unknown_dd0[0xdd4 - 0xdd0];
    unsigned char field_dd4;          // +0xdd4
    unsigned char field_dd5;          // +0xdd5
    char unknown_dd6[0xdd7 - 0xdd6];
    unsigned char field_dd7;          // +0xdd7
    char unknown_dd8[0xdd9 - 0xdd8];
    unsigned char field_dd9;          // +0xdd9
    unsigned char field_dda;          // +0xdda
    char unknown_ddb[0x1439b - 0xddb];
    UnitType* types;                  // +0x1439b
    char unknown_1439f[0x1487f - 0x1439f];
    Anim* anims[22];                  // +0x1487f, this function uses [21]
    char unknown_148d7[0x38a47 - 0x148d7];
    unsigned int frame;               // +0x38a47
    char unknown_38a4b[0x391bf - 0x38a4b];
    int field_391bf;                  // +0x391bf
};

struct Rect {
    int x1, y1, x2, y2;
};

#pragma pack(pop)

extern Game* g_game;
extern Entry* DAT_00512344;
extern double TWO_PI;                    // 6.28318530717958
extern double ONE_EIGHTH;                // 0.125

void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int color);
void __stdcall DrawString(void* surface, const char* text, int x, int y, int maxWidth);
int __stdcall GetGroundHeight(Pos* pos);
int __cdecl FUN_004b70ef(int angle, int radius);
int __cdecl FUN_004b7123(int angle, int radius);
void __stdcall DrawFrame(void* surface, void* bmp, int x, int y);
void __stdcall DrawFrameBlended(void* dest, void* bmp, int x, int y);
int __stdcall IsUnitVisibleToPlayer(Player* owner, Unit* unit);
void __stdcall DrawRangeCircle(void* surface, View* view, Pos* pos,
                            int radius, int color, const char* text, int index);
void __stdcall DrawWeaponCoverage(void* surface, View* view, Order* order,
                            Pos* out, int unused);

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
static int ScreenX(View* v, int x)
{
    return x - v->scroll_x + 0x80;
}

static int ScreenY(View* v, int z, int y)
{
    return z - (y >> 1) - v->scroll_y + 0x20;
}

// FUNCTION: 0x438c00
void __stdcall DrawBuildFootprint(void* surface, View* view, Order* order,
                            Pos* out, int unused)
{
    if (order->type == 0)
        return;
    UnitType* type = &g_game->types[order->type];
    Pos lo = order->pos + type->lo;
    Pos hi = order->pos + type->hi;
    Rect r;
    r.x1 = ScreenX(view, lo.x.whole);
    r.y1 = ScreenY(view, lo.z.whole, lo.y.whole);
    r.x2 = ScreenX(view, hi.x.whole);
    r.y2 = ScreenY(view, hi.z.whole, lo.y.whole);
    int level = __min(__max(g_game->frame - order->timestamp, 0), 10);
    int dx = (r.x2 - r.x1) * level / 10;
    int dy = (r.y2 - r.y1) * level / 10;
    unsigned char outer;
    unsigned char inner;
    if (order->unit->flag4) {
        outer = g_game->field_dce;
        inner = g_game->field_dd5;
    } else {
        outer = g_game->field_dcc;
        inner = g_game->field_dd4;
    }
    DrawLine(surface, r.x1 + dx - 1, r.y1 - 1, r.x1 + dx - 1, r.y2 + 1, outer);
    DrawLine(surface, r.x2 - dx + 1, r.y1 - 1, r.x2 - dx + 1, r.y2 + 1, outer);
    DrawLine(surface, r.x1 - 1, r.y1 + dy - 1, r.x2 + 1, r.y1 + dy - 1, outer);
    DrawLine(surface, r.x1 - 1, r.y2 - dy + 1, r.x2 + 1, r.y2 - dy + 1, outer);
    DrawLine(surface, r.x1 + dx, r.y1, r.x1 + dx, r.y2, inner);
    DrawLine(surface, r.x2 - dx, r.y1, r.x2 - dx, r.y2, inner);
    DrawLine(surface, r.x1, r.y1 + dy, r.x2, r.y1 + dy, inner);
    DrawLine(surface, r.x1, r.y2 - dy, r.x2, r.y2 - dy, inner);
    *out = order->pos;
}

// The offset of the point at this angle and distance from the centre.
static inline Vec3 Offset(int angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    // Named before negation: `v.z = -FUN_004b7123(...)` changes register use.
    int z = FUN_004b7123(angle, distance);
    v.z = -z;
    return v;
}

static inline Pos operator-(const Pos& p, const Vec3& v)
{
    Pos r;
    r.x.value = p.x.value - v.x;
    r.y.value = p.y.value - v.y;
    r.z.value = p.z.value - v.z;
    return r;
}

// Draws a ring of n + 1 line segments around a 16.16 map position,
// where n = radius * 2pi / 8, and the label text at the end of segment number
// index * 3 (or of the last segment when that one ended at 0, 0).
// FUNCTION: 0x438ea0
void __stdcall DrawRangeCircle(void* surface, View* view, Pos* pos, int radius,
                            int color, const char* text, int index)
{
    int angle = 0;
    if (radius) {
        int lx = 0;
        int ly = 0;
        // Product order d = radius * a, n = d * b: picks the right constants.
        double d = radius * TWO_PI;
        int n = (int)(d * ONE_EIGHTH);
        int i = 0;
        // Uninitialised on purpose: when the ring has no segments the
        // original reloads both from their (never written) home slot.
        int x2;
        int y2;
        if (i <= n) {
            int step = 0x10000 / n;
            int rad = radius << 16;
            do {
                Pos p1 = *pos - Offset(angle, rad);
                p1.y.whole = __max(pos->y.whole, GetGroundHeight(&p1));
                angle += step;
                Pos p2 = *pos - Offset(angle, rad);
                p2.y.whole = __max(pos->y.whole, GetGroundHeight(&p2));
                int sx = view->scroll_x;
                int sy = view->scroll_y;
                // y2 before x2: orders the two reloads on the n < 0 path.
                y2 = p2.z.whole - (p2.y.whole >> 1) - sy + 0x20;
                x2 = p2.x.whole - sx + 0x80;
                DrawLine(surface, p1.x.whole - sx + 0x80,
                             p1.z.whole - (p1.y.whole >> 1) - sy + 0x20, x2, y2, color);
                // Compared as i == index * 3, not `index *= 3`: x2 and y2 would take i's slot.
                if (i == index * 3) {
                    lx = x2;
                    ly = y2;
                }
                i++;
            } while (i <= n);
        }
        if (text) {
            if (lx == 0 && ly == 0) {
                lx = x2;
                ly = y2;
            }
            DrawString(surface, text, lx, ly + 4, -1);
        }
    }
}

// Suspected bug: the weapon3 test reads slots[0].flags (unit+0x1f) but the
// range from slots[2].weapon (unit+0x48); slots[2].flags is at unit+0x57.
// FUNCTION: 0x4390a0
void __stdcall DrawUnitRangeRings(void* surface, View* view, Order* order,
                            int unused1, int unused2)
{
    Unit* unit = order->unit;
    UnitType* def = unit->def;
    int index = 0;
    if (g_game->field_391bf == 0) {
        short mincloak = def->minCloakDistance;
        if (mincloak != 0 && (unit->field_10e & 4)) {
            DrawRangeCircle(surface, view, &order->unit->pos, mincloak, g_game->field_dda, 0, 0);
        }
        if ((def->flags & 0x10000000) && def->weapon_220 != 0) {
            int r = def->weapon_220->field_d6;
            r = r >> 1;
            unsigned int t = (g_game->frame % 60) * r * 2 / 60;
            // Single ternary: any if-form spills radius.
            int radius = (t < 8) ? 8 : t;
            if (radius >= r)
                radius = r;
            // Read as &order->unit->pos: a unit local would change it to lea.
            DrawRangeCircle(surface, view, &order->unit->pos, radius, g_game->field_dd7, 0, 0);
            if (unit->field_0 != 0) {
                DrawRangeCircle(surface, view, &order->unit->pos, def->kamikazeDistance,
                             g_game->field_dd7, 0, 0);
                return;
            }
            DrawRangeCircle(surface, view, &order->unit->pos, def->sight, g_game->field_dd7, 0, 0);
            return;
        }
    } else {
        short mincloak = def->minCloakDistance;
        if (mincloak != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, mincloak, g_game->field_dd9,
                         "mincloak", index++);
        }
        if (def->sight != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, def->sight, g_game->field_dd9,
                         "sight", index++);
        }
        if (def->radar != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, def->radar, g_game->field_dd9,
                         "radar", index++);
        }
        if (def->sonar != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, def->sonar, g_game->field_dd9,
                         "sonar", index++);
        }
        if (def->radarJam != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, def->radarJam, g_game->field_dd9,
                         "radarjam", index++);
        }
        if (def->sonarJam != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, def->sonarJam, g_game->field_dd9,
                         "sonarjam", index++);
        }
        if (def->buildDistance != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, def->buildDistance, g_game->field_dd9,
                         "build distance", index++);
        }
        if (def->maneuver != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, def->maneuver, g_game->field_dd9,
                         "maneuver", index++);
        }
        if (def->kamikazeDistance != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, def->kamikazeDistance,
                         g_game->field_dd9, "kamikazedistance", index);
        }
        int color;
        if (g_game->frame & 1)
            color = g_game->field_dcf;
        else
            color = g_game->field_dd7;
        if ((unit->slots[0].flags & 2) && unit->slots[0].weapon->range != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, unit->slots[0].weapon->range, color,
                         "weapon1 range", 0);
        }
        if ((unit->slots[1].flags & 2) && unit->slots[1].weapon->range != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, unit->slots[1].weapon->range, color,
                         "weapon2 range", 1);
        }
        // Original bug: tests slots[0].flags (unit+0x1f) but reads slots[2].weapon
        // (unit+0x48); slots[2].flags is at unit+0x57.
        if ((unit->slots[0].flags & 2) && unit->slots[2].weapon->range != 0) {
            DrawRangeCircle(surface, view, &order->unit->pos, unit->slots[2].weapon->range, color,
                         "weapon3 range", 2);
        }
    }
}

// The three 16.16 steps of the interpolated position. The original keeps them
// in memory (the values are stored as each _allmul/_allshr pair finishes and
// the three adds with `start` happen only after all three), which is what
// building them in a helper returning a Pos reproduces.
static Pos offset_004394e0(int dx, int dy, int dz, int f)
{
    Pos d;
    d.x.value = (int)(((__int64)dx * f) >> 16);
    d.y.value = (int)(((__int64)dy * f) >> 16);
    d.z.value = (int)(((__int64)dz * f) >> 16);
    return d;
}

// The walk itself, as an inline method on a struct that holds the whole loop
// state. Per the guide, "an inlined function boundary changes the order MSVC
// evaluates things in and which registers it keeps values in"; moving the loop
// behind this boundary is what moves `order` out of edi and into ebx (the
// register the original loads it into).
struct Trail_004394e0 {
    Pos start;
    Vec3 delta;
    int dist;
    int pos;
    int idx;
    void* surface;
    View* view;
    Anim* anim;

    void Run()
    {
        for (; pos < dist; pos += 0x300000) {
            int f = (int)(((__int64)pos << 16) / dist);
            Pos o = offset_004394e0(delta.x, delta.y, delta.z, f);
            Pos p;
            p.x.value = start.x.value + o.x.value;
            p.y.value = start.y.value + o.y.value;
            p.z.value = start.z.value + o.z.value;
            DrawFrame(surface, *(void**)((char*)anim + idx * 8 + 0x28),
                         p.x.whole - view->scroll_x + 0x80,
                         p.z.whole - (p.y.whole >> 1) - view->scroll_y + 0x20);
            idx = (idx + 1) % anim->count;
        }
    }
};

// Snapshots the position the caller passed in `out`,
// calls DrawWeaponCoverage (which draws the order's icon and writes the new position
// to `out`), and when `flag` is set walks the line from the snapshot to the
// new position in 0x300000 steps, drawing frame `idx` of g_game->anims[21] at
// each step. idx starts at (frames since order->timestamp, clamped at 0) /
// max(1, anim->field_2c) % anim->count.
// FUNCTION: 0x4394e0
void __stdcall DrawPathAnim(void* surface, View* view,
                            Order* order, Pos* out, int flag)
{
    Pos start = *out;
    DrawWeaponCoverage(surface, view, order, out, flag);
    if (flag == 0)
        return;

    // Order matters: end, then the clamped age, then the deltas; no `int&` for the timestamp.
    Pos end = *out;
    int t = __max(g_game->frame - order->timestamp, 0);
    // Plain-int Vec3: Length() converts each component to its own double.
    Vec3 d;
    d.x = end.x.value - start.x.value;
    d.y = end.y.value - start.y.value;
    d.z = end.z.value - start.z.value;
    int dist = d.Length();
    if (dist < 0x10000)
        return;

    Anim* anim = g_game->anims[21];
    int pos = (t % 30) * 0x300000 / 30;
    unsigned short len = anim->field_2c;
    int frames = len < 1 ? 1 : (int)len;
    unsigned int idx = (t / frames) % anim->count;

    // The walk stays behind the inlined Trail::Run(); written inline it changes register use.
    Trail_004394e0 tr;
    tr.start = start;
    tr.delta = d;
    tr.dist = dist;
    tr.idx = idx;
    tr.surface = surface;
    tr.view = view;
    tr.pos = pos;
    tr.anim = anim;
    tr.Run();
}

// FUNCTION: 0x439740
void __stdcall DrawWeaponCoverage(void* surface, View* view, Order* order,
                            Pos* out, int unused)
{
    char buf[0x40];
    Pos pos;
    Unit* u = order->unit;
    if (order->target != 0) {
        if (IsUnitVisibleToPlayer(u->owner, order->target) == 0 && (order->flags & 0x200000) != 0) {
            pos.x.value = order->cached.x << 16;
            pos.y.value = order->target->pos.y.value;
            pos.z.value = order->cached.y << 16;
        } else {
            pos = order->target->pos;
            order->cached.x = pos.x.whole;
            order->cached.y = pos.z.whole;
            order->flags |= 0x200000;
        }
    } else {
        pos = order->pos;
    }
    if (DAT_00512344[order->kind].field_10 == 0) {
        *out = pos;
        return;
    }
    if (g_game->field_391bf != 0 &&
        (DAT_00512344[order->kind].field_10 == 1 || DAT_00512344[order->kind].field_10 == 2)) {
        int color;
        if (g_game->frame & 1)
            color = g_game->field_dcf;
        else
            color = g_game->field_dd7;
        for (int i = 0; i < 3; i++) {
            if (u->slots[(unsigned char)i].flags & 2) {
                if (u->slots[i].weapon->field_d6 != 0) {
                    sprintf(buf, "weapon %d - area of effect", i);
                    DrawRangeCircle(surface, view, &pos, u->slots[i].weapon->field_d6, color, buf, 0);
                }
                if (u->slots[i].weapon->field_e0 != 0) {
                    sprintf(buf, "weapon %d - coverage", i);
                    DrawRangeCircle(surface, view, &pos, u->slots[i].weapon->field_e0, color, buf, 1);
                }
            }
        }
        unsigned short len = u->def->attackLength;
        if (len != 0)
            DrawRangeCircle(surface, view, &pos, len, color, "attack length", 2);
    }
    Anim* anim = g_game->anims[DAT_00512344[order->kind].field_10];
    unsigned int n = g_game->frame / ((unsigned int)anim->field_2c * 2);
    n = n % anim->count;
    void* bmp = *(void**)((char*)anim + n * 8 + 0x28);
    DrawFrameBlended(surface, bmp, pos.x.whole - view->scroll_x + 0x80,
                 pos.z.whole - (pos.y.whole >> 1) - view->scroll_y + 0x20);
    *out = pos;
}

// Draws an ellipse (radius `height`, 0.89 of it vertically) of 16 segments
// around an object's screen position, then copies the position to `out`.
// FUNCTION: 0x4399f0
void __stdcall DrawOrderRangeRing(void* surface, View* view, Order* order,
                            Pos* out, int unused)
{
    Pos p;
    int height;
    if (order->target != 0) {
        p = order->target->pos;
        height = order->target->def->field_178;
    } else {
        p = order->pos;
        height = 0x20;
    }
    int ry = (int)(height * 0.89);
    int xc = p.x.whole - view->scroll_x + 0x80;
    int yc = p.z.whole - view->scroll_y - (p.y.whole >> 1) + 0x20;
    int x1 = xc + height;
    int y1 = yc;
    for (int angle = 0x1000; angle <= 0x10000; angle += 0x1000) {
        int nx = FUN_004b7123(angle, height) + xc;
        int ny = FUN_004b70ef(angle, ry) + yc;
        DrawLine(surface, x1, y1, nx, ny, g_game->field_dd7);
        x1 = nx;
        y1 = ny;
    }
    *out = p;
}
