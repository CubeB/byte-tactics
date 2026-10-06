// Decompiled by Opus, space-bunny-free, deepseek-v4.1-flash, Claude Sonnet 5.5, GPT-6.1-sol, deepseek-v4.1, mimo-v2.6-pro, DeepSeek V4.1 Flash, GPT-6, Claude Opus 5.5, mimo-v2.6-flash, MiMo-V2.6-Pro, muse-spark-1.3-free, longcat-2.5-preview-free, Space Bunny Free and Sonnet 5.5. Names are provisional.
//
// The unit_position module: a unit's position, height, cell, heading, the
// attach/detach order (0x48aac0 and 0x48ab70) and the per-unit state packet
// writer/reader (0x48b200 to 0x48b920). The module's views of Game and Unit are
// one definition each, with every field at its own offset.

#include <stdio.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

#pragma pack(push, 1)

union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

struct Vec3 {
    int x;                             // +0x0
    Fixed y;                           // +0x4
    int z;                             // +0x8
};

struct Point16 {
    short x;
    short y;
};

// The unit's tail at +0x64: 0x48b3f0 and 0x48b200 read it as three shorts,
// 0x48b920 as an int and a short.
struct Tail16 {
    unsigned short a, b, c;
};

struct Tail32 {
    int a;
    unsigned short b;
};

class Unit;
class CobScript;
class UnitMotion;
class PlayerData;
class BitWriter;
class BitReader;
struct Block;
struct Beacon;
struct Order;
struct Spawn;
struct UnitType;
struct Player;

class Mission {
public:
    char unknown_0[0xd4c];
    int waterDoesDamage;               // +0xd4c
    int waterDamage;                   // +0xd50
};

// The player behind a unit (the unit's +0x96 and g_game's player array): the
// unit list at +0x67, the player kind byte at +0x73.
struct Player {
    int f0;                            // +0x00
    int id;                            // +0x04
    char unknown_8[0x18 - 8];
    int ticks;                         // +0x18
    char unknown_1c[0x67 - 0x1c];
    Unit* f67;                         // +0x67
    Unit* f6b;                         // +0x6b
    unsigned short field_6f;           // +0x6f
    char unknown_71[0x73 - 0x71];
    unsigned char f73;                 // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char f146;                // +0x146
    char unknown_147[0x14b - 0x147];
};

// The unit's type; +0x241 is the flag word the position code tests: bit 12
// floats, bit 19 floats on water, bit 20 can leave the water.
struct UnitType {
    char unknown_0[0x1fa];
    unsigned int f1fa;                 // +0x1fa
    char unknown_1fe[0x200 - 0x1fe];
    unsigned short f200;               // +0x200
    char unknown_202[0x22c - 0x202];
    unsigned char draft;               // +0x22c
    char unknown_22d[0x241 - 0x22d];
    union {
        struct {
            unsigned int low : 12;
            unsigned int floats : 1;   // bit 12
            unsigned int unknown_13 : 6;
            unsigned int on_water : 1; // bit 19
            unsigned int over_water : 1;// bit 20
            unsigned int unknown_21 : 11;
        } bits;
        unsigned int all;
    } f241;                            // +0x241
};

// The +0x110 flag word, one bit view per function that reads it.
union Flags {
    struct {
        unsigned int colour : 2;       // bits 0..1
        unsigned int b2 : 1;
        unsigned int b3 : 1;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int mid6 : 7;         // bits 6..12
        unsigned int b13 : 1;
        unsigned int b14 : 1;
        unsigned int b15 : 1;
        unsigned int b16 : 1;
        unsigned int b17 : 1;
        unsigned int mid18 : 10;       // bits 18..27
        unsigned int b28 : 1;
        unsigned int b29 : 1;
        unsigned int b30 : 1;
        unsigned int b31 : 1;
    } bits;
    unsigned int all;
};

// What a unit's +0 points at: its motion, the serialisation interface at +0
// and the int of 0x48b200 at +0x20. 0x48ab70's two bit field is at +0x2e.
class UnitMotion {
public:
    PlayerData* player;                // +0x00
    char unknown_4[0x20 - 4];
    int field_20;                      // +0x20
    char unknown_24[0x2e - 0x24];
    unsigned char bits_2e : 2;         // +0x2e
    char unknown_2f[0x8a - 0x2f];

    void UpdateMotion(Unit* u);
};

// The serialisation interface behind a unit's owner: the writer 0x48b710 calls
// slot +0x20, the reader 0x48b920 slot +0x24.
class PlayerData {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual int v7();                              // +0x1c
    virtual void WriteTo(BitWriter* stream);       // +0x20
    virtual void ReadFrom(BitReader* reader);      // +0x24
};

class Class_0047cb00 {
public:
    char unknown_0[6];
    void* head;                        // +6
    void FUN_0047cb00(void* node);
};

class Class_0047cb40 {
public:
    char unknown_0[6];
    int head;                          // +6
    void FUN_0047cb40(int node);
};

