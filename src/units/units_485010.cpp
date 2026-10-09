// Decompiled by Opus, space-bunny-free, Sonnet 5.5, deepseek-v4.1-flash, GPT-6.1-sol, claude-opus-5-5, mimo-v2.6-pro, GPT-6, DeepSeek V4.1 Flash, deepseek-v4.1, Space Bunny Free, Haiku, Claude Opus 5.5, Claude Sonnet 5.5, claude-sonnet-5-5 and Sonnet. Names are provisional.
// The unit code: map cell and height helpers, the unit memory pool, creating
// a unit (InitUnitFromType, InitUnitScript, InitUnit, CreateUnit and the
// network CreateUnitFromPacket) and killing units (KillUnit, ApplyUnitDeath).

#include <stdio.h>
// Included only for its declarations: the symbol counter of GetCellMeanHeight needs them.
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

class Mission {
public:
    int GetGameType();
};

class MissionConditions;
class CobScript;
class UnitMotion;
class UnitResources;
struct ObjectState_00485d40;
struct Object3do;                      // object definition, only passed on

#pragma pack(push, 1)
struct Cell {
    unsigned short unit;               // +0x0
    char unknown_2[0x4 - 0x2];
    unsigned char height;              // +0x4
    unsigned char high;                // +0x5
    unsigned char low;                 // +0x6
    char metal;
    unsigned short feature;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct ShortPair_485a40 {
    short x;                           // +0x0
    short y;                           // +0x2
};

struct Entry_486360 {
    char unknown_0[0xf4];
    unsigned short next;               // +0xf4
    char unknown_f6[0x100 - 0xf6];
};

struct PlayerInfo {
    char unknown_0[0x95];
    unsigned char side;                // +0x95
    char unknown_96[5];
    unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1,
        b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1, b14 : 1, b15 : 1;   // +0x9b
};

struct Unit;

// Unused here: these forward declarations take the symbol ids that keep 0x4854a0 matching (docs/c2-regalloc.md).
struct Sound;
#include "../network/player.h"

struct Name_004864b0 {
    char name[0x232];                  // +0x0
};

struct Data_00485d40 {                 // the definition data at type+0x18e
    char unknown_0[8];
};

union TypeFlags {                      // the type's word at +0x241
    unsigned int all;
    struct {
        unsigned int movOrder : 2;     // bits 0-1
        unsigned int fireOrder : 2;    // bits 2-3
        unsigned int canAttack : 1;    // bit 4
        unsigned int b5 : 1;
        unsigned int b6 : 1;
        unsigned int b7 : 1;           // bit 7
        unsigned int b8 : 1;
        unsigned int b9 : 1;           // bit 9
        unsigned int b10 : 1;
        unsigned int b11 : 1;
        unsigned int b12 : 1;
        unsigned int b13 : 1;
        unsigned int b14 : 1;
        unsigned int b15 : 1;
        unsigned int hi : 16;          // bits 16-31
    } bits;
    struct {
        unsigned int lo : 18;
        unsigned int bit18 : 1;
        unsigned int mid : 5;
        unsigned int bit24 : 1;
        unsigned int top : 7;
    };
};

struct UnitType {                      // 0x249 bytes
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x14a - 0x40];
    ShortPair_485a40 offset;           // +0x14a
    char unknown_14e[0x15a - 0x14e];
    int limit;                         // +0x15a
    char unknown_15e[0x16e - 0x15e];
    union {
        int modelMaxY;                 // +0x16e
        struct {
            short unknown_16e;
            short x170;                // +0x170
        };
    };
    char unknown_172[0x18a - 0x172];
    float x18a;                        // +0x18a
    Data_00485d40* data;               // +0x18e
    char unknown_192[0x1bc - 0x192];
    unsigned short field_1bc;          // +0x1bc
    char unknown_1be[0x1fa - 0x1be];
    union {
        unsigned int maxHealth;        // +0x1fa
        short hp;
    };
    char unknown_1fe[0x202 - 0x1fe];
    short x202;                        // +0x202
    char unknown_204[0x210 - 0x204];
    unsigned short field_210;          // +0x210
    char unknown_212[0x22e - 0x212];
    unsigned char buildMenuPageCount;  // +0x22e
    unsigned char mobile;              // +0x22f
    char unknown_230[0x241 - 0x230];
    TypeFlags flags;                   // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Fixed_00485070 {
    unsigned short frac;
    short whole;
};

struct Pos_00485070 {
    Fixed_00485070 x;                  // +0x0
    Fixed_00485070 y;                  // +0x4
    Fixed_00485070 z;                  // +0x8
};

struct Pos_00485a40 {
    int x, y, z;
};

struct FixedParts_004853b0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_004853b0 {
    int value;
    FixedParts_004853b0 parts;
};

struct Vec3_004853b0 {
    Fixed_004853b0 x;
    Fixed_004853b0 y;
    Fixed_004853b0 z;
};

struct WeaponAimCobCb {                // 0x1c bytes, vtable 0x4fd6f0
    void* vtable;                      // +0x0
    char unknown_4[0x1c - 0x4];
};

class SpatialBucket {
public:
    char unknown_0[6];
    void* head;
    void UnlinkUnit(void* node);
};

union Flags_485a40 {
    unsigned int all;
    struct {
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int b2 : 1;
        unsigned int b3 : 1;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int b6 : 1;
        unsigned int b7 : 1;
        unsigned int b8 : 1;
        unsigned int b9 : 1;
        unsigned int b10 : 1;
        unsigned int b11 : 1;
        unsigned int b12 : 1;
        unsigned int b13 : 1;
        unsigned int b14 : 1;
        unsigned int b15 : 1;
        unsigned int b16 : 1;
        unsigned int b17 : 1;
        unsigned int moveOrder : 2;    // bits 18-19
        unsigned int fireOrder : 2;    // bits 20-21
        unsigned int f22_23 : 2;       // bits 22-23
        unsigned int f24_25 : 2;       // bits 24-25
        unsigned int f26_27 : 2;       // bits 26-27
        unsigned int b28 : 1;
        unsigned int b29 : 1;
        unsigned int b30 : 1;
        unsigned int b31 : 1;
    } bits;
    struct {
        unsigned int mode : 2;         // bits 0-1
        unsigned int unknown_bits : 12;
        unsigned int bit14 : 1;
        unsigned int unknown_bits2 : 17;
    };
};

union Flags114_485a40 {
    unsigned int all;
    struct {
        unsigned int b0 : 1;
    } bits;
};

class UnitResources {
public:
    int unknown[6];
    float field_18;                    // +0x18
    int unknown_1c[5];
    Player* player;                    // +0x30
    void Reset(unsigned char playerIndex);
};

