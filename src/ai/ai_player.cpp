// Decompiled by Opus, GPT-6, Claude Opus 5.5, Sonnet, Haiku, GPT-6 Astra, deepseek-v4.1-flash, GPT-6.1-sol, mimo-v2.6-pro, space-bunny-free, deepseek-v4.1, Sonnet 5.5 and DeepSeek V4.1 Flash. Names are provisional.
// The AI player translation unit, parts 1 and 2 joined in address order: the
// plan, weight and limit console commands, the SquadManager and its SquadTimer
// family, the reaction and weapon retarget helpers, the type-table weapon
// scores, the player AI object's tables, placement helpers and the player dump.
//
// Kept in files of their own, each matching only in its old file's compilation
// context:
//   0x40c530 to 0x40d5b0  ai_player_40c530.cpp (the out-of-line std::vector
//     members: their register allocation follows the emissions in that file,
//     and four of them fall out of their windows next to the functions above)
//   0x407d40, 0x407e70, 0x407e90  ai_player_407d40.cpp, ai_player_407e70.cpp
//     (the constructor's vtable stores need the plain and the derived view of
//     SpatialTimer apart)
//   0x408090  ai_player_408090.cpp (the joined file folds the cell index's
//     width load into the imul, so the function is two bytes short)
//   0x408100  ai_player_408100.cpp (its symbol count comes from ta_types.h)
//   0x408620  ai_player_408620.cpp (in the joined file the definition's table
//     access takes the opposite SIB base)
//   0x408f30  ai_player_408f30.cpp (the vector::insert built with /Gi)
//   0x409730  ai_player_409730.cpp (ComputeBaseWeights needs that file's
//     cut-down <vector> and its small symbol count)
//   0x40aa40  player_ai.cpp
//   0x40b1c0  ai_player_40b1c0.cpp (in the joined file the compiler encodes its
//     table access as [pointer + id] instead of the original's [id + pointer])
//   0x40b530  ai_player_40b530.cpp (its std::_Construct hook, declared before
//     <vector>, changes every vector<Unit*> copy in the file)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <minmax.h>
#include <math.h>
#include <vector>
#include <windows.h>

#pragma pack(push, 1)

// The AI commands' argument list. 0x406c90 reads count and the Get* helpers;
// 0x409f80 and 0x40a040 call InitArgs through the same object.
class CommandArgs {
public:
    char unknown_0[0xd0];
    // The union stays for the match: its type ids set GetBuildRating (0x40bb00)'s registers.
    union {
        int count;                     // +0xd0
    };
    char* GetArg(int index, char* fallback);
    float GetFloatArg(int index, float default_val);
    int GetIntArg(int index, int fallback);
    CommandArgs* InitArgs();
};

// 0x40-byte set (512 bits).
class UnitTypeSet {
public:
    int bits[16];
    void AddTypeOrCategory(char* text, int* out);
};

struct WeaponDef {                     // 0x115 bytes
    char unknown_0[0x111];
    union {
        unsigned int flags;            // +0x111
        unsigned char flags8;          // the same byte, read as a byte in 0x40b7b0
        struct {
            unsigned int unknown_bits0 : 7;
            unsigned int flag7 : 1;
            unsigned int flag8 : 1;
            unsigned int unknown_bits9 : 17;
            unsigned int flag26 : 1;
            unsigned int unknown_bits27 : 3;
            unsigned int flag30 : 1;
            unsigned int unknown_bit31 : 1;
        };
    };
};

struct UnitWeaponSlot {                // 0x1c bytes, one of a unit's three
    char unknown_0[0xc];               // the aim target pair and the aim callback
    WeaponDef* weapon;                 // +0xc
    char unknown_10[0x1b - 0x10];
    unsigned char flags;               // +0x1b
};

struct Order {
    char unknown_0[0x42];
    unsigned int flags;                // +0x42
};

union Fixed {                          // a 16.16 fixed-point value
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

struct Vec3 {
    union {
        int x;
        Fixed xf;
        struct {
            unsigned short xFrac;
            short xWhole;
        };
    };
    union {
        int y;
        Fixed yf;
        struct {
            unsigned short yFrac;
            short yWhole;
        };
    };
    union {
        int z;
        Fixed zf;
        struct {
            unsigned short zFrac;
            short zWhole;
        };
    };

    Vec3 operator+(Vec3& o) { Vec3 r; r.x = x + o.x; r.y = y + o.y; r.z = z + o.z; return r; }
    Vec3 operator-(Vec3& o) { Vec3 r; r.x = x - o.x; r.y = y - o.y; r.z = z - o.z; return r; }
    void operator+=(Vec3& v) { x += v.x; y += v.y; z += v.z; }
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

// The 3-argument constructor, as a helper: the joined file's symbol count
// needs Vec3 to have no user constructors (0x40bfe0's MoveTowards inlining).
static inline Vec3 MakeVec3(int x, int y, int z)
{
    Vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

struct Point16 {
    short x;
    short y;
};

struct Sub_00409520 {
    char unknown_0[0xd4];
    unsigned short field_d4;           // +0xd4
    char unknown_d6[6];
    int field_dc;                      // +0xdc
    char unknown_e0[0x2a];
    char field_10a;                    // +0x10a
};

// A 32-bit word of bits, tested by unit type id.
struct Bits {
    unsigned* words;
    int Test(unsigned short index) const { return (words[index>>5]&(1u<<(index&31)))!=0; }
};

struct UnitDef {                       // 0x249 bytes
    char name[0x20];                   // +0x0
    union {
        char description[0x249 - 0x20];   // +0x20
        struct {
            char unknown_20[0xa0 - 0x20];
            char side[0xbe - 0xa0];       // +0xa0
            char command[0x14a - 0xbe];   // +0xbe, run as a console command
            Point16 origin;               // +0x14a
            char unknown_14e[0x152 - 0x14e];
            union {
                int count;                // +0x152
                int field_152;
            };
            union {
                unsigned short* ids;      // +0x156
                int field_156;
            };
            char unknown_15a[0x186 - 0x15a];
            float field_186;              // +0x186
            float field_18a;              // +0x18a
            char unknown_18e[0x1c0 - 0x18e];
            short field_1c0;              // +0x1c0
            char unknown_1c2[0x1ce - 0x1c2];
            union {
                float value;              // +0x1ce
                float field_1ce;
            };
            char unknown_1d2[0x1ee - 0x1d2];
            Sub_00409520* arr[3];         // +0x1ee
            char unknown_1fa[0x202 - 0x1fa];
            short range;                  // +0x202
            char unknown_204[0x22d - 0x204];
            char field_22d;               // +0x22d
            char unknown_22e;
            char field_22f;               // +0x22f
            char unknown_230;
            // The type sets: pointers to bit words, one view named by use.
            union {
                Bits bad[3];              // +0x231
                unsigned int* weaponCategories[3];
            };
            union {
                Bits exclude;             // +0x23d
                unsigned int* categories;
            };
            union {
                unsigned int flags;       // +0x241
                struct {
                    unsigned int unknown_241 : 5;
                    unsigned int field_5 : 1;
                    unsigned int rest : 26;
                };
                struct {
                    unsigned int unused : 6;
                    unsigned int builder : 1;
                    unsigned int unused7 : 4;
                    unsigned int flying : 1;
                    unsigned int unused12 : 20;
                };
            };
            union {
                unsigned int flags2;      // +0x245
                struct {
                    unsigned int unused245 : 12;
                    unsigned int special : 1;
                    unsigned int unused13 : 19;
                };
                struct {
                    unsigned int unknown_bits : 4;
                    unsigned int flag4 : 1;
                    unsigned int unknown_bits5 : 27;
                };
                struct {
                    unsigned int unknown_bits0 : 12;
                    unsigned int flag12 : 1;
                    unsigned int unknown_bits13 : 19;
                };
                struct {
                    unsigned int bit0_3 : 4;
                    unsigned int flag : 1;
                    unsigned int bit5_31 : 27;
                };
            };
        };
    };
};

struct Player;
struct Group;
struct Economy;
class SquadTimer;

struct Unit {                          // 0x118 bytes
    int motion;                       // +0x0
    UnitWeaponSlot weapons[3];         // +0x4, stride 0x1c
    char unknown_58[0x5c - 0x58];
    Order* orders;                     // +0x5c
    char unknown_60[0x6a - 0x60];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef* def;                      // +0x92
    union {
        Player* owner;                 // +0x96
        Economy* economy;
    };
    char unknown_9a[0xa6 - 0x9a];
    union {
        unsigned short category;       // +0xa6
        unsigned short id;
    };
    char unknown_a8[0xac - 0xa8];
    int group;                         // +0xac
    char unknown_b0[0xf4 - 0xb0];
    unsigned char player;              // +0xf4
    unsigned char state;               // +0xf5
    char unknown_f6[0xff - 0xf6];
    unsigned char ownerIndex;          // +0xff
    char unknown_100[0x104 - 0x100];
    float progress;                    // +0x104
    unsigned char allied[6];           // +0x108
    union {
        unsigned short flags10e;       // +0x10e
        unsigned char activateFlags;
        unsigned char status;
        unsigned char active;
    };
    // The unit's state flags: each handler names its own bits.
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;
            unsigned int rest : 30;
        };
    };
    char unknown_114[4];