struct Unit {
    UnitMotion* motion;                // +0x00
    char unknown_4[0x64 - 4];
    union {
        Tail16 tail16;                 // +0x64
        Tail32 tail32;                 // +0x64
    };
    Vec3 pos;                          // +0x6a
    Point16 cell;                      // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point16 origin;                    // +0x7e
    Class_0047cb00* list;              // +0x82
    Unit* owner;                       // +0x86
    Unit* first;                       // +0x8a
    Unit* next;                        // +0x8e
    UnitType* type;                    // +0x92
    Player* player;                    // +0x96
    CobScript* f9a;                    // +0x9a
    Block* block;                      // +0x9e
    char unknown_a2[0xa6 - 0xa2];
    unsigned short field_a6;           // +0xa6
    unsigned short id;                 // +0xa8
    char unknown_aa[0xf5 - 0xaa];
    unsigned char ff5;                 // +0xf5
    unsigned char ff6;                 // +0xf6
    unsigned char ff7;                 // +0xf7
    char unknown_f8;                   // +0xf8
    signed char f9;                    // +0xf9
    unsigned char ffa;                 // +0xfa
    int ffb;                           // +0xfb
    unsigned char playerIndex;         // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                   // +0x104
    short field_108;                   // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char field_10e;           // +0x10e
    char unknown_10f[0x110 - 0x10f];
    union {
        unsigned int flags;            // +0x110
        Flags f110;                    // +0x110
    };
    char unknown_114[0x118 - 0x114];

    void SetStateBits(unsigned char mask, int set);
};

union F14373 {
    struct {
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int rest : 30;
    } bits;
    unsigned int all;
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_1c3f[0x2a44 - 0x1b63 - 10 * 0x14b];
    unsigned char f2a44;               // +0x2a44
    char unknown_2a45[0x1427f - 0x2a45];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14353 - 0x14280];
    int f14353;                        // +0x14353
    Unit* units;                       // +0x14357
    char unknown_1435b[0x14371 - 0x1435b];
    short f14371;                      // +0x14371
    F14373 f14373;                     // +0x14373
    char unknown_14377[0x14393 - 0x14377];
    int field_14393;                   // +0x14393, bit count for the type index
    char unknown_14397[0x37ee6 - 0x14397];
    unsigned short field_37ee6;        // +0x37ee6, the unit count
    char unknown_37ee8[0x38a47 - 0x37ee8];
    int ticks;                         // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* mode;                     // +0x391e9
};

// The seven byte order record the caller at 0x48aac0 builds on its stack and
// 0x48ab70 and 0x48b3f0 read.
struct Order {
    unsigned char type;                // +0x00
    unsigned short id1;                // +0x01
    unsigned short id2;                // +0x03
    char param;                        // +0x05
    unsigned char param2;              // +0x06
};

// The spawn record 0x48b3f0 and 0x48b920 build for CreateUnitFromPacket.
struct Spawn {
    unsigned char player;              // +0x00
    unsigned short type;               // +0x01
    unsigned short id;                 // +0x03
    Vec3 pos;                          // +0x05
    union {
        Tail16 words;                  // +0x11
        Tail32 dword;                  // +0x11
    } tail;
};

struct Block {
    char unknown_0[0x10];
    int field_10;                      // +0x10
};

class CobScript {
public:
    char unknown_0[8];
    void RunScripts(int n);
};

class BitWriter {
public:
    int bit;                           // +0x0 current word index
    int index;                         // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10

    void GrowBuffer();
    BitWriter();
    void SetByteAt(int index, unsigned char value);
    void FreeBuffer();
    void WriteBits(int value, int bits);
};

// Bit reader, the counterpart of the writer (see src/network/net_stats.cpp).
class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08

    int ReadBits(int bits);
    int ReadSignedBits(int bits);

    int ReadBit()
    {
        int r = (data[index] & (1 << bit)) != 0;
        if (++bit == 32) {
            bit = 0;
            index++;
        }
        return r;
    }
};

#pragma pack(pop)

int __stdcall GetGroundHeight(Vec3* pos);
void __stdcall AlignUnitToGround(Unit* unit);
void __stdcall RemoveUnitFromMap(Unit* unit);
void __stdcall AddUnitToMap(Unit* unit);
void __stdcall UpdateUnitLineOfSight(Unit* unit);
int __cdecl FUN_004b715a(int x, int z);
int __cdecl GetLocalDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall ApplyAttachUnit(Order* order);
void __stdcall AddBeCarriedOrder(Beacon* beacon);
void __stdcall FUN_0048c9b0(Unit* u);
void __stdcall UpdateWindGenerator(Unit* u);
void __stdcall UpdateUnitWeapons(Unit* u);
void __stdcall RunOrders(Unit* u);
void __stdcall FUN_0043bad0(Unit* u);
void __stdcall UpdateUnitHeight(Unit* u);
void __stdcall KillUnit(Unit* u, int n);
void __stdcall DamageUnit(int a, Unit* u, int damage, int kind, int flag);
int __stdcall FUN_0041bd10(Unit* u, Unit* u2, float f);
void __stdcall SendUnitStates(Player* p);
void __stdcall FUN_0048d790(void);
int __stdcall IsKeyDown(int n);
void __stdcall FUN_0041c2e0(int n);
Unit* __stdcall CreateUnitFromPacket(unsigned char player, Spawn* spawn);
void __stdcall WriteUnitState(BitWriter* stream, Unit* u);
void __stdcall ReadUnitState(BitReader* reader, Unit* u);

extern float DAT_004fd750;

// Snaps a unit's height (pos.y, 16.16 fixed point) to the ground. Units
// whose type has the flag at +0x241 bit 12 (floating) stay at least at the
// sea level minus the type's draft. The ground height is fetched twice on
// the floating path: the source used a max() macro.
// FUNCTION: 0x48a7f0
void __stdcall SnapUnitToGround(Unit* unit)
{
    extern Game* g_game;
    if (unit->type->f241.bits.floats) {
        unit->pos.y.value = max(GetGroundHeight(&unit->pos), g_game->seaLevel - unit->type->draft) << 16;
    } else {
        unit->pos.y.value = GetGroundHeight(&unit->pos) << 16;
    }
}

