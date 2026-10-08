// Decompiled by GPT-6, Claude Opus 5.5, space-bunny-free, deepseek-v4.1, deepseek-v4.1-flash and DeepSeek V4.1 Flash. Names are provisional.
// PlayerAI's RefreshUnitLists, which matches only in a file of its own: the
// addressing mode of its weight load follows this file's symbol ids. The rest
// of the class, its constructor and its other methods are in
// ai_player.cpp.
#include <windows.h>
#include <vector>
#include <math.h>
// Only for its symbol ids: RefreshUnitLists matches only in a window of the
// symbol count.
#include <io.h>

template<class T> struct List : std::vector<T> { void Clear() { clear(); } };

struct Point16 {
    short x;
    short y;
};

struct Vec3 {
    int x;
    int y;
    int z;
    Vec3 operator+(const Vec3& o) const { Vec3 r; r.x = x + o.x; r.y = y + o.y; r.z = z + o.z; return r; }
};

struct Vec { int x,y,z; Vec(int a,int b,int c):x(a),y(b),z(c){} };

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    // The value is taken as a float parameter: gives the original's fld/fstp copy.
    Elem_0040cc40(short x, short y, float k) { pos.x = x; pos.y = y; key = k; }
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

typedef std::vector<Elem_0040cc40> ElemVec;

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

#pragma pack(push,1)
struct Player { char pad[0x108]; unsigned char allied[0x3e]; unsigned char index; int IsAllied(unsigned char p) const { return allied[p]; } };
struct Def { char pad[0x156]; int builder; char pad15a[0x22f-0x15a]; char mobile; char pad230[0x241-0x230]; unsigned flags; char pad245[0x249-0x245]; };
struct Unit { char pad[0x6a]; int x,y,z; char pad76[0x92-0x76]; Def* def; Player* owner; char pad9a[12]; unsigned short id; char pada8[0x104-0xa8]; float progress; char pad108[6]; unsigned char active; char pad10f; unsigned flags; int pad114; unsigned char PlayerIndex() const { return owner->index; } int Ready() const { return (flags&0x10000000) && !(flags&0x4000); } };

#include "../units/unit_def.h"

struct Mission {
    char unknown_0[0xd30];
    int surfaceMetal;                  // +0xd30
};

struct Feature {
    char unknown_0[0xf0];
    float value;                       // +0xf0
    char unknown_f4[0xfe - 0xf4];
    unsigned short flags;              // +0xfe
};

struct Cell {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x14357 - 0x14273];
    Unit* units;                       // +0x14357
    Unit* unitsEnd;                    // +0x1435b
    char unknown_1435f[0x1438f - 0x1435f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def* defs;                         // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* net;                      // +0x391e9
};

class PlayerAI {
public:
    Player* owner;                     // +0x00
    unsigned char index;               // +0x04
    List<Unit*> visible;               // +0x05
    List<Unit*> known;                 // +0x15
    List<Unit*> factories;             // +0x25
    Vec centre;                        // +0x35
    char unknown_41[0x4d - 0x41];
    ElemVec cells;                     // +0x4d
    char unknown_5d[0x65 - 0x5d];
    std::vector<Elem_0040cfb0> vec_65; // +0x65
    int builders;                      // +0x75
    int hasSpecial;                    // +0x79
    std::vector<short> counts;         // +0x7d
    std::vector<unsigned char> vec_8d; // +0x8d
    std::vector<char> weights;         // +0x9d
    std::vector<Elem_0040d4f0> vec_ad; // +0xad
    std::vector<Elem_0040d550> vec_bd; // +0xbd
    std::vector<Elem_0040d550> values; // +0xcd
    std::vector<Elem_0040d550> locked; // +0xdd
    unsigned int lastTick;             // +0xed
    Point16 spacing0;                  // +0xf1
    Point16 offset0;                   // +0xf5
    int margin0;                       // +0xf9
    Point16 spacing1;                  // +0xfd
    Point16 offset1;                   // +0x101
    int margin1;                       // +0x105
    int field_109;                     // +0x109

