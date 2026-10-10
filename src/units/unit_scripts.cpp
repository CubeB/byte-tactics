// Decompiled by DeepSeek V4.1 Flash, Opus, space-bunny-free, Claude Opus 5.5, deepseek-v4.1, GPT-6, deepseek-v4.1-flash, Space Bunny Free and claude-opus-5-5. Names are provisional.

// Must stay: decides which register GetOrderCursor's GetFeature results use.
#include <windows.h>
// Must stay: GetPieceOffset's main path depends on the symbols it declares.
#include <string.h>

#include "../util/vec3.h"

static inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

#include "cob_script.h"

// Unused here: these headers take the symbol ids that keep
// GetPieceOffset (0x43def0) and the piece position functions after it matching (docs/c2-regalloc.md).
#include "../map/mission.h"
#include "../sound/sound.h"


struct Unit_0043e490;
struct Feature_0043e490;

#pragma pack(push, 1)
struct UnitDef_0043db50 {
    char unknown_0[0x170];
    short field_170;                   // +0x170
    char unknown_172[0x22c - 0x172];
    unsigned char draft;               // +0x22c
};

struct Game {
    char unknown_0[0x2a42];
    unsigned char localPlayer;    // +0x2a42
    unsigned char playerIndex;    // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int mapWidthTiles;  // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    int featureCount; // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature_0043e490* features;        // +0x1426f
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x37efa - 0x14280];
    int multiplayer; // +0x37efa
};

struct Object {
    char unknown_0[0x6a];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x9a - 0x76];
    CobScript* script;                 // +0x9a
};

struct Unit {
    char unknown_0[0x70];
    short field_70;                    // +0x70
    char unknown_72[0x92 - 0x72];
    UnitDef_0043db50* type;            // +0x92
    char unknown_96[0x9a - 0x96];
    CobScript* script;                 // +0x9a
    char unknown_9e[0x10a - 0x9e];
    int state;                         // +0x10a
    char unknown_10e[0x110 - 0x10e];
    unsigned int flags;                // +0x110

    int CanLoad(Unit_0043e490* other);
    int CanReclaim(Unit_0043e490* other);
    int CanRepair(Unit_0043e490* other);
};
#pragma pack(pop)

class Class_0043db50 {
public:
    void UpdateSfxOccupy(Unit* unit);
};

extern Game* g_game;

// FUNCTION: 0x43db50
void Class_0043db50::UpdateSfxOccupy(Unit* unit)
{
    int seaLevel = g_game->seaLevel;
    int y = unit->field_70;
    int newState = unit->state;
    if ((unit->flags & 3) == 1 || (unit->flags & 3) == 2) {
        if (y > seaLevel) {
            newState = 4;
        } else {
            if (y - seaLevel > -5)
                newState = 1;
            if (unit->type->draft + y == seaLevel)
                newState = 2;
            if (unit->type->field_170 + y < seaLevel)
                newState = 3;
        }
    } else {
        newState = 0;
    }
    if (unit->state != newState) {
        unit->script->StartScriptWithArgs("setSFXoccupy", 0, 1, 1, newState, 0, 0, 0);
        unit->state = newState;
    }
}

void __stdcall RotateByAngles(Vec3* in, Vec3* out, short* angles);

struct Ptr_0043def0 {
    char unknown_0[0x10];
    int f10;                           // +0x10
    int f14;                           // +0x14
    int f18;                           // +0x18
};

#pragma pack(push, 2)
struct Item_0043def0 {
    Ptr_0043def0* p;                   // +0x00
    int x;                             // +0x04
    int y;                             // +0x08
    int z;                             // +0x0c
    short f10;                         // +0x10
    short f12;                         // +0x12
    short f14;                         // +0x14
    char unknown_16[0x32 - 0x16];
    Item_0043def0* next;               // +0x32
};