// Places a unit's height (pos.y, 16.16 fixed point) on the ground, at the sea
// level or on the water surface, depending on the flags in the unit's type
// (+0x241): bit 12 floats, bit 19 floats on water, bit 20 can leave the water.
// Only runs for a unit that belongs to somebody and whose type can be off the
// ground; the flag at +0x110 bit 16 asks for this to be redone.
//
// The bit 19 branch builds the height in a 16.16 `Fixed` union local and
// copies the whole union into pos.y (as 0x4589c0 does with its `Fixed yv`).
// Every spelling that assigned an int let MSVC 5 fold
// `(draft * 0xffff + sea) << 16` into `(sea - draft) << 16`, or, with the fold
// blocked by a pointer or volatile, rotate the registers (88.8% at best); the
// union copy keeps the original's product, sum and shift in place.
// FUNCTION: 0x48a870
void __stdcall UpdateUnitHeight(Unit* unit)
{
    extern Game* g_game;
    if ((unit->flags & 0x10000) || unit->type->f241.bits.floats) {
        unit->flags &= ~0x10000;
        if (unit->motion && (unit->flags & 3) == 1) {
            if (unit->type->f241.bits.over_water) {
                if (unit->type->f241.bits.floats) {
                    unit->pos.y.value = max(GetGroundHeight(&unit->pos), g_game->seaLevel - unit->type->draft) << 16;
                } else {
                    unit->pos.y.value = GetGroundHeight(&unit->pos) << 16;
                }
            } else if (unit->type->f241.bits.on_water) {
                Fixed h;
                h.value = unit->type->draft * 0xffff + g_game->seaLevel;
                h.value <<= 16;
                unit->pos.y = h;
            } else {
                AlignUnitToGround(unit);
            }
        }
    }
}

// Heading from one position to another in the x/z plane.
// FUNCTION: 0x48a980
int __stdcall GetHeadingBetween(Vec3* from, Vec3* to)
{
    return FUN_004b715a(from->x - to->x, from->z - to->z);
}

// Heading from one position to another in the x/z plane (like 0x48a980),
// or `def` when the two positions coincide in that plane.
// FUNCTION: 0x48a9b0
unsigned short __stdcall GetHeadingBetweenOrDefault(Vec3* from, Vec3* to, unsigned short def)
{
    int dx = from->x - to->x;
    int dz = from->z - to->z;
    if (dx == 0 && dz == 0)
        return def;
    return FUN_004b715a(dx, dz);
}

// Moves a unit to a new position. The cell (the two shorts at +0x76) and the
// two low bits of the flags at +0x110 only change when the new position falls
// in a different cell (the fixed-point WorldToCell of the unit type's origin
// at +0x7e) or the low flag bits differ. Then the unit is detached
// (RemoveUnitFromMap), the cell and the flags are updated, and it is put back into
// the map (AddUnitToMap) with its path redone (UpdateUnitLineOfSight). Either way flag
// 0x10000 (the "position changed" bit UpdateUnitHeight tests) is set, and the new
// flags value is returned.
//
// MATCH (206 of 206 bytes; was 87.2%, Claude Sonnet 5.5 #755). The flag update
// was already right (`and edi, 3` masking param_5 in place and `and al, 0xfc`
// clearing the two low bits, thanks to the `(short)` cast on the OR operand). The
// last difference was the first block: the original sign-extends origin.x into eax,
// shifts it there and copies it to ecx (`movsx eax, ax; shl eax, 0x13; mov ecx, eax`)
// after storing the origin copy, which is what writing the offset as a
// multiplication, `origin.x * 0x80000`, gives (the shift `origin.x << 19` sign-extends
// straight into ecx). The declaration-count sweep has only two states (84.2 and 87.2
// percent, neither a match), so compiler state was not involved, and named
// temporaries for the shifted offsets, a flags-carrying helper, a local copy of
// param_5 and reordering the condition did nothing.
static inline Point16 WorldToCell(Vec3 v, Point16 origin)
{
    Point16 c;
    c.x = (v.x - origin.x * 0x80000 + 0x80000) >> 20;
    c.y = (v.z - origin.y * 0x80000 + 0x80000) >> 20;
    return c;
}

// FUNCTION: 0x48a9f0
int __stdcall SetUnitPosition(Unit* unit, Vec3 pos, int param_5)
{
    Point16 cell = WorldToCell(pos, unit->origin);
    if (cell.x == unit->cell.x && cell.y == unit->cell.y && param_5 == (unit->flags & 3)) {
        unit->pos = pos;
    } else {
        RemoveUnitFromMap(unit);
        unit->pos = pos;
        unit->cell = cell;
        unit->flags = (unit->flags & 0xfffffffc) | (short)(param_5 & 3);
        AddUnitToMap(unit);
        UpdateUnitLineOfSight(unit);
    }
    unit->flags |= 0x10000;
    return unit->flags;
}