struct Unit {                          // 0x118 bytes
    UnitMotion* obj;                   // +0x0
    char unknown_4[0x8 - 0x4];
    WeaponAimCobCb sub_8;              // +0x8
    WeaponAimCobCb sub_24;             // +0x24
    WeaponAimCobCb sub_40;             // +0x40
    char unknown_5c[0x64 - 0x5c];
    short bank;                    // +0x64
    unsigned short heading;           // +0x66
    short pitch;                    // +0x68
    Pos_00485a40 pos;                  // +0x6a
    ShortPair_485a40 screen;           // +0x76
    short losCacheCellX;                    // +0x7a
    short losCacheCellZ;                    // +0x7c
    ShortPair_485a40 offset;           // +0x7e
    SpatialBucket* list;               // +0x82
    Unit* owner;                       // +0x86
    Unit* first;                       // +0x8a
    Unit* next;                        // +0x8e
    UnitType* type;                    // +0x92
    Player* player;                    // +0x96
    CobScript* script;                 // +0x9a
    ObjectState_00485d40* state;       // +0x9e
    char unknown_a2[0xa6 - 0xa2];
    unsigned short typeId;             // +0xa6
    unsigned short id;           // +0xa8
    short hoverBobPhase;                    // +0xaa
    unsigned int group;             // +0xac
    int workTime;                      // +0xb0
    char unknown_b4[0xb8 - 0xb4];
    short killCount;                    // +0xb8
    short netDirtyFlags;                    // +0xba
    UnitResources playerRef;           // +0xbc
    Unit* parent;                      // +0xf0
    unsigned char lastAttackerSlot;            // +0xf4
    unsigned char lastDamageType;            // +0xf5
    unsigned char healthPercent;            // +0xf6
    unsigned char prevHealthPercent;            // +0xf7
    unsigned char losSightFrameIdx;            // +0xf8
    unsigned char transportPiece;            // +0xf9
    unsigned char recentlyDamagedTimer;            // +0xfa
    int postTransferHoldoff;                      // +0xfb
    unsigned char playerIndex;            // +0xff
    int unfinishedInitZero;                     // +0x100
    float buildLeft;                   // +0x104
    short health;                   // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char activateFlags;           // +0x10e
    struct {
        unsigned char lo : 4;          // +0x10f
        unsigned char hi : 4;
    } cobStateFlags;
    Flags_485a40 flags;                // +0x110
    Flags114_485a40 zBufferFlag;         // +0x114
    void ReleaseWeapons(unsigned char index);
    void SetStateBits(int a, int b);
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14223 - 0x2a44];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    char unknown_1422b[0x14233 - 0x1422b];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    Entry_486360* features;            // +0x1426f
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char limit;               // +0x1427f
    char debugMode;
    unsigned short mapFlags;           // +0x14281
    char unknown_14283[0x14287 - 0x14283];
    Cell* cells;                       // +0x14287
    char unknown_1428b[0x1434f - 0x1428b];
    unsigned short unitsPerPlayer;     // +0x1434f
    unsigned short poolCount;          // +0x14351
    char unknown_14353[0x14357 - 0x14353];
    union {
        Unit* units;                   // +0x14357
        unsigned char* pool;
    };
    union {
        Unit* unitsEnd;                // +0x1435b
        unsigned char* poolEnd;        // the same bytes, from the pool's end
    };
    void* hotUnits;                    // +0x1435f
    void* hotRadar;                    // +0x14363
    char unknown_14367[0x1436f - 0x14367];
    unsigned short focusUnitId;        // +0x1436f
    char unknown_14371[0x14373 - 0x14371];
    unsigned int autoFollowFlags;      // +0x14373
    Object3do** definitions;           // +0x14377
    char unknown_1437b[0x1439b - 0x1437b];
    UnitType* unitTypes;               // +0x1439b
    char unknown_1439f[0x37ee6 - 0x1439f];
    unsigned short maxUnits;           // +0x37ee6
    char unknown_37ee8[0x37eee - 0x37ee8];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[4];
    int mode;                          // +0x37ef6
    char unknown_37efa[0x37f06 - 0x37efa];
    unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1,
        b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1, b14 : 1, b15 : 1;   // +0x37f06
    char unknown_37f08[0x37f5f - 0x37f08];
    Name_004864b0 names[8];            // +0x37f5f
    char unknown_390ef[0x391e9 - 0x390ef];
    Mission* mission;                  // +0x391e9
    MissionConditions* conditions;     // +0x391ed
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

struct Point16_00485010 {
    short x;
    short y;
};

static inline Cell* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// FUNCTION: 0x485010
int __stdcall GetCellHeight(Point16_00485010* p)
{
    Cell* cell = GetCell(p->x, p->y);
    if (cell)
        return cell->height;
    return 0;
}

// Returns the average of the two height bytes (+5, +6) of the map cell under a
// 16.16 fixed-point position, or -1 off the map.

static inline Cell* GetCellCachedWidth(int x, int y)
{
    if (x >= 0) {
        // Width read into its own local after the x >= 0 test: decides the register tie.
        int w = g_game->width;
        if (x < w && y >= 0 && y < g_game->height)
            return &g_game->cells[w * y + x];
    }
    return 0;
}

// FUNCTION: 0x485140
int __stdcall GetCellMeanHeight(Pos_00485070* p)
{
    // One statement for x and y: settles the order of the two height-byte loads.
    int x = p->x.whole / 16, y = p->z.whole / 16;
    Cell* cell = GetCellCachedWidth(x, y);
    if (cell)
        return (cell->low + cell->high) >> 1;
    return -1;
}

// Converts map cell (x, y) to a 16.16 fixed-point position (cell * 16 in x
// and z, the cell's height byte in y); leaves `out` unchanged off the map.
// The zeroing is an inlined memset (a zero register stored three times
// through a copy of the pointer).

// FUNCTION: 0x485330
void __stdcall GetCellPosition(int x, int y, Pos_00485070* out)
{
    Cell* cell = GetCell(x, y);
    if (cell) {
        memset(out, 0, sizeof(*out));
        out->x.whole = x << 4;
        out->y.whole = cell->height;
        out->z.whole = y << 4;
    }
}

// Clamps a 16.16 fixed-point position's x and z to the map
// ([0, baseX) and [0, baseY) in whole units).

