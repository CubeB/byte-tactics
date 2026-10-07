// Decompiled by Claude Opus 5.5. Names are provisional.
// Order handler for an aircraft waiting in place: state 0 stops the unit and
// records the order unit's position in whole map units, state 1 ends the wait
// when FUN_0043b700 finds a unit that FUN_0043b1f0 accepts, and state 2 either
// queues VTOL_LANDIFCAN or moves to a random point near the recorded spot
// before waiting again.

struct Vec3 {
    int x, y, z;
    void operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; }
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit;

class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x21c]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Unit {
    void* type;
    char pad4[0x6a - 4];
    unsigned short posXFraction; short posX;
    int posY;
    unsigned short posZFraction; short posZ;
    char pad76[0x8a - 0x76]; int field_8a;
    char pad8e[4]; UnitDef* def;
    char pad96[0x110 - 0x96]; unsigned int flags;
    void ClaimWeapons(int);
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[4]; Unit* unit;
    char pad12[0x22 - 0x12]; Vec3 pos;
    short x; short z;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, int a, Vec3* b, int c, int d, int e);
};
#pragma pack(pop)

int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
Unit* __stdcall FUN_0043b700(Unit*);
int __stdcall FUN_0043b1f0(Unit*, Unit*, int);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// FUNCTION: 0x40f7d0
int __stdcall VtolStandbyOrder(Unit* unit, Order* order, int flags)
{
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            unit->ClaimWeapons(3);
            order->flags |= 0x10000;
            ((Class_00439e80*)order)->FUN_00439e80(1);
            order->x = order->unit->posX;
            order->z = order->unit->posZ;
            return 1;
        }
        break;
    case 1: {
        Unit* other = FUN_0043b700(unit);
        if (other && FUN_0043b1f0(unit, other, 0)) {
            order->flags = 0;
            order->state = 0;
            return 3;
        }
        return 1;
    }
    case 2:
        if ((unit->def->flags & 0x800) && (unit->flags & 3) == 2) {
            if (unit->field_8a) {
                short angle = RandomInt(0x10000);
                Vec3 p;
                p.x = order->x << 16;
                p.y = 0;
                p.z = order->z << 16;
                int distance = (RandomInt(0x20) + 8) << 16;
                p += Offset(angle, distance);
                Class_0044e2d0* obj = new Class_0044e2d0(order, p);
                ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c);
                ((Class_004388d0*)order)->FUN_004388d0((int)obj);
                ((Class_00439e80*)order)->FUN_00439e80(RandomInt(0xf) + 0x1e);
                order->state = 1;
                return 2;
            }
            AppendOrder(unit, new Class_0043a1f0(Class_00438760("VTOL_LANDIFCAN"), 0, &order->pos, 0, 0, 0));
            return 5;
        }
        order->flags |= 0x10000;
        ((Class_00439e80*)order)->FUN_00439e80(RandomInt(0x1e) + 0x1e);
        order->state = 1;
        return 2;
    }
    return 7;
}
