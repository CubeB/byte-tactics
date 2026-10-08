// Decompiled by Opus, GPT-6, Claude Opus 5.5, Sonnet, Haiku, GPT-6 Astra, deepseek-v4.1-flash, GPT-6.1-sol, mimo-v2.6-pro, space-bunny-free and DeepSeek V4.1 Flash. Names are provisional.
// The AI player, second part: the weight and limit tables, the AI console
// command setup, the player AI object's placement grid, the reaction and build
// helpers and the player dump, gathered in address order.
//
// Kept in files of their own, each matching only in its old file's compilation
// context:
//   0x409730  ai_player_409730.cpp (ComputeBaseWeights needs that file's
//     cut-down <vector> and its small symbol count)
//   0x40b1c0  ai_player_40b1c0.cpp (in this file the compiler encodes its table
//     access as [pointer + id] instead of the original's [id + pointer])
//   0x40b530  ai_player_40b530.cpp (its std::_Construct hook, declared before
//     <vector>, changes every vector<Unit*> copy in the file)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <minmax.h>
#include <math.h>
#include <vector>

#pragma pack(push, 1)

struct Point16 {
    short x;
    short y;
};

struct Vec {
    int x;
    int y;
    int z;
};

// The world position of the player AI object and of a build site.
struct Vec3 {
    int x;
    int y;
    int z;

    int Length() const
    {
        // One double local per component: `(double)x * x` reorders the sum.
        double fx = x;
        double fy = y;
        double fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
    void Scale(int s)                  // s is 16.16 fixed point
    {
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
    }
};

static inline Vec3 operator-(const Vec3& p, const Vec3& q)
{
    Vec3 r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    return r;
}

struct Vec3_40b0d0 {
    int x;
    int y;
    int z;
};

struct Struct_0040ba80 {
    int a, b, c;
};

struct Struct_0040bab0 {
    int a, b, c;
};

struct Vec3_0040beb0 {
    int x;
    int y;
    int z;

    int Length() const
    {
        // One double local per component: `(double)x * x` reorders the sum.
        double fx = x;
        double fy = y;
        double fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
    void Scale(int s)                  // s is 16.16 fixed point
    {
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
    }
};

static inline Vec3_0040beb0 operator-(const Vec3_0040beb0& p, const Vec3_0040beb0& q)
{
    Vec3_0040beb0 r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    return r;
}

// A map cell and its sort key, the element of the player AI's cell vector.
struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    // Defined out of line below, not in the class, so the compiler emits it.
    Elem_0040cc40(const Elem_0040cc40& o);
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

// The player AI's per-type ratings and values.
struct Elem_0040cfb0 {
    char a;
    char b;
    char c;
};

struct Elem_0040d4f0 {
    unsigned char value;
};

struct Elem_0040d550 {
    int unknown_0;
};

struct Pos_00409160 {
    short x, y;
    int unknown_4;
    Pos_00409160(short ax, short ay) : unknown_4(0) { x = ax; y = ay; }
};

struct Unit;

struct UnitList_00409160 {
    std::vector<Unit*> units;
};

struct Group_00409160 {
    UnitList_00409160 list;
};

// The AI commands' argument list.
class CommandArgs {
public:
    char unknown_0[0xd0];
    int field_d0;
    CommandArgs* InitArgs();
};

class Class_004b74f0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
};

// A 32-bit word of bits, tested by unit type id.
struct Bits {
    unsigned* words;
    int Test(unsigned short index) const { return (words[index>>5]&(1u<<(index&31)))!=0; }
};

// The unit type table entry. The description string of 0x40c250 and the typed
// fields of the other files cover the same bytes, so they are one union.
struct UnitDef {
    char name[0x20];                           // +0x0
    union {
        char description[0x249 - 0x20];        // +0x20
        struct {
            char unknown_20[0xa0 - 0x20];
            char field_a0[0xbe - 0xa0];        // +0xa0
            char command[0x14a - 0xbe];        // +0xbe, run as a console command
            Point16 origin;                    // +0x14a
            char unknown_14e[0x152 - 0x14e];
            int count;                         // +0x152
            unsigned short* ids;               // +0x156
            char unknown_15a[0x1ce - 0x15a];
            float value;                       // +0x1ce
            char unknown_1d2[0x202 - 0x1d2];
            short range;                       // +0x202
            char unknown_204[0x231 - 0x204];
            Bits bad[3];                       // +0x231
            Bits exclude;                      // +0x23d
            union {
                unsigned int flags;            // +0x241
                struct {
                    unsigned int unknown_241 : 5;
                    unsigned int field_5 : 1;
                    unsigned int rest : 26;
                };
            };
            char unknown_245[0x249 - 0x245];
        };
    };
};

class Mission {
public:
    char unknown_0[0xd30];
    int threshold;                     // +0xd30
    const char* GetNameSlot(int);
    int GetGameType();
};

// A player as the unit and economy code sees it.
struct Player {
    int active;                        // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x8c - 0x74];
    float energy;                      // +0x8c
    char unknown_90[0x98 - 0x90];
    float metal;                       // +0x98
    char unknown_9c[0xa4 - 0x9c];
    float energyCapacity;              // +0xa4
    float metalCapacity;               // +0xa8
    char unknown_ac[0x14b - 0xac];
};

// The per-player info array at +0x1b8e, which overlaps the players array.
struct PlayerInfo {
    char name[0x48];
    unsigned char control;             // +0x48
    char unknown_49[0x14b - 0x49];
};

struct Game {
    char unknown_0[0x1b63];
    union {
        Player players[10];            // +0x1b63
        struct {
            char unknown_2b[0x1b8e - 0x1b63];
            PlayerInfo playerInfo[10]; // +0x1b8e
        };
    };
    char unknown_287c[0x14223 - 0x287c];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    char unknown_1422b[0x1438f - 0x1422b];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitDef* defs;                     // +0x1439b
    char unknown_1439f[0x37eee - 0x1439f];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x37f30 - 0x37ef2];
    unsigned char flags;               // +0x37f30
    char unknown_37f31[0x38a47 - 0x37f31];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* net;                      // +0x391e9
};