// FUNCTION: 0x48aac0
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4)
{
    Order packet;
    if ((unit->flags & 0x10000000) && !(unit->flags & 0x20000000) && unit->first == 0
        && (target == 0
            || ((target->flags & 0x10000000) && target != unit && target->owner == 0))) {
        packet.type = 0xa;
        if (unit == 0)
            packet.id1 = 0;
        else
            packet.id1 = unit->id;
        if (target == 0)
            packet.id2 = 0;
        else
            packet.id2 = target->id;
        packet.param = p3;
        packet.param2 = p4;
        BroadcastPacket(GetLocalDpid(), &packet, 7);
        ApplyAttachUnit(&packet);
    }
}

// The order record handler at 0x48aac0 builds a seven byte record on its stack
// (type 0x0a, two unit ids, two bytes) and passes it here after it has been
// through FUN_004fdb0 and BroadcastPacket, so this runs the order on the sending
// player's own units. The two ids index the unit array at g_game+0x14357
// (0x118 byte entries) and both index 0 means "no unit".
//
// The first unit (esi) must be live (flag 0x10000000 at +0x110), not dying
// (0x20000000 clear) and not already a container (its first child at +0x8a is
// null). The second unit (edi), if there is one, must be live, must not be the
// same unit and must have no attached unit at +0x86. Then the first unit is
// unlinked from the tree it is in (walking the sibling chain through +0x8a /
// +0x8e of its owner at +0x86, or asking the owner's list object at +0x82 to
// unlink it when it is a root), the order byte at +5 is stored at +0xf9, and
// the unit is either re-attached under the second unit (which then also gets
// bit 17 of +0x110 when the order byte is 0xff) or detached (bit 17 cleared,
// the list object's push is run). The byte at +6 is then blended into the two
// low bits of the type's byte at +0x2e (the XOR blend is MSVC 5's read
// modify write of a two bit field), and a "BECARRIED" child is created when the
// owner is a human or computer player, its unit list is not empty, a second
// unit was given, and that unit's type does not have bit 9 of the word at
// +0x241 set. FUN_0048c9b0 then refreshes the order, and it is only reached on
// the paths that got that far: every test that fails jumps past it.
//
// MATCH. The block that stores bit 17 is a named bool plus a bitfield store,
// with the comparison done on a `char` against -1 and its result masked with
// `& 1` before the assignment:
//
//     bool v = ((order->param == -1) & 1);
//     u->f110.bits.b17 = v;
//
// That combination gives every part of the original's ten instruction block:
// the `& 1` keeps the field width mask `and eax, 1` (which a plain bitfield
// store of the comparison drops), the named bool keeps the compare's byte
// operand in its own register (cl instead of folding it into the result
// register), and the `char` against -1 is a byte compare (`cmp cl, 0xff`) whose
// operand load is scheduled between the storage word's load and its clear.
// Earlier passes tried each half alone: a bitfield store of `param == 0xff`
// with an `unsigned char` sinks the clear below the comparison and puts the
// operand in dl, and `& 1` inside a whole word read modify write keeps the
// operand in cl but loses the width mask and gets the value computed first.
// FUNCTION: 0x48ab70
void __stdcall ApplyAttachUnit(Order* order)
{
    extern Game* g_game;
    Unit* u = !order->id1 ? 0 : &g_game->units[order->id1];
    Unit* t = !order->id2 ? 0 : &g_game->units[order->id2];
    if (u) {
        unsigned int f = u->f110.all;
        if (f & 0x10000000) {
            if (!(f & 0x20000000)) {
                if (u->first == 0) {
                    if (t == 0
                        || ((t->f110.all & 0x10000000) && t != u && t->owner == 0)) {
                        if (u->owner != 0) {
                            Unit* n = u->owner;
                            Unit** link = &n->first;
                            n = n->first;
                            while (n != u) {
                                link = &n->next;
                                n = n->next;
                            }
                            *link = u->next;
                        } else {
                            u->list->FUN_0047cb00(u);
                        }
                        u->f9 = order->param;
                        if (t) {
                            u->owner = t;
                            u->next = t->first;
                            t->first = u;
                            bool v = ((order->param == -1) & 1);
                            u->f110.bits.b17 = v;
                        } else {
                            u->next = 0;
                            u->f110.bits.b17 = 0;
                            u->owner = 0;
                            ((Class_0047cb40*)u->list)->FUN_0047cb40((int)u);
                        }
                        u->motion->bits_2e = order->param2;
                        if (u->player->f0) {
                            unsigned char k = u->player->f73;
                            if (k == 1 || k == 2) {
                                if (t && !(t->type->f241.bits.low & 0x200))
                                    AddBeCarriedOrder((Beacon*)u);
                            }
                        }
                        FUN_0048c9b0(u);
                    }
                }
            }
        }
    }
}

