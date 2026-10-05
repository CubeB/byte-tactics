// Decompiled by GPT-6. Names are provisional.
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
struct Game {
    char unknown_0[0x1b63];
    char players[1][0x14b];            // +0x1b63
    char unknown_1[0x14233 - 0x1b63 - 0x14b];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_2[0x1438f - 0x1423b];
    int count;                         // +0x1438f
};

class PlayerAI {
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
    std::vector<short> vec_7d;  // +0x7d
    std::vector<unsigned char> vec_8d; // +0x8d
    std::vector<unsigned char> vec_9d; // +0x9d
    std::vector<Elem_0040d4f0> vec_ad; // +0xad
    std::vector<Elem_0040d550> vec_bd; // +0xbd
    std::vector<Elem_0040d550> values; // +0xcd
    std::vector<Elem_0040d550> locked; // +0xdd
    unsigned int lastTick;             // +0xed
    char unknown_f1[0x109 - 0xf1];
    int field_109;                     // +0x109


};
#pragma pack(pop)

extern PlayerAI* g_playerAI[];
// FUNCTION: 0x40b390
void __stdcall DestroyPlayerAI(int player)
{
    delete g_playerAI[player];
    g_playerAI[player]=0;
}
