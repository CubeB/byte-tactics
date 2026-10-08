// Decompiled by deepseek-v4.1-flash, deepseek-v4.1, DeepSeek V4.1 Flash, Opus, opus, claude-opus-5-5, Claude Sonnet 5.5, claude-sonnet-5-5, Sonnet, Sonnet 5.5, Haiku, space-bunny-free, Space Bunny Free, GPT-6, GPT-6-Luna, GPT-6.1-sol, mimo-v2.6-pro and mimo-v2.6-flash. Names are provisional.
//
// The terrain module: the map cells and their ground heights and features,
// whether a unit can sit at a position, adding units to and removing them from
// the map, the object grid and its visitors, and the feature picker for the
// build-site scan.
//
// Four of the module's functions match only in their own file's symbol
// context, so they stay in files of their own, each for a different reason:
// 0x47cc30 AddUnitToMap: the symbol count moves the owner index multiply onto
// the other operand.
// 0x47d0e0 RemoveUnitFromMap: the count clears bit 14 of g_game's id, which
// decides how the cell index multiply folds.
// 0x47d820 GetFootprintHeight: the count changes the operand order of the
// footprint mask load.
// 0x47e2d0 CanPlaceFootprintAt: the symbol ids move the def load and the
// feature lookup's operand order.
//
// The full windows.h stays: its declaration count decides how the cell index
// multiply folds.
// <stdlib.h>, <string.h> and <math.h> stay: their symbol ids decide operand
// orders in the cell multiply, the bounds guard and the explored arm.
// <vector> and <shlobj.h> stay for their symbol ids too: without them the
// register allocation of the grid visitors and of 0x47db70 changes.
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <shlobj.h>

struct Unit;

#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Cell {                           // 13 bytes per cell
    union {
        unsigned short field_0;         // +0x0, id of the unit owning the cell
        short field_0s;                 // +0x0
    };
    unsigned short field_2;             // +0x2
    unsigned char field_4;              // +0x4
    unsigned char field_5;              // +0x5, highest floor
    unsigned char field_6;              // +0x6, lowest floor
    unsigned char field_7;              // +0x7
    union {
        unsigned short feature;         // +0x8
        short field_8;                  // +0x8
    };
    unsigned char spotY;                // +0xa
    unsigned char spotX;                // +0xb
    unsigned char flags;                // +0xc
};

union Flags {
    struct {
        unsigned int unknown_0 : 26;
        unsigned int flag26 : 1;
        unsigned int flag27 : 1;
        unsigned int unknown_1 : 4;
    } bits;
    int all;
};

struct Player {
    int active;                         // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                 // +0x73
};

struct Owner {                          // 10 bytes: a grid cell's list head
    char unknown_0[6];
    Unit* first;                        // +0x6
};

struct UnitDef {
    char unknown_0[0x14a];
    Point origin;                       // +0x14a, footprint in map cells
    unsigned char* mask;                // +0x14e, one byte per footprint cell
    char unknown_152[0x1be - 0x152];
    short maxwaterdepth;                // +0x1be
    short minwaterdepth;                // +0x1c0
    char unknown_1c2[0x228 - 0x1c2];
    unsigned char maxslope;             // +0x228
    unsigned char maxwaterslope;        // +0x229
    char unknown_22a[0x22c - 0x22a];
    unsigned char draft;                // +0x22c
    char unknown_22d[0x22f - 0x22d];
    unsigned char mobile;               // +0x22f
    char unknown_230[0x241 - 0x230];
    unsigned int flags;                 // +0x241
};

struct Feature {
    char unknown_0[0xec];
    float weightA;                      // +0xec
    float weightB;                      // +0xf0
    char unknown_f4[0xfe - 0xf4];
    union {
        unsigned char flags;            // +0xfe
        unsigned short flags16;         // +0xfe
        struct {
            unsigned char unknown_6 : 6;
            unsigned char steep : 1;    // +0xfe, bit 6
        };
    };
};

struct Unit {                           // 0x118 bytes
    union {
        Unit* unit;                     // +0x0
        unsigned char* motion;         // +0x0
    };
    char unknown_4[0x26 - 0x4];
    unsigned int lastTick;              // +0x26, when the unit last moved
    char unknown_2a[0x6a - 0x2a];
    union {
        Vec3 position;                  // +0x6a
        struct {
            int x;                      // +0x6a
            char unknown_6e[4];
            int y;                      // +0x72
        };
    };
    Point pos;                          // +0x76
    char unknown_7a[4];
    Point size;                         // +0x7e, footprint in map cells
    Owner* owner;                       // +0x82
    int unknown_86;                     // +0x86
    Unit* child;                        // +0x8a
    Unit* next;                         // +0x8e
    UnitDef* def;                       // +0x92
    Player* player;                     // +0x96
    char unknown_9a[0xa8 - 0x9a];
    unsigned short id;            // +0xa8, the owner's own id
    char unknown_aa[0xff - 0xaa];
    unsigned char playerId;             // +0xff
    char unknown_100[0x10f - 0x100];
    unsigned char bit0 : 1;             // +0x10f
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    Flags flags;                        // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Grid {
    Owner* cells;                       // +0x0
    unsigned int width;                 // +0x4
    unsigned int height;                // +0x8
};