struct Weapon {
    char pad[0x111];
    unsigned char flags;               // +0x111
};

struct Slot {
    char pad[12];
    Weapon* weapon;
    char pad10[12];
};

struct Unit {
    char unknown_0[4];
    Slot weapons[3];                   // +0x4
    char unknown_58[0x6a - 0x58];
    Vec pos;                           // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef* def;                      // +0x92
    Player* owner;                     // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short id;                 // +0xa6
    char unknown_a8[0xff - 0xa8];
    unsigned char player;              // +0xff
    char unknown_100[0x10e - 0x100];
    unsigned char status;              // +0x10e
    char unknown_10f;
    unsigned int flags;                // +0x110
    int Ready() const { return (flags&0x10000000) && !(flags&0x4000); }
};

class PlayerAI {
public:
    char* player;                      // +0x00
    unsigned char index;               // +0x04
    UnitList_00409160 list_5;          // +0x05
    UnitList_00409160 list_15;         // +0x15
    Group_00409160 group_25;           // +0x25
    Vec3 pos_35;                       // +0x35
    Vec3 pos_41;                       // +0x41
    std::vector<Elem_0040cc40> cells;  // +0x4d
    Pos_00409160 center;               // +0x5d
    std::vector<Elem_0040cfb0> vec_65; // +0x65
    int field_75;                      // +0x75
    int field_79;                      // +0x79
    std::vector<short> vec_7d;         // +0x7d
    std::vector<unsigned char> vec_8d; // +0x8d
    std::vector<unsigned char> vec_9d; // +0x9d
    std::vector<Elem_0040d4f0> vec_ad; // +0xad
    std::vector<Elem_0040d550> vec_bd; // +0xbd
    std::vector<Elem_0040d550> values; // +0xcd
    std::vector<Elem_0040d550> locked; // +0xdd
    unsigned int lastTick;             // +0xed
    char unknown_f1[0x109 - 0xf1];
    int range;                         // +0x109

    PlayerAI(unsigned char player);
    void InitUnitTables();
    void ComputeBaseWeights();
    void RefreshUnitLists();
    void BuildFeatureCells();
    bool FindCellNearFeatures(UnitDef* type, Vec3* pos, std::vector<Elem_0040cc40>* list, int range, Point16* out);
    bool FindRandomPlacementCell(UnitDef* type, Vec3* pos, int range, Point16* out);
};

