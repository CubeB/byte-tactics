// Decompiled by Claude Opus 5.5. Names are provisional.
// Constructor of a player's AI state object (DAT_005119c0[player], built by
// 0x40b320; 0x40b390 destroys it). Its out-of-line STL callees are
// std::vector<Unit*>::vector(const allocator&) (0x40c510),
// std::vector<unsigned short>::size() (0x40d000) and
// std::vector<Elem_0040cfb0>::size() (0x40cc80), among others.
//
// Which calls MSVC 5 inlines here depends on its inline budget, and the
// member layout below is what reproduces the original's choices: the unit
// lists at +0x05 and +0x15 are one wrapper deep (their vector constructors
// are inlined), the one at +0x25 two wrappers deep (its vector constructor
// is called out of line), the first resize() inlines size() but not
// insert/erase, the next two call size() out of line, and the last four
// resize() calls are out of line. The fill values of the struct vectors live
// in block scopes so they share the dead parameter slot, as in the original.
#include <vector>

struct Unit {
    int unknown_0;
};

struct Elem_0040cfb0 {
    char a;
    char b;
    char c;
};

struct Elem_0040d4f0 {
    char value;
};

struct Elem_0040d550 {
    int unknown_0;
};

struct Point16 {
    short x;
    short y;
};

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

struct Vec3_00409160 {
    int x, y, z;
    Vec3_00409160(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
    Vec3_00409160() { *this = Vec3_00409160(0, 0, 0); }
};

struct Pos_00409160 {
    short x, y;
    int unknown_4;
    Pos_00409160(short ax, short ay) : unknown_4(0) { x = ax; y = ay; }
};

struct UnitList_00409160 {
    std::vector<Unit*> units;
};

struct Group_00409160 {
    UnitList_00409160 list;
};

#pragma pack(push, 1)
struct Game_00409160 {
    char unknown_0[0x1b63];
    char players[1][0x14b];            // +0x1b63
    char unknown_1[0x14233 - 0x1b63 - 0x14b];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_2[0x1438f - 0x1423b];
    int count;                         // +0x1438f
};

class Class_00409160 {
public:
    char* player;                      // +0x00
    unsigned char index;               // +0x04
    UnitList_00409160 list_5;          // +0x05
    UnitList_00409160 list_15;         // +0x15
    Group_00409160 group_25;           // +0x25
    Vec3_00409160 pos_35;              // +0x35
    Vec3_00409160 pos_41;              // +0x41
    std::vector<Elem_0040cc40> vec_4d; // +0x4d
    Pos_00409160 center;               // +0x5d
    std::vector<Elem_0040cfb0> vec_65; // +0x65
    int field_75;                      // +0x75
    int field_79;                      // +0x79
    std::vector<unsigned short> vec_7d; // +0x7d
    std::vector<unsigned char> vec_8d; // +0x8d
    std::vector<unsigned char> vec_9d; // +0x9d
    std::vector<Elem_0040d4f0> vec_ad; // +0xad
    std::vector<Elem_0040d550> vec_bd; // +0xbd
    std::vector<Elem_0040d550> values; // +0xcd
    std::vector<Elem_0040d550> locked; // +0xdd
    unsigned int lastTick;             // +0xed
    char unknown_f1[0x109 - 0xf1];
    int field_109;                     // +0x109

    Class_00409160(unsigned char player);
};
#pragma pack(pop)

extern Game_00409160* g_game;

class Class_0040a150 {
public:
    void FUN_0040a150();
};

class Class_00409470 {
public:
    void FUN_00409470();
};

// FUNCTION: 0x409160
Class_00409160::Class_00409160(unsigned char p)
    : player(g_game->players[p]), index(p),
      center(g_game->width / 2, g_game->height / 2), field_75(0)
{
    lastTick = 0;
    field_109 = 0;
    ((Class_0040a150*)this)->FUN_0040a150();
    int n = g_game->count;
    vec_9d.resize(n, 0);
    vec_7d.resize(n, 0);
    {
        Elem_0040cfb0 e;
        e.a = 0;
        e.b = 0;
        e.c = 0;
        vec_65.resize(n, e);
    }
    {
        Elem_0040d4f0 f;
        f.value = 0;
        vec_ad.resize(n, f);
    }
    {
        Elem_0040d550 g;
        g.unknown_0 = 0;
        vec_bd.resize(n, g);
        g.unknown_0 = 0;
        values.resize(n, g);
        g.unknown_0 = 0;
        locked.resize(n, g);
    }
    ((Class_00409470*)this)->FUN_00409470();
}