struct Game {
    char unknown_0[0x2a43];
    unsigned char player;               // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14253 - 0x1423b];
    int featureCount;                   // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    union {
        unsigned char* field_1426f;     // +0x1426f, the terrain bytes
        Feature* features;              // +0x1426f, the feature names
    };
    union {
        unsigned short* field_14273;    // +0x14273, one bit per player
        unsigned short* visibilityMask; // +0x14273
    };
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14281 - 0x14280];
    unsigned char losFlags;             // +0x14281
    char unknown_14282[0x14287 - 0x14282];
    Cell* cells;                        // +0x14287
    char unknown_1428b[0x1429f - 0x1428b];
    union {
        Grid grid;                      // +0x1429f
        struct {
            Owner* owners;              // +0x1429f
            unsigned int ownerCols;     // +0x142a3
        };
    };
    char unknown_142ab[0x142b7 - 0x142ab];
    Owner* defaultOwner;                // +0x142b7
    char unknown_142bb[0x14357 - 0x142bb];
    Unit* units;                        // +0x14357
    char unknown_1435b[0x38a47 - 0x1435b];
    int field_38a47;                    // +0x38a47
};

class MovementClass {
public:
    void* field_0;                      // +0x0
    short footprintX;                   // +0x4
    short footprintY;                   // +0x6
    short minHeight;                    // +0x8
    short maxHeight;                    // +0xa
    unsigned char wetSlope;             // +0xc
    unsigned char wetSlope2;            // +0xd
    unsigned char drySlope;             // +0xe
    unsigned char drySlope2;            // +0xf
    unsigned int width;                 // +0x10
    unsigned int height;                // +0x14
    unsigned int* data;                 // +0x18
    unsigned int lastTick;              // +0x1c
};

union Fixed {
    int value;
    int v;
    struct { short lo; short hi; } p;
};

struct MapSize_0047d2e0 {
    unsigned int width;
    unsigned int height;
    int Contains(unsigned int tx, unsigned int ty) { return tx < width && ty < height; }
};