    void SetStateBits(int which, int on);
    int Ready() const;
};

// Unused here: these forward declarations take the symbol ids that keep 0x406f80, 0x408670, 0x4086d0 and 0x4089a0 matching (docs/c2-regalloc.md).
struct Sound;
struct HapiBank;
struct TdfFile;
struct TdfRecord;
struct Gadget;
struct Layer;
struct UnitSync;
struct Packet;
struct Pathfinder;
#include "../network/player.h"

inline int Unit::Ready() const { return (flags & 0x10000000) && !(flags & 0x4000); }

struct Economy {                       // a unit's resources
    char unknown_0[0x8c];
    float energy;                      // +0x8c
    float field_90;                    // +0x90
    float field_94;                    // +0x94
    float cost;                        // +0x98
};

struct Group {                         // 0x20 bytes, one of a player's squads
    Player* player;                    // +0x0
    int id;                            // +0x4
    char unknown_8[0x10 - 0x8];
    std::vector<Unit*> units;          // +0x10

    void Send(unsigned char mode, int remove, Unit* target, Vec3* pos, int flags, int extra);
};

void __stdcall OrderSquad(Player*, int, unsigned char, int, Unit*, Vec3*, int, int);

#include "squad_manager.h"

struct Target_00406f50 {
    char unknown_0[0xbb];
    unsigned short bit0_4 : 5;         // +0xbb
    unsigned short flag5 : 1;          // 0x20
    unsigned short flag6 : 1;          // 0x40
    unsigned short bit7_15 : 9;
};

struct Obj_00406f50 {
    char unknown_0[0x52];
    Target_00406f50* target;           // +0x52
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class SquadTimer {
public:
    SquadManager* owner;               // +0x4
    Group* group;                      // +0x8
    int next;                          // +0xc
    unsigned int player;               // +0x10

    SquadTimer(SquadManager* p, Group* q);
    virtual void OnTimer();                         // slot 0
    virtual ~SquadTimer() {}                        // slot 1
    int TryGetBaseBuildOrScoutCentroid(Vec3* out);
    int GetAveragePosition(Vec3* out);
    int CountGroupUnitsInRadius(Vec3* pos, int radius);
};

// Vtable 0x4fc988, constructor 0x407930, ??_G 0x407980.
class AssaultTimer : public SquadTimer {
public:
    int minimum;                       // +0x14
    int maximum;                       // +0x18
    int limit;                         // +0x1c
    int kind;                          // +0x20
    int attacking;                     // +0x24

    AssaultTimer(SquadManager* p, Group* q, int a, int b);
    virtual void OnTimer();                         // slot 0, 0x4077e0
    void RebalanceAssaultGroupByCentroid(int kind, int limit);
};

// Vtable 0x4fc990, constructor 0x4079a0, ??_G 0x4079d0.
class EscortTimer : public SquadTimer {
public:
    int other;                         // +0x14

    EscortTimer(SquadManager* p, Group* q, int a);
    virtual void OnTimer();                         // slot 0, 0x4079f0
};

// Vtable 0x4fc998, constructor 0x407a90, ??_G 0x407ac0.
class SquadScoutTimer : public SquadTimer {
public:
    SquadScoutTimer(SquadManager* p, Group* q);
    virtual void OnTimer();                         // slot 0, 0x407ae0
};

// Vtable 0x4fc9a8, constructor 0x4085d0, ??_G 0x408600.
class ScoutTimer : public SquadTimer {
public:
    ScoutTimer(SquadManager* p, Group* q);
    virtual void OnTimer();                         // slot 0, 0x408100
};

// Vtable 0x4fc9b0, constructor 0x4087e0, ??_G 0x408810.
class BuildTimer : public SquadTimer {
public:
    BuildTimer(SquadManager* p, Group* q);
    virtual void OnTimer();                         // slot 0, 0x4086d0
};

// Vtable 0x4fc9a0, constructor 0x407d40, ??_G 0x407e70. Its constructor and
// slot 0 stay in ai_player_407d40.cpp and ai_player_407e70.cpp.
class SpatialTimer : public SquadTimer {
public:
    Vec3 a;                            // +0x14
    Vec3 b;                            // +0x20
    Vec3 c;                            // +0x2c
    int field_38;                      // +0x38