// MATCH (Claude Opus 5.5, #5296). What was left at 98.7% (the `off = 0` store
// scheduled early, the loop head loading off before g_game, and the
// `if (0) { g_leak = &off; }` escape that kept off in memory) all came from
// one thing: the front-end symbol id of g_game against those of the locals.
// With `extern g_game;` at file scope g_game is numbered before every local
// of the function. Declared inside the function after off, it is numbered
// after it, and then the plain source (no escape, the stores in the
// original's order `*cnt = 0; i = 0; off = 0;`) compiles to the original:
// off stays in its frame slot, g_game is the SIB base at the loop head and is
// loaded first.
//
// Checked with tools/c2prio.py --symbols and enum padding in scratch copies:
// with g_game back at file scope (id 258), an enum of 65400 or 65450 entries
// between g_game and the function also matches (off's id wraps past 65536
// to 166 or 216 in 16 bits, below g_game's 258), while 65500 (off at 266)
// and no padding both give 75.5% / 844 bytes. So the deciding comparison is
// g_game's id against off's, modulo 65536, as in 0x493bf0 (SIB base order of
// the g_game stores there). The other explanation is a lost header prefix that
// puts the 65536 wrap between g_game and this function (docs/c2-regalloc.md,
// "Symbol ids"); no real header set reaches that, so the function-scope extern
// is what is written here. The merged Game view keeps g_game at file scope out
// of this file so the block-scope extern stays a separate symbol.
//
// The merged Game view is `int ticks` (0x48b710 divides it signed), so the two
// `% 30` tests here cast it to unsigned to keep the original's `div`.
//
// Kept from earlier passes, still needed: the flat continue chain with
// PlayerMore(i) (a static inline testing the byte counter, which gives the
// original's unfolded `xor al,al / cmp al,0xa / jae` entry test), and
// RunOrders, FUN_0043bad0 and the def block inside
// `if (k3 == 1 || k3 == 2)` (Claude Opus 5.5, #5106).
static inline int PlayerMore(unsigned char i)
{
    if (i >= 10) return 0;
    return 1;
}

// FUNCTION: 0x48ad30
void __stdcall UpdateAllUnits(void)
{
    int* cnt;
    unsigned char i;
    int off;
    // Declared here, after the locals, not at file scope: see the notes above.
    extern Game* g_game;
    cnt = &g_game->f14353;
    *cnt = 0;
    i = 0;
    off = 0;
    for (; i < 10; i++, off += 0x14b) {
        if (!PlayerMore(i)) continue;
        Player* p = (Player*)((char*)&g_game->players[0] + off);
        if (p->f0 == 0) continue;
        unsigned char k = p->f73;
        if (k != 1 && k != 2 && k != 3) continue;
        if (p->f146 == 0xa) continue;
        {
            Unit* last = p->f6b;
            Unit* u = p->f67;
            while (u <= last) {
                    if (u->field_a6 != 0) {
                        (*cnt)++;
                        UpdateWindGenerator(u);
                        if (p->f0 != 0) {
                            unsigned char k2 = p->f73;
                            if (k2 == 1 || k2 == 2) {
                                UpdateUnitWeapons(u);
                            }
                        }
                        if (u->f9a != 0) {
                            u->f9a->RunScripts(1);
                        }
                        if (u->ffa != 0) {
                            u->ffa--;
                        }
                        if (u->ffb != 0) {
                            u->ffb--;
                        }
                        if (u->f110.bits.b4 != 0) {
                            if (!(u->f110.bits.b5) || u->field_104 != 0.0f || u->ffb != 0
                                || (u->owner != 0 && !(u->owner->f110.bits.b30))) {
                                u->f110.bits.b4 = 0;
                            }
                        }
                        if ((unsigned int)g_game->ticks % 30 == 0) {
                            int v = u->field_108 * 100 / u->type->f1fa;
                            if (v < 0) {
                                v = 0;
                            }
                            if (v > 100) {
                                v = 100;
                            }
                            u->ff7 = u->ff6;
                            u->ff6 = v;
                        }
                        Player* pl = u->player;
                        if (pl->f0 != 0) {
                            unsigned char k3 = pl->f73;
                            if (k3 == 1 || k3 == 2) {
                                if (g_game->mode->waterDoesDamage != 0
                                    && g_game->mode->waterDamage != 0
                                    && (unsigned int)g_game->ticks % 30 == 0 && u->pos.y.whole <= g_game->seaLevel
                                    && !u->type->f241.bits.floats) {
                                    DamageUnit(0, u, g_game->mode->waterDamage, 0xb, 0);
                                }
                                if (u->type->f200 != 0 && u->field_108 < u->type->f1fa
                                    && (g_game->ticks & 7) == 0) {
                                    int n = u->type->f200 * 8;
                                    FUN_0041bd10(u, u, (float)(n / 30));
                                }
                                RunOrders(u);
                                FUN_0043bad0(u);
                                if (u->motion != 0) {
                                    u->motion->UpdateMotion(u);
                                    UpdateUnitHeight(u);
                                }
                            }
                        }
                        if (u->f110.bits.b14) {
                            KillUnit(u, u->ff5);
                        }
                    }
                    u = (Unit*)((char*)u + 0x118);
                }
                if (g_game->f2a44 & 1) {
                    if (p->f0 != 0) {
                        unsigned char k4 = p->f73;
                        if (k4 == 1 || k4 == 2) {
                            SendUnitStates(p);
                        }
                    }
                }
            }
        }
    if (g_game->f14373.bits.b1) {
        if (!IsKeyDown(0xf9)) {
            g_game->f14371--;
            if (g_game->f14371 <= 0) {
                g_game->f14371 = 0x5a;
                FUN_0048d790();
                FUN_0041c2e0(0);
            }
        }
    }
}