struct ByteMap_0047d2e0 {
    unsigned char* data;
    MapSize_0047d2e0 size;
    int Contains(int x, int y) { return x < size.width && y < size.height; }
    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct Los_0047d2e0 {
    char unknown_0[0x7c];
    ByteMap_0047d2e0 explored;
};

struct Node_0047cb00 {
    char unknown_0[0x8e];
    Node_0047cb00* next;                // +0x8e
};

class SpatialBucket {
public:
    char unknown_0[6];
    union {
        Node_0047cb00* head;            // +6
        int field_6;                    // +6
    };
    void UnlinkUnit(Node_0047cb00* node);
    void PrependUnit(int param_1);
};

class ClaimFootprintVisitor {
public:
    virtual void ClaimFootprint(Unit* obj);
};

class Visitor_0047e750 {
public:
    virtual void Visit(Unit* obj) = 0;
};

class DamagedAllyCollector {
public:
    virtual void CollectDamagedAlly(Unit* unit);
};

struct Node_0047e570 {
    char unknown_0[0x8e];
    Node_0047e570* next;     // +0x8e
    char unknown_92[0xf9 - 0x92];
    char id;                 // +0xf9
};

struct Struct_0047e570 {
    char unknown_0[0x86];
    int field_86;            // +0x86
    Node_0047e570* list;     // +0x8a
};

struct Entry {
    Vec3 pos;                          // +0x0
    float val;                         // +0xc
};

#pragma pack(pop)

extern Game* g_game;
extern ClaimFootprintVisitor g_claimFootprintVtable[];
extern int DAT_0051e684;
extern int DAT_0051e688;
void __stdcall UpdateCellHeightRange(Point pos, Point size);
void __stdcall RefreshAllPassMaps(Point pos, Point size);
// Stays in a file of its own: see the note at its definition.
void __stdcall RemoveUnitFromMap(Unit* unit);

Cell* __stdcall GetMapCell(int x, int y);

// FUNCTION: 0x47c790
void __stdcall ClaimFootprintCells(Unit* obj)
{
    unsigned int f = obj->flags.all;
    unsigned char b = (unsigned char)((f & 0x8000000) >> 27);
    if (!(b & 1))
        return;
    f &= ~0x8000000;
    obj->flags.all = f;
    Point size = obj->size;
    if (f & 0x20000000) {
        Cell* cell = GetMapCell(obj->pos.x, obj->pos.y);
        int index = 0;
        for (int j = size.y; j > 0; j--) {
            for (int i = size.x; i > 0; i--) {
                if (obj->def->mask[index] & (obj->bit2 ? 2 : 4)) {
                    Unit* rec;
                    unsigned short id = cell->field_0;
                    if (id == 0)
                        goto a_write;
                    rec = &g_game->units[id];
                    if (rec->player->active == 0)
                        goto a_b;
                    if (rec->player->type != 3)
                        goto a_b;
                    rec->flags.all |= 0x8000000;
                    obj->flags.all |= 0x4000000;
                a_write:
                    cell->field_0 = obj->id;
                    goto a_next;
                a_b:
                    rec->flags.all |= 0x4000000;
                    obj->flags.all |= 0x8000000;
                a_next: ;
                } else if (cell->field_0 == obj->id) {
                    cell->field_0 = 0;
                }
                index++;
                cell++;
            }
            cell += g_game->width - size.x;
        }
    } else if ((obj->flags.all & 3) == 1) {
        Cell* cell = GetMapCell(obj->pos.x, obj->pos.y);
        for (int j = size.y; j > 0; j--) {
            for (int i = size.x; i > 0; i--) {
                Unit* rec;
                unsigned short id = cell->field_0;
                if (id == 0)
                    goto b_write;
                rec = &g_game->units[id];
                if (rec->player->active == 0)
                    goto b_b;
                if (rec->player->type != 3)
                    goto b_b;
                rec->flags.all |= 0x8000000;
                obj->flags.all |= 0x4000000;
            b_write:
                cell->field_0 = obj->id;
                goto b_next;
            b_b:
                rec->flags.all |= 0x4000000;
                obj->flags.all |= 0x8000000;
            b_next: ;
                cell++;
            }
            cell += g_game->width - size.x;
        }
    } else {
        Cell* cell = GetMapCell(obj->pos.x, obj->pos.y);
        for (int j = size.y; j > 0; j--) {
            for (int i = size.x; i > 0; i--) {
                Unit* rec;
                unsigned short id = cell->field_2;
                if (id == 0)
                    goto c_write;
                rec = &g_game->units[id];
                if (rec->player->active == 0)
                    goto c_b;
                if (rec->player->type != 3)
                    goto c_b;
                rec->flags.all |= 0x8000000;
                obj->flags.all |= 0x4000000;
            c_write:
                cell->field_2 = obj->id;
                goto c_next;
            c_b:
                rec->flags.all |= 0x4000000;
                obj->flags.all |= 0x8000000;
            c_next: ;
                cell++;
            }
            cell += g_game->width - size.x;
        }
    }
}

// Runs VisitObjectsInArea over an area with a stack visitor of ClaimFootprintVisitor, as
// ForceNeighborFootprintReclaim does for an object's area.
void __stdcall VisitObjectsInArea(Point pos, Point size, ClaimFootprintVisitor* visitor);

// FUNCTION: 0x47cac0
void __stdcall ClaimFootprintsInArea(Point pos, Point size)
{
    ClaimFootprintVisitor visitor;
    VisitObjectsInArea(pos, size, &visitor);
}

// FUNCTION: 0x47caf0
void FUN_0047caf0(void)
{
}

// FUNCTION: 0x47cb00
void SpatialBucket::UnlinkUnit(Node_0047cb00* node)
{
    Node_0047cb00** pp = &head;
    Node_0047cb00* n = head;
    while (n != node) {
        pp = &n->next;
        n = n->next;
    }
    *pp = node->next;
    node->next = 0;
}

// FUNCTION: 0x47cb40
void SpatialBucket::PrependUnit(int param_1)
{
    int temp = field_6;
    *(int*)((char*)param_1 + 0x8e) = temp;
    field_6 = param_1;
}

// FUNCTION: 0x47cb60
void __stdcall SetOwner(Unit* unit, Owner* owner)
{
    Owner* cur = unit->owner;
    if (owner != cur) {
        if (unit->unknown_86 == 0) {
            if (cur != 0) {
                Unit** pp = &cur->first;
                while (*pp != unit)
                    pp = &(*pp)->next;
                *pp = unit->next;
                unit->next = 0;
            }
            unit->next = owner->first;
            owner->first = unit;
        }
        unit->owner = owner;
    }
}

// Detaches a unit from its owner: RemoveUnitFromMap first, then (unless +0x86 is
// set) unlinks it from the owner's list (head at owner +0x6, link at unit
// +0x8e) and clears the owner. The reverse of 0x47cb60.
void __stdcall RemoveUnitFromMap(Unit* unit);

// FUNCTION: 0x47cbd0
void __stdcall ClearFootprintAndUnlink(Unit* unit)
{
    RemoveUnitFromMap(unit);
    if (unit->unknown_86 == 0) {
        Owner* cur = unit->owner;
        if (cur != 0) {
            Unit** pp = &cur->first;
            while (*pp != unit)
                pp = &(*pp)->next;
            *pp = unit->next;
            unit->next = 0;
        }
    }
    unit->owner = 0;
}

// Can a unit's footprint stand on the map cell `cell`? The guards are the map
// bounds, then the line-of-sight tests (the player's bit in the shared
// visibility mask, then either the explored byte map or the mask again,
// depending on flag 2 of g_game+0x14281), then a walk of the footprint cells
// that accumulates the build cost into DAT_0051e688 and the height envelope
// into the returned DAT_0051e684.
extern int DAT_0051e684;
extern int DAT_0051e688;

int __stdcall GetCellHeight(Point* p);

struct Pos_0047d2e0 {
    Fixed x, y, z;
};

struct Position_0047d2e0 {              // 16.16 fixed point, only high words read
    short xFrac;
    short x;
    short yFrac;
    short y;
    short zFrac;
    short z;
};

// The helpers read the position through the six-short Position cast.
static inline int IsExplored_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fixed* hgt)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    if (los->explored.size.Contains(tx, ty) && los->explored.Get(tx, ty) != 0)
        return 1;
    return 0;
}

static inline int IsSeen_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fixed* hgt, unsigned int bit)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    // Declared after tx/ty: makes the multiply width-first.
    unsigned int w = los->explored.size.width;
    int r;
    if (!los->explored.size.Contains(tx, ty))
        r = 0;
    else
        r = (g_game->field_14273[w * ty + tx] & bit) != 0;
    return r;
}