    SpatialTimer(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0, 0x407e90
};

// The AI controller (SquadManager) and the assault slot (AssaultTimer) methods
// that had class views of their own are declared on those classes now, and the
// placement-grid fields of the old player-AI view sit on PlayerAI.
//
// Unused here: the symbol ids these declarations take keep the allocation
// (docs/c2-regalloc.md); they stand where the removed class views stood.
char FindGameCdDrive(int);
int FUN_00490200();
int AreAllPlayersReady(void);
int CountActiveAIPlayers(void);
int CountComputerPlayers(void);
int CountHumanPlayers(void);
int ExpireOldestMessage(void);
int AreAllSlotsEmpty(void);
int CheckAlliedVictory(void);
int CheckMapCrc(void);
char IsGonzo();
int FindFreeSlot(void);
int FindOpenSlot(void);
int GetBuildSiteHeight(void);
int GetCdPathMismatch(void);
int GetCdPosition(void);
int GetCobChecksum(int);
int GetCpuFamily(void);
char IsMemFussy();
int GetCursorSprite(void);
int GetDebugFillPattern(void);
char IsBackAlign();
char IsMemSet();
char IsPentiumOrBetter();

// Unused here: the symbol ids these declarations take keep GetBuildRating (0x40bb00)
// matching now that the weapon slot view is one type (docs/c2-regalloc.md).
void RegisterUnitOrders(void);
void RegisterGroundOrders(void);
void RegisterVtolOrders(void);
void StepAllGafSequences(void);
void ResetNetStats(void);
void InitCommands(void);
int UpdatePlacementGhostValidity(void);

// Unused here: these take the symbol ids of the removed argument-list view, which
// keep GetBuildRating (0x40bb00) matching (docs/c2-regalloc.md).
void ResetCameraState(void);
void FindLocalCommander(void);
void ClampCameraPosition(void);
void ClampCameraTarget(void);
void UpdateScreenShake(void);
void BeginMouseScroll(void);

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Feature {
    char unknown_0[0xf0];
    float value;                       // +0xf0
    char unknown_f4[0xfe - 0xf4];
    unsigned short flags;              // +0xfe
};

#include "../map/cell.h"

class Mission {
public:
    char unknown_0[0xd30];
    union {
        int surfaceMetal;              // +0xd30
        int threshold;
    };
    const char* GetNameSlot(int);
    int GetGameType();
};

// The per-player info array at +0x1b8e, which overlaps the players array.
struct PlayerInfo {
    char name[0x48];
    unsigned char control;             // +0x48
    char unknown_49[0x14b - 0x49];
};

// The players array and its info-array view at +0x1b8e. The union is the
// compiler's anonymous one, so both names stay reachable.
struct Game {
    char unknown_0[0x1b63];
    union {
        Player players[10];            // +0x1b63
        struct {
            char unknown_2b[0x1b8e - 0x1b63];
            PlayerInfo playerInfo[10]; // +0x1b8e
        };
    };
    char unknown_1[0x2a43 - 0x287c];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2[0x14223 - 0x2a44];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    int mapWidth;                      // +0x1422b
    int mapHeight;                     // +0x1422f
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_6[0x1426f - 0x1423b];
    Feature* features;                 // +0x1426f
    unsigned short* visibilityMask;    // +0x14273
    char unknown_7[0x14281 - 0x14277];
    unsigned short mapFlags;           // +0x14281
    char unknown_8[0x14357 - 0x14283];
    Unit* units;                       // +0x14357
    Unit* unitsEnd;                    // +0x1435b
    char unknown_9[0x1438f - 0x1435f];
    int count;                         // +0x1438f
    char unknown_10[0x1439b - 0x14393];
    UnitDef* defs;                     // +0x1439b
    char unknown_11[0x37ee6 - 0x1439f];
    unsigned short maxUnits;           // +0x37ee6
    char unknown_12[0x37eee - 0x37ee8];
    int difficulty;                    // +0x37eee
    char unknown_13[0x37f30 - 0x37ef2];
    unsigned char matchFlags;          // +0x37f30, tested with 0x4 in 0x40b7b0
    char unknown_14[0x38a47 - 0x37f31];
    unsigned int ticks;                // +0x38a47
    char unknown_15[0x391e9 - 0x38a4b];
    Mission* net;                      // +0x391e9
};

// A map cell and its sort key, the element of the player AI's cell vector.
struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    // Defined out of line below, not in the class, so the compiler emits it.
    Elem_0040cc40(const Elem_0040cc40& o);
    // The value is taken as a float parameter: gives the original's fld/fstp copy.
    Elem_0040cc40(short x, short y, float k) { pos.x = x; pos.y = y; key = k; }
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

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

struct Vec3_00409160 {
    int x, y, z;
    Vec3_00409160(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
    Vec3_00409160() { *this = Vec3_00409160(0, 0, 0); }
};

struct UnitList_00409160 {
    std::vector<Unit*> units;
    void Clear() { units.clear(); }
};

struct Group_00409160 {
    UnitList_00409160 list;
    void Clear() { list.Clear(); }
};

class PlayerAI {
public:
    Player* owner;                     // +0x00
    unsigned char index;               // +0x04
    // Wrapper depth decides which vector constructors inline: visible and
    // known are one level deep, factories two.
    UnitList_00409160 visible;         // +0x05
    UnitList_00409160 known;           // +0x15
    Group_00409160 factories;          // +0x25
    Vec3_00409160 centre;              // +0x35
    Vec3_00409160 pos_41;              // +0x41
    std::vector<Elem_0040cc40> cells;  // +0x4d
    Pos_00409160 center;               // +0x5d
    std::vector<Elem_0040cfb0> vec_65; // +0x65
    int builders;                      // +0x75
    int hasSpecial;                    // +0x79
    std::vector<short> counts;         // +0x7d
    std::vector<unsigned char> vec_8d; // +0x8d
    std::vector<unsigned char> weights; // +0x9d
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