// The object behind the constructor of 0x407350's family: the placement grid
// fields of the same player AI object.
struct Sub {
    short v0;                          // +0x00
    short v1;                          // +0x02
    short v2;                          // +0x04
    short v3;                          // +0x06
    int   v4;                          // +0x08
};

struct Class_0040a150 {
    char unknown_0[0xf1];
    Sub  s0;                           // +0xf1
    Sub  s1;                           // +0xfd
    void InitPlacementGrid();
};

struct Class_004800c0 {
public:
    void EraseSwapBack(Unit**);
};

#pragma pack(pop)

extern Game* g_game;
extern PlayerAI* g_playerAI[];

void EnableAICommands();
int __stdcall ExecuteCommandText(char* text, int len, Class_004b74f0* vars, int param_4);
void LoadDefaultAIScript();
int __stdcall RandomInt(int range);
float __stdcall GetEnergyUse(int);
int __stdcall GetWeaponRange(Unit*, unsigned char);
int __stdcall WeaponCanReachUnit(Unit*, Unit*, unsigned char);
void __stdcall GetVisibleEnemiesInRadius(int player, const Vec* pos, int radius, int flags, std::vector<Unit*>* out);
float __stdcall GetNetEnergy(Player*);
float __stdcall GetNetMetal(Player*);
float __stdcall GetEnergyIncome(Player*);
float __stdcall GetMetalIncome(Player*);
int __stdcall FUN_00406ee0(int, unsigned short, int);
int __stdcall GetBuildRating(unsigned int player, unsigned short id);

static inline float Max(float a, float b) { return a > b ? a : b; }

static inline Vec3 MoveTowards(const Vec3* from, const Vec3* to, int maxLen)
{
    Vec3 d = *to - *from;
    int len = d.Length();
    if (maxLen >= len)
        return *to;
    d.Scale((int)(((__int64)maxLen << 16) / len));
    Vec3 r;
    r.x = from->x + d.x;
    r.y = from->y + d.y;
    r.z = from->z + d.z;
    return r;
}

static inline void CellToWorld(Vec3* out, Point16 p, Point16 origin) {
    out->x=(origin.x+p.x*2)<<19;
    out->z=(origin.y+p.y*2)<<19;
}

// 0x409730 (in ai_player_409730.cpp): its ids are made small with a cut-down
// <vector>, and the merged file cannot reproduce them.

// FUNCTION: 0x409dc0
void __stdcall ScaleUnitWeights(int player, unsigned int* mask, float scale, int lock)
{
    PlayerAI* p = g_playerAI[player];
    for (unsigned short i = 1; i < g_game->count; i++) {
        if (mask[i >> 5] & (1 << (i & 0x1f))) {
            // Own local: the address is then encoded as [offset + base].
            int off = i * 4;
            if (*(int*)(off + (int)p->vec_bd.begin()) == 0) {
                p->vec_ad[i].value = (unsigned char)__min(__max((int)(p->vec_ad[i].value * scale), 0), 100);
                if (lock)
                    *(int*)(off + (int)p->vec_bd.begin()) = 1;
            }
        }
    }
}

// FUNCTION: 0x409e90
void __stdcall SetUnitLimits(int player, unsigned int* mask, int value, int lock)
{
    PlayerAI* p = g_playerAI[player];
    for (unsigned short i = 1; i < g_game->count; i++) {
        if ((mask[i >> 5] & (1 << (i & 0x1f))) && p->locked[i].unknown_0 == 0) {
            p->values[i].unknown_0 = value;
            if (lock)
                p->locked[i].unknown_0 = 1;
        }
    }
}

// FUNCTION: 0x409f20
int __stdcall IsUnderLimit(int player, unsigned short index, int value)
{
    if (index >= 1 && index < g_game->count) {
        int* values = (int*)&g_playerAI[player]->values[0];
        if (values[index] == -1)
            return 1;
        return value < values[index];
    }
    return 0;
}

