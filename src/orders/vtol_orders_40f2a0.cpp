// Decompiled by Claude Opus 5.5. Names are provisional.
// Order handler that ends a transport (the unit script's "EndTransport").
// When unit->field_82 equals g_game->field_142b7 the unit heads towards the
// map centre instead. State 0 prepares the order (FUN_0040f200 is defined
// here because /Ob2 inlined it), state 1 tries the unit's own spot, then
// twelve random nearby cells snapped to the map grid, then a point circling
// the order's position; state 2 finishes.
// Match notes: WorldToCell needs `origin.x * 0x80000` (the `<< 19` spelling
// computes the shift straight into esi instead of `mov ecx, edx; shl ecx,
// 0x13; mov esi, ecx`); the post-loop flags test needs the (unsigned char)
// cast for `mov ebx, 0xe0; test bl, al`; and the circle position is summed
// into a temporary and then copied (`Vec3 dest = sum;`), which keeps all three
// sums live at once so y lands in edi.
#define max(a, b) (((a) > (b)) ? (a) : (b))

struct Point { short x, y; };
struct Vec3 {
    int x, y, z;
};

struct Unit;
class Class_0043d210 {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004898b0 { public: void ClaimWeapons(int); };
class Class_0048b090 { public: void SetStateBits(int, int); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0044e730 { public: void FUN_0044e730(short); };
class Class_004b0940 { public: void StartScript(const char*, int, int); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x21c]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[8]; Point footprint;
    int field_82; int field_86;
    char pad8a[8]; UnitDef* def;
    char pad96[4]; Class_004b0940* script;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x22 - 0xa]; Vec3 pos;
    char pad2e[8]; int angle; int field_3a;
    char pad3e[0x4a - 0x3e]; int field_4a;
};
struct Game {
    char pad0[0x1422b]; int width; int height;
    char pad14233[0x1427f - 0x14233]; unsigned char seaLevel;
    char pad14280[0x142b7 - 0x14280]; int field_142b7;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
int __stdcall GetGroundHeight(Vec3*);
int __stdcall FUN_0047e2d0(Unit*, Vec3*);
Vec3 __stdcall FUN_0040f790(const Vec3& a, const Vec3& b);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}
static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - origin.x * 0x80000 + 0x80000) >> 20;
    c.y = (v.z - origin.y * 0x80000 + 0x80000) >> 20;
    return c;
}
static inline void CellToWorld(Point origin, Point c, Vec3* v)
{
    v->x = (origin.x + c.x * 2) << 19;
    v->z = (origin.y + c.y * 2) << 19;
}

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    ((Class_004898b0*)unit)->ClaimWeapons(3);
    if (unit->field_86)
        AttachUnitToPiece(unit, 0, -1, 2);
    ((Class_0048b090*)unit)->SetStateBits(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// FUNCTION: 0x40f2a0
int __stdcall VtolLandIfCanOrder(Unit* unit, Order* order, int flags)
{
    if (order->field_4a)
        return 5;
    if (flags & 0x40)
        return 5;
    if (unit->field_82 == g_game->field_142b7) {
        Vec3 centre;
        centre.x = g_game->width / 2 << 16;
        centre.z = g_game->height / 2 << 16;
        short angle = GetHeadingBetween(&unit->pos, &centre);
        Vec3 dest = FUN_0040f790(unit->pos, Offset(angle, 0x3200000));
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x80);
        order->flags |= 0xe0;
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        return 2;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            if (order->pos.x == 0 && order->pos.z == 0 && order->pos.y == 0)
                order->pos = unit->pos;
            int a = FUN_004b6c30(0x10000);
            order->angle = a;
            order->field_3a = a & 1;
            FUN_0040f200(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        if (FUN_0047e2d0(unit, &unit->pos)) {
            unit->script->StartScript("EndTransport", 0, 1);
            Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
            int h = max(GetGroundHeight(&unit->pos), g_game->seaLevel);
            ((Class_0044e6c0*)obj)->FUN_0044e6c0(h <= g_game->seaLevel ? GetGroundHeight(&unit->pos) - g_game->seaLevel : 0);
            ((Class_004388d0*)order)->FUN_004388d0((int)obj);
            order->flags = 0xe0;
            ((Class_0048b090*)unit)->SetStateBits(1, 0);
            return 1;
        }
        for (int r = 0x40; r < 0x100; r += 0x10) {
            Vec3 p = unit->pos;
            p.x += (FUN_004b6c30(r * 2 + 1) - r) << 16;
            p.z += (FUN_004b6c30(r * 2 + 1) - r) << 16;
            Point fp = unit->footprint;
            Point cell = WorldToCell(p, fp);
            CellToWorld(fp, cell, &p);
            if (FUN_0047e2d0(unit, &p)) {
                ((Class_004388d0*)order)->FUN_004388d0((int)new Class_0044e2d0(order, p));
                order->flags = 0xe0;
                return 2;
            }
        }
        if ((unsigned char)flags & 0xe0)
            order->angle -= 0x5555;
        Vec3 off = Offset(order->angle, 0xa00000);
        Vec3 sum;
        sum.x = order->pos.x + off.x;
        sum.y = order->pos.y + off.y;
        sum.z = order->pos.z + off.z;
        Vec3 dest = sum;
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x40);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= 0xe0;
        return 2;
    }
    case 2:
        if (flags & 0x20) {
            unit->type->SetFlightMode(unit, 1);
            return 5;
        }
        return 8;
    }
    return 7;
}