// Writes one unit's state into a bit stream (the writer of 0x415c10, the read
// counterpart is 0x48b3f0). The unit's type index (+0xa6) goes out in a bit
// count taken from g_game+0x14393, and a unit with none of that is done.
// The "advance one bit" tail is the tail of WriteBits's fast path written
// out by hand, as in the matched 0x44f4a0, and it is the store through
// stream->data that makes the compiler reload the unit's link pointer and test
// it a second time. `!= 0.0f` is what MSVC 5 turns into the fcomp / C3 test
// the original uses, and writing the conditional value as `!link ? 0 : ...`
// (not `link ? ... : 0`) is what lays the zero arm out ahead of the load; the
// `& 0xffff` is redundant on an unsigned short but the original keeps it.
// FUNCTION: 0x48b200
void __stdcall WriteUnitState(BitWriter* stream, Unit* u)
{
    extern Game* g_game;
    stream->WriteBits(u->field_a6, g_game->field_14393);
    if (u->field_a6 == 0)
        return;
    stream->WriteBits(u->field_108, 0x10);
    stream->WriteBits((u->field_104 != 0.0f) ? 1 - (int)(u->field_104 * -254.0f) : 0, 8);
    stream->WriteBits(u->field_10e, 8);
    stream->WriteBits(u->flags & 3, 2);
    if (u->owner) {
        stream->data[stream->bit] |= 1 << stream->index;
        stream->index++;
        if (stream->index == 0x20) {
            stream->index = 0;
            stream->bit++;
            if (stream->bit == stream->capacity) {
                stream->GrowBuffer();
            }
            stream->data[stream->bit] = 0;
        }
        stream->WriteBits((!u->owner ? 0 : u->owner->id) & 0xffff, 0xf);
        stream->WriteBits(u->f9, 8);
    } else {
        stream->index++;
        if (stream->index == 0x20) {
            stream->index = 0;
            stream->bit++;
            if (stream->bit == stream->capacity) {
                stream->GrowBuffer();
            }
            stream->data[stream->bit] = 0;
        }
        stream->WriteBits(u->pos.x, 0x20);
        stream->WriteBits(u->pos.y.value, 0x20);
        stream->WriteBits(u->pos.z, 0x20);
        stream->WriteBits(u->tail16.b, 0x10);
        stream->WriteBits(u->tail16.c, 0x10);
        stream->WriteBits(u->tail16.a, 0x10);
        if (u->motion)
            stream->WriteBits(u->motion->field_20, 0x20);
    }
}