// FUNCTION: 0x409f80
void __stdcall FUN_00409f80(int player)
{
    PlayerAI* p = g_playerAI[player];
    EnableAICommands();
    for (unsigned short i = 1; i < g_game->count; i++) {
        UnitDef* def = &g_game->defs[i];
        if (def->field_5) {
            if (p->vec_bd[i].unknown_0 != 1) {
                int len = strlen(def->command);
                if (len != 0) {
                    Class_004b74f0 vars;
                    ((CommandArgs*)&vars)->InitArgs();
                    ExecuteCommandText(def->command, len, &vars, -1);
                }
            }
        }
    }
}

// FUNCTION: 0x40a040
void __stdcall FUN_0040a040(int player)
{
    PlayerAI* p = g_playerAI[player];
    EnableAICommands();
    for (unsigned short i = 1; i < g_game->count; i++) {
        UnitDef* def = &g_game->defs[i];
        if (def->field_5) {
            if (p->locked[i].unknown_0 != 1) {
                int len = strlen(def->command);
                if (len != 0) {
                    Class_004b74f0 vars;
                    ((CommandArgs*)&vars)->InitArgs();
                    ExecuteCommandText(def->command, len, &vars, -1);
                }
            }
        }
    }
}

// For each active player of type 2, calls InitUnitTables on its entry in
// g_playerAI, then calls LoadDefaultAIScript.
// FUNCTION: 0x40a100
void ResetAIPlayers()
{
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0 && g_game->players[i].type == 2) {
            g_playerAI[i]->InitUnitTables();
        }
    }
    LoadDefaultAIScript();
}

// Initialises two 12-byte sub-structures of the object. Each Sub is
// { short v0, v1, v2, v3; int v4; }; the object has one at +0xf1 (s0) and a
// second at +0xfd (s1). v0/v1 become a size from the base v4 plus a random
// amount (+8), and v2/v3 become a random offset centred on that size
// (rand(size) - size/2).
// FUNCTION: 0x40a150
void Class_0040a150::InitPlacementGrid()
{
    s0.v4 = 3;
    s0.v0 = RandomInt(10) + s0.v4 + 8;
    s0.v1 = RandomInt(3) + s0.v4 + 8;
    s0.v2 = RandomInt(s0.v0) - s0.v0 / 2;
    s0.v3 = RandomInt(s0.v1) - s0.v1 / 2;
    s1.v4 = 6;
    s1.v0 = RandomInt(0x14) + s1.v4 + 8;
    s1.v1 = RandomInt(3) + s1.v4 + 8;
    s1.v2 = RandomInt(s1.v0) - s1.v0 / 2;
    s1.v3 = RandomInt(s1.v1) - s1.v1 / 2;
}

// FUNCTION: 0x40a5b0
Elem_0040cc40::Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key)
{
}

// FUNCTION: 0x40ad70
void __stdcall FUN_0040ad70(int unused1, int unused2, int param_3)
{
    GetEnergyUse(param_3);
}

// Both loops push_back into the output vector: the first calls insert
// (0x408f30) out of line, the second inlines it and calls _Ucopy (0x406c10),
// _Ufill (0x406c40) and size (0x40c560), all members of std::vector<Unit*>
// (#135).
// FUNCTION: 0x40ad80
void __stdcall GetVisibleEnemiesInRadius(int player,const Vec* pos,int radius,int flags,std::vector<Unit*>* out)
{
    int radius2=radius*radius;
    std::vector<Unit*>& visible=g_playerAI[player]->list_5.units;
    std::vector<Unit*>::iterator it;
    for(it=visible.begin();it!=visible.end();++it) {
        Unit* unit=*it;
        int dz=pos->z-unit->pos.z;
        int dx=pos->x-unit->pos.x;
        int d=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
        if(d<=radius2 && unit->Ready()) out->push_back(unit);
    }
    PlayerAI* owner=g_playerAI[player];
    if(owner->field_79 && out->empty()) {
        for(it=owner->list_15.units.begin();it!=owner->list_15.units.end();++it) {
            Unit* unit=*it;
            int dz=pos->z-unit->pos.z;
        int dx=pos->x-unit->pos.x;
        int d=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
        if(d<=radius2 && unit->Ready()) out->push_back(unit);
        }
    }
}