    // In ai_player.cpp: it builds the unit lists one and two wrapper
    // levels deep to spend its inline budget as the original does.
    PlayerAI(unsigned char player);
    void InitUnitTables();
    // In ai_player_409730.cpp: it needs a cut-down <vector> for its symbol ids.
    void ComputeBaseWeights();
    bool FindCellNearFeatures(UnitDef* type, Vec3* pos, ElemVec* list, int range, Point16* out);
    bool FindRandomPlacementCell(UnitDef* type, Vec3* pos, int range, Point16* out);
    void BuildFeatureCells();
    // Stays in this file: its weight load's addressing mode follows the file's
    // symbol ids.
    void RefreshUnitLists();
    void UpdateEveryThirtyTicks();
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall RandomInt(int range);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
int __stdcall CanPlaceUnitFootprint(UnitDef* type, short a, Point16 cell, int b);
int __stdcall CanBuildAt(UnitDef* type, Point16 cell, int a, int b);
int GetBuildSiteMetal(void);
void __stdcall MakeHeap(Elem_0040cc40* first, Elem_0040cc40* last, int*, Elem_0040cc40*);
void __stdcall PopHeapFirst(Elem_0040cc40* first, Elem_0040cc40* last, Elem_0040cc40* dest,
                            Elem_0040cc40 val, int*);
Cell* __stdcall GetMapCell(int x, int y);
int __stdcall IsUnitVisibleToPlayer(Player*,Unit*);

static inline Point16 WorldToCell(Vec3 v, Point16 origin)
{
    Point16 c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

static inline int DistSq(const Point16& a, const Point16& b)
{
    int dy = a.y - b.y;
    int dx = a.x - b.x;
    return dx * dx + dy * dy;
}

// std::pop_heap(f, l) as the inline template expands it.
static inline void PopHeap(Elem_0040cc40* f, Elem_0040cc40* l)
{
    PopHeapFirst(f, l - 1, l - 1, Elem_0040cc40(*(l - 1)), (int*)0);
}

static inline Vec3 Direction(short angle, int scale)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}

// Clears the three unit lists (+0x05, +0x15, +0x25) through
// std::vector<Unit*>::erase (0x40c9f0) and refills them with insert
// (0x408f30).
// FUNCTION: 0x40aa40
void PlayerAI::RefreshUnitLists()
{
    visible.Clear();
    factories.Clear();
    known.Clear();
    builders=0; hasSpecial=0;
    struct { float x,y,z,total; } sum={0,0,0,0};
    std::fill(counts.begin(),counts.end(),(short)0);
    for(Unit* u=g_game->units+1;u<=g_game->unitsEnd;++u) {
        if(u->Ready()) {
            if(!owner->IsAllied(u->PlayerIndex())) {
                if(IsUnitVisibleToPlayer(owner,u) && !(u->flags&0x8000)) visible.push_back(u);
                if((unsigned char)(u->flags>>8)&1) known.push_back(u);
            } else if(u->PlayerIndex()==owner->index && u->progress==0.0) {
                ++counts[u->id];
                if(u->def->builder) ++builders;
                if((u->def->flags&0x40) && (u->def->flags&0x200) && (u->active&1)) factories.push_back(u);
                if((u->def->flags&0x400) && (u->active&1)) hasSpecial=1;
                float weight=weights[u->id];
                sum.total+=weight;
                sum.x-=(float)u->x*weight*-0.0000152587890625f;
                sum.y-=(float)u->y*weight*-0.0000152587890625f;
                sum.z-=(float)u->z*weight*-0.0000152587890625f;
            }
        }
    }
    if(sum.total!=0.0f) { sum.x/=sum.total; sum.y/=sum.total; sum.z/=sum.total; }
    int ix=(int)(sum.x*65536.0);
    int iy=(int)(sum.y*65536.0);
    int iz=(int)(sum.z*65536.0);
    centre=Vec(ix,iy,iz);
}