// The per unit reader of a player's unit packet (the writer is the matched
// 0x48b200, the packet builder 0x48b710, the packet reader 0x48b920). The unit's
// type index comes in first in g_game+0x14393 bits: a type of 0 means "this unit
// is gone", and the unit is only flagged (0x4000) if it still had one. A type
// that differs from the unit's own makes it again through 0x4861d0, from a
// spawn record built on the stack: player 9, the new type, the unit's own id,
// its position and its tail. Then the block at +0x9e is cleared, and the
// unit's own state comes in: 16 bits at +0x108, the alpha byte (which also
// rescale u->f104 when it moved), the state byte at +0x10e (set through
// 0x48b090 once set and once cleared with its complement), two bits of
// colour, and then one bit that picks the rest: set, the two ids and one more
// byte go to 0x48ab70 as a seven byte order record and the function is done.
// Clear, a unit that already has an owner sends 0xff as the order's parameter,
// and the position (three 32 bit words) and the tail (three 16 bit words) are
// read; the position is converted from world units to the unit's 1/16 units
// against the fixed point pair at +0x7e, and if it, the tail owner and the
// colour are all unchanged the position is stored as it is, else the unit is
// unlinked, the position, the 1/16 pair and the colour are stored and the unit
// is re-registered (0x47cc30 then 0x4827b0). The last word in the packet goes
// to the int at +0x20 of whatever the unit's own first pointer (at +0x00, not
// the owner at +0x86 that the order record tests) points at.
//
// MATCHES (790 bytes). What the earlier 80.8% attempt had left as "one
// allocator decision" was really three, and each needed its own source shape:
//
// 1. The order record's byte at +6 is the COLOUR, not the complement of the
//    state byte. That is what puts the colour in ebx: the original's four uses
//    of ebx between 0x48b532 and 0x48b6b3 are the two `order.param2` byte
//    stores, the compare and the bitfield store, and nothing ever nots it, so
//    the not-ed state temp is dead at the push of 0x48b516 and leaves ebx free
//    for the colour. 0x48ab70's own comments agree: the byte at +6 is blended
//    into two bits of the type's byte, and the matched writer 0x48b200 sends
//    the colour as a two bit field of its own. `~state` there is 80.8% with
//    ebx, ebp and the frame all shuffled; `colour` is 82.7% with the right
//    registers.
// 2. The colour is read as `int` but written through `(unsigned short)` in the
//    bitfield store. A plain `int` local live across nine calls gets a frame
//    home in MSVC 5 (`mov ebx,eax` *and* `mov [esp+0x34],ebx`, then a reload at
//    the compare), and only the narrow store stops the home from existing. The
//    narrowing cast is free (no instruction) and the value keeps the original's
//    32 bit `cmp ebx, eax`, where an `unsigned short` local narrows it to
//    `cmp bx, ax` (94.3%). 82.7% -> 95.2%.
// 3. `fixed.x * 0x80000` instead of `(fixed.x << 19)`. Both lower to the same
//    `shl`, but the multiply gives the front end a different tree to walk, and
//    that is what produces the original's order: spill the pair to the frame,
//    take the high half out of the frame first, then the low half out of the
//    register. With the shift the high half is extracted five instructions
//    late (95.2% -> MATCH). Reading +0x7e as a two short struct and copying it
//    into a named local is also needed: it is the only way to get the high
//    half out of the frame at all.
// 4. The unit pointer read at the very end is +0x00, not the owner at +0x86
//    that the 0x48b590 order record tests. Two different pointers, and the
//    first attempt had both at +0x86.
// 5. In the first order record the source assigns `param` before `param2`, and
//    only that order leaves MSVC 5's `mov byte [esp+0x16], bl` after the two
//    read calls, where the reverse order hoists it above them.
//
// The rest of the earlier notes still hold and are worth keeping:
// - `int zero = 0` after the first read is what puts a 0 in ebp, and it is
//   compared with `cmp ax, bp` (16 bit) and stored and pushed from ebp. Any
//   spelling that lets the front end fold the 0 (a literal, or the declaration
//   before the call) gives `test ax,ax`, `mov [eax+0x10],0` and `push 0`.
// - `(u->tail.a & ~0xff) | u->player` is what the first argument of 0x4861d0
//   is: MSVC 5 keeps the mask in the loaded dword and only replaces the low
//   byte (`mov edx,[u+0x64] / ... / mov dl,[u+0xff] / push edx`), so the
//   argument really does carry the tail's upper three bytes into a parameter
//   that CreateUnitFromPacket uses as a player index (see "suspected bug" below).
// - `__int64 alpha = (unsigned int)reader->ReadBits(8)` gives the original's
//   `fild qword` with a constant 0 in the high word. A signed int gives a
//   `cdq` the original does not have and `unsigned __int64` does not convert
//   to float at all in VC5.
// - 0x48b090's first parameter is a byte: the state byte is spilled with a
//   byte store and pushed as a dword read back from that slot, and the second
//   call pushes ebx (only its low byte is not-ed), neither of which MSVC 5
//   does for an int parameter.
// - The two order records share the slot of the 64 bit alpha, and its type
//   byte is never stored: the original leaves the alpha's low byte there.
// - The frame is 0x20 with the saved registers at the bottom, so the order
//   record, the alpha, the tail and the spawn record all share those 32 bytes,
//   the state byte and the 1/16 pair share the first dead argument slot, and
//   the fixed point copy at +0x7e lives in the second one (MSVC 5 does overlay
//   locals on dead incoming arguments).
//
// Dead ends, all of which the fix above supersedes, kept so nobody repeats
// them: one shared order record instead of two (66.8%); reading the 1/16
// pair's y before its x (80.2%, and it moves the np frame slot with it); the
// colour compared through `u->f110.bits.colour` or written as a hand written
// `u->f110.all = (u->f110.all & ~3) | colour` (both give the xor/and/xor
// bitfield form, or `and al, 0xfc`); an `int colour` with an `& 3` at the read
// (89.8%, but the mask is emitted eagerly and the store then needs the
// xor/and/xor form); `unsigned short colour` as the local (94.3%, right
// registers but `cmp bx, ax`); the colour through a `static inline` read
// helper, which MSVC 5 inlines back to a range analysed int (66.4%); a union
// around the colour (61.5%); the fixed pair read as two shorts out of the unit
// (70.7%, two loads instead of one dword load and a spill).
// FUNCTION: 0x48b3f0
void __stdcall ReadUnitState(BitReader* reader, Unit* u)
{
    extern Game* g_game;
    unsigned short type = (unsigned short)reader->ReadBits(g_game->field_14393);
    int zero = 0;
    if (type == zero) {
        if (u->field_a6 != zero)
            u->f110.bits.b14 = 1;
        return;
    }
    if (u->field_a6 != type) {
        Spawn spawn;
        spawn.type = type;
        spawn.id = u->id;
        spawn.player = 9;
        spawn.pos = u->pos;
        spawn.tail.words = u->tail16;
        CreateUnitFromPacket((u->tail16.a & ~0xff) | u->playerIndex, &spawn);
    }
    ((Block*)u->block)->field_10 = zero;
    u->field_108 = reader->ReadBits(0x10);
    __int64 alpha = (unsigned int)reader->ReadBits(8);
    float scale = (float)alpha * DAT_004fd750;
    if (scale != u->field_104) {
        u->field_104 = scale;
        u->f110.bits.b13 = 1;
    }
    unsigned char state = (unsigned char)reader->ReadBits(8);
    u->SetStateBits(state, 1);
    u->SetStateBits((unsigned char)~state, zero);
    int colour = reader->ReadBits(2);
    if (reader->ReadBit()) {
        Order order;
        order.id1 = u->id;
        order.id2 = (unsigned short)reader->ReadBits(0xf);
        order.param = (unsigned char)reader->ReadSignedBits(8);
        order.param2 = (unsigned char)colour;
        ApplyAttachUnit(&order);
        return;
    }
    if (u->owner) {
        Order order;
        order.id1 = u->id;
        order.id2 = 0;
        order.param = 0xff;
        order.param2 = (unsigned char)colour;
        ApplyAttachUnit(&order);
    }
    Vec3 pos;
    pos.x = reader->ReadBits(0x20);
    pos.y.value = reader->ReadBits(0x20);
    pos.z = reader->ReadBits(0x20);
    Tail16 tail;
    tail.b = (unsigned short)reader->ReadBits(0x10);
    tail.c = (unsigned short)reader->ReadBits(0x10);
    tail.a = (unsigned short)reader->ReadBits(0x10);
    Point16 fixed = u->origin;
    Point16 np;
    np.x = (short)((pos.x - fixed.x * 0x80000 + 0x80000) >> 20);
    np.y = (short)((pos.z - fixed.y * 0x80000 + 0x80000) >> 20);
    if (np.x == u->cell.x && np.y == u->cell.y && colour == (u->f110.all & 3)) {
        u->pos = pos;
    } else {
        RemoveUnitFromMap(u);
        u->pos = pos;
        u->cell = np;
        u->f110.bits.colour = (unsigned int)(unsigned short)colour;
        AddUnitToMap(u);
        UpdateUnitLineOfSight(u);
    }
    u->f110.bits.b16 = 1;
    u->tail16 = tail;
    if (u->motion)
        u->motion->field_20 = reader->ReadBits(0x20);
}