// FUNCTION: 0x40b0d0
bool __stdcall HasReadyUnitInRange(int player, Vec3_40b0d0* p, int range)
{
    int r2 = range * range;
    PlayerAI* t = g_playerAI[player];
    std::vector<Unit*>& units = t->list_5.units;
    std::vector<Unit*>::iterator it = units.begin();
    if (it != units.end()) {
        do {
            Unit* u = *it;
            int dz = p->z - u->pos.z;
            int dx = p->x - u->pos.x;
            if ((int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32) <= r2
                && (u->flags & 0x10000000)
                && !(u->flags & 0x4000))
                return true;
        } while (++it != units.end());
    }
    return false;
}

// Sums the player's signed byte table (the vector at +0x8d) over the units
// within `range` of `pos`; the distance is the 64-bit high product (>> 32).
// 0x40b1c0 (in ai_player_40b1c0.cpp): in this file the compiler encodes the
// table access as [pointer + id] instead of the original's [id + pointer].

// Runs the periodic update of player `player`'s object (the body of
// 0x40ad20, inlined): at most once every 30 ticks.
// FUNCTION: 0x40b2c0
void __stdcall UpdatePlayerAI(int player)
{
    PlayerAI* p = g_playerAI[player];
    if (p && g_game->ticks >= p->lastTick + 0x1e) {
        p->RefreshUnitLists();
        p->lastTick = g_game->ticks;
        if (RandomInt(0x1e) == 0) {
            p->ComputeBaseWeights();
        }
    }
}

// FUNCTION: 0x40b320
void __stdcall CreatePlayerAI(int player)
{
    PlayerAI*& slot = g_playerAI[player];
    slot = new PlayerAI(player);
    slot->ComputeBaseWeights();
}

// FUNCTION: 0x40b370
void __stdcall RebuildFeatureCells(int param_1)
{
    ((PlayerAI*)g_playerAI[param_1])->BuildFeatureCells();
}

// FUNCTION: 0x40b390
void __stdcall DestroyPlayerAI(int player)
{
    delete g_playerAI[player];
    g_playerAI[player]=0;
}

// 0x40b530 (in ai_player_40b530.cpp): its std::_Construct hook, declared
// before <vector>, changes every vector<Unit*> copy in the file.

// FUNCTION: 0x40b7b0
Unit* __stdcall FindWeaponTarget(Unit* unit,unsigned char weapon,int useRange)
{
    int ai=unit->owner->active && unit->owner->type==2;
    Unit* fallback=0;
    int fallbackDistance=0x7fffffff;
    int bestDistance=0x7fffffff;
    Unit* best=0;
    std::vector<Unit*> candidates;
    if(useRange) GetVisibleEnemiesInRadius(unit->player,&unit->pos,GetWeaponRange(unit,weapon),0,&candidates);
    else GetVisibleEnemiesInRadius(unit->player,&unit->pos,unit->def->range,0,&candidates);
    for(int count=0;count<50;++count) {
        if(candidates.empty()) break;
        std::vector<Unit*>::iterator it=candidates.begin()+RandomInt(candidates.size());
        Unit* target=*it;
        ((Class_004800c0*)&candidates)->EraseSwapBack(it);
        if((target->flags&0x10000000) && !(target->flags&0x4000) &&
           ((target->def->flags&0x8000) || ai || (g_game->flags&4)) &&
           ((unit->def->flags&0x10000000) || WeaponCanReachUnit(unit,target,weapon)) &&
           (useRange || !unit->def->exclude.Test(target->id)) &&
           (!(unit->weapons[weapon].weapon->flags&0x80) || !(target->status&0x10))) {
            int dz=unit->pos.z-target->pos.z;
            int dx=unit->pos.x-target->pos.x;
            int d=RandomInt((int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32));
            if(unit->def->bad[weapon].Test(target->id)) {
                if(d<fallbackDistance) { fallbackDistance=d; fallback=target; }
            } else if(d<bestDistance) { bestDistance=d; best=target; }
        }
    }
    if(best) return best;
    return fallback;
}

// FUNCTION: 0x40ba80
void __stdcall GetBasePosition(int index, Struct_0040ba80* out)
{
    Struct_0040ba80* entry = (Struct_0040ba80*)((char*)g_playerAI[index] + 0x35);
    *out = *entry;
}