    PlayerAI(unsigned char player);
    void InitUnitTables();
    void InitPlacementGrid();
    void ComputeBaseWeights();
    bool FindCellNearFeatures(UnitDef* type, Vec3* pos, std::vector<Elem_0040cc40>* list, int range, Point16* out);
    bool FindRandomPlacementCell(UnitDef* type, Vec3* pos, int range, Point16* out);
    void BuildFeatureCells();
    void RefreshUnitLists();
    void UpdateEveryThirtyTicks();
};

struct Vec {
    int x;
    int y;
    int z;
};

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

struct Class_004800c0 {
public:
    void EraseSwapBack(Unit**);
};

#pragma pack(pop)

extern Game* g_game;
extern int g_aiCommandsEnabled;
extern char DAT_005119b8[];
extern PlayerAI* g_playerAI[];

void __stdcall ScaleUnitWeights(int player, UnitTypeSet* set, float value, int count);
void __stdcall SetUnitLimits(int player, UnitTypeSet* set, int value, int param_4);
int __stdcall IsUnderLimit(int player, unsigned short id, int value);
typedef void (__stdcall* Command_00406f00)(CommandArgs* args);
void __stdcall RegisterCommand(const char* name, Command_00406f00 fn, int flags);
void __stdcall NotifyUnitRefs(Unit*, int);
int __stdcall RandomInt(int);
void __stdcall DeleteOrders(Unit*, int);
int __stdcall WeaponCanReachUnit(Unit*, Unit*, unsigned char);
int __stdcall IssueAttackOrder(Unit*, Unit*, int);
Unit* __stdcall GetWeaponTargetUnit(Unit*, int);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
void __stdcall SetWeaponTargetUnit(Unit*, int, unsigned int);
unsigned int __stdcall GetOrderFlags(Unit*);
void __stdcall QueueUnitSpeechIfNotVisible(Unit*, int, int);
void __stdcall SetUnitSquad(Unit* unit, int squad);
int* __stdcall FindTargetableProjectile(Unit* unit, unsigned int weapon);
void __stdcall SetWeaponTargetPos(Unit* unit, int* param_2, unsigned int weapon);
void __stdcall ClearWeaponTarget(Unit* unit, unsigned int weapon);
void __stdcall GetBasePosition(int index, Vec3* out);
unsigned short __stdcall ChooseBuildOption(unsigned int player, Unit* unit);
int __stdcall GetBuilderCount(unsigned int player);
Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit* unit, Unit* target, Vec3* pos);
void __stdcall AddOrder(Class_00438760 kind, int remove, Unit* unit, Unit* target, Vec3* pos, int param_6, int param_7);
float __stdcall GetNetEnergy(Economy* economy);
void __stdcall QueueBuildOrder(char* name, Unit* unit, int count);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
void __stdcall MakeHeap(Elem_0040cc40* first, Elem_0040cc40* last, int*, Elem_0040cc40*);
void __stdcall PopHeapFirst(Elem_0040cc40* first, Elem_0040cc40* last, Elem_0040cc40* dest,
                            Elem_0040cc40 val, int*);
Cell* __stdcall GetMapCell(int x, int y);
int __stdcall IsUnitVisibleToPlayer(Player*, Unit*);
int __stdcall CanPlaceUnitFootprint(UnitDef* type, short a, Point16 cell, int b);
int __stdcall CanBuildAt(UnitDef* type, Point16 cell, int a, int b);
int GetBuildSiteMetal(void);
float __stdcall GetEnergyUse(UnitDef* p);

void EnableAICommands();
int __stdcall ExecuteCommandText(char* text, int len, CommandArgs* vars, int param_4);
void LoadDefaultAIScript();
float __stdcall GetEnergyUse(int);
int __stdcall GetWeaponRange(Unit*, unsigned char);
void __stdcall GetVisibleEnemiesInRadius(int player, const Vec* pos, int radius, int flags, std::vector<Unit*>* out);
float __stdcall GetNetEnergy(Player*);
float __stdcall GetNetMetal(Player*);
float __stdcall GetEnergyIncome(Player*);
float __stdcall GetMetalIncome(Player*);
int __stdcall IsCountBelowAiLimit(int, unsigned short, int);
int __stdcall GetBuildRating(unsigned int player, unsigned short id);
int __stdcall FindWeaponTarget(Unit* unit, unsigned int weapon, int param_3);
void __stdcall GetBasePosition(int index, Struct_0040ba80* out);

// The original keeps the four 0.0f comparisons at 0x4fc968, apart from the
// file's other 0.0f literal (0x4fc9b8): one constant per site keeps the
// placed addresses right.
static const float Zero_004fc968 = 0.0f;

static inline int Contains(unsigned int* bits, unsigned short index)
{
    return bits[index >> 5] & (1 << (index & 31));
}

// Inlined copy of DirectionFromAngle.
static inline Vec3 Direction(short angle, int scale)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}

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

void Group::Send(unsigned char mode, int remove, Unit* target, Vec3* pos, int flags, int extra)
{
    OrderSquad(player, id, mode, remove, target, pos, flags, extra);
}

// FUNCTION: 0x406c90
void __stdcall CmdPlan(CommandArgs* args)
{
    g_aiCommandsEnabled = 0;
    for (int i = 1; i < args->count; ++i) {
        if (!_strcmpi(args->GetArg(1, DAT_005119b8), "any")) g_aiCommandsEnabled = 1;
        if (g_game->difficulty == 0 && !_strcmpi(args->GetArg(i, DAT_005119b8), "easy")) g_aiCommandsEnabled = 1;
        if (g_game->difficulty == 1 && !_strcmpi(args->GetArg(i, DAT_005119b8), "medium")) g_aiCommandsEnabled = 1;
        if (g_game->difficulty == 2 && !_strcmpi(args->GetArg(i, DAT_005119b8), "hard")) g_aiCommandsEnabled = 1;
    }
}

// The original calls it out of line from 0x409f80 and 0x40a040; the joined
// file would inline the store into them.
#pragma auto_inline(off)
// FUNCTION: 0x406da0
void EnableAICommands()
{
    g_aiCommandsEnabled = 1;
}
#pragma auto_inline(on)

// Chat command handler (compare 0x406e40): parses a set from argument 1 and
// a float from argument 2, then calls ScaleUnitWeights for every player whose
// field_74 is set.
// FUNCTION: 0x406db0
void __stdcall CmdWeight(CommandArgs* args)
{
    if (g_aiCommandsEnabled != 0) {
        int count;
        UnitTypeSet set;
        memset(&set, 0, sizeof(set));
        set.AddTypeOrCategory(args->GetArg(1, DAT_005119b8), &count);
        float value = args->GetFloatArg(2, 0);
        // A narrow index, as in 0x406e40: MSVC then counts the loop down in a
        // separate register.
        for (char i = 0; i < 10; i++) {
            if (g_game->players[i].ai != 0) {
                ScaleUnitWeights(i, &set, value, count);
            }
        }
    }
}

// FUNCTION: 0x406e40
void __stdcall CmdLimit(CommandArgs* args)
{
    if (g_aiCommandsEnabled != 0) {
        int count;
        UnitTypeSet set;
        memset(&set, 0, sizeof(set));
        set.AddTypeOrCategory(args->GetArg(1, DAT_005119b8), &count);
        int value = args->GetIntArg(2, 0);
        // A narrow index: MSVC then counts the loop down in a separate
        // register instead of testing the player offset.
        for (char i = 0; i < 10; i++) {
            if (g_game->players[i].active != 0 && g_game->players[i].type == 2) {
                SetUnitLimits(i, &set, value, count);
            }
        }
    }
}

// FUNCTION: 0x406ee0
int __stdcall IsCountBelowAiLimit(unsigned char player, unsigned short id, int value)
{
    return IsUnderLimit(player, id, value);
}

// Registers the "plan", "weight" and "limit" console commands.
// FUNCTION: 0x406f00
void RegisterAICommands()
{
    RegisterCommand("plan", CmdPlan, 8);
    RegisterCommand("weight", CmdWeight, 8);
    RegisterCommand("limit", CmdLimit, 8);
}

// FUNCTION: 0x406f40
void FUN_00406f40(void)
{
}

// FUNCTION: 0x406f50
void SquadManager::MarkOwnerNetDirtyFromDamageSplit(Obj_00406f50* obj, int a, int b)
{
    Target_00406f50* t = obj->target;
    if (t) {
        if (a > b * 2) {
            t->flag6 = 1;
            return;
        }
        t->flag5 = 1;
    }
}

// FUNCTION: 0x406f80
void __stdcall ReactToAttack(Unit* attacker, Unit* unit, int unused)
{
    NotifyUnitRefs(unit,16);
    if (attacker && !attacker->category) attacker=0;
    if ((unit->def->flags2&0x1000) && unit->owner->active && unit->owner->type==2) {
        unit->owner->ai->nextAction=RandomInt(300)+g_game->ticks+30;
        DeleteOrders(unit,0);
    }
    if (attacker && unit->owner->active && (unit->owner->type==1 || unit->owner->type==2) &&
        (unit->def->flags&0x10010000) && unit->progress==Zero_004fc968 && !unit->owner->allied[attacker->owner->index]) {
        int ordered=0;
        if ((!unit->orders || (unit->orders->flags&0x20000)) &&
            !Contains(unit->def->categories,attacker->category) &&
            !Contains(unit->def->weaponCategories[0],attacker->category) && WeaponCanReachUnit(unit,attacker,0))
            ordered=IssueAttackOrder(unit,attacker,0);
        if (!ordered && (unit->flags&0x300000)) {
            for (unsigned char i=0;i<3;++i) {
                UnitWeaponSlot* weapon=&unit->weapons[i];
                if ((weapon->flags&2) && (weapon->flags&0x10) && WeaponCanReachUnit(unit,attacker,i) &&
                    !((unsigned char)(weapon->weapon->flags>>26)&1)) {
                    Unit* target=GetWeaponTargetUnit(unit,i);
                    if (!target || !WeaponCanReachUnit(unit,target,i) || Contains(unit->def->weaponCategories[i],target->category))
                        SetWeaponTargetUnit(unit,attacker,i);
                }
            }
        }
    }
    if (!(GetOrderFlags(unit)&0x80) && (unit->player!=unit->ownerIndex || unit->state==1))
        QueueUnitSpeechIfNotVisible(unit,2,0);
}

// FUNCTION: 0x4071f0
Unit* SquadManager::FindNearestEnemyUnit(int x,int y,int z)
{
    int best=0x7fffffff;
    Unit* result=0;
    for(unsigned char i=0;i<10;++i) {
        Player* p=&g_game->players[i];
        // The original retains the player-index range check inside the loop.
        if(i>=10) continue;
        if(p->active && (p->type==1 || p->type==2 || p->type==3) && p->index!=10 && !player->allied[p->index]) {
            Unit* u=p->unitsBegin;
            Unit* last=p->unitsEnd;
            for(;u<=last;++u) {
                if((u->flags&0x10000000) && u->mode!=2 && !(u->flags&0x8000) && !(u->flags10e&4)) {
                    int dz=z-u->pos.z;
                    int dx=x-u->pos.x;
                    int distance=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
                    if(distance<best) { result=u; best=distance; }
                }
            }
        }
    }
    return result;
}

// Constructor of SquadTimer, the base of a family of small classes with
// two virtual slots: slot 0 is a method each class overrides, slot 1 the
// virtual destructor.
//
//   class           vtable    constructor  ??_G      slot 0
//   SquadTimer  0x4fc980  0x407350     0x407390  0x407380 (empty)
//   AssaultTimer  0x4fc988  0x407930     0x407980  0x4077e0
//   EscortTimer  0x4fc990  0x4079a0     0x4079d0  0x4079f0
//   SquadScoutTimer  0x4fc998  0x407a90     0x407ac0  0x407ae0
//   SpatialTimer  0x4fc9a0  0x407d40     0x407e70  0x407e90
//   ScoutTimer  0x4fc9a8  0x4085d0     0x408600  0x408100
//   BuildTimer  0x4fc9b0  0x4087e0     0x408810  0x4086d0
// FUNCTION: 0x407350
SquadTimer::SquadTimer(SquadManager* p, Group* q)
    : owner(p), group(q), next(0), player(p->index)
{
}

// Slot 0 of SquadTimer (vtable 0x4fc980), empty in the base class; every
// derived class overrides it (the family is listed at the constructor).
// FUNCTION: 0x407380
void SquadTimer::OnTimer()
{
}

// The compiler-generated scalar deleting destructor of SquadTimer
// (vtable 0x4fc980), the base of the family listed at the constructor. Its
// destructor is empty and inline, so only the vtable store is left.
// FUNCTION: 0x407390 ??_GSquadTimer@@UAEPAXI@Z

// Asks three of the owner's timers in turn
// for their average position (0x407410) and returns 1 as soon as one has it.
// The owner's constructor (0x408cb0) fills an array of ten timer pointers at
// +0x11, so the three used here are entries 5, 1 and 4.
// FUNCTION: 0x4073b0
int SquadTimer::TryGetBaseBuildOrScoutCentroid(Vec3* out)
{
    if (owner->timers[5]->GetAveragePosition(out))
        return 1;
    if (owner->timers[1]->GetAveragePosition(out))
        return 1;
    return owner->timers[4]->GetAveragePosition(out) != 0;
}

// Averages the positions (16.16 fixed point, integer parts at
// +0x6c/+0x70/+0x74) of the units in the group that `group` points to; returns
// 0 when the group is empty.
// FUNCTION: 0x407410
int SquadTimer::GetAveragePosition(Vec3* out)
{
    Group* g = group;
    int n = g->units.size();
    if (n == 0)
        return 0;
    int x = 0, y = 0, z = 0;
    for (std::vector<Unit*>::iterator it = g->units.begin(); it != g->units.end(); ++it) {
        x += (*it)->pos.xf.whole;
        y += (*it)->pos.yf.whole;
        z += (*it)->pos.zf.whole;
    }
    // Built as a temporary and assigned whole: per-field stores interleave with the divisions.
    *out = MakeVec3(x / n << 16, y / n << 16, z / n << 16);
    return 1;
}

// FUNCTION: 0x4074a0
int SquadTimer::CountGroupUnitsInRadius(Vec3* pos, int radius)
{
    int count = 0;
    int squared = radius * radius;
    Unit* u = group->player->unitsBegin;
    Unit* last = group->player->unitsEnd;
    for (; u <= last; ++u) {
        if (u->group == group->id) {
            int dz = pos->z - u->pos.zf.value;
            int dx = pos->x - u->pos.xf.value;
            int distance = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
            if (distance <= squared)
                ++count;
        }
    }
    return count;
}

// Method of the SquadTimer family, called by AssaultTimer::OnTimer
// (0x4077e0) with its kind and limit fields. Balances this object's group
// against the owner's member of the given kind: takes one unit from it when
// this group is empty, sends the unit farthest from the group's centre to that
// kind while its squared distance is at least limit * group size, then takes
// over every unit of the other group that lies closer than that.
// SetUnitSquad(unit, id) moves a unit to a group.
// FUNCTION: 0x407560
void AssaultTimer::RebalanceAssaultGroupByCentroid(int kind, int limit)
{
    SquadTimer* other = owner->timers[kind];
    if (other == this)
        return;
    if (group->units.empty()) {
        if (other->group->units.empty())
            return;
        SetUnitSquad(other->group->units[0], group->id);
    }
    int sx = 0, sz = 0;
    std::vector<Unit*>::iterator it;
    for (it = group->units.begin(); it != group->units.end(); ++it) {
        sx += (*it)->pos.xWhole;
        sz += (*it)->pos.zWhole;
    }
    int cx, cz;
    while (1) {
        Group* g = group;
        cx = sx / (int)g->units.size();
        cz = sz / (int)g->units.size();
        if (g->units.size() == 1)
            break;
        int maxd = 0;
        std::vector<Unit*>::iterator best = g->units.end();
        for (it = g->units.begin(); it != g->units.end(); ++it) {
            int dx = (*it)->pos.xWhole - cx;
            int dz = (*it)->pos.zWhole - cz;
            int d = dx * dx + dz * dz;
            if (d > maxd) {
                maxd = d;
                best = it;
            }
        }
        if (maxd < limit * (int)g->units.size())
            break;
        SetUnitSquad(*best, kind);
        sx -= (*best)->pos.xWhole;
        sz -= (*best)->pos.zWhole;
    }
    std::vector<Unit*> list;
    for (it = other->group->units.begin(); it != other->group->units.end(); ++it) {
        int dx = (*it)->pos.xWhole - cx;
        int dz = (*it)->pos.zWhole - cz;
        // Own statement: inside the comparison the size() ternary goes first.
        int d = dx * dx + dz * dz;
        if (d < limit * (int)group->units.size())
            list.push_back(*it);
    }
    for (it = list.begin(); it != list.end(); ++it)
        SetUnitSquad(*it, group->id);
}

// SquadTimer::TryGetBaseBuildOrScoutCentroid, inlined: the position of the first of three of
// the owner's squads that has one.
static inline int Rally(SquadManager*& owner, Vec3* pos)
{
    if (owner->timers[5]->GetAveragePosition(pos))
        return 1;
    if (owner->timers[1]->GetAveragePosition(pos))
        return 1;
    if (owner->timers[4]->GetAveragePosition(pos))
        return 1;
    return 0;
}

// FUNCTION: 0x4077e0
void AssaultTimer::OnTimer()
{
    Vec3 pos;
    Vec3 retreat;
    next = g_game->ticks + 300;
    RebalanceAssaultGroupByCentroid(kind, limit);
    if (!group->units.empty()) {
        if ((int)group->units.size() <= minimum || (!attacking && (int)group->units.size() < maximum)) {
            if (Rally(owner, &retreat)) {
                attacking = 0;
                group->Send(2, 0, 0, &retreat, 160, 0);
                return;
            }
        }
        attacking = 1;
        GetAveragePosition(&pos);
        Unit* target = owner->FindNearestEnemyUnit(pos);
        if (target)
            group->Send(3, 0, target, 0, 0, 0);
    }
}

// Constructor of AssaultTimer (vtable 0x4fc988), derived from SquadTimer
// (the family is listed at 0x407350). The owner creates two of them, with
// (3, 20000) and (7, 50000).
// FUNCTION: 0x407930
// FUNCTION: 0x407980 ??_GAssaultTimer@@UAEPAXI@Z
AssaultTimer::AssaultTimer(SquadManager* p, Group* q, int a, int b)
    : SquadTimer(p, q), limit(b), kind(a)
{
    // limit and kind stay in the initialiser list, the rest in the body.
    attacking = 0;
    maximum = 6;
    minimum = 3;
}

// Constructor of EscortTimer (vtable 0x4fc990), derived from SquadTimer
// (the family is listed at 0x407350). The owner creates two of them, with 2
// and 6.
// FUNCTION: 0x4079a0
// FUNCTION: 0x4079d0 ??_GEscortTimer@@UAEPAXI@Z
EscortTimer::EscortTimer(SquadManager* p, Group* q, int a)
    : SquadTimer(p, q), other(a)
{
}

// Slot 0: every 150 ticks, when both this squad and the owner's squad
// `other` have units, sends this squad to the other's average position.
// FUNCTION: 0x4079f0
void EscortTimer::OnTimer()
{
    next = g_game->ticks + 150;
    SquadTimer* o = owner->timers[other];
    if (!group->units.empty()
        && !o->group->units.empty()) {
        Vec3 pos;
        if (o->GetAveragePosition(&pos)) {
            Group* g = group;
            OrderSquad(g->player, g->id, 2, 0, 0, &pos, 0, 0);
        }
    }
}

// Constructor of SquadScoutTimer (vtable 0x4fc998), derived from SquadTimer
// (the family is listed at 0x407350) without new fields.
// FUNCTION: 0x407a90
// FUNCTION: 0x407ac0 ??_GSquadScoutTimer@@UAEPAXI@Z
SquadScoutTimer::SquadScoutTimer(SquadManager* p, Group* q)
    : SquadTimer(p, q)
{
}

struct FixedParts_00407ae0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_00407ae0 {
    int value;
    FixedParts_00407ae0 parts;
};

static inline Fixed_00407ae0 MakeFixed(int i)
{
    Fixed_00407ae0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

// Copies the 12-byte position at +0x35 of g_playerAI[index] into *out.
void __stdcall GetBasePosition(int index, Vec3* out);

static inline Vec3 GetRallyPoint(int index)
{
    Vec3 p;
    GetBasePosition(index, &p);
    return p;
}

// Slot 0 of SquadScoutTimer (vtable 0x4fc998), derived from SquadTimer
// (the family is listed at 0x407350).
// Sets `next` to 30..929 ticks from now. A group of fewer than 5 units
// either moves (mode 9) to the unit nearest its average position, or, when
// GetBasePosition gives a rally point for the player, is sent to 2..3 random
// points around it (mode 2 first, then mode 9). A bigger group is sent to a
// random point on the map edge. The unit FindNearestEnemyUnit returns is used
// without a null check.
// FUNCTION: 0x407ae0
void SquadScoutTimer::OnTimer()
{
    Vec3 dest;
    // Computed first: in one expression with next it folds into a lea.
    int delay = RandomInt(900) + 30;
    next = g_game->ticks + delay;
    if ((int)group->units.size() < 5) {
        // Read through an inline returning by value: pos must not be the local
        // whose address goes to GetBasePosition.
        Vec3 pos = GetRallyPoint(player);
        if ((pos.xWhole | pos.zWhole) == 0) {
            GetAveragePosition(&dest);
            Unit* target = owner->FindNearestEnemyUnit(dest);
            group->Send(9, 1, 0, &target->pos, 0, 0);
        } else {
            int n = RandomInt(2) + 2;
            int w = g_game->baseX / 8, h = g_game->baseY / 8;
            for (int i = 0; i < n; i++) {
                // dx and dz stay named: the sum then lands in pos's register.
                int dx = (RandomInt(w) - w / 2) << 16;
                dest.x = pos.x + dx;
                dest.y = pos.y;
                int dz = (RandomInt(h) - h / 2) << 16;
                dest.z = pos.z + dz;
                if (i == 0)
                    group->Send(2, 0, 0, &dest, 0, 0);
                else
                    group->Send(9, 1, 0, &dest, 0, 0);
            }
        }
    } else {
        dest.y = 0;
        if (RandomInt(2)) {
            dest.x = RandomInt(g_game->baseX) << 16;
            dest.z = (RandomInt(2) ? MakeFixed(0) : MakeFixed(g_game->baseY - 1)).value;
        } else {
            dest.x = (RandomInt(2) ? MakeFixed(0) : MakeFixed(g_game->baseX - 1)).value;
            dest.z = RandomInt(g_game->baseY) << 16;
        }
        group->Send(9, 0, 0, &dest, 0, 0);
    }
}

// The constructor. Its vtable reference makes the compiler emit the scalar
// deleting destructor here too: the destructor is trivial, so only the inlined
// base destructor's store of 0x4fc980 is left.
// FUNCTION: 0x4085d0
// FUNCTION: 0x408600 ??_GScoutTimer@@UAEPAXI@Z
ScoutTimer::ScoutTimer(SquadManager* p, Group* q)
    : SquadTimer(p, q)
{
}


// FUNCTION: 0x408670
void __stdcall UpdateConverter(Unit* unit)
{
    Economy* economy = unit->economy;
    if (economy->cost + economy->cost < economy->energy) {
        if (GetNetEnergy(economy) > Zero_004fc968 && RandomInt(5) != 0) {
            unit->SetStateBits(1, 1);
        }
    } else {
        unit->SetStateBits(1, 0);
    }
}

// Slot 0: every 30 ticks, switches each idle converter on when energy is
// plentiful and off otherwise, and queues a build order for each idle
// builder.
// FUNCTION: 0x4086d0
void BuildTimer::OnTimer()
{
    next = g_game->ticks + 30;
    for (std::vector<Unit*>::iterator it = group->units.begin(); it != group->units.end(); ++it) {
        Unit* u = *it;
        if ((u->flags & 0x20000000) && (u->flags & 0x10000000) && !(u->flags & 0x4000)) {
            if (u->def->field_22d) {
                if (u->economy->cost + u->economy->cost < u->economy->energy) {
                    if (GetNetEnergy(u->economy) > Zero_004fc968 && RandomInt(5))
                        u->SetStateBits(1, 1);
                } else
                    u->SetStateBits(1, 0);
            } else if (u->def->field_152 && !u->orders) {
                unsigned short id = ChooseBuildOption(player, u);
                if (id)
                    QueueBuildOrder((char*)&g_game->defs[id].description[0], u, 1);
            }
        }
    }
}

// Constructor of BuildTimer (vtable 0x4fc9b0), derived from SquadTimer
// (the family is listed at 0x407350) without new fields.
// FUNCTION: 0x4087e0
// FUNCTION: 0x408810 ??_GBuildTimer@@UAEPAXI@Z
BuildTimer::BuildTimer(SquadManager* p, Group* q)
    : SquadTimer(p, q)
{
}

// FUNCTION: 0x408830
void SquadManager::AssignSquads()
{
    for (Unit* u = player->unitsBegin; u <= player->unitsEnd; ++u) {
        if(u->flags&0x20) {
            if(u->def->special) u->flags=(u->flags&~0x80000)|0x40000;
            else u->flags=(u->flags&~0x40000)|0x80000;
            u->flags=(u->flags&~0x100000)|0x200000;
            if(!u->group) {
                if(u->flags&0x20000000) {
                    if(u->flags&0x80000000) SetUnitSquad(u,5);
                    else SetUnitSquad(u,1);
                } else if(u->def->builder) SetUnitSquad(u,4);
                else if(u->def->flying) SetUnitSquad(u,8);
                else if(u->def->field_1c0>0) SetUnitSquad(u,7);
                else if(u->flags&0x80000000) SetUnitSquad(u,3);
            }
        }
    }
}

// FUNCTION: 0x408920
void __stdcall RetargetWeapon(Unit* unit, unsigned int weapon)
{
    if (unit->weapons[weapon].weapon->flag30) {
        int* p = FindTargetableProjectile(unit, weapon);
        if (p)
            SetWeaponTargetPos(unit, p + 1, weapon);
        else
            ClearWeaponTarget(unit, weapon);
    } else if ((unit->flags & 0x300000) == 0x200000) {
        int r = FindWeaponTarget(unit, weapon, 1);
        if (r)
            SetWeaponTargetUnit(unit, r, weapon);
        else
            ClearWeaponTarget(unit, weapon);
    }
}

// Walks the player's units round-robin through the cursor at +0x39, a slice of
// them per call, and for each finished unit that has a weapon without a valid
// target, retargets that weapon with RetargetWeapon.
// FUNCTION: 0x4089a0
void SquadManager::RetargetWeapons(int force)
{
    for (int i = 0; i <= g_game->maxUnits / 30; i++) {
        if (cursor && cursor != player->unitsEnd)
            cursor++;
        else
            cursor = player->unitsBegin;
        if (cursor->category != 0 && cursor->progress == Zero_004fc968 && (cursor->flags & 0x80000000)
            && (cursor->flags & 0x300000) == 0x200000) {
            // The counter stays a byte, and the flag8 test keeps its (unsigned char) cast.
            for (unsigned char w = 0; w < 3; w++) {
                if ((cursor->weapons[w].flags & 2) && (cursor->weapons[w].flags & 0x10)
                    && !(unsigned char)cursor->weapons[w].weapon->flag8
                    && (force || !cursor->weapons[w].weapon->flag26)) {
                    Unit* target = GetWeaponTargetUnit(cursor, w);
                    if (target && (player->allied[target->owner->index]
                        || Contains(cursor->def->weaponCategories[w], target->category)
                        || (cursor->weapons[w].weapon->flag7 && (target->activateFlags & 0x10))))
                        target = 0;
                    if (!target)
                        RetargetWeapon(cursor, w);
                }
            }
        }
    }
}

// FUNCTION: 0x408bf0
void SquadManager::TickTimers()
{
    if (--countdown <= 0) {
        countdown = 30;
        AssignSquads();
    }
    for (int i = 0; i < 10; i++) {
        if (timers[i] != 0 && timers[i]->next <= g_game->ticks) {
            timers[i]->OnTimer();
        }
    }
}

// FUNCTION: 0x408c40
void SquadManager::TickIfActive()
{
    if (player->active != 0 && player->type == 2) {
        if (--countdown <= 0) {
            countdown = 30;
            AssignSquads();
        }
        for (int i = 0; i < 10; i++) {
            if (timers[i] != 0 && timers[i]->next <= g_game->ticks) {
                timers[i]->OnTimer();
            }
        }
        RetargetWeapons(1);
        return;
    }
    RetargetWeapons(0);
}

// Constructor of SquadManager, the owner of the SquadTimer family (listed
// at 0x407350): one per player, it creates a timer object for nine of its
// ten slots, each given the player's unit group of the same index.
// The family constructors stay defined in this file before this one, bodies
// included: the inlining budget depends on it.
// FUNCTION: 0x408cb0
SquadManager::SquadManager(Player* p)
{
    player = p;
    index = p->index;
    field_9 = 0;
    countdown = 30;
    cursor = 0;
    nextAction = 0;
    for (int i = 0; i < 10; i++)
        timers[i] = 0;
    timers[1] = new BuildTimer(this, &player->groups[1]);
    timers[4] = new ScoutTimer(this, &player->groups[4]);
    timers[5] = new SquadTimer(this, &player->groups[5]);
    timers[2] = new AssaultTimer(this, &player->groups[2], 3, 20000);
    timers[3] = new EscortTimer(this, &player->groups[3], 2);
    timers[6] = new AssaultTimer(this, &player->groups[6], 7, 50000);
    timers[7] = new EscortTimer(this, &player->groups[7], 6);
    timers[8] = new SquadScoutTimer(this, &player->groups[8]);
    timers[9] = new SpatialTimer(this, &player->groups[9]);
}

// FUNCTION: 0x408f10
void SquadManager::DeleteTimers()
{
    for (int i = 0; i < 10; i++) {
        if (timers[i] != 0) {
            delete timers[i];
        }
    }
}

// Constructor of a player's AI state object (g_playerAI[player], built by
// 0x40b320; 0x40b390 destroys it). Its out-of-line STL callees are
// std::vector<Unit*>::vector(const allocator&) (0x40c510),
// std::vector<short>::size() (0x40d000) and
// std::vector<Elem_0040cfb0>::size() (0x40cc80), among others.
// FUNCTION: 0x409160
PlayerAI::PlayerAI(unsigned char p)
    : owner(&g_game->players[p]), index(p),
      center(g_game->width / 2, g_game->height / 2), builders(0)
{
    lastTick = 0;
    field_109 = 0;
    InitPlacementGrid();
    int n = g_game->count;
    weights.resize(n, 0);
    counts.resize(n, 0);
    // Fill values stay in block scopes so they share the dead parameter slot.
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
    InitUnitTables();
}

// Resets the per-unit-type tables: weights start at 40 for immobile types
// plus 20 for the flagged ones.
// FUNCTION: 0x409470
void PlayerAI::InitUnitTables()
{
    int n = g_game->count;
    for (int i = 0; i < n; ++i) {
        UnitDef* def = &g_game->defs[i];
        weights[i] = 0;
        if (!def->field_22f)
            weights[i] += 40;
        if (def->field_156)
            weights[i] += 20;
        counts[i] = 0;
        vec_ad[i].value = 100;
        vec_bd[i].unknown_0 = 0;
        values[i].unknown_0 = -1;
        locked[i].unknown_0 = 0;
    }
}

// The weapon-score helpers of the type table, no callers in the exe: both were
// inlined into 0x409730's loop.
#define MIN(a, b) (((a) > (b)) ? (b) : (a))

// Sums a score over the three sub-objects at +0x1ee: each live sub-object
// contributes its +0xdc value / 100 plus its +0xd4 value / 40 plus 5. The
// seed is 1, or 0xb when bit 4 of the flags at +0x245 is set. The result is
// clamped to [-100, 100].
// FUNCTION: 0x409520
int __stdcall RateWeapons(UnitDef* p)
{
    int result = 1;
    if (p->flag4)
        result = 0xb;
    Sub_00409520** pp = p->arr;
    for (int i = 3; i != 0; i--) {
        Sub_00409520* s = *pp;
        if (s->field_10a != 0)
            result = result + s->field_d4 / 40 + s->field_dc / 100 + 5;
        pp++;
    }
    if (MIN(result, 100) < -100)
        return -100;
    return MIN(result, 100);
}

// FUNCTION: 0x4095d0
int __stdcall RateUnitType(UnitDef* p)
{
    int result = 1;
    if (p->field_1ce != 0.0f)
        result = 0xb;
    if (p->field_22d != 0)
        result += 10;
    if (GetEnergyUse(p) < 0.0f)
        result += 10;
    result = (int)((int)(result - p->field_18a * -0.01f) - p->field_186 * -0.002f);
    int extra = 1;
    if (p->flag4)
        extra = 0xb;
    Sub_00409520** pp = p->arr;
    for (int i = 3; i != 0; i--) {
        Sub_00409520* s = *pp;
        if (s->field_10a != 0)
            extra = extra + s->field_d4 / 40 + s->field_dc / 100 + 5;
        pp++;
    }
    result += (signed char)((MIN(extra, 100) < -100) ? -100 : MIN(extra, 100));
    if (MIN(result, 100) < -100)
        return -100;
    return MIN(result, 100);
}

// Picks a build cell near a world position: every candidate in `list` (a
// vector of cells with a score) within `range` cells goes into a max-heap
// keyed on minus the squared distance, then the cells are popped nearest
// first and tried with CanBuildAt. The best-scoring cell (GetBuildSiteMetal)
// wins; once one is found, candidates more than 160 beyond the first hit's
// squared distance stop the search.
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

// The original calls it out of line from 0x406ee0; the joined file would
// inline the body into it.
#pragma auto_inline(off)
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
#pragma auto_inline(on)

// FUNCTION: 0x409f80
void __stdcall ParseDownloadableAiWeightScripts(int player)
{
    PlayerAI* p = g_playerAI[player];
    EnableAICommands();
    for (unsigned short i = 1; i < g_game->count; i++) {
        UnitDef* def = &g_game->defs[i];
        if (def->field_5) {
            if (p->vec_bd[i].unknown_0 != 1) {
                int len = strlen(def->command);
                if (len != 0) {
                    CommandArgs vars;
                    vars.InitArgs();
                    ExecuteCommandText(def->command, len, &vars, -1);
                }
            }
        }
    }
}

// FUNCTION: 0x40a040
void __stdcall ReparseAiWeightScriptsIfLimitNotSticky(int player)
{
    PlayerAI* p = g_playerAI[player];
    EnableAICommands();
    for (unsigned short i = 1; i < g_game->count; i++) {
        UnitDef* def = &g_game->defs[i];
        if (def->field_5) {
            if (p->locked[i].unknown_0 != 1) {
                int len = strlen(def->command);
                if (len != 0) {
                    CommandArgs vars;
                    vars.InitArgs();
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

// Initialises the two placement-grid blocks of the object: spacing0/offset0
// with margin0 (+0xf1) and spacing1/offset1 with margin1 (+0xfd). Each
// spacing component becomes its margin plus 8 plus a random amount, and each
// offset component a random offset centred on the spacing
// (rand(spacing) - spacing/2).
// FUNCTION: 0x40a150
void PlayerAI::InitPlacementGrid()
{
    margin0 = 3;
    spacing0.x = RandomInt(10) + margin0 + 8;
    spacing0.y = RandomInt(3) + margin0 + 8;
    offset0.x = RandomInt(spacing0.x) - spacing0.x / 2;
    offset0.y = RandomInt(spacing0.y) - spacing0.y / 2;
    margin1 = 6;
    spacing1.x = RandomInt(0x14) + margin1 + 8;
    spacing1.y = RandomInt(3) + margin1 + 8;
    offset1.x = RandomInt(spacing1.x) - spacing1.x / 2;
    offset1.y = RandomInt(spacing1.y) - spacing1.y / 2;
}

// FUNCTION: 0x40a260
bool PlayerAI::FindCellNearFeatures(UnitDef* type, Vec3* pos, std::vector<Elem_0040cc40>* list, int range, Point16* out)
{
    if (list->empty())
        return false;
    std::vector<Elem_0040cc40> heap;
    heap.reserve(list->size());
    int rangeSq = range * range;
    Point16 center = WorldToCell(*pos, type->origin);
    for (std::vector<Elem_0040cc40>::iterator p = list->begin(); p != list->end(); p++) {
        int d = DistSq(p->pos, center);
        if (d <= rangeSq) {
            heap.push_back(*p);
            heap.back().key = -d;
        }
    }
    // Written out, not an inline make_heap helper: the inline count sets the budget.
    if (2 <= heap.end() - heap.begin())
        MakeHeap(heap.begin(), heap.end(), (int*)0, (Elem_0040cc40*)0);
    int limit = -1;
    int best = 0;
    Point16 result;
    while (!heap.empty()) {
        Point16 cell = heap.front().pos;
        cell.x -= (type->origin.x - 3) / 2;
        cell.y -= (type->origin.y - 3) / 2;
        int d = DistSq(cell, center);
        if (limit >= 0 && d > limit + 160)
            break;
        if (CanBuildAt(type, cell, 0, 0) && GetBuildSiteMetal() > best) {
            result = cell;
            best = GetBuildSiteMetal();
            if (limit == -1)
                limit = d;
        }
        PopHeap(heap.begin(), heap.end());
        heap.pop_back();
    }
    if (best == 0)
        return false;
    if (out)
        *out = result;
    return true;
}

// Picks a random build cell near a world position: up to 30 tries of a
// random direction and distance (within `range` cells) from `pos`, snapped
// to the class's placement grid (spacing, offset and a random jitter reduced
// by a margin; the second grid is used for types whose field_1c0 is
// non-negative). A cell is accepted when CanPlaceUnitFootprint allows the type there
// and the score GetBuildSiteMetal is at most the type's footprint area times
// twice net->surfaceMetal.
// FUNCTION: 0x40a5b0
Elem_0040cc40::Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key)
{
}

// FUNCTION: 0x40a5d0
bool PlayerAI::FindRandomPlacementCell(UnitDef* type, Vec3* pos, int range, Point16* out)
{
    int areaY = type->origin.y;
    int areaX = type->origin.x;
    int threshold = g_game->net->surfaceMetal * areaY * areaX * 2;
    Point16 spacing = type->field_1c0 < 0 ? spacing0 : spacing1;
    Point16 offset = type->field_1c0 < 0 ? offset0 : offset1;
    int margin = type->field_1c0 < 0 ? margin0 : margin1;
    for (int i = 0; i < 30; i++) {
        int dist = RandomInt(range) << 16;
        int angle = RandomInt(0x10000);
        Vec3 v = Direction(angle, dist) + *pos;
        Point16 cell = WorldToCell(v, type->origin);
        cell.x = cell.x / spacing.x * spacing.x + offset.x + RandomInt(spacing.x - margin - type->origin.x);
        cell.y = cell.y / spacing.y * spacing.y + offset.y + RandomInt(spacing.y - margin - type->origin.y);
        if (CanPlaceUnitFootprint(type, 0, cell, 1) && GetBuildSiteMetal() <= threshold) {
            if (out)
                *out = cell;
            return true;
        }
    }
    return false;
}

// Rebuilds the list of candidate cells: clears the vector at +0x4d, then
// walks every map cell and adds (x, y, feature value) for each cell whose
// feature (index below 0xfffb) has a non-zero value at +0xf0 and bit 9 of
// its flags word set. 0x40a260 later sorts these by distance.
// The feature's flags are the 16-bit word at +0xfe, tested with 0x200, as in
// 0x422040 (the same test) and 0x423160.
// FUNCTION: 0x40a7b0
void PlayerAI::BuildFeatureCells()
{
    cells.clear();
    int w = g_game->width;
    for (int y = 0; y < g_game->height; y++) {
        Cell* row = GetMapCell(0, y);
        for (int x = 0; x < w; x++) {
            if (row[x].feature < 0xfffb) {
                Feature* f = &g_game->features[row[x].feature];
                // Keep flags a 16-bit word tested with 0x200, not a byte at +0xff.
                if (f->value != 0.0f && (f->flags & 0x200))
                    cells.push_back(Elem_0040cc40(x, y, f->value));
            }
        }
    }
}

// Every 30 ticks refreshes the unit lists, and now and then the base weights.
// FUNCTION: 0x40ad20
void PlayerAI::UpdateEveryThirtyTicks()
{
    if (g_game->ticks >= lastTick + 0x1e) {
        this->RefreshUnitLists();
        lastTick = g_game->ticks;
        if (RandomInt(0x1e) == 0) {
            this->ComputeBaseWeights();
        }
    }
}
// FUNCTION: 0x40ad70
void __stdcall ProbeUnitDefEnergyRate(int unused1, int unused2, int param_3)
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
    std::vector<Unit*>& visible=g_playerAI[player]->visible.units;
    std::vector<Unit*>::iterator it;
    for(it=visible.begin();it!=visible.end();++it) {
        Unit* unit=*it;
        int dz=pos->z-unit->pos.z;
        int dx=pos->x-unit->pos.x;
        int d=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
        if(d<=radius2 && unit->Ready()) out->push_back(unit);
    }
    PlayerAI* owner=g_playerAI[player];
    if(owner->hasSpecial && out->empty()) {
        for(it=owner->known.units.begin();it!=owner->known.units.end();++it) {
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
    std::vector<Unit*>& units = t->visible.units;
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
    if(useRange) GetVisibleEnemiesInRadius(unit->ownerIndex,(const Vec*)&unit->pos,GetWeaponRange(unit,weapon),0,&candidates);
    else GetVisibleEnemiesInRadius(unit->ownerIndex,(const Vec*)&unit->pos,unit->def->range,0,&candidates);
    for(int count=0;count<50;++count) {
        if(candidates.empty()) break;
        std::vector<Unit*>::iterator it=candidates.begin()+RandomInt(candidates.size());
        Unit* target=*it;
        ((Class_004800c0*)&candidates)->EraseSwapBack(it);
        if((target->flags&0x10000000) && !(target->flags&0x4000) &&
           ((target->def->flags&0x8000) || ai || (g_game->matchFlags&4)) &&
           ((unit->def->flags&0x10000000) || WeaponCanReachUnit(unit,target,weapon)) &&
           (useRange || !unit->def->exclude.Test(target->id)) &&
           (!(unit->weapons[weapon].weapon->flags8&0x80) || !(target->status&0x10))) {
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
void __stdcall GetMirroredBasePosition(int index, int unused, Struct_0040bab0* out)
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
    if(!IsCountBelowAiLimit(player,type,owner->counts[type])) return 0;
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
        if (strcmp(unit->def->side, g_game->defs[chosen].side) != 0)
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
    if (ai->field_109<maximum) ai->field_109+=160;
    Vec3 pos=MoveTowards(from,(const Vec3*)&ai->centre,ai->field_109<<16);
    Point16 cell;
    int result;
    if (type->value!=0.0f && g_game->net->threshold<RandomInt(255))
        result=ai->FindCellNearFeatures(type,&pos,&ai->cells,ai->field_109*4,&cell);
    else result=ai->FindRandomPlacementCell(type,&pos,ai->field_109,&cell);
    if (result) {
        CellToWorld(out,cell,type->origin);
        ai->field_109=0;
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
int __stdcall HasSpecialUnit(int param_1)
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