// Takes the caller's x but derives ty and the width itself: no spelling in
// the caller gives the original's width-first multiply.
static inline unsigned short VisWord_0047d2e0(Los_0047d2e0* los, int tx,
    Position_0047d2e0* pos, Fixed* hgt)
{
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    unsigned int w = los->explored.size.width;
    return g_game->field_14273[w * ty + tx];
}

// Blocked_ and Terrain_ stay early-return helpers.
static int Blocked_0047d2e0(Cell* c)
{
    unsigned short v = c->field_8;
    if (v == 0xffff)
        return 0;
    if (v < 0xfffb) {
        if ((int)v >= g_game->featureCount)
            return 1;
        return (g_game->field_1426f[v * 0x100 + 0xfe] >> 6) & 1;
    }
    if (v != 0xfffe)
        return 1;
    Cell* ref = c - (c->spotY * g_game->width + c->spotX);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return (g_game->field_1426f[v2 * 0x100 + 0xfe] >> 6) & 1;
}

static unsigned char* Terrain_0047d2e0(
Cell* c)
{
    if (c == 0)
        return 0;
    unsigned short v = c->field_8;
    if (v < 0xfffb) {
        if ((int)v >= g_game->featureCount)
            return 0;
        return g_game->field_1426f + v * 0x100;
    }
    if (v != 0xfffe)
        return 0;
    Cell* ref = c - (c->spotY * g_game->width + c->spotX);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return g_game->field_1426f + v2 * 0x100;
}

// FUNCTION: 0x47d2e0
int __stdcall CanBuildAt(UnitDef* unit, Point cell, short type, Los_0047d2e0* los)
{
    int ok;
    DAT_0051e684 = 0;
    DAT_0051e688 = 0;
    Point origin = unit->origin;
    // The guard reads cell.x and cell.y directly.
    if (cell.x < 1 || cell.y < 1 || cell.x + origin.x >= g_game->width ||
        cell.y + origin.y >= g_game->height)
        return 0;
    int cols = origin.x;
    int x;
    int y;
    ok = 1;
    if (los != 0) {
        Pos_0047d2e0 pos;
        // A separate Fix local: it shares the dead los argument slot with the bit spill.
        Fixed hgt;
        pos.x.v = (origin.x + cell.x * 2) << 19;
        pos.z.v = (origin.y + cell.y * 2) << 19;
        hgt.v = GetCellHeight(&cell) << 16;
        x = pos.x.p.hi >> 5;
        y = (pos.z.p.hi - (hgt.p.hi >> 1)) >> 5;
        if (!los->explored.size.Contains(x, y))
            return 0;
        // Computed after the first Contains test.
        unsigned int bit = 1 << g_game->player;
        if ((VisWord_0047d2e0(los, x, (Position_0047d2e0*)&pos, &hgt) & bit) == 0)
            return 0;
        if ((g_game->losFlags & 2) == 2)
            ok = IsExplored_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt);
        else
            ok = IsSeen_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt, bit);
    }
    // The cell pointer is computed before the min6/max5 initialisers.
    Cell* c = &g_game->cells[cell.y * g_game->width + cell.x];
    unsigned char min6 = 0xff;
    unsigned char max5 = 0;
    unsigned char max5b = 0;
    int found80 = 0;
    int foundFE20 = 0;
    int index = 0;
    int row;
    int col;
    row = 0;
    if (origin.y > row) {
        // Rotated do/while with a positive bottom test.
        do {
            for (col = 0; col < cols; col++) {
                DAT_0051e688 += c->field_7;
                int m = unit->mask[index++];
                if (m & 8) {
                    if (c->field_6 < min6)
                        min6 = c->field_6;
                    if (c->field_5 > max5)
                        max5 = c->field_5;
                }
                if ((m & 0x10) && c->field_5 > max5b)
                    max5b = c->field_5;
                if ((m & 1) && (c->flags & 2) && ok)
                    return 0;
                if ((m & 6) && c->field_0s != 0 && c->field_0s != type && ok)
                    return 0;
                if (m & 0x20) {
                    if (Blocked_0047d2e0(c) != 0)
                        return 0;
                }
                if (m & 0x40) {
                    unsigned char* e = Terrain_0047d2e0(c);
                    if (e != 0 && (e[0xff] & 2))
                        return 0;
                }
                if (m & 0x80) {
                    found80 = 1;
                    unsigned char* e = Terrain_0047d2e0(c);
                    if (e != 0 && (e[0xfe] & 0x20))
                        foundFE20 = 1;
                }
                ++c;
            }
            row++;
            c = (g_game->width - ((int)cols)) + c;
        } while (row < origin.y);
    }
    if (found80 && !foundFE20)
        return 0;
    unsigned char r;
    if (max5 < min6) {
        r = g_game->seaLevel - unit->draft;
    } else {
        if (max5 - min6 > unit->maxslope)
            return 0;
        r = min6;
    }
    if (max5b > r)
        return 0;
    if (min6 < g_game->seaLevel - unit->maxwaterdepth)
        return 0;
    if ((max5 > max5b ? max5 : max5b) > g_game->seaLevel - unit->minwaterdepth)
        return 0;
    DAT_0051e684 = r;
    return 1;
}

