// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash. Names are
// provisional.
// Slot 0 of Class_00407d40 (vtable 0x4fc9a0), derived from Class_00407350
// (the family is listed in 0x407350.cpp, whose declarations this copies).
// Sets field_c to 30..179 ticks from now. When the group has units, moves the
// probe point b by the step c (one time in ten it restarts from a with a new
// random direction of length 0x140 map units), and when the owner can see or
// has explored b, keeps b as the new target a if a random roll favours its
// FUN_0040b1c0 score. Then orders every unit whose def has flag4 set, and
// that is active or can reach a (WeaponCanReachPos), to move to a with the order
// FUN_0043f0e0 picks.
//
// The explored-map test is the inlined player method 0x475470 describes: a
// {data, width, height} ByteMap at +0x7c whose Get() keeps `width * y + x`
// as an unfolded index (the data pointer is loaded after the multiply).
// The coordinates are plain ints, so `pos->x >> 5` stays a 32-bit sar; the
// earlier attempt's `unsigned int tx` shortened it to a 16-bit shift. All of
// the scratch registers then fall into place.
//
// The dead std::vector local is the `push 0; call operator delete` after the
// loop. `delay` must be computed first or the sum folds into one lea (as in
// 0x407ae0); Offset() as in 0x406300 keeps the call order and the zero y in
// ebp; `FUN_004b6c30(r) > FUN_004b6c30(field_38)` calls the field_38 roll
// first.
#include <vector>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int baseX;                          // +0x14223
    int baseY;                          // +0x14227
    char unknown_1422b[0x14281 - 0x1422b];
    unsigned char flags;                // +0x14281
    char unknown_14282[0x38a47 - 0x14282];
    int ticks;                          // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

struct Vec3_00407d40 {
    int x, y, z;

    Vec3_00407d40() {}
    Vec3_00407d40(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
    void operator+=(const Vec3_00407d40& v) { x += v.x; y += v.y; z += v.z; }
};

struct Position_00408090 {              // 16.16 fixed point; only high words read
    short xFrac;
    short x;                            // +0x2
    short yFrac;
    short y;                            // +0x6
    short zFrac;
    short z;                            // +0xa
};

struct MapSize_00408090 {
    unsigned int width;                 // +0x0
    unsigned int height;                // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00408090 {
    unsigned char* data;                // +0x0
    MapSize_00408090 size;              // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct Player_00408090 {
    char unknown_0[0x7c];
    ByteMap_00408090 explored;          // +0x7c
};

int __stdcall FUN_00408090(Player_00408090* player, Position_00408090* pos);

static inline int IsExplored(Player_00408090* player, Position_00408090* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (player->explored.size.Contains(tx, ty) && player->explored.Get(tx, ty))
        return 1;
    return 0;
}

static inline int IsVisible(Player_00408090* player, Position_00408090* pos)
{
    if ((g_game->flags & 2) == 2)
        return IsExplored(player, pos);
    return FUN_00408090(player, pos);
}

#pragma pack(push, 1)
struct UnitDef_00407e90 {
    char unknown_0[0x245];
    unsigned int unknown_bits : 4;
    unsigned int flag4 : 1;             // +0x245 bit 4
};

struct Unit {
    int field_0;                        // +0x0
    char unknown_4[0x6a - 0x4];
    Vec3_00407d40 pos;                  // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_00407e90* def;              // +0x92
};
#pragma pack(pop)

struct Group_00407e90 {
    Player_00408090* player;            // +0x0
    int id;                             // +0x4
    char unknown_8[0x10 - 0x8];
    std::vector<Unit*> units;           // +0x10
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() : index(0) {}
};

struct Class_00408cb0 {                 // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;              // +0x4
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class Class_00407350 {
public:
    Class_00408cb0* owner;              // +0x4
    void* field_8;                      // +0x8
    int field_c;                        // +0xc
    unsigned int field_10;              // +0x10

    Class_00407350(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0
    virtual ~Class_00407350() {}                    // slot 1
};

// Vtable 0x4fc9a0, constructor 0x407d40, ??_G 0x407e70.
class Class_00407d40 : public Class_00407350 {
public:
    Vec3_00407d40 a;                    // +0x14
    Vec3_00407d40 b;                    // +0x20
    Vec3_00407d40 c;                    // +0x2c
    int field_38;                       // +0x38

    Class_00407d40(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x407e90
};

int __stdcall FUN_004b6c30(int range);
int __cdecl FUN_004b70ef(int angle, int distance);
int __cdecl FUN_004b7123(int angle, int distance);

static inline Vec3_00407d40 Offset(int angle, int distance)
{
    Vec3_00407d40 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

int __stdcall FUN_0040b1c0(int index, Vec3_00407d40* pos, int range);
int __stdcall WeaponCanReachPos(Unit* unit, Vec3_00407d40* from, Vec3_00407d40* to, int flags);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char mode, Unit* unit,
                                      Unit* target, Vec3_00407d40* pos);
void __stdcall AddOrder(Class_00438760 kind, int remove, Unit* unit,
                            Unit* target, Vec3_00407d40* pos, int a, int b);

// FUNCTION: 0x407e90
void Class_00407d40::FUN_00407380()
{
    int delay = FUN_004b6c30(150) + 30;
    field_c = g_game->ticks + delay;
    if (((Group_00407e90*)field_8)->units.empty())
        return;
    std::vector<Unit*> unused;
    if (FUN_004b6c30(10) == 0) {
        b = a;
        int angle = FUN_004b6c30(0x10000);
        c = Offset(angle, 0x1400000);
    }
    b += c;
    if (IsVisible(((Group_00407e90*)field_8)->player, (Position_00408090*)&b)) {
        int r = FUN_0040b1c0(field_10, &b, 0xa0);
        if (FUN_004b6c30(r) > FUN_004b6c30(field_38)) {
            field_38 = r;
            a = b;
        }
    }
    for (std::vector<Unit*>::iterator it = ((Group_00407e90*)field_8)->units.begin();
         it != ((Group_00407e90*)field_8)->units.end(); ++it) {
        Unit* u = *it;
        if (u->def->flag4) {
            if (u->field_0 || WeaponCanReachPos(u, &u->pos, &a, 0)) {
                Class_00438760 kind = FUN_0043f0e0(3, u, 0, &a);
                if (kind.index)
                    AddOrder(kind, 0, u, 0, &a, 0, 0);
            }
        }
    }
}