// Builds one player's whole unit list as a single packet and sends it: a
// header of 0x2c, a 16-bit length (written as zero, patched at the end) and
// the game's tick counter, which is also stored in the player at +0x18. Then
// every unit of the player that is live, whose owner exists and whose owner
// accepts it, until the stream is half full; then a 16-bit -1 end marker, one
// more bit, the player's own unit (the one at ticks % the unit count) written
// in full, the length patched into the header as two bytes, and the packet.
//
// The operand order of the byte-wide add in the first length byte is the one
// register choice no source rewrite settled; the compiler state these two
// headers leave behind does (36 sets in tools/headers.py's list, this the
// smallest).
// FUNCTION: 0x48b710
void __stdcall SendUnitStates(Player* p)
{
    extern Game* g_game;
    BitWriter stream;
    stream.WriteBits(0x2c, 8);
    stream.WriteBits(0, 0x10);
    stream.WriteBits(g_game->ticks, 0x20);
    p->ticks = g_game->ticks;
    for (Unit* u = p->f67; u <= p->f6b; u++) {
        if (!(u->flags & 0x10000000))
            continue;
        if (!u->motion)
            continue;
        if (!u->motion->player->v7())
            continue;
        stream.WriteBits(u->id - u->player->field_6f, 0x10);
        stream.WriteBits(u->field_a6, g_game->field_14393);
        u->motion->player->WriteTo(&stream);
        // `>> 3`, not `/ 8`: that is the original's `add ecx, 7; sar ecx, 3`
        // with no sign fix-up.
        if (((stream.index + 7) >> 3) + stream.bit * 4 >= 0x200)
            break;
    }
    stream.WriteBits(-1, 0x10);
    int i = g_game->ticks % g_game->field_37ee6;
    stream.data[stream.bit] |= 1 << stream.index;
    stream.index++;
    if (stream.index == 0x20) {
        stream.index = 0;
        stream.bit++;
        if (stream.bit == stream.capacity)
            stream.GrowBuffer();
        stream.data[stream.bit] = 0;
    }
    WriteUnitState(&stream, &p->f67[i]);
    // The packet's length, little-endian at bytes 1 and 2, is only known here.
    // The `char` cast is what makes MSVC 5 narrow the first sum to a byte and
    // push the register unmasked; the second is pushed as a dword.
    stream.SetByteAt(
        1, (char)(((stream.index + 7) >> 3) + (unsigned char)stream.bit * 4));
    stream.SetByteAt(
        2, (char)((((stream.index + 7) >> 3) + stream.bit * 4) >> 8));
    BroadcastPacket(p->id, stream.data, ((stream.index + 7) >> 3) + stream.bit * 4);
    stream.FreeBuffer();
}

// Reads one player's unit packet (the writer is the matched 0x48b710): an 8 bit
// header byte, a 16 bit length, then the game tick (32 bits), which is stored
// in the player at +0x18. Then a list of 16 bit unit indices, terminated by
// -1: for each one the unit's type index is read in g_game+0x14393 bits and,
// when it differs from the unit's own type at +0xa6, the unit is rebuilt from
// a spawn record through 0x4861d0 (whose reader 0x48b3f0 builds the same
// record, byte for byte, at 0x48b44c). Each unit then reads its own state
// through the virtual reader at its owner's iface vtable +0x24. Afterwards
// every live owned unit is cleaned up (0x43dd20 and 0x48a870), and one more
// stream bit picks the unit (tick % unit count) handed to the per unit reader
// 0x48b3f0.
//
// `spawn` is declared inside the `if`: at function scope MSVC schedules the
// two struct copies (pos, tail) one after the other instead of interleaved.
// FUNCTION: 0x48b920
void __stdcall ReceiveUnitStates(Player* p, unsigned int* data)
{
    extern Game* g_game;
    BitReader reader;

    reader.data = data;
    reader.index = 0;
    reader.bit = 0;
    reader.ReadBits(8);
    reader.ReadBits(0x10);
    int tick = reader.ReadBits(0x20);
    p->ticks = tick;
    if (p->f67 == 0)
        return;

    short index = (short)reader.ReadBits(0x10);
    while (index != -1) {
        Unit* unit = &p->f67[index];
        unsigned short type = (unsigned short)reader.ReadBits(g_game->field_14393);
        if (unit->field_a6 != type) {
            Spawn spawn;
            spawn.id = unit->id;
            spawn.type = type;
            spawn.player = 9;
            spawn.pos = unit->pos;
            spawn.tail.dword = unit->tail32;
            CreateUnitFromPacket(unit->playerIndex, &spawn);
        }
        unit->motion->player->ReadFrom(&reader);
        index = (short)reader.ReadBits(0x10);
    }

    for (Unit* u = p->f67; u <= p->f6b;
         u = (Unit*)((char*)u + 0x118)) {
        if ((u->flags & 0x10000000) && u->motion) {
            u->motion->UpdateMotion(u);
            UpdateUnitHeight(u);
        }
    }

    if (reader.ReadBit())
        ReadUnitState(&reader, &p->f67[tick % g_game->field_37ee6]);
}