// Caller, for whoever names this: the only caller is the thunk 0x47dac0, which
// forwards its own two arguments unchanged (`push arg2; push arg1; call`, so the
// last push is the callee's first parameter) and, when the scan succeeds, clears
// bit 2 of the byte at obj+0x10f, sets it from `flag & 1`, sets
// obj->[0x110] |= 0x8000000, calls 0x47c790(obj) and then 0x440a40 with the
// object's point at +0x76 and its point at +0x7e pushed by value. So arg1 is the
// object being placed and arg2 is a flag, as declared here.
//
// The windows.h include stays: without it both sums change operand order.
// FUNCTION: 0x47d970
int __stdcall IsFootprintClear(Unit* obj, int flag)
{
    short* pp = &obj->pos.x;                  // x end reads pos.x through the alias
    Point* q = &obj->pos;            // y end reads pos.y through this one
    Point* r = &obj->size;
    short xend = pp[0] + obj->size.x;
    short yend = q->y + r->y;
    // Stays a 4-byte Point copy, not two short locals.
    Point p = obj->pos;
    if (p.x < 1 || p.y < 1 || xend >= g_game->width || yend >= g_game->height)
        return 0;
    int width = g_game->width;
    unsigned char bit = flag ? 2 : 4;
    int n = 0;
    for (int y = p.y; y < yend; y++) {
        Cell* c = g_game->cells + y * width;
        for (int x = p.x; x < xend; x++) {
            // The post-increment stays inside the mask read, not at the loop bottom.
            if ((obj->def->mask[n++] & bit) && c[x].field_0 != 0 && c[x].field_0 != obj->id)
                return 0;
        }
    }
    return 1;
}

int __stdcall IsFootprintClear(Unit* obj, int flag);
void __stdcall ClaimFootprintCells(Unit* obj);
void __stdcall RefreshAllPassMaps(Point pos, Point size);

// FUNCTION: 0x47dac0
void __stdcall SetYardOpen(Unit* obj, int flag)
{
    if (IsFootprintClear(obj, flag)) {
        obj->bit2 = flag;
        obj->flags.all |= 0x8000000;
        ClaimFootprintCells(obj);
        RefreshAllPassMaps(obj->pos, obj->size);
    }
}

void __stdcall VisitObjectsInArea(Point pos, Point size, ClaimFootprintVisitor* visitor);
void __stdcall RefreshAllPassMaps(Point pos, Point size);

// The visitor is declared after the flag update so that it reuses obj's
// stack slot; size and pos go through locals (in that order) so that pos is
// loaded early, as in the original.
// FUNCTION: 0x47db20
void __stdcall ForceNeighborFootprintReclaim(Unit* obj)
{
    obj->flags.all |= 0x8000000;
    ClaimFootprintVisitor visitor;
    Point size = obj->size;
    Point pos = obj->pos;
    VisitObjectsInArea(pos, size, &visitor);
    RefreshAllPassMaps(obj->pos, obj->size);
}

// Suspected original bug, 0x47dd05: the cell's owner word is compared against
// the raw low 16 bits of the second argument, and that argument is never
// dereferenced anywhere in the function. So either the parameter really is an
// owner id dressed up as a pointer, or the original meant other->field_0.
int __stdcall CanBuildAt(UnitDef* unit, Point cell, short type, Los_0047d2e0* los);

// Returns non-zero when the cell's ground does not take a unit: water, a cliff
// edge, or a feature type whose name entry has the "steep" bit set.
static inline int SteepCell(Cell* c)
{
    unsigned short f = c->feature;
    if (f == 0xffff)
        return 0;
    if (f < 0xfffb) {
        if (f >= g_game->featureCount)
            return 1;
        return g_game->features[f].steep;
    }
    if (f != 0xfffe)
        return 1;
    c = c - (c->spotY * g_game->width + c->spotX);
    f = c->feature;
    if (f >= 0xfffb)
        return 0;
    return g_game->features[f].steep;
}

// FUNCTION: 0x47db70
int __stdcall CanPlaceUnitFootprint(UnitDef* unit, UnitDef* other, Point cell, int flags)
{
    Point fp = unit->origin;
    if (cell.x < 0 || cell.y < 0)
        return flags == 2;
    int fx = fp.x;
    if (fx + cell.x >= g_game->width)
        return flags == 2;
    if (fp.y + cell.y >= g_game->height)
        return flags == 2;
    if (!unit->mobile)
        return CanBuildAt(unit, cell, 0, 0);
    Cell* c = &g_game->cells[cell.y * g_game->width + cell.x];
    // Declared ahead of the assignments: places the tolerance load.
    int minHeight, maxHeight, stride;
    stride = g_game->width - fx;
    unsigned char tolerance = unit->maxslope;
    minHeight = g_game->seaLevel - unit->maxwaterdepth;
    maxHeight = g_game->seaLevel - unit->minwaterdepth;
    if (flags != 1)
        return 1;
    // `c += stride` stays in the increment expression: places `row++`.
    for (int row = 0; row < fp.y; row++, c += stride) {
        for (int col = 0; col < fx; col++, c++) {
            if (SteepCell(c))
                return 0;
            if (c->field_0 != 0 && c->field_0 != (unsigned short)other)
                return 0;
            if (c->field_6 < minHeight)
                return 0;
            if (c->field_5 > maxHeight)
                return 0;
            if (c->field_5 - c->field_6 > tolerance) {
                if (c->field_6 >= g_game->seaLevel)
                    return 0;
                if (c->field_5 - c->field_6 > unit->maxwaterslope)
                    return 0;
            }
        }
    }
    return 1;
}