// Built as a bitfield struct in a local: plain integer arithmetic folds (size - 1) << 16.
static inline Fixed_004853b0 MakeFixed(int i)
{
    Fixed_004853b0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

// FUNCTION: 0x4853b0
void __stdcall ClampPositionToMap(Vec3_004853b0* p)
{
    if (p->x.value < 0)
        p->x.value = 0;
    else if (p->x.value >= MakeFixed(g_game->baseX).value)
        p->x = MakeFixed(g_game->baseX - 1);
    if (p->z.value < 0)
        p->z.value = 0;
    else if (p->z.value >= MakeFixed(g_game->baseY).value)
        p->z = MakeFixed(g_game->baseY - 1);
}


// Copies one player's visibility bit into another player's bit on every cell.
// FUNCTION: 0x485420
void __stdcall ShareMapInfo(unsigned char from, unsigned char to)
{
    unsigned short fromBit = 1 << from;
    unsigned short toBit = 1 << to;
    int n = g_game->width * g_game->height / 4;
    for (int i = 0; i < n; i++) {
        unsigned short* p = &g_game->visibilityMask[i];
        if (*p & fromBit)
            *p |= toBit;
    }
}

// This translation unit stands in for <algorithm> with __stdcall
// instantiations, because the original was built with /Gz and its sort helpers
// are callee-clean (0x488810, 0x488920, 0x488960). The three helper templates
// carry their real global names (QuickSort/InsertShift/Partition) so
// the recursive and out-of-line calls at 0x485691/0x48574b/0x48576d/0x485778
// reference the names data/symbols.csv already has, while the body and
// one-level inline stay exactly as the std:: originals. The std headers are
// included for their __cdecl helper templates (copy_backward) and type traits.

#include <iterator>
#include <xutility>

#define _ALGORITHM_

template<class _RI, class _Ty, class _Pr> void __stdcall QuickSort(_RI _F, _RI _L, _Pr _P, _Ty *);
template<class _RI, class _Ty, class _Pr> _RI __stdcall Partition(_RI _F, _RI _L, _Ty _Piv, _Pr _P);
template<class _RI, class _Ty, class _Pr> void __stdcall InsertShift(_RI _L, _Ty _V, _Pr _P);

namespace std {
const int _CHUNK_SIZE = 7;
const int _SORT_MAX = 16;

template<class _Ty, class _Pr> inline _Ty __stdcall _Median(_Ty _X, _Ty _Y, _Ty _Z, _Pr _P)
    {if (_P(_X, _Y))
        return (_P(_Y, _Z) ? _Y : _P(_X, _Z) ? _Z : _X);
    else
        return (_P(_X, _Z) ? _X : _P(_Y, _Z) ? _Z : _Y); }

template<class _FI1, class _FI2> inline void __stdcall iter_swap(_FI1 _X, _FI2 _Y)
    {_Iter_swap(_X, _Y, _Val_type(_X)); }
template<class _FI1, class _FI2, class _Ty> inline void __stdcall _Iter_swap(_FI1 _X, _FI2 _Y, _Ty *)
    {_Ty _Tmp = *_X;
    *_X = *_Y, *_Y = _Tmp; }

template<class _RI, class _Pr> inline void __stdcall sort(_RI _F, _RI _L, _Pr _P)
    {_Sort_0(_F, _L, _P, _Val_type(_F)); }
template<class _RI, class _Ty, class _Pr> inline void __stdcall _Sort_0(_RI _F, _RI _L, _Pr _P, _Ty *)
    {if (_L - _F <= _SORT_MAX)
        _Insertion_sort(_F, _L, _P);
    else
        {QuickSort(_F, _L, _P, (_Ty *)0);
        _Insertion_sort(_F, _F + _SORT_MAX, _P);
        for (_F += _SORT_MAX; _F != _L; ++_F)
            InsertShift(_F, _Ty(*_F), _P); }}
template<class _RI, class _Pr> inline void __stdcall _Insertion_sort(_RI _F, _RI _L, _Pr _P)
    {_Insertion_sort_1(_F, _L, _P, _Val_type(_F)); }
template<class _RI, class _Ty, class _Pr> inline void __stdcall _Insertion_sort_1(_RI _F, _RI _L, _Pr _P, _Ty *)
    {if (_F != _L)
        for (_RI _M = _F; ++_M != _L; )
            {_Ty _V = *_M;
            if (!_P(_V, *_F))
                InsertShift(_M, _V, _P);
            else
                {copy_backward(_F, _M, _M + 1);
                *_F = _V; }}}
}

template<class _RI, class _Ty, class _Pr> void __stdcall InsertShift(_RI _L, _Ty _V, _Pr _P)
    {for (_RI _M = _L; _P(_V, *--_M); _L = _M)
        *_L = *_M;
    *_L = _V; }

template<class _RI, class _Ty, class _Pr> _RI __stdcall Partition(_RI _F, _RI _L, _Ty _Piv, _Pr _P)
    {for (; ; ++_F)
        {for (; _P(*_F, _Piv); ++_F)
            ;
        for (; _P(_Piv, *--_L); )
            ;
        if (_L <= _F)
            return (_F);
        std::iter_swap(_F, _L); }}

template<class _RI, class _Ty, class _Pr> void __stdcall QuickSort(_RI _F, _RI _L, _Pr _P, _Ty *)
    {for (; std::_SORT_MAX < _L - _F; )
        {_RI _M = Partition(_F, _L, std::_Median(_Ty(*_F),
            _Ty(*(_F + (_L - _F) / 2)), _Ty(*(_L - 1)), _P), _P);
        if (_L - _M <= _M - _F)
            QuickSort(_M, _L, _P, std::_Val_type(_F)), _L = _M;
        else
            QuickSort(_F, _M, _P, std::_Val_type(_F)), _F = _M; }}

#include <algorithm>

void* __cdecl GameAllocIgnoreTag(const char* name, unsigned int size);
int __stdcall ComparePlayers(Player* a, Player* b);

// FUNCTION: 0x4854a0
void __stdcall AllocateUnitMemory(void)
{
    g_game->focusUnitId = 0;
    g_game->autoFollowFlags &= 0xfffffffd;
    g_game->unitsPerPlayer = g_game->maxUnits;
    g_game->poolCount = (unsigned short)(g_game->maxUnits * 10 + 1);

    unsigned char* pool = g_game->pool = (unsigned char*)GameAllocIgnoreTag("UNIT MEMORY", g_game->poolCount * 0x118);
    memset(pool, 0, g_game->poolCount * 0x118);

    unsigned int ten = g_game->maxUnits * 10;
    g_game->hotUnits = GameAllocIgnoreTag("HOT UNITS", ten * 2);
    g_game->hotRadar = GameAllocIgnoreTag("HOT RADAR UNITS", ten * 10);
    g_game->poolEnd = g_game->pool + g_game->poolCount * 0x118 - 0x118;

    unsigned short n;
    for (n = 0; n < g_game->poolCount; n++) {
        *(unsigned short*)(pool + n * 0x118 + 0xa8) = n;
        *(UnitType**)(pool + n * 0x118 + 0x92) = g_game->unitTypes;
    }

    Player* v[10];
    int k;
    for (k = 0; k < 10; k++)
        v[k] = (Player*)((unsigned char*)g_game->players + k * 0x14b);

    std::sort(v, v + 10, ComparePlayers);

    pool[0xff] = 0xff;
    *(unsigned int*)(pool + 0x96) = 0;
    int i;
    for (i = 0; i < 10; i++) {
        Player* item = v[i];
        int c = g_game->maxUnits * i + 1;
        *(unsigned char**)((char*)item + 0x67) = pool + c * 0x118;
        *(unsigned char**)((char*)item + 0x6b) =
            *(unsigned char**)((char*)item + 0x67) + g_game->maxUnits * 0x118 - 0x118;
        *(unsigned short*)((char*)item + 0x6f) =
            *(unsigned short*)(*(unsigned char**)((char*)item + 0x67) + 0xa8);
        *(unsigned short*)((char*)item + 0x71) =
            *(unsigned short*)(*(unsigned char**)((char*)item + 0x6b) + 0xa8);
        // Item fields read back directly, no `slot` local: keeps the edx/esi roles.
        for (unsigned char* q = *(unsigned char**)((char*)item + 0x67);
             q <= *(unsigned char**)((char*)item + 0x6b); q += 0x118) {
            *(void**)(q + 0x96) = item;
            q[0xff] = *(unsigned char*)((char*)item + 0x146);
            *(unsigned int*)(q + 0xac) = 0xffffffffu;
        }
    }
}

// Ordering of two items: by the key at +4 when the mode object reports 3,
// otherwise by address.
// FUNCTION: 0x485940
int __stdcall ComparePlayers(Player* a, Player* b)
{
    if (g_game->mission->GetGameType() == 3)
        return a->id < b->id;
    return a < b;
}

void __stdcall KillUnit(Unit* unit, int param_2);
void __cdecl GameFreeThunk(void* p);

// FUNCTION: 0x485980
void FreeUnitMemory(void)
{
    Unit* u = g_game->units;
    Unit* end = g_game->unitsEnd;
    if (u != 0) {
        for (; u <= end; u = (Unit*)((char*)u + 0x118)) {
            if (u->typeId != 0)
                KillUnit(u, 8);
        }
    }
    if (g_game->hotRadar != 0)
        GameFreeThunk(g_game->hotRadar);
    g_game->hotRadar = 0;
    if (g_game->hotUnits != 0)
        GameFreeThunk(g_game->hotUnits);
    g_game->hotUnits = 0;
    if (g_game->units != 0)
        GameFreeThunk(g_game->units);
    g_game->units = 0;
}

void __stdcall ResetWeaponTarget(Unit* unit, int index);
void __stdcall SetUnitSquad(Unit* unit, int param_2);
int __stdcall RandomInt(int range);

// Sets up a unit from its unit type: flags, offsets, screen position, weapon
// targets and the squad. The screen Y projection uses the passed position Z.
// FUNCTION: 0x485a40
void __stdcall InitUnitFromType(Unit* unit, Pos_00485a40 pos, int param_5)
{
    unit->type = &g_game->unitTypes[(unsigned short)unit->typeId];
    unit->flags.bits.b28 = 1;
    unit->flags.bits.b29 = (unit->type->mobile == 0);
    unit->flags.bits.b14 = 0;
    unit->offset = unit->type->offset;
    unit->flags.bits.b31 = unit->type->flags.bits.hi;
    unit->zBufferFlag.bits.b0 = unit->type->flags.bits.b7;
    unit->flags.bits.b30 = unit->type->flags.bits.b9;

    if (param_5) {
        unit->buildLeft = 0;
        unit->health = unit->type->hp;
    } else {
        unit->buildLeft = 1.0f;
        unit->unfinishedInitZero = 0;
        unit->health = 0;
    }

    unit->cobStateFlags.lo = 0;
    unit->flags.bits.b0 = 1;
    unit->flags.bits.b1 = 0;
    unit->flags.bits.b2 = 0;
    unit->flags.bits.b3 = 0;
    unit->flags.bits.b4 = 0;
    unit->flags.bits.b5 = 1;
    unit->flags.bits.b10 = 0;
    unit->flags.bits.b11 = 0;
    unit->flags.bits.b16 = 1;
    unit->flags.bits.b17 = 0;
    unit->healthPercent = 0;
    unit->prevHealthPercent = 0;
    unit->activateFlags = 0;
    unit->workTime = 0;
    unit->pitch = 0;
    unit->pos = pos;

    ShortPair_485a40 off = unit->offset;
    ShortPair_485a40 screen;
    screen.x = (short)((pos.x - off.x * 0x80000 + 0x80000) >> 20);
    screen.y = (short)((pos.z - off.y * 0x80000 + 0x80000) >> 20);
    unit->screen = screen;

    unit->heading = (short)(RandomInt(unit->type->field_210)
                             + (0x8000 - unit->type->field_210 / 2));
    unit->bank = 0;
    unit->losCacheCellX = 0;
    unit->losCacheCellZ = 0;
    unit->recentlyDamagedTimer = 0;
    unit->postTransferHoldoff = 0;
    unit->flags.bits.b9 = (unit->player->index == g_game->playerIndex);
    unit->flags.bits.b8 = 0;

    for (int i = 0; i < 3; i++) {
        ResetWeaponTarget(unit, i);
        ((Unit*)unit)->ReleaseWeapons(i);
    }

    unit->netDirtyFlags = 0;
    unit->killCount = 0;
    unit->parent = 0;
    unit->lastAttackerSlot = 0xa;
    unit->flags.bits.moveOrder = unit->type->flags.bits.movOrder;
    unit->flags.bits.fireOrder = unit->type->flags.bits.fireOrder;
    unit->flags.bits.b11 = unit->type->flags.bits.canAttack;
    unit->flags.bits.f26_27 = 0;
    if (unit->type->buildMenuPageCount > 1) {
        unit->flags.bits.f22_23 = 3;
        unit->flags.bits.f24_25 = 0;
    } else {
        unit->flags.bits.f22_23 = 0;
        unit->flags.bits.f24_25 = 0;
    }

    unit->losSightFrameIdx = 0;
    unit->playerRef.Reset(unit->playerIndex);
    unit->transportPiece = 0xff;
    unit->hoverBobPhase = (short)RandomInt(0x10000);
    SetUnitSquad(unit, 0);
}

// Builds the piece-tree "Object State" block of a placed object, called from
// the three places that finish placing a unit. The block comes from
// CreatePlayerObjectState, which also reorders the entries to match the build list, when
// the definition reached through +0x92 has object data at +0x18e; a 0x544-byte
// variable block is allocated for the object first and told about that data.
// Without the data the plain CreateObjectState builds the block and the owner
// pointer at +0xc is filled in by hand. Both arms store the block at +0x9e and
// clear its flag at +0x10.
//
// Both calls take the definition object (g_game->definitions[id]) as their
// argument.


struct ObjectState_00485d40 {
    char unknown_0[8];
    int field_8;                       // +0x8
    void* field_c;                     // +0xc
    int field_10;                      // +0x10
};

struct Elem_4b0610 {
    int value;         // +0x0
    char pad[0xa0];    // pad to stride 0xa4
};

class CobScript {
public:
    int field_4;                   // +0x4
    int field_8;                   // +0x8
    char unknown_c[0x10 - 0xc];
    void* ptr10;                   // +0x10
    void* ptr14;                   // +0x14
    char unknown_18[0x1c - 0x18];
    Elem_4b0610 arr[8];            // +0x1c
    int field_53c;                 // +0x53c

    CobScript();

    virtual void SetPieceTranslation(int, int, int) = 0;  // slot 0
    virtual void SetPieceRotation(int, int, int) = 0;  // slot 1
    virtual void SetPieceVisible(int, int) = 0;       // slot 2
    virtual void SetPieceCached(int, int) = 0;        // slot 3
    virtual void SetPieceShaded(int, int) = 0;        // slot 4
    virtual int GetPieceTranslation(int, int) = 0;    // slot 5
    virtual int GetPieceRotation(int, int) = 0;       // slot 6
    virtual int IsPieceVisible(int);                  // slot 7
    virtual int IsPieceCached(int);                   // slot 8
    virtual int IsPieceShaded(int);                   // slot 9
    virtual void ExplodeLegacy(int, int, int);        // slot 10
    virtual void PlaySoundNoop(int);                  // slot 11
    virtual void EmitSfx(int, int);                   // slot 12
    virtual void ExplodePiece(int, unsigned int);     // slot 13
    virtual void AttachUnit(unsigned short, int, int); // slot 14
    virtual void DropUnit(unsigned short);            // slot 15
    virtual void SetUnitValue(int, int);              // slot 16
    virtual int GetUnitValue(int, int, int, int, int); // slot 17
    virtual int IsCarryingUnit(int);                  // slot 18
    virtual int GetTransporterId();                   // slot 19
    virtual ~CobScript();                             // slot 20

    void SetCob(Data_00485d40* data);
    void StartScript(const char* name, int a, int b);
    int QueryScript(char* name, int* a, int* b, int c, int d);
    int StartScriptWithArgs(char* name, void* a, int b, int c, int d, int e, int f, int g);
};

class UnitScript : public CobScript {
public:
    void* field_540;               // +0x540

    virtual void SetPieceTranslation(int, int, int);  // slot 0
    virtual void SetPieceRotation(int, int, int);     // slot 1
    virtual void SetPieceVisible(int, int);           // slot 2
    virtual void SetPieceCached(int, int);            // slot 3
    virtual void SetPieceShaded(int, int);            // slot 4
    virtual int GetPieceTranslation(int, int);        // slot 5
    virtual int GetPieceRotation(int, int);           // slot 6
    virtual int IsPieceVisible(int);                  // slot 7, 0x480e30
    virtual int IsPieceCached(int);                   // slot 8, 0x480e50
    virtual int IsPieceShaded(int);                   // slot 9, 0x480e70
    virtual void ExplodeLegacy(int, int, int);        // slot 10, 0x480e90
    virtual void PlaySoundNoop(int);                  // slot 11, 0x480ea0
    virtual void EmitSfx(int, int);                   // slot 12, 0x480eb0
    virtual void ExplodePiece(int, unsigned int);     // slot 13, 0x481140
    virtual void AttachUnit(unsigned short, int, int); // slot 14, 0x481340
    virtual void DropUnit(unsigned short);            // slot 15, 0x4813b0
    virtual void SetUnitValue(int, int);              // slot 16, 0x480b20
    virtual int GetUnitValue(int, int, int, int, int); // slot 17, 0x480770
    virtual int IsCarryingUnit(int);                  // slot 18, 0x481430
    virtual int GetTransporterId();                   // slot 19, 0x481470

    void SetObjectState(ObjectState_00485d40* state);
};

void* __cdecl operator new(size_t size);
ObjectState_00485d40* __stdcall CreatePlayerObjectState(Object3do* obj, Data_00485d40* data, int player);
ObjectState_00485d40* __stdcall CreateObjectState(Object3do* obj);

// FUNCTION: 0x485d40
void __stdcall InitUnitScript(Unit* self)
{
    Object3do* obj = g_game->definitions[self->typeId];
    if (self->type->data) {
        self->script = new UnitScript;
        self->script->SetCob(self->type->data);
        self->state = CreatePlayerObjectState(obj, self->type->data, (int)self);  // the owner, as an int
        ((UnitScript*)self->script)->SetObjectState(self->state);
        self->script->StartScript("Create", 0, 1);
    } else {
        self->script = 0;
        self->state = CreateObjectState(obj);
        self->state->field_c = self;
    }
    // Shared tail after the if/else: reloads the block in both exits.
    self->state->field_10 = 0;
}

// The compiler-generated scalar deleting destructor of UnitScript, the
// only class derived from CobScript (src/units/cob.cpp). Its
// vtable at 0x4fd698 has the base's 21 slots, all overridden; slot 20 holds
// this function. The derived class has no destructor of its own, so the
// implicit one only calls the base destructor.
//
// The overrides live at 0x480770-0x481470. Slots 7-19 here carry the base
// names, with their addresses (from 0x4fd698) noted beside them. All 20 are
// defined as members of UnitScript (src/units/unit_script.cpp).
//
// InitUnitScript builds the object (`new` of 0x544 bytes, then this class's
// vtable).
// The global below exists only to emit the vtable and this COMDAT, as in
// src/game/data_files_42a870.cpp.

// FUNCTION: 0x485e30 ??_GUnitScript@@UAEPAXI@Z
static UnitScript* s_object;
// A namespace-scope `new` would construct the object during CRT init, and the
// base constructor reads a global that is not set until later.
UnitScript* emit_00485e30() { return new UnitScript; }

class UnitMotion {
public:
    char unknown_0[0x2f];

    UnitMotion(Unit* unit);
    void DestroyObject();
};

// Creates the unit's 0x2f-byte object (constructor 0x43dc00) and copies a
// value from the unit type.
// FUNCTION: 0x485e50
void __stdcall CreateUnitMotion(Unit* unit)
{
    unit->obj = new UnitMotion(unit);
    unit->heading = unit->type->field_210;
}

extern void* g_weaponAimCobVtable[];

void __stdcall InitUnitWeaponSlots(Unit* unit);
void __stdcall UpdateMetalExtraction(Unit* unit);

// Initialises a unit: finds its unit type, gives the three 0x1c-byte members
// at +8, +0x24 and +0x40 their vtable, stores the type id at +0xa6, then hands
// the unit to the position/flags setter, the object builder and two more
// methods. When the unit type's +0x22f is 1 the unit also gets a new
// UnitMotion and the type's +0x210 copied to +0x66, the tail of the
// matched CreateUnitMotion inlined.
// The three vtable stores are guarded by `if (unit)` but the +0xa6 store right
// after them, and the new expression at the end, are not: a null unit pointer
// writes to 0xa6, then to 0, and reads 0x92. Kept as the original has it.
// FUNCTION: 0x485e90
void __stdcall InitUnit(int unitType, Pos_00485a40 pos, int param_5, Unit* unit)
{
    UnitType* type = &g_game->unitTypes[(unsigned short)unitType];
    if (unit) {
        WeaponAimCobCb* sub = (WeaponAimCobCb*)((char*)unit + 8);
        for (int i = 0; i < 3; i++) {
            sub->vtable = g_weaponAimCobVtable;
            sub = (WeaponAimCobCb*)((char*)sub + 0x1c);
        }
    }
    unit->typeId = (short)unitType;
    InitUnitFromType(unit, pos, param_5);
    InitUnitScript(unit);
    InitUnitWeaponSlots(unit);
    UpdateMetalExtraction(unit);
    if (type->mobile == 1) {
        unit->obj = new UnitMotion(unit);
        unit->heading = unit->type->field_210;
    }
}

// Creates a unit of a given type for a player: refuses when the type is not
// buildable or the player already has its limit of that type, takes the
// requested unit slot (or the first free one) from the player's unit list,
// initialises it (InitUnit inlined) and registers it.

class MissionConditions {
public:
    void NotifyUnitCreated(Unit* unit);
    void NotifyUnitDied(Unit* unit);
};

void __stdcall UpdateUnitHeight(Unit* unit);
void __stdcall AddUnitToMap(Unit* unit);
void __stdcall SendNewUnit(Unit* unit);
void __stdcall BroadcastBuilderLink(Unit* a, Unit* b);
void __stdcall RevealNewUnit(Unit* unit);

static inline void __stdcall InitUnit_00485e90(unsigned short unitType, Pos_00485a40 pos,
                                               int param_5, Unit* unit)
{
    UnitType* type = &g_game->unitTypes[unitType];
    if (unit) {
        WeaponAimCobCb* sub = (WeaponAimCobCb*)((char*)unit + 8);
        for (int i = 0; i < 3; i++) {
            sub->vtable = g_weaponAimCobVtable;
            sub = (WeaponAimCobCb*)((char*)sub + 0x1c);
        }
    }
    unit->typeId = unitType;
    InitUnitFromType(unit, pos, param_5);
    InitUnitScript(unit);
    InitUnitWeaponSlots(unit);
    UpdateMetalExtraction(unit);
    if (type->mobile == 1) {
        unit->obj = new UnitMotion(unit);
        unit->heading = unit->type->field_210;
    }
}

// FUNCTION: 0x485f50
Unit* __stdcall CreateUnit(unsigned char player, unsigned short typeId, Pos_00485a40 pos,
                                      int param_5, int mode, unsigned short id)
{
    int off = player * 0x14b;
    Player* pl = (Player*)((char*)g_game + off + 0x1b63);
    if (typeId == 0)
        return 0;
    UnitType* type = &g_game->unitTypes[typeId];
    if (!(type->flags.all & 0x800000))
        return 0;
    if (type->limit != -1) {
        int count = 0;
        for (Unit* u = pl->unitsBegin; u <= pl->unitsEnd; u++) {
            if (u->typeId == typeId)
                count++;
        }
        if (count >= type->limit)
            return 0;
    }
    Unit* unit;
    for (unit = pl->unitsBegin; unit <= pl->unitsEnd; unit++) {
        if (id != 0) {
            unit = &g_game->units[id];
            if (unit < pl->unitsBegin || unit > pl->unitsEnd)
                return 0;
        }
        if (unit->typeId == 0)
            goto found;
        if (id != 0)
            return 0;
    }
    return 0;
found:
    InitUnit_00485e90(typeId, pos, param_5, unit);
    unit->flags.mode = mode;
    UpdateUnitHeight(unit);
    AddUnitToMap(unit);
    SendNewUnit(unit);
    if (param_5) {
        if (type->mobile == 0)
            BroadcastBuilderLink(unit, unit);
        if (unit->type->flags.bit18)
            ((Unit*)unit)->SetStateBits(1, 1);
        if (unit->type->flags.bit24) {
            unit->lastDamageType = 7;
            unit->flags.bit14 = 1;
        }
    }
    RevealNewUnit(unit);
    // Array subscript, not a byte-offset cast: it fixes the SIB operand order.
    g_game->players[player].unitCount++;
    g_game->players[player].unitsCreated++;
    g_game->conditions->NotifyUnitCreated(unit);
    return unit;
}

// Creates a unit from a spawn record (the 0x11-byte record the build and
// placement code fills in: player, unit type, unit id, position). It takes the
// unit slot for the record's id out of the unit array, refuses to go on when
// that player has no unit list, clears the slot when it is still in use,
// initialises the unit (the matched InitUnit, inlined), then registers it
// with UpdateUnitHeight, AddUnitToMap, RevealNewUnit and the list manager and bumps
// the player's counters at +0x144 and +0x140.

#pragma pack(push, 1)
struct Spawn_004861d0 {                  // 0x11 bytes
    unsigned char player;                // +0x0
    unsigned short type;                 // +0x1
    unsigned short id;                   // +0x3
    Pos_00485a40 pos;                    // +0x5
};
#pragma pack(pop)

// FUNCTION: 0x4861d0
Unit* __stdcall CreateUnitFromPacket(unsigned char player, Spawn_004861d0* spawn)
{
    Player* pl = &g_game->players[player];
    Unit* unit;
    if (spawn->id == 0) {
        unit = 0;
    } else {
        unit = &g_game->units[spawn->id];
    }
    if (pl->unitsBegin == 0) {
        return 0;
    }
    if (unit->typeId != 0) {
        KillUnit(unit, 0);
    }
    InitUnit_00485e90(spawn->type, spawn->pos, 0, unit);
    UpdateUnitHeight(unit);
    AddUnitToMap(unit);
    RevealNewUnit(unit);
    // Indexed again from g_game, not through pl: keeps the player index live early.
    g_game->players[player].unitCount++;
    g_game->players[player].unitsCreated++;
    g_game->conditions->NotifyUnitCreated(unit);
    return unit;
}

struct Result_486360 {
    char unknown_0[0x18];
    int field_18;                    // +0x18
    int field_1c;                    // +0x1c
};

void* __stdcall GetMapCell(int x, int y);
// GetGroundHeight (units_485070.cpp) and FindHighestPointOnLine (units_4851c0.cpp)
// only match in files of their own: their symbol ids have to stay below a low limit.
int __stdcall GetGroundHeight(Pos_00485070* pos);
// Also declared for the symbol count that AllocateUnitMemory needs.
int __stdcall FindHighestPointOnLine(Pos_00485a40 a, Pos_00485a40 b);
Result_486360* __stdcall PlaceFeature(void* target, unsigned short id, Pos_00485070* pos, void* field_64, unsigned char owner);
void __stdcall EmitSmoke(Pos_00485070* pos, int a, int b, int c);

// FUNCTION: 0x486360
void __stdcall CreateUnitCorpse(Unit* unit, int depth, int flag)
{
    unsigned short id = unit->type->field_1bc;
    for (; depth > 1; depth--) {
        if (id >= 0xfffb) {
            return;
        }
        id = g_game->features[id].next;
    }
    if (id < 0xfffb) {
        void* target = GetMapCell(unit->screen.x, unit->screen.y);
        if (target != 0) {
            Pos_00485070* pos = (Pos_00485070*)&unit->pos;
            if (GetGroundHeight(pos) <= g_game->limit) {
                Result_486360* r = PlaceFeature(target, id, pos, &unit->bank, unit->playerIndex);
                if (r != 0) {
                    if (!(unit->type->flags.all & 0x1000000)) {
                        r->field_18 = -11468;
                        r->field_1c = 0;
                    }
                    flag = 0;
                }
            } else {
                PlaceFeature(target, id, pos, &unit->bank, unit->playerIndex);
            }
            if (flag) {
                EmitSmoke(pos, 0xf, 900, 9);
            }
        }
    }
}

// FUNCTION: 0x486460
int __stdcall IsUnitCommander(Unit* unit)
{
    return _strcmpi(g_game->names[unit->player->info->side].name, unit->type->name) == 0;
}

// A live unit that changed its type (the linked unit name at g_game+0x37f5f
// matches the unit type's name) recomputes a "Killed" kill count and sends the
// 0xb byte command record at +0xa it builds here to its own player. param_2 is
// the command kind: 7 means a spy / non-kill path, 4/5/9 or a positive
// health skip the recount, otherwise the kills are a percentage of the
// type's maxHealth.

#pragma pack(push, 1)
struct Cmd_004864b0 {
    unsigned char type;                // +0x0
    unsigned short unitId;             // +0x1
    int killerId;                      // +0x3
    unsigned short parentId;           // +0x7
    signed char amount;                // +0x9
    // 4-bit bitfields, not a plain char: they give the XOR read-modify-writes.
    unsigned char count : 4;           // +0xa
    unsigned char kind : 4;            // +0xa
};
#pragma pack(pop)

int __stdcall GetSlotDpid(unsigned char index);
int __stdcall BroadcastPacket(int id, unsigned char* packet, int size);
void __stdcall ApplyUnitDeath(Cmd_004864b0* cmd, int param);
void __stdcall PopUntilNamedLayout(int param);
void __stdcall KillPlayerUnits(unsigned char player);

// FUNCTION: 0x4864b0
void __stdcall KillUnit(Unit* unit, int param_2)
{
    if ((unit->flags.all & 0x10000000) != 0) {
        int same = _strcmpi(g_game->names[unit->player->info->side].name,
                            unit->type->name) == 0;
        if (same) {
            unit->player->flags &= 0xfffe;
        }
        int amount;
        int flag;
        if (param_2 == 7) {
            flag = 1;
            amount = 0;
        } else if (param_2 == 4 || param_2 == 5 || param_2 == 9 || unit->health > 0) {
            amount = 0;
            flag = 0;
        } else {
            amount = ((int)(unit->health * -100 / unit->type->maxHealth) + unit->prevHealthPercent) / 2;
            if (amount < 1)
                amount = 1;
            if (amount > 100)
                amount = 100;
            unit->script->QueryScript("Killed", &amount, &flag, 0, 0);
        }
        if (unit->buildLeft != 0.0f) {
            flag = 0;
        }
        Cmd_004864b0 cmd;
        // unitId before amount: sets the register roles.
        cmd.unitId = unit->id;
        cmd.amount = amount;
        cmd.type = 0xc;
        cmd.count = flag;
        cmd.kind = param_2;
        cmd.killerId = GetSlotDpid(unit->lastAttackerSlot);
        if (unit->parent == 0)
            cmd.parentId = 0;
        else
            cmd.parentId = unit->parent->id;
        if (unit->player->active != 0 &&
            (unit->player->type == 1 || unit->player->type == 2)) {
            BroadcastPacket(unit->player->id, (unsigned char*)&cmd, 0xb);
        }
        ApplyUnitDeath(&cmd, 1);
        if (same && g_game->mode != 0 && unit->player->active != 0 &&
            (unit->player->type == 1 || unit->player->type == 2)) {
            PopUntilNamedLayout(1);
            KillPlayerUnits(unit->playerIndex);
        }
    }
}

// Handles the "unit died" record that 0x4864b0 builds: credits the kill, updates the
// kill leaderboard ("%s has taken the lead with %d kills"), then tears the unit down.

extern char g_killedScriptName[];
extern char g_takenLeadFormat[];

void __stdcall AddEyeball(void* pos, int a, int b, int c);
unsigned char __stdcall FindSlotByDpid(int id);
void __stdcall DeleteOrders(void* unit, int flag);
void __stdcall RemoveSpeechOfUnit(void* unit);
void __stdcall RemoveUnitProjectiles(void* unit);
void __stdcall AttachUnitToPiece(void* unit, void* builder, int a, int c);
void __stdcall DamageUnit(void* a, void* b, int c, int d, int e);
void __stdcall ClearFootprintAndUnlink(void* unit);
void __stdcall RemoveUnitLineOfSight(void* unit);
void __stdcall AddCdActivitySample(int flag);
char* __stdcall Translate(char* text);
void __stdcall AddMessage(char* text, int a, int b, int c);
void __stdcall FlashScorePanelKillLoss(int a, int b);
void __stdcall DetonateUnitWeapon(void* unit, int flag);
void __stdcall ClearUnitRefs(void* unit);
void __stdcall FreeObjectState(void* state);
void __cdecl operator delete(void* p);
void __stdcall AnnouncePlayerLeft(int id);
void __stdcall AnnounceForcesDestroyed(void* player);

// FUNCTION: 0x4866d0
void __stdcall ApplyUnitDeath(Cmd_004864b0* cmd, int local)
{
    Unit* unit;
    if (cmd->unitId == 0)
        unit = 0;
    else
        unit = &g_game->units[cmd->unitId];
    if ((unit->flags.all & 0x10000000) == 0)
        return;

    if (unit->player->index == g_game->playerIndex)
        AddEyeball(&unit->pos, unit->type->x202, unit->type->x170, 60);
    Unit* parent;
    if (cmd->parentId == 0)
        parent = 0;
    else
        parent = &g_game->units[cmd->parentId];
    unit->parent = parent;
    unit->lastAttackerSlot = FindSlotByDpid(cmd->killerId);
    g_game->conditions->NotifyUnitDied(unit);
    DeleteOrders(unit, 1);
    RemoveSpeechOfUnit(unit);
    SetUnitSquad(unit, -1);
    RemoveUnitProjectiles(unit);
    if (unit->owner != 0)
        AttachUnitToPiece(unit, 0, -1, 1);
    while (unit->first != 0) {
        unsigned char depth = cmd->kind != 3 ? 6 : 3;
        DamageUnit(unit->parent, unit->first, 30000, depth, 0);
        AttachUnitToPiece(unit->first, 0, -1, 1);
    }
    ClearFootprintAndUnlink(unit);
    if ((g_game->mapFlags & 2) == 2)
        RemoveUnitLineOfSight(unit);
    if (local == 0 && cmd->amount > 0)
        unit->script->StartScriptWithArgs(g_killedScriptName, 0, 1, 1, cmd->amount, 0, 0, 0);

    int credited = 0;
    switch (cmd->kind) {
    case 5:
        if (unit->lastAttackerSlot == 10 || unit->lastAttackerSlot == unit->playerIndex)
            break;
    case 1:
    case 6:
        if (unit->player != 0) {
            unit->player->losses++;
            if (unit->lastAttackerSlot != 10 && unit->buildLeft == 0.0f && unit->playerIndex != unit->lastAttackerSlot)
                g_game->players[unit->lastAttackerSlot].kills++;
            int same = _strcmpi(g_game->names[unit->player->info->side].name,
                                unit->type->name) == 0;
            if (same) {
                if (unit->lastAttackerSlot != 10)
                    g_game->players[unit->lastAttackerSlot].commanderKills++;
                unit->player->commanderLosses++;
            }
            if (unit->parent != 0 && unit->buildLeft == 0.0f && unit->playerIndex != unit->lastAttackerSlot)
                unit->parent->killCount++;
            if (unit->lastAttackerSlot == g_game->localPlayer)
                AddCdActivitySample(5);
            credited = 1;
        }
        break;
    case 3:
        if (unit->player != 0 && g_game->players[g_game->localPlayer].field_129[unit->player->index] == 0) {
            unit->player->losses++;
            int same = _strcmpi(g_game->names[unit->player->info->side].name,
                                unit->type->name) == 0;
            if (same)
                unit->player->commanderLosses++;
            credited = 1;
        }
        break;
    }

    if (credited && unit->lastAttackerSlot != 10) {
        Player* rec = &g_game->players[unit->lastAttackerSlot];
        if (rec->active != 0 && (rec->type == 1 || rec->type == 2 || rec->type == 3)
            && rec->index != 10
            && (g_game->mission->GetGameType() == 3 || g_game->mission->GetGameType() == 2)
            && rec->rank > 0) {
            int rank = rec->rank;
            int best = rank;
            int mine = g_game->mode == 2 ? rec->commanderKills : rec->kills;
            int i = 10;
            Player* p = g_game->players;
            // do/while over a Player pointer: otherwise the frame grows.
            do {
                if (p->type != 0) {
                    bool hid = p->info->b6;
                    if (!hid) {
                        // Two compare arms: the compiler merges their setg.
                        int ahead;
                        if (g_game->mode == 2)
                            ahead = mine > p->commanderKills;
                        else
                            ahead = mine > p->kills;
                        if (ahead) {
                            if (p->rank < best)
                                best = p->rank;
                        }
                    }
                }
                p++;
                i--;
            } while (i != 0);
            if (best < rank) {
                i = 10;
                p = g_game->players;
                do {
                    if (p->rank >= best && p->rank < rec->rank)
                        p->rank = p->rank + 1;
                    p++;
                    i--;
                } while (i != 0);
                rec->rank = best;
                if (best == 0) {
                    char text[100];
                    sprintf(text, Translate(g_takenLeadFormat), rec->name,
                            g_game->mode == 2 ? rec->commanderKills : rec->kills);
                    AddMessage(text, 2, 0, 10);
                }
            }
        }
        if (g_game->b7)
            FlashScorePanelKillLoss(unit->lastAttackerSlot, unit->player->index);
    }

    if (cmd->kind == 5 && unit->parent != 0) {
        // Field pointer, not a Unit* copy; and the copy into f is needed too.
        Unit** par = &unit->parent;
        float health = 1.0f - unit->buildLeft;
        float f = health;
        f *= unit->type->x18a;
        if ((*par)->playerRef.player->active != 0 && (*par)->playerRef.player->type == 2) {
            switch (g_game->difficulty) {
            case 0:
                (*par)->playerRef.field_18 = (*par)->playerRef.field_18 - f * -0.5;
                break;
            case 1:
                (*par)->playerRef.field_18 = (*par)->playerRef.field_18 - f * -0.7;
                break;
            default:
                (*par)->playerRef.field_18 += f;
            }
        } else {
            (*par)->playerRef.field_18 += f;
        }
    }
    if (cmd->amount > 0 && unit->buildLeft == 0.0f)
        DetonateUnitWeapon(unit, cmd->kind == 3);
    if (cmd->count > 0) {
        // Local flag: without it the script's virtual delete uses another register.
        int flag = cmd->kind != 7;
        CreateUnitCorpse(unit, cmd->count, flag);
    }

    ClearUnitRefs(unit);
    if (unit->script != 0) {
        delete unit->script;
        unit->script = 0;
    }
    if (unit->state != 0) {
        FreeObjectState(unit->state);
        unit->state = 0;
    }
    UnitMotion* head = unit->obj;
    if (head != 0) {
        head->DestroyObject();
        operator delete(head);
        unit->obj = 0;
    }
    unit->typeId = 0;
    // Keep this order: adjacent clears would fold into one `and`.
    unit->flags.all &= ~0x10000000;
    unit->type = g_game->unitTypes;
    unit->flags.all &= ~0x30;
    unit->player->unitCount--;
    if (unit->player->unitCount == 0) {
        if (g_game->mission->GetGameType() == 3)
            AnnouncePlayerLeft(unit->player->id);
        if (g_game->mission->GetGameType() == 2)
            AnnounceForcesDestroyed(unit->player);
    }
}

// FUNCTION: 0x486e80
void __stdcall KillUnitsOfType(short id)
{
    Unit* u = g_game->units;
    Unit* end = g_game->unitsEnd;
    if (id != 0 && u != 0) {
        for (; u <= end; u = (Unit*)((char*)u + 0x118)) {
            if ((short)u->typeId == id) {
                KillUnit(u, 8);
            }
        }
    }
}

// FUNCTION: 0x486ed0
void KillAllUnits(void)
{
    Unit* u = g_game->units;
    Unit* end = g_game->unitsEnd;
    if (u != 0) {
        for (; u <= end; u = (Unit*)((char*)u + 0x118)) {
            if (u->typeId != 0) {
                KillUnit(u, 8);
            }
        }
    }
}

// Walks one player slot's unit list and either damages every unit whose owner
// is a human or computer player (DamageUnit) or fires its second weapon and
// flags it (DetonateUnitWeapon / KillUnit).

// FUNCTION: 0x486f10
void __stdcall KillPlayerUnits(unsigned char player)
{
    Player* p = &g_game->players[player];
    if (p != 0) {
        if (p->unitCount != 0) {
            // Indexed again from g_game, not through p: keeps the index base clean.
            Unit* u = g_game->players[player].unitsBegin;
            Unit* last = g_game->players[player].unitsEnd;
            if (u != 0) {
                for (; u <= last; u++) {
                    unsigned int flags = u->flags.all;
                    if (flags & 0x10000000) {
                        if (!(flags & 0x4000)) {
                            Player* d = u->player;
                            if (d->active != 0 &&
                                (d->type == 1 || d->type == 2)) {
                                DamageUnit(u, u, 0x7530, 3, 0);
                            } else {
                                DetonateUnitWeapon(u, 1);
                                u->flags.all |= 0x4000;
                                KillUnit(u, 3);
                            }
                        }
                    }
                }
            }
        }
    }
}