// FUNCTION: 0x40bab0
void __stdcall FUN_0040bab0(int index, int unused, Struct_0040bab0* out)
{
    out->a = (g_game->baseX << 16) -
             ((Struct_0040bab0*)((char*)g_playerAI[index] + 0x35))->a;
    out->b = 0;
    out->c = (g_game->baseX << 16) -
             ((Struct_0040bab0*)((char*)g_playerAI[index] + 0x35))->c;
}

// Both <minmax.h> and <math.h> are needed: together they set the SIB slot of
// the flags test and the register use.
// FUNCTION: 0x40bb00
int __stdcall GetBuildRating(int player,unsigned short type)
{
    // The pointers into the vectors' storage, as the original file spells them.
#pragma pack(push, 1)
    struct Owner_40bb00 {
        char pad[0x69];                // +0x00
        Elem_0040cfb0* ratings;        // +0x69
        char pad6d[0x81-0x6d];
        short* counts;                 // +0x81
        char pad85[0xb1-0x85];
        unsigned char* weights;        // +0xb1
    };
#pragma pack(pop)
    Owner_40bb00* owner=(Owner_40bb00*)g_playerAI[player];
    Player* p=&g_game->players[player];
    if(p->energy<50.0f) return 0;
    if(p->metal<25.0f) return 0;
    if(g_game->net->GetGameType()==1 && (g_game->defs[type].flags&0x20)) return 0;
    int energyCap=min(1000,(int)p->energyCapacity);
    int metalCap=min(500,(int)p->metalCapacity);
    int energy=(int)Max(0.0f,(energyCap-p->energy)*0.125f);
    int metal=(int)Max(0.0f,(metalCap-p->metal)*0.25f);
    if(GetNetEnergy(p)<1.0f) energy+=20;
    if(GetNetMetal(p)<1.0f) metal+=20;
    if(GetEnergyIncome(p)<50.0f) energy+=100;
    else if(GetEnergyIncome(p)<200.0f) energy+=10;
    if(GetMetalIncome(p)<3.0f) metal+=100;
    else if(GetMetalIncome(p)<5.0f) metal+=20;
    int metal2=min(max(metal,0),100);
    int energy2=min(max(energy-metal2,0),100);
    int normal=max(100-metal2-energy2,0);
    if(!FUN_00406ee0(player,type,owner->counts[type])) return 0;
    Elem_0040cfb0* r=&owner->ratings[type];
    return (normal*r->a+r->b*metal2+r->c*energy2)*owner->weights[type]/10000;
}

// FUNCTION: 0x40bdb0
unsigned short __stdcall ChooseBuildOption(unsigned int player, Unit* unit)
{
    unsigned short chosen = 0;
    int total = 0;
    for (int i = 0; i < unit->def->count; i++) {
        unsigned short id = unit->def->ids[i];
        int r = GetBuildRating(player, id);
        if (r > 0) {
            total += r;
            if (RandomInt(total) < r)
                chosen = id;
        }
    }
    if (chosen != 0) {
        if (strcmp(unit->def->field_a0, g_game->defs[chosen].field_a0) != 0)
            chosen = 0;
    }
    return chosen;
}

// Moves `from` towards `to` by at most maxLen: returns `to` when it is within
// maxLen, else `from` plus the difference scaled (16.16 fixed point) to length
// maxLen. Returns the vector by value (hidden return buffer).
// FUNCTION: 0x40beb0
Vec3_0040beb0 __stdcall MoveToward(const Vec3_0040beb0* from, const Vec3_0040beb0* to,
                                     int maxLen)
{
    // From an inline operator- returning by value: keeps it in memory.
    Vec3_0040beb0 d = *to - *from;
    int len = d.Length();
    if (maxLen >= len)
        return *to;
    d.Scale((int)(((__int64)maxLen << 16) / len));
    Vec3_0040beb0 r;
    r.x = from->x + d.x;
    r.y = from->y + d.y;
    r.z = from->z + d.z;
    return r;
}