// Snaps a 16.16 fixed-point world position to the centre of its grid cell
// (cells are 16 units, relative to the unit's origin in 8-unit steps), then
// sets the height from the cell. The two conversions are inlined helpers that
// take the origin and the position by value.
// GetFootprintHeight stays in a file of its own: see the note at its definition.
int __stdcall GetFootprintHeight(UnitDef* unit, Point cell);

// The origin stays the last parameter: it is read before the position.
static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

static inline void CellToWorld(Point origin, Point c, Vec3* v)
{
    v->x = (origin.x + c.x * 2) << 19;
    v->z = (origin.y + c.y * 2) << 19;
}

// FUNCTION: 0x47ddc0
void __stdcall SnapWorldPosToFootprint(UnitDef* unit, Vec3* pos)
{
    if (unit->mobile)
        return;
    Point cell = WorldToCell(*pos, unit->origin);
    CellToWorld(unit->origin, cell, pos);
    pos->y = GetFootprintHeight(unit, cell) << 16;
}

// Decides whether a unit with the given footprint can stand on one map cell:
// the cell's feature must not block it, a unit already standing there must not
// have moved more recently than the mover, and the cell's ground height and
// slope must be inside the unit's limits. Returns 0 when the cell is unusable,
// 1 when it is usable but the slope is a near miss, 3 when it is fully usable.
// FUNCTION: 0x47de60
int __stdcall GetPassMapCellValue(MovementClass* obj, Cell* cell)
{
    int blocked;
    unsigned short feature = cell->feature;
    if (feature == 0xffff) {
        blocked = 0;
    } else if (feature < 0xfffb) {
        if (feature >= g_game->featureCount) {
            blocked = 1;
        } else {
            blocked = (g_game->features[feature].flags >> 6) & 1;
        }
    } else if (feature != 0xfffe) {
        blocked = 1;
    } else {
        // The width stays an lvalue: a pointer to the field, not a copy of it.
        // A copy has two uses, so it must live in a register, and the one it
        // wins is ecx, which the original uses for spotX; that costs the
        // `mov ecx,[edx+0x14233]` and the register-to-register imul. As a
        // pointer the width folds into `imul eax,[edx+0x14233]` exactly as the
        // original has it, and the two spelled index uses still share one
        // block, which is the other half the original wants.
        int* width = &g_game->width;
        unsigned short f2 =
            (cell - (cell->spotY * *width + cell->spotX))->feature;
        if (0xfffb <= (cell - (cell->spotY * *width + cell->spotX))->feature) {
            blocked = 0;
        } else {
            blocked = (g_game->features[f2].flags >> 6) & 1;
        }
    }
    if (blocked)
        return 0;
    if (cell->field_0 != 0) {
        // A reference, not a pointer: gives the original's two-step unit load.
        Unit*& unit = g_game->units[cell->field_0].unit;
        if (!unit || unit->lastTick < obj->lastTick)
            return 0;
    }
    int minHeight = g_game->seaLevel - obj->minHeight;
    if ((int)cell->field_6 < minHeight)
        return 0;
    int maxHeight = g_game->seaLevel - obj->maxHeight;
    if ((int)cell->field_5 > maxHeight)
        return 0;
    unsigned char diff = cell->field_5 - cell->field_6;
    if (cell->field_6 < g_game->seaLevel) {
        if (diff > obj->drySlope2)
            return !(obj->drySlope < diff);
    } else {
        if (diff > obj->wetSlope2)
            return !(obj->wetSlope < diff);
    }
    return 3;
}

// This is the pathfinder's "can this rectangle be crossed" scan.
// Kept only for its effect on compiler state: without a header the match breaks.
// FUNCTION: 0x47dfc0
int __stdcall GetPassMapFootprintValue(MovementClass* obj, int x, int y, int w, int h)
{
    if (x < 0 || y < 0)
        return 0;
    if (x + w >= g_game->width)
        return 0;
    if (y + h >= g_game->height)
        return 0;
    int index = y * g_game->width + x;
    unsigned int result = 3;
    int row;
    Cell* cell = &g_game->cells[index];
    int rowStep = g_game->width - w;
    int minHeight = g_game->seaLevel - obj->minHeight;
    int maxHeight = g_game->seaLevel - obj->maxHeight;
    // Increments live in the for headers: schedules the pointer advance.
    for (row = 0; row < h; row++, cell += rowStep) {
        for (int col = 0; col < w; col++, cell++) {
            unsigned short feature = cell->feature;
            int blocked;
            if (feature == 0xffff) {
                blocked = 0;
            } else if (feature < 0xfffb) {
                if (feature >= g_game->featureCount)
                    blocked = 1;
                else
                    blocked = (g_game->features[feature].flags >> 6) & 1;
            } else if (feature != 0xfffe) {
                blocked = 1;
            } else {
                Cell* other =
                    cell - (cell->spotY * g_game->width + cell->spotX);
                unsigned short f2 = other->feature;
                if (f2 >= 0xfffb)
                    blocked = 0;
                else
                    blocked = (g_game->features[f2].flags >> 6) & 1;
            }

            if (blocked)
                return 0;
            if (cell->field_0 != 0) {
                // A reference, not a pointer: gives the original's two-step load.
                Unit*& unit = g_game->units[cell->field_0].unit;
                if (!unit || unit->lastTick < obj->lastTick)
                    return 0;
            }
            // Compared inline via the cast: a named local adds spill/reload pairs.
            if ((int)cell->field_6 < minHeight)
                return 0;
            if ((int)cell->field_5 > maxHeight)
                return 0;
            unsigned char diff = cell->field_5 - cell->field_6;
            if (cell->field_6 < g_game->seaLevel) {
                if (diff > obj->drySlope2) {
                    if (diff > obj->drySlope)
                        return 0;
                    if (result > 1)
                        result = 1;
                }
            } else {
                if (diff > obj->wetSlope2) {
                    if (diff > obj->wetSlope)
                        return 0;
                    if (result > 1)
                        result = 1;
                }
            }
        }
    }
    return result;
}