struct Block_0043def0 {
    int count;                         // +0x00
    char unknown_4[0x22 - 4];
    Item_0043def0 items[1];            // +0x22
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Object_0043def0 {
    char unknown_0[0x64];
    short f64;                         // +0x64
    short f66;                         // +0x66
    short f68;                         // +0x68
    char unknown_6a[0x9e - 0x6a];
    Block_0043def0* recs;              // +0x9e
};
#pragma pack(pop)

// Returns the world position of animation piece `index` of `obj` (with z
// negated, as the callers add it to obj->pos at +0x6a):
//
//   obj->recs (+0x9e) points to a table whose count is at +0x00 and whose
//   pieces start at +0x22, each 0x36 bytes. A piece has an offset pointer at
//   +0x00, x/y/z at +0x04/+0x08/+0x0c, its three angles at
//   +0x10/+0x12/+0x14 and a `next` at +0x32. `obj` has three base angles at
//   +0x64/+0x66/+0x68.
//
//   The base piece's offset (p->+0x10/+0x14/+0x18 plus x/y/z) seeds the
//   result; each node of the `next` chain rotates the result by its angles
//   (RotateByAngles), adding the object's base angles on the last node, then
//   adds its own offset.
// FUNCTION: 0x43def0
Vec3 __stdcall GetPieceOffset(Object_0043def0* obj, int index)
{
    if (obj == 0 || obj->recs == 0) {
        Vec3 v;
        v.x = 0;
        v.y = 0;
        v.z = 0;
        return v;
    }
    Block_0043def0* block = obj->recs;
    // One `out` filled in both arms, returned once; `result` outside the else.
    Vec3 out;
    Vec3 result;
    if (index < 0 || index >= block->count) {
        out.x = 0;
        out.y = 0;
        out.z = 0;
    } else {
        Item_0043def0* item = &block->items[index];
        result.x = item->p->f10 + item->x;
        result.y = item->p->f14 + item->y;
        result.z = item->p->f18 + item->z;
        for (Item_0043def0* n = item->next; n != 0; n = n->next) {
            short angles[3];
            angles[0] = n->f14;
            angles[2] = n->f10;
            angles[1] = n->f12;
            if (n->next == 0) {
                angles[0] = angles[0] + obj->f64;
                angles[2] = angles[2] + obj->f68;
                angles[1] = angles[1] + obj->f66;
            }
            RotateByAngles(&result, &result, angles);
            result.x += n->p->f10 + n->x;
            result.y += n->p->f14 + n->y;
            result.z += n->p->f18 + n->z;
        }
        out.x = result.x;
        out.y = result.y;
        out.z = -result.z;
    }
    return out;
}

// Must stay: its symbol count sets register allocation from 43e060 on.
#include <io.h>

// FUNCTION: 0x43e060
Vec3 __stdcall GetPiecePosition(Object* obj, int param)
{
    return obj->pos + GetPieceOffset((Object_0043def0*)obj, param);
}

struct Geom_0043e0b0 {
    char pad0[4];
    int count;                         // +0x04
};

#pragma pack(push, 1)
struct Piece_0043e0b0 {
    Geom_0043e0b0* geom;               // +0x00
    char pad4[0x22 - 4];
    Vec3* verts;                       // +0x22
};

struct Obj_0043e0b0 {
    char pad0[0x6a];
    Vec3 pos;                          // +0x6a
    char pad76[0x9e - 0x76];
    char* table;                       // +0x9e
};
#pragma pack(pop)

// Bounding-box centre of the vertices linked to a piece of the object's piece
// table (obj + 0x9e). The table holds a count at +0 and 0x36-byte piece
// records from +0x22; each record has a geometry pointer at +0 (its count is
// at +4) and a Vec3 vertex array at +0x22. The centre is the average of the
// per-component minima and maxima, offset by the object's world position at
// +0x6a. The minima and maxima start at 0, so a piece whose vertices are all
// positive on an axis keeps 0 as its minimum.
// FUNCTION: 0x43e0b0
void __stdcall GetPieceCenter(Obj_0043e0b0* obj, Vec3* out, int value)
{
    // Declared before the piece pointer: the register-resident ones zero first.
    int minx = 0, miny = 0, minz = 0;
    int maxx = 0, maxy = 0, maxz = 0;
    Piece_0043e0b0* p = (Piece_0043e0b0*)(obj->table + value * 0x36 + 0x22);
    Vec3* v = p->verts;
    int n = p->geom->count;
    // Written as while (n > 0) { ...; n--; } so the compiler rotates the loop.
    while (n > 0) {
        int x = v->x;
        if (x < minx) minx = x;
        if (x > maxx) maxx = x;
        int y = v->y;
        if (y < miny) miny = y;
        if (y > maxy) maxy = y;
        int z = v->z;
        if (z < minz) minz = z;
        if (z > maxz) maxz = z;
        v++;
        n--;
    }
    out->x = (maxx + minx) / 2 + obj->pos.x;
    out->y = (maxy + miny) / 2 + obj->pos.y;
    out->z = (maxz + minz) / 2 + obj->pos.z;
}

struct Rec_0043e180 {
    short a;                       // +0x00
    char pad[0x30];                // +0x02
    short b;                       // +0x32
    short c;                       // +0x34
};

#pragma pack(push, 2)
struct Obj_0043e180 {
    char pad0[0x64];
    short f64;                     // +0x64
    short f66;                     // +0x66
    short f68;                     // +0x68
    char pad1[0x9e - 0x6a];
    Rec_0043e180* recs;            // +0x9e
};
#pragma pack(pop)

struct Out_0043e180 {
    short x;
    short y;
    short z;
};

// Operand order (object field first, record second) must stay in both helpers.
static inline short SumY_0043e180(Obj_0043e180* obj, Rec_0043e180* q)
{
    return obj->f66 + q->c;
}

static inline short SumZ_0043e180(Obj_0043e180* obj, Rec_0043e180* q)
{
    return obj->f68 + q->b;
}

// Builds a three-short offset from the object's position fields (+0x64,
// +0x66, +0x68) and the animation record at obj->recs[index] (+0x9e): the
// record's +0x32/+0x34 words pair with y/z and the next record's +0 word
// with x.
// FUNCTION: 0x43e180
Out_0043e180 __stdcall GetPieceAngles(Obj_0043e180* obj, int index)
{
    Rec_0043e180* r = obj->recs;
    Out_0043e180 p;
    p.z = SumZ_0043e180(obj, &r[index]);
    p.y = SumY_0043e180(obj, &r[index]);
    p.x = obj->f64 + r[index + 1].a;
    return p;
}

// Asks the unit's script which piece the weapon (0..2) aims from
// ("QueryPrimary", "QuerySecondary", "QueryTertiary") and returns it.
// FUNCTION: 0x43e1e0
int __stdcall QueryWeaponPiece(Object* obj, unsigned char weapon)
{
    char* names[3] = { "QueryPrimary", "QuerySecondary", "QueryTertiary" };
    int piece = 0;
    obj->script->QueryScript(names[weapon], &piece, 0, 0, 0);
    return piece;
}

// Returns the world position of a weapon's aim piece (the unit's position
// plus the piece offset, like 0x43e060); a negative piece is first asked
// from the unit's script (0x43e1e0, inlined).
// FUNCTION: 0x43e240
void __stdcall GetWeaponPiecePosition(Object* obj, Vec3* out, unsigned char weapon, int piece)
{
    if (piece < 0)
        piece = QueryWeaponPiece(obj, weapon);
    *out = obj->pos + GetPieceOffset((Object_0043def0*)obj, piece);
}

// Returns the world position of a weapon's aim piece: it first asks the
// unit's script for the "AimFrom" piece, and if the script has none
// (-1), falls back to the "Query" piece. The unit's position is then
// added to the piece offset.
// FUNCTION: 0x43e2e0
void __stdcall GetAimFromPosition(Object* obj, Vec3* out, unsigned char weapon)
{
    char* names[3] = { "AimFromPrimary", "AimFromSecondary", "AimFromTertiary" };
    int piece = -1;
    obj->script->QueryScript(names[weapon], &piece, 0, 0, 0);
    // Each branch builds its own GetPieceOffset call.
    if (piece == -1) {
        char* qnames[3] = { "QueryPrimary", "QuerySecondary", "QueryTertiary" };
        int q = 0;
        obj->script->QueryScript(qnames[weapon], &q, 0, 0, 0);
        *out = obj->pos + GetPieceOffset((Object_0043def0*)obj, q);
    } else {
        *out = obj->pos + GetPieceOffset((Object_0043def0*)obj, piece);
    }
}

#pragma pack(push, 2)
struct Obj_0043e3c0 {
    char unknown_0[0x9a];
    CobScript* table;                  // +0x9a
};
#pragma pack(pop)

// Looks up the "SweetSpot" entry in the object's name table at +0x9a and
// passes the value found on to GetPieceCenter.
// FUNCTION: 0x43e3c0
void __stdcall GetSweetSpot(Obj_0043e3c0* obj, int param_2)
{
    int value = 0;
    obj->table->QueryScript("SweetSpot", &value, 0, 0, 0);
    GetPieceCenter((Obj_0043e0b0*)obj, (Vec3*)param_2, value);
}

// Asks the unit's script for its nano piece and returns that piece's world
// position (the unit's position plus the piece offset), like 0x43e060.
// FUNCTION: 0x43e400
void __stdcall GetNanoPiecePosition(Object* obj, Vec3* out)
{
    int piece = 0;
    obj->script->QueryScript("QueryNanoPiece", &piece, 0, 0, 0);
    *out = obj->pos + GetPieceOffset((Object_0043def0*)obj, piece);
}

// Returns 0 for the byte codes 5, 10 and 14, otherwise 1.
// FUNCTION: 0x43e470
int __stdcall OrderModeTakesTarget(unsigned char type)
{
    if (type != 5 && type != 10 && type != 14)
        return 1;
    return 0;
}

#pragma pack(push, 1)
struct Node_0043e490 {
    char unknown_0[0x111];
    union {
        unsigned int f111; // +0x111
        struct {
            unsigned int b0_7 : 8;
            unsigned int b8 : 1;
            unsigned int b9_31 : 23;
        } f111b;
    };
};

struct Flags_0043e490 {
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
    unsigned int b15_31 : 17;
};

struct UnitDef {
    char unknown_0[0x156];
    int f156; // +0x156
    char unknown_15a[0x1ee - 0x15a];
    Node_0043e490* f1ee; // +0x1ee
    char unknown_1f2[0x241 - 0x1f2];
    union {
        unsigned int f241; // +0x241
        Flags_0043e490 f241b;
    };
    union {
        unsigned int f245; // +0x245
        Flags_0043e490 f245b;
    };
};

struct Size_0043e490 {
    unsigned int width;  // +0x0
    unsigned int height; // +0x4
};

#include "../network/player.h"

struct Stats_0043e490 {
    char unknown_0[0x8c];
    float f8c; // +0x8c
    char unknown_90[0x98 - 0x90];
    float f98; // +0x98
};

struct Limits_0043e490 {
    char unknown_0[0xc0];
    float fc0; // +0xc0
    float fc4; // +0xc4
};

struct Unit_0043e490 {
    int moving; // +0x0
    char unknown_4[0x10 - 0x4];
    Node_0043e490* f10; // +0x10
    char unknown_14[0x48 - 0x14];
    Limits_0043e490* f48; // +0x48
    char unknown_4c[0x6a - 0x4c];
    char f6a[0x86 - 0x6a]; // +0x6a
    Unit_0043e490* f86; // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitDef* def;            // +0x92
    Player* player; // +0x96
    char unknown_9a[0xec - 0x9a];
    Stats_0043e490* fec; // +0xec
    char unknown_f0[0xfb - 0xf0];
    int ffb;             // +0xfb
    unsigned char owner; // +0xff
    char unknown_100[0x104 - 0x100];
    float f104; // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int f110; // +0x110
};

struct Pos_0043e490 {
    short xf, x;
    short yf, y;
    short zf, z;
};

#include "../map/cell.h"

struct Feature_0043e490 {
    char unknown_0[0xfe];
    unsigned char flags; // +0xfe
    char unknown_ff;
};
#pragma pack(pop)

Cell* __stdcall GetMapCellAtPosition(Pos_0043e490* pos);
int __stdcall WeaponCanReachPos(Unit_0043e490* unit, void* slot, Pos_0043e490* pos, int which);
int __stdcall WeaponCanReachUnit(Unit_0043e490* unit, Unit_0043e490* target, int which);

// The same test as IsPointVisible: is the map square under pos in sight of the
// local player? The width is read again through unit->player for the index;
// MSVC folds it back into the CSE'd load, which it then reloads into edx for
// the imul as the original does.
static inline int Visible(Unit_0043e490* unit, Pos_0043e490* pos) {
    Player* p = unit->player;
    int y, x;
    x = pos->x >> 5;
    y = (pos->z - (pos->y >> 1)) >> 5;
    return (unsigned int)x < p->exploredWidth && (unsigned int)y < p->exploredHeight &&
           0 != ((1 << g_game->playerIndex) & g_game->visibilityMask[x + y * unit->player->exploredWidth]);
}

// The feature on a map cell, as GetFeature in 0x4237d0.cpp but with the id in
// a local: 0xfffe marks a cell covered by a larger feature whose origin cell
// lies (offsetY, offsetX) cells back.
static inline Feature_0043e490* GetFeature(Cell* cell) {
    if (cell == 0)
        return 0;
    unsigned short id = cell->feature;
    if (id < 0xfffb) {
        if (id >= g_game->featureCount)
            return 0;
        return &g_game->features[id];
    }
    if (id != 0xfffe)
        return 0;
    id = (cell - (cell->offsetY * g_game->mapWidthTiles + cell->offsetX))->feature;
    if (id >= 0xfffb)
        return 0;
    return &g_game->features[id];
}

// Returns `result` from the calling function when the unit's def has `mask`
// set and pos holds a visible feature flagged 0x80. A statement macro in the
// usual do/while(0) form: the loop emits no code, but it is load-bearing for
// register allocation. Without it (the same if-statement written out at each
// site) case 2 loads unit into ebx before the target test instead of after it,
// and the extra reload block costs 20 bytes (85.0%).
#define RECLAIM_CHECK(def, unit, pos, mask, result)                  \
    do {                                                             \
        if (((def)->f245 & (mask)) && Visible((unit), (pos))) {      \
            Feature_0043e490* f = GetFeature(GetMapCellAtPosition(pos));     \
            if (f && (f->flags & 0x80))                              \
                return (result);                                     \
        }                                                            \
    } while (0)

static inline int Selectable(Unit_0043e490* t) {
    return t && t->owner == g_game->localPlayer && (t->f110 & 0x20) && t->f104 == 0.0f && t->ffb == 0 &&
           (t->f86 == 0 || (t->f86->f110 & 0x40000000));
}

// Returns the cursor/action code for an order of type
// `mode` given by `unit` on `target` / `pos`; GetOrderType is the sibling that
// returns the action's name.
// FUNCTION: 0x43e490
int __stdcall GetOrderCursor(unsigned char mode, Unit_0043e490* unit, Unit_0043e490* target,
                           Pos_0043e490* pos) {
    // Declared friendly, enemy, then def (a real local); g_game read directly.
    UnitDef* def;
    Node_0043e490* node;
    int friendly;
    int enemy;
    friendly = 0;
    enemy = 0;
    def = unit->def;
    if (target) {
        if (unit->player->allied[target->player->index])
            friendly = 1;
        else
            enemy = 1;
    }

    switch (mode) {
    case 3:
        if ((def->f245 & 0x10) && def->f1ee->f111b.b8)
            return 2;
        if (def->f245b.b4) {
            node = unit->f10;
            if (unit->moving)
                return 1;
            if (target)
                return WeaponCanReachUnit(unit, target, 0) ? 1 : 3;
            if (!WeaponCanReachPos(unit, unit->f6a, pos, 0) || (node->f111 & 0x20000))
                return 3;
            return 1;
        }
        break;
    case 9:
        return def->f245b.b6 ? 7 : 0x13;
    case 8:
        return ((Unit*)unit)->CanRepair(target) ? 6 : 0x13;
    case 7:
        if (!(def->f245 & 0x20) || !friendly)
            break;
        if ((def->f241 & 0x800) || !(target->def->f241 & 0x800))
            return 5;
        break;
    case 12:
        // Every check goes through the macro: an inline helper blows the budget.
        RECLAIM_CHECK(def, unit, pos, 0x400, 0xb);
        if (target && ((Unit*)unit)->CanReclaim(target))
            return 0xb;
        break;
    case 13:
        if ((def->f245 & 0x1000) && target && unit->player != target->player)
            return 4;
        break;
    case 6:
        if (!target || !((Unit*)unit)->CanLoad(target))
            break;
        return def->f241b.b11 ? 8 : 0xc;
    case 5:
        return def->f245b.b8 ? 0xd : 0x13;
    case 14:
        if (def->f156 == 0 || unit->moving == 0)
            break;
        return 0x10;
    case 4:
        if (def->f245b.b14) {
            // f48 pointer declared before the fec one.
            Limits_0043e490* l = unit->f48;
            Stats_0043e490* s = unit->fec;
            if (s->f8c < l->fc0 || s->f98 < l->fc4)
                return 3;
            return 1;
        }
        break;
    case 11:
        return 9;
    case 2:
        if (!(def->f245 & 0x80))
            break;
        RECLAIM_CHECK(def, unit, pos, 0x800, 0xa);
        if (!target || !unit->moving)
            return 0xe;
        if ((def->f245 & 0x1000) && enemy)
            return 4;
        if (enemy && ((Unit*)unit)->CanReclaim(target))
            return 0xb;
        if (friendly && ((Unit*)unit)->CanRepair(target) && target->f104 != 0.0f)
            return 6;
        if (friendly && ((Unit*)unit)->CanRepair(target))
            return 6;
        if ((def->f241 & 0x800) && (target->def->f241 & 0x200))
            return 0xd;
        if (((Unit*)unit)->CanLoad(target))
            return def->f241b.b11 ? 8 : 0xc;
        if ((def->f245 & 0x20) && friendly)
            return 5;
        return 0xe;
    case 1:
        if (g_game->multiplayer == 1) {
            if (Selectable(target))
                return 0xf;
            if (enemy)
                return 0x11;
            if (friendly)
                return 0x12;
            RECLAIM_CHECK(def, unit, pos, 0x800, 0x12);
            RECLAIM_CHECK(def, unit, pos, 0x400, 0x12);
            break;
        }
        // Self calls, not a goto loop: they become the original's jump to the top.
        if ((def->f245 & 0x10) && enemy)
            return GetOrderCursor(3, unit, target, pos);
        if ((def->f245 & 0x400) && enemy)
            return GetOrderCursor(0xc, unit, target, pos);
        if (target && ((Unit*)unit)->CanRepair(target) && target->f104 != 0.0f)
            return 6;
        if (Selectable(target))
            return 0xf;
        RECLAIM_CHECK(def, unit, pos, 0x800, 0xa);
        RECLAIM_CHECK(def, unit, pos, 0x400, 0xb);
        return def->f245b.b7 ? 0xe : 0x13;
    }
    return 0x13;
}