// FUNCTION: 0x40bfe0
int __stdcall FindBuildPosition(int player, const Vec3* from, UnitDef* type, Vec3* out)
{
    PlayerAI* ai=g_playerAI[player];
    int maximum=g_game->baseX > g_game->baseY ? g_game->baseX : g_game->baseY;
    if (ai->range<maximum) ai->range+=160;
    Vec3 pos=MoveTowards(from,&ai->pos_35,ai->range<<16);
    Point16 cell;
    int result;
    if (type->value!=0.0f && g_game->net->threshold<RandomInt(255))
        result=ai->FindCellNearFeatures(type,&pos,&ai->cells,ai->range*4,&cell);
    else result=ai->FindRandomPlacementCell(type,&pos,ai->range,&cell);
    if (result) {
        CellToWorld(out,cell,type->origin);
        ai->range=0;
    }
    return result;
}

// FUNCTION: 0x40c200
int __stdcall GetUnitCount(int param_1, unsigned int param_2)
{
    unsigned int idx = param_2 & 0xffff;
    void* p = g_playerAI[param_1];
    int* ptr = (int*)((char*)p + 0x81);
    short* arr = (short*)(*ptr);
    return (int)arr[idx];
}

// FUNCTION: 0x40c230
int __stdcall GetBuilderCount(int param_1)
{
    char* base = (char*)&g_playerAI;
    void* ptr = *(void**)(base + param_1 * 4);
    return *(int*)((char*)ptr + 0x75);
}

// FUNCTION: 0x40c250
void __stdcall DumpPlayerAI(int player, FILE* file)
{
    PlayerAI* ai=g_playerAI[player];
    char buffer[256];
    unsigned int hours=g_game->ticks/108000;
    int remaining=g_game->ticks-hours*108000;
    int minutes=remaining/1800;
    int seconds=(remaining-minutes*1800)/30;
    sprintf(buffer,"%02d:%02d:%02d",hours,minutes,seconds);
    fprintf(file,"gametime: '%s'\r\n",buffer);
    fprintf(file,"player:   '%s' num: %d\r\n",g_game->playerInfo[player].name,player);
    switch(g_game->playerInfo[player].control) {
    case 1: strcpy(buffer,"HUMAN"); break;
    case 2: strcpy(buffer,"AI"); break;
    default: strcpy(buffer,"INVALID"); break;
    }
    fprintf(file,"controller: %s\r\n",buffer);
    fprintf(file,"terrain:    '%s'\r\n",g_game->net->GetNameSlot(1));
    fprintf(file,"profile:    '%s'\r\n",g_game->net->GetNameSlot(7));
    const char* difficulties[]={"EASY","MEDIUM","HARD"};
    fprintf(file,"difficulty: '%s'\r\n",difficulties[g_game->difficulty]);
    fprintf(file,"================================================\r\n");
    fprintf(file,"<limit> - <base:baseML:baseEL> : <end result - before economy-based tweaks> - <unit name>\r\n");
    for (unsigned short i=1;i<g_game->count;++i) {
        UnitDef* type=&g_game->defs[i];
        if (ai->values[i].unknown_0<0) fprintf(file,"n/a ");
        else fprintf(file,"%4d",ai->values[i].unknown_0);
        sprintf(buffer," - %3d : %3d : %3d = %3d - '%s\t\t:%s'\r\n",ai->vec_65[i].a,ai->vec_65[i].b,ai->vec_65[i].c,ai->vec_ad[i].value,type->description,type->name);
        // Original passes the formatted unit text as a format string, so percent signs are interpreted again.
        fprintf(file,buffer);
    }
}

// FUNCTION: 0x40c4f0
int __stdcall FUN_0040c4f0(int param_1)
{
    return *(int*)((char*)g_playerAI[param_1] + 0x79);
}

// std::vector<Unit*>::vector(const allocator&) from MSVC 5's <vector>, out of
// line: copies the empty allocator byte and zeroes _First, _Last and _End.
// It is also the default constructor (the allocator is a default argument,
// so each caller pushes the address of a temporary allocator). 0x409160
// calls it for its unit list at +0x25.
// 0x4152f0 builds a local with it, then calls 0x40c560 (size) and 0x40c530
// (the destructor) on that same local; 0x410850 also builds a local with it.

// Explicit instantiation of the whole class emits this constructor.
template class std::vector<Unit*>;

// FUNCTION: 0x40c510 ??0?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAE@ABV?$allocator@PAUUnit@@@1@@Z