// Tests whether a unit whose footprint is footprintX x footprintY cells can sit
// at cell (x, y): its own rectangle plus the four strips that touch it (row
// above, column right, row below, column left), each one cell wider or taller so
// the corners are covered too. GetPassMapFootprintValue returns 3 for a rectangle that is
// completely free, 1 when something is in the way and 0 when the rectangle
// leaves the map, so the first failure is passed straight back to the caller
// while any later failure is reported as a plain 1.
int __stdcall GetPassMapFootprintValue(MovementClass* obj, int x, int y, int w, int h);

// The two footprint fields are copied into locals before the first call: that
// keeps them in registers across the five calls instead of loading each field
// again, and it is what puts the `h + 1` and `x - 1` values of the third and
// second calls into the two stack slots the last call reads back.
// FUNCTION: 0x47e1f0
unsigned int __stdcall GetPassMapFootprintValueWithEdgeStrips(MovementClass* obj, int x, int y)
{
    int h = obj->footprintY;
    int w = obj->footprintX;
    unsigned int r = GetPassMapFootprintValue(obj, x, y, w, h);
    if (r <= 1)
        return r;
    if (GetPassMapFootprintValue(obj, x - 1, y - 1, w + 1, 1) != 3)
        return 1;
    if (GetPassMapFootprintValue(obj, x + w, y - 1, 1, h + 1) != 3)
        return 1;
    if (GetPassMapFootprintValue(obj, x, y + h, w + 1, 1) != 3)
        return 1;
    return GetPassMapFootprintValue(obj, x - 1, y, 1, h + 1) == 3 ? 3 : 1;
}

// FUNCTION: 0x47e570
int __stdcall IsPadSlotFree(Struct_0047e570* p, int id)
{
    if (p->field_86 != 0) {
        return 0;
    }
    for (Node_0047e570* n = p->list; n != 0; n = n->next) {
        if (n->id == id) {
            return 0;
        }
    }
    return 1;
}

// Scans every grid cell overlapping the rectangle [pos, pos+size), calling
// visitor->ClaimFootprint for each object (and child) whose own rectangle overlaps.
// FUNCTION: 0x47e5c0
void __stdcall VisitObjectsInArea(Point pos, Point size, ClaimFootprintVisitor* visitor)
{
    // Declared apart from its assignment: keeps the sum from fusing.
    int sumy;
    Grid* grid = &g_game->grid;
    // Declaration order of the bounds is fixed: other orders change allocation.
    int xstart = -1 + (pos.x >> 3);
    int ystart = -1 + (pos.y >> 3);
    int sumx = pos.x + size.x;
    sumy = pos.y + size.y;
    int xend = (sumx >> 3) + 1;
    // Recomputed from pos.y + size.y, not shifted from sumy.
    int ye = ((pos.y + size.y) >> 3) + 1;
    for (int x = xstart; x <= xend; x++) {
        for (int y = ystart; y <= ye; y++) {
            if (x >= grid->width) {
                continue;
            }
            if (y >= grid->height) {
                continue;
            }
            for (Unit* o = grid->cells[grid->width * y + x].first; o != 0; o = o->next) {
                Point op = o->pos;
                Point os = o->size;
                if (pos.x < os.x + op.x && sumx > op.x && pos.y < os.y + op.y && sumy > op.y) {
                    visitor->ClaimFootprint(o);
                }
                for (Unit* c = o->child; c != 0; c = c->next) {
                    Point cs = c->size;
                    Point cp = c->pos;
                    if (pos.x < cs.x + cp.x && sumx > cp.x && pos.y < cp.y + cs.y && sumy > cp.y) {
                        visitor->ClaimFootprint(c);
                    }
                }
            }
        }
    }
}

static inline int Clamp_0047e750(int v, unsigned int size)
{
    if (v < size) {
        return v;
    }
    if (v < 0) {
        return 0;
    }
    return size - 1;
}

// FUNCTION: 0x47e750
void __stdcall VisitObjectsInRect(int x1, int y1, int x2, int y2, Visitor_0047e750* visitor)
{
    int cx1 = Clamp_0047e750(x1 >> 23, g_game->grid.width);
    int cy1 = Clamp_0047e750(y1 >> 23, g_game->grid.height);
    int cx2 = Clamp_0047e750(x2 >> 23, g_game->grid.width);
    int cy2 = Clamp_0047e750(y2 >> 23, g_game->grid.height);
    for (int y = cy1; y <= cy2; y++) {
        for (int x = cx1; x <= cx2; x++) {
            for (Unit* o = g_game->grid.cells[g_game->grid.width * y + x].first; o != 0; o = o->next) {
                if (o->position.x >= x1 && o->position.x <= x2 && o->position.z >= y1 && o->position.z <= y2) {
                    visitor->Visit(o);
                }
            }
        }
    }
}

// Sibling of 0x47e750, but the query is a point with a range instead of a
// rectangle: the cells the circle covers are walked and every object in them
// whose distance from the point is within the range is handed to the visitor
// (the one in 0x405d90 and its two siblings). The distance test is the fixed
// point one the game uses everywhere: only the high dword of each 64 bit product
// is kept, so the 32 bit add of the two terms can never overflow.
static inline int Clamp_0047e890(int v, unsigned int size)
{
    if (v < size) {
        return v;
    }
    if (v < 0) {
        return 0;
    }
    return size - 1;
}

// Four separate one-line helpers taking the point pointer: keeps x outranking pos.
static inline int ClampX_0047e890(Vec3* p, int range, unsigned int size)
{
    return Clamp_0047e890((p->x - range) >> 23, size);
}

static inline int ClampZ_0047e890(Vec3* p, int range, unsigned int size)
{
    return Clamp_0047e890((p->z - range) >> 23, size);
}

static inline int ClampX2_0047e890(Vec3* p, int range, unsigned int size)
{
    return Clamp_0047e890((p->x + range) >> 23, size);
}

static inline int ClampZ2_0047e890(Vec3* p, int range, unsigned int size)
{
    return Clamp_0047e890((p->z + range) >> 23, size);
}

// Pointer arguments, z difference first: written inline the code changes.
static inline int Dist2_0047e890(Vec3* b, Vec3* a)
{
    int dz = a->z - b->z;
    int dx = a->x - b->x;
    return (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
}

// FUNCTION: 0x47e890
void __stdcall VisitObjectsInRange(Vec3* pos, int range, DamagedAllyCollector& visitor)
{
    int cx1 = ClampX_0047e890(pos, range, g_game->grid.width);
    int cy1 = ClampZ_0047e890(pos, range, g_game->grid.height);
    int cx2 = ClampX2_0047e890(pos, range, g_game->grid.width);
    int cy2 = ClampZ2_0047e890(pos, range, g_game->grid.height);
    int range2 = (int)(((__int64)range * range) >> 32);
    for (int y = cy1; y <= cy2; y++) {
        for (int x = cx1; x <= cx2; x++) {
            for (Unit* o = g_game->grid.cells[g_game->grid.width * y + x].first; o != 0; o = o->next) {
                if (Dist2_0047e890(pos, &o->position) <= range2) {
                    visitor.CollectDamagedAlly(o);
                }
            }
        }
    }
}

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

void* __stdcall GetMapCellAtPosition(Vec3* pos);
unsigned short __stdcall GetCellFeature(void* cell);
int __stdcall RandomInt(int range);

// FUNCTION: 0x47ea40
int __stdcall PickRandomReclaimableResourcesInRadius(Vec3* center, Fixed radius, Vec3** out1, float* val1, Vec3** out2, float* val2)
{
    int countB = 0;
    int countA = 0;
    int max = radius.p.hi * radius.p.hi / 256;
    Entry* a = (Entry*)operator new(max * sizeof(Entry));
    Entry* b = (Entry*)operator new(max * sizeof(Entry));
    int half = radius.value / 2;
    Vec3 pos;
    for (pos.z = center->z - half; pos.z <= center->z + half; pos.z += 0x300000) {
        for (pos.x = center->x - half; pos.x <= center->x + half; pos.x += 0x300000) {
            void* cell = GetMapCellAtPosition(&pos);
            if (!cell)
                continue;
            unsigned short index = GetCellFeature(cell);
            if (index >= 0xfffb)
                continue;
            unsigned short fl = g_game->features[index].flags16;
            if (!(fl & 0x80))
                continue;
            if (!(fl & 0x100))
                continue;
            if (g_game->features[index].weightA != 0.0f) {
                b[countB].pos = pos;
                b[countB].val = g_game->features[index].weightA;
                countB++;
            }
            if (g_game->features[index].weightB != 0.0f) {
                a[countA].pos = pos;
                a[countA].val = g_game->features[index].weightB;
                countA++;
            }
        }
    }
    int found = 0;
    if (countB != 0) {
        int best = 0;
        float bestVal = 0.0f;
        for (int k = 0; k < 3; k++) {
            int i = RandomInt(countB);
            if (bestVal < b[i].val) {
                bestVal = b[i].val;
                best = i;
            }
        }
        **out1 = b[best].pos;
        *val1 = b[best].val;
        found = 1;
    } else {
        *out1 = 0;
        *val1 = 0;
    }
    if (countA != 0) {
        float bestVal = 0.0f;
        int best = 0;
        for (int k = 0; k < 3; k++) {
            int i = RandomInt(countA);
            if (bestVal < a[i].val) {
                bestVal = a[i].val;
                best = i;
            }
        }
        **out2 = a[best].pos;
        *val2 = a[best].val;
        found = 1;
    } else {
        *out2 = 0;
        *val2 = 0;
    }
    operator delete(b);
    operator delete(a);
    return found;
}

void __stdcall ClaimFootprintCells(Unit* obj);

// The visitor's slot: the class's virtual method, defined out of line (the
// vtable needs it under the class's own name).
// FUNCTION: 0x47ed30
void ClaimFootprintVisitor::ClaimFootprint(Unit* obj)
{
    ClaimFootprintCells(obj);
}
