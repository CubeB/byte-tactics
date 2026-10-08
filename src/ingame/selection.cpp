// Decompiled by deepseek-v4.1-flash, Opus, space-bunny-free, Sonnet, GPT-5.6-Terra, GPT-6.1-sol, deepseek-v4.1, mimo-v2.6-pro, Space Bunny Free, Claude Opus 5.5, GPT-6, Claude Sonnet 5.5, claude-opus-5-5, mimo-v2.6-flash and Haiku. Names are provisional.
// The selection module: the selected-unit list and box selection, the same-type
// and category selectors, the click and rubber-band picking, the order issue to
// the selection, and the squads (CTRL_F).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Unit;
void __stdcall CopyDwordIfNonNull(Unit**, Unit* const*);
namespace std {
// Non-template overload forwarding to the __stdcall 0x406c70: it pops its own arguments.
inline void _Construct(Unit** dest, Unit* const& src) { CopyDwordIfNonNull(dest, &src); }
}
#include <vector>

#pragma pack(push, 1)

// Positions are read through this union (whole part at +2): gives the movsx loads.
union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    } parts;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Point {
    int x;
    int y;
};

struct Rect {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

// The unit type table entry, the object at a unit's +0x92.
struct UnitType {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x156 - 0x40];
    int ids;                           // +0x156
    char unknown_15a[0x160 - 0x15a];
    short f160;                        // +0x160
    char unknown_162[2];
    short f164;                        // +0x164
    char unknown_166[2];
    short f168;                        // +0x168
    char unknown_16a[2];
    short f16c;                        // +0x16c
    char unknown_16e[2];
    short f170;                        // +0x170
    char unknown_172[2];
    short f174;                        // +0x174
    int field_176;                     // +0x176
    int field_17a;                     // +0x17a
    int field_17e;                     // +0x17e
    char unknown_182[0x245 - 0x182];
    unsigned char flags;               // +0x245
};

// The flags dword at +0x110 mixes plain masks with bitfields: a bitfield test
// is a shift, a masked test is an and, and a byte test is a byte load, so the
// union keeps every spelling the callers use.
union UnitFlags {
    unsigned int raw;                  // +0x110
    unsigned char byte;                // +0x110, the low byte
    struct {
        unsigned int bits0 : 4;
        unsigned int selected : 1;     // bit 4
        unsigned int done : 1;         // bit 5
        unsigned int state : 2;        // bits 6-7
        unsigned int high : 24;
    };
    struct {
        unsigned int bits0b : 4;
        unsigned int bit4 : 1;         // bit 4
        unsigned int bit5 : 1;         // bit 5
        unsigned int bit6 : 1;         // bit 6
        unsigned int bit7 : 1;         // bit 7
        unsigned int bits8 : 22;
        unsigned int bit30 : 1;        // bit 30
        unsigned int bit31 : 1;
    };
    struct {
        char unknown_110;
        char unknown_111;
        char unknown_112;
        unsigned char highByte;        // +0x113
    };
};

struct Unit {                          // 0x118 bytes
    char unknown_0[0x64];
    short angles[3];                   // +0x64, heading, aim and pitch
    Fixed pos_x;                       // +0x6a
    Fixed pos_y;                       // +0x6e
    Fixed pos_z;                       // +0x72
    char unknown_76[0x86 - 0x76];
    Unit* owner;                       // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType* def;                     // +0x92
    char unknown_96[0xa6 - 0x96];
    short unitDefIndex;                    // +0xa6, the type index
    unsigned short id;                 // +0xa8
    char unknown_aa[0xac - 0xaa];
    int group;                      // +0xac
    char unknown_b0[0xfb - 0xb0];
    int postTransferHoldoff;                      // +0xfb
    unsigned char player;              // +0xff
    char unknown_100[0x104 - 0x100];
    float buildLeft;                   // +0x104
    char unknown_108[0x110 - 0x108];
    UnitFlags flags;                   // +0x110
    char unknown_114[0x118 - 0x114];
};

// A 0x40-byte set of unit type ids, as in 0x488d30.
class UnitTypeSet {
public:
    int bits[16];
};

struct PlayerInfo {
    char unknown_0[0x95];
    unsigned char index;               // +0x95
};

#include "../network/player.h"

// The bit set here is bit 4 of the 12-bit group at 0x37ebe (see 0x41a120.cpp).
union Orders_37ebe {
    unsigned short flags;              // +0x37ebe
    unsigned char byte;                // +0x37ebe, the low byte
    struct {
        unsigned short bits0 : 4;
        unsigned short bit4 : 1;
        unsigned short bits5 : 11;
    };
};

struct Slot_0048cd80 {
    unsigned short field_0;            // +0x0
    int x;                             // +0x2
    int y;                             // +0x6
};

struct Name_0048d630 {
    char name[0x232];                  // +0x00
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63, 0x14b each
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;              // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x2c76 - 0x2a44];
    Point view;                        // +0x2c76
    char unknown_2c7e[0x2c92 - 0x2c7e];
    int rect_x1;                       // +0x2c92
    int rect_x2;                       // +0x2c96
    int rect_y1;                       // +0x2c9a
    int rect_y2;                       // +0x2c9e
    int rect_x3;                       // +0x2ca2
    int rect_y3;                       // +0x2ca6
    int pos;                           // +0x2caa
    char unknown_2cae[0x2cba - 0x2cae];
    unsigned short hoverUnitId;        // +0x2cba
    char unknown_2cbc[0x2cc3 - 0x2cbc];
    unsigned char orderMode;           // +0x2cc3
    char unknown_2cc4[0x142bb - 0x2cc4];
    Rect rect_142bb;                   // +0x142bb
    char unknown_142cb[0x1431f - 0x142cb];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x14357 - 0x14327];
    Unit* units;                       // +0x14357
    Unit* unitsEnd;                    // +0x1435b
    unsigned short* list;              // +0x1435f
    Slot_0048cd80* list2;              // +0x14363
    int count;                         // +0x14367
    int count2;                        // +0x1436b
    unsigned short focusUnitId;        // +0x1436f
    char unknown_14371[0x14377 - 0x14371];
    void** models;                     // +0x14377
    char unknown_1437b[0x37e27 - 0x1437b];
    Rect rect;                         // +0x37e27
    char unknown_37e37[0x37e9c - 0x37e37];
    unsigned short unitIndex;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    Orders_37ebe orders;               // +0x37ebe
    char unknown_37ec0[0x37f5f - 0x37ec0];
    Name_0048d630 playerNames[10];     // +0x37f5f
};
#pragma pack(pop)

extern Game* g_game;

void SelectStopOrder(void);
int __stdcall PopUntilNamedLayout(int force);
void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);
void __stdcall CenterCameraOnMapPosition(Vec3* p, int param_2);
UnitTypeSet* __stdcall GetCategoryMask(char* name);

// The unit's screen bounding box: the six shorts of the type's 3D box, the
// scroll offsets, and the game's rubber-band rect. Builds g_game->list (short
// array at +0x1435f, count at +0x14367) with the ids of every unit whose screen
// bounding box intersects the limit rect at +0x37e27 and which the local player
// can see.
//
// The unit type stores a 3D box in six shorts at +0x160..+0x174, four bytes
// apart: x1, y1, z1, x2, y2, z2. The screen box is (x + xOff - scrollX,
// z + zOff - scrollY - (y + yOff) / 2), so the top corner pairs x1 with y2
// and z1 and the bottom corner pairs x2 with y1 and z2.
struct Cell_0048bae0 {
    char unknown_0[4];
    unsigned char height;              // +0x4
    char unknown_5[8];
};

Cell_0048bae0* __stdcall GetMapCellAtPosition(Vec3* pos);
int __stdcall IsUnitVisibleToPlayer(Player* player, Unit* unit);

// FUNCTION: 0x48bae0
void CollectVisibleUnitIds(void)
{
    int count = 0;
    unsigned short* out = g_game->list;
    Rect* rect = &g_game->rect;
    Player* player = &g_game->players[g_game->playerIndex];
    for (Unit* u = g_game->units; u <= g_game->unitsEnd; u++) {
        if (u->unitDefIndex != 0) {
            UnitType* def = u->def;
            int ux = u->pos_x.parts.whole;
            int uy = u->pos_y.parts.whole;
            int camy = g_game->scrollY;
            int x1 = def->f160 + ux - g_game->scrollX;
            int y2 = def->f170 + uy;
            int z1 = def->f168 + u->pos_z.parts.whole - camy;
            int x2 = def->f16c + ux - g_game->scrollX;
            int y1 = def->f164 + uy;
            int z2 = def->f174 + u->pos_z.parts.whole - g_game->scrollY;
            if ((u->flags.raw & 3) != 1) {
                Cell_0048bae0* c = GetMapCellAtPosition((Vec3*)&u->pos_x);
                if (c != 0) {
                    int h = c->height;
                    if (y1 > h)
                        y1 = h;
                }
            }
            // Named locals, with top declared before right: keeps register use and the final adds.
            int left = x1 + 0x80;
            int top = z1 - (y2 >> 1) + 0x20;
            int right = x2 + 0x80;
            int bottom = z2 - (y1 >> 1) + 0x20;
            if (left <= rect->right && right >= rect->left
                && top <= rect->bottom && bottom >= rect->top) {
                if (u->player == g_game->playerIndex
                    || IsUnitVisibleToPlayer(player, u)) {
                    *out++ = u->id;
                    count++;
                }
            }
        }
    }
    g_game->count = count;
}

// Returns 1 when the unit's type id (+0xa8) is in the game's list of ids at
// +0x1435f (count at +0x14367).
// FUNCTION: 0x48bcb0
int __stdcall IsUnitVisible(char* unit)
{
    short* ids = (short*)g_game->list;
    int n = g_game->count;
    for (int i = 0; i < n; i++) {
        if (ids[i] == *(short*)(unit + 0xa8))
            return 1;
    }
    return 0;
}

// FUNCTION: 0x48bd00
void ClearSelection(void)
{
    for (Unit* u = g_game->units; u <= g_game->unitsEnd; u++)
        u->flags.raw &= 0xffffff2f;
    PopUntilNamedLayout(0);
}

// Scans the local player's unit list for the same "build finished and the
// builder is dead or gone" state the victory condition check in 0x48f250
// tests, marks each such unit with flag 0x10 and drops the current selection
// (+0x37e9c) to none, then issues the STOP order and sets order flag 0x10.
// FUNCTION: 0x48bd50
void SelectAllIdleUnits(void)
{
    Player* pl = &g_game->players[g_game->player];
    Unit* u = pl->unitsBegin;
    if (u <= pl->unitsEnd) {
        do {
            if ((u->flags.raw & 0x20) && u->buildLeft == 0.0f && u->postTransferHoldoff == 0
                && (u->owner == 0 || (u->owner->flags.raw & 0x40000000))) {
                u->flags.raw |= 0x10;
                g_game->unitIndex = 0;
            }
            u = (Unit*)((char*)u + 0x118);
        } while (u <= pl->unitsEnd);
    }
    SelectStopOrder();
    g_game->orders.byte |= 0x10;
}

// Builds a 512-entry bitmap of the type indices (+0xa6) of the local player's
// selected units (bit 4 of the unit flags at +0x110), then sets bit 4 on every
// unit of that player that is tagged (bit 5), whose float at +0x104 is 0.0f,
// whose dword at +0xfb is 0, that has either no attached unit at +0x86 or one
// whose flags carry 0x40000000, and whose type index is in the bitmap. It then
// drops the current selection (+0x37e9c) to none, issues the STOP order
// (SelectStopOrder) and sets order flag 0x10 at +0x37ebe. It is the mirror image
// of 0x48c9b0, which clears the same bit under the opposite conditions, and it
// repeats the loop 0x48bd50 does without the two-pass bitmap.

// The flags word at +0x110 is read whole in the second loop and through the
// bitfield view in the first, which is what the original does: bit 4 through
// the bitfield (shr 4; test cl, 1), bits 5, 30 and the store as a dword
// (test bl, 0x20; test dword [...], esi; or ebx, 0x10).
static inline unsigned int* FlagsPtr(Unit* u)
{
    return (unsigned int*)&u->flags;
}

// FUNCTION: 0x48be00
void SelectUnitsOfSameTypes(void)
{
    Player* player = &g_game->players[g_game->player];
    unsigned int selected[16];
    memset(selected, 0, sizeof(selected));
    {
        for (Unit* u = player->unitsBegin; u <= player->unitsEnd; u++) {
            if (u->flags.selected) {
                unsigned int v = u->unitDefIndex & 0xffff;
                selected[v >> 5] |= 1 << (v & 0x1f);
            }
        }
    }
    for (Unit* u = player->unitsBegin; u <= player->unitsEnd; u++) {
        unsigned int f = *FlagsPtr(u);
        if (f & 0x20) {
            if (u->buildLeft == 0.0f) {
                if (u->postTransferHoldoff == 0) {
                    if (u->owner == 0 || (*FlagsPtr(u->owner) & 0x40000000)) {
                        unsigned int v = u->unitDefIndex & 0xffff;
                        if (selected[v >> 5] & (1 << (v & 0x1f)))
                            *FlagsPtr(u) = f | 0x10;
                    }
                }
            }
        }
    }
    g_game->unitIndex = 0;
    SelectStopOrder();
    g_game->orders.bit4 = 1;
}

// Sets or clears flag 0x10 on every finished unit of the local player that
// stands in the line-of-sight bitmask named by the first argument, then clears
// the selected unit and re-sends the stop order.
// FUNCTION: 0x48bf30
void __stdcall SelectUnitsByCategory(char* name, int param_2)
{
    int* mask = (int*)GetCategoryMask(name);
    int player = g_game->player;
    // Head read through q, end read through p: two pointers fix the address forms.
    Player* p = &g_game->players[player];
    Player* q = &g_game->players[player];
    Unit* u = q->unitsBegin;

    for (; u <= p->unitsEnd; u++) {
        unsigned int flags = u->flags.raw;
        if ((flags & 0x20) && u->buildLeft == 0.0f && u->postTransferHoldoff == 0
            && (u->owner == 0 || (u->owner->flags.raw & 0x40000000))) {
            unsigned short bits = u->unitDefIndex;
            unsigned int bit = 1 << (bits & 0x1f);
            if (mask[bits >> 5] & bit) {
                u->flags.raw = flags | 0x10;
            } else if (param_2 == 0) {
                u->flags.raw = flags & ~0x10;
            }
        }
    }
    g_game->unitIndex = 0;
    SelectStopOrder();
    g_game->orders.bit4 = 1;
}

// Clears flag bit 5 (0x20) on every unit, then walks the selection list at
// +0x1435f and puts flag bit 4 (0x10) back on the local player's units that
// are still idle: bit 5 set, no work left at +0x104, no target at +0xfb, and
// either no link object at +0x86 or bit 0x40 set in its byte at +0x113. If any
// unit qualified, the single selected unit at +0x37e9c is cleared and bit
// 0x10 is set in the order byte at +0x37ebe to refresh the orders menu.
// FUNCTION: 0x48c030
void SelectAllVisibleUnits(void)
{
    int found = 0;
    for (Unit* u = g_game->units; u <= g_game->unitsEnd; u++)
        u->flags.raw &= 0xffffff2f;
    PopUntilNamedLayout(0);
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* u = &g_game->units[list[i]];
        unsigned int flags = u->flags.raw;
        if ((flags & 0x20) && u->buildLeft == 0.0f && u->postTransferHoldoff == 0
            && (u->owner == 0 || (u->owner->flags.highByte & 0x40))
            && u->player == g_game->player) {
            found = 1;
            u->flags.raw = flags | 0x10;
        }
    }
    if (found) {
        SelectStopOrder();
        g_game->unitIndex = 0;
        g_game->orders.byte |= 0x10;
    }
}

// FUNCTION: 0x48c150
void ClearUnitSelectFlags(void)
{
    for (Unit* u = g_game->units; u <= g_game->unitsEnd; u++)
        u->flags.raw &= 0xffffff3f;
}

static inline Unit* GetUnit_0048c190(unsigned short i)
{
    return i == 0 ? 0 : g_game->units + i;
}

// FUNCTION: 0x48c190
Unit* __stdcall FindNextSelectedUnit(Unit* unit, int dir)
{
    Player* p = &g_game->players[g_game->player];
    unsigned short x = unit == 0 ? 0 : unit->id;
    if (x < p->firstIndex || x > p->lastIndex) {
        x = p->firstIndex;
    }
    if (dir) {
        short j = x;
        while ((unsigned short)j != p->firstIndex) {
            --j;
            Unit* v = GetUnit_0048c190((unsigned short)j);
            if (v->flags.selected) {
                return v;
            }
        }
        j = p->lastIndex + 1;
        while ((unsigned short)j != x) {
            --j;
            Unit* v = GetUnit_0048c190((unsigned short)j);
            if (v->flags.selected) {
                return v;
            }
        }
    } else {
        short j;
        for (j = x; (unsigned short)j != p->lastIndex; j++) {
            Unit* v = GetUnit_0048c190((unsigned short)(j + 1));
            if (v->flags.selected) {
                return v;
            }
        }
        for (j = p->firstIndex - 1; (unsigned short)j != x; j++) {
            Unit* v = GetUnit_0048c190((unsigned short)(j + 1));
            if (v->flags.selected) {
                return v;
            }
        }
    }
    return 0;
}

// FUNCTION: 0x48c320
void MarkLocalVisibleUnits(void)
{
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* u = &g_game->units[list[i]];
        if (u->player == g_game->player)
            u->flags.state = 1;
    }
}

// Rubber-band unit selection: converts the drag rectangle in map units to
// screen coordinates, marks every unit of the local player inside it
// selected, then normalises the player's selection state and reports how
// many units ended up selected.
struct Select_0048c390 {
    char unknown_0[8];
    unsigned int flags;                 // +8
};

extern char s_SelectMultipleUnits_00508d8c[];

int __stdcall PlaySoundByName(char* msg, int a);

// FUNCTION: 0x48c390
int __stdcall SelectUnitsInBox(void* param_1)
{
    int found = 0;
    // Declared in the order ymin, xmin, ymax, xmax, reading g_game->scrollX/Y
    // directly (no scroll temporaries): fixes the order of the loads.
    int ymin = (g_game->rect_x1 - g_game->scrollX) + 0x80;
    int xmin = ((g_game->rect_y1 - (g_game->rect_x2 >> 1)) - g_game->scrollY) + 0x20;
    int ymax = (g_game->rect_y2 - g_game->scrollX) + 0x80;
    int xmax = ((g_game->rect_y3 - (g_game->rect_x3 >> 1)) - g_game->scrollY) + 0x20;
    int t;
    if (ymin > ymax) { t = ymin; ymin = ymax; ymax = t; }
    if (xmin > xmax) { t = xmin; xmin = xmax; xmax = t; }
    int toggle = (((Select_0048c390*)param_1)->flags >> 2) & 1;
    if (!toggle) {
        for (Unit* v = g_game->units; v <= g_game->unitsEnd; v++)
            v->flags.raw &= 0xffffff2f;
        PopUntilNamedLayout(0);
    }
    Player* p = &g_game->players[g_game->player];
    int count = 0;
    Unit* last;
    for (Unit* u = p->unitsBegin; u <= p->unitsEnd; u++) {
        if ((u->flags.raw & 0x20) && u->buildLeft == 0.0f && !u->postTransferHoldoff &&
            (!u->owner || (u->owner->flags.raw & 0x40000000))) {
            int sy = (u->pos_x.parts.whole - g_game->scrollX) + 0x80;
            int sx = (u->pos_z.parts.whole - g_game->scrollY - (u->pos_y.parts.whole >> 1)) + 0x20;
            if (sy >= ymin && sy <= ymax && sx >= xmin && sx <= xmax) {
                if (!toggle)
                    u->flags.selected = 1;
                else
                    u->flags.selected = !u->flags.selected;
                found = 1;
            }
            if (u->flags.selected) { last = u; count++; }
        }
    }
    g_game->unitIndex = 0;
    for (Unit* v = g_game->units; v <= g_game->unitsEnd; v++)
        v->flags.raw &= 0xffffff3f;
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* v = &g_game->units[list[i]];
        if (v->player == g_game->player)
            v->flags.state = 1;
    }
    if (found)
        g_game->orders.byte |= 0x10;
    if (!count)
        return 0;
    if (count == 1) {
        QueueUnitSpeech(last, 1, 0);
        return 1;
    }
    PlaySoundByName(s_SelectMultipleUnits_00508d8c, 0);
    return 1;
}

// Builds the four corners of an object's bounding box footprint at the box's low
// y, rotates each corner by the object's three angles, converts it to screen space
// and runs a point-in-polygon test against the rotated quad.
void __stdcall RotateByAngles(Vec3* in, Vec3* out, short* angles);
void __stdcall GetObjectBounds(void* obj, Vec3* lo, Vec3* hi, int arg);
int __stdcall PointInPolygon(Point* pts, int n, int px, int py);

// FUNCTION: 0x48c6a0
int __stdcall HitTestUnitScreenHull(Unit* obj, Point* p)
{
    void** models = g_game->models;
    Vec3 corners[4];
    Point pts[4];
    int i;
    // The three screen offsets stay in one Vec3: that local fixes the 0x88 frame.
    Vec3 o;
    Vec3 lo;
    Vec3 hi;
    GetObjectBounds(models[(unsigned short)obj->unitDefIndex], &lo, &hi, 0);
    corners[0].x = lo.x;
    corners[0].y = lo.y;
    corners[0].z = lo.z;
    corners[1].x = hi.x;
    corners[1].y = lo.y;
    corners[1].z = lo.z;
    corners[2].x = hi.x;
    corners[2].y = lo.y;
    corners[2].z = hi.z;
    corners[3].x = lo.x;
    corners[3].y = lo.y;
    corners[3].z = hi.z;
    o.x = obj->pos_x.value - (g_game->scrollX << 16);
    o.y = obj->pos_y.value;
    o.z = obj->pos_z.value - (g_game->scrollY << 16);
    short* angles = obj->angles;
    for (i = 0; i < 4; i++) {
        Vec3 v;
        RotateByAngles(&corners[i], &v, angles);
        pts[i].x = (short)((v.x + o.x) >> 16) + 0x80;
        pts[i].y = ((short)((o.z - v.z) >> 16) - ((short)((v.y + o.y) >> 16) >> 1)) + 0x20;
    }
    return PointInPolygon(pts, 4, p->x, p->y) != 0;
}

// Toggles bit 4 (0x10) of the currently selected unit at +0x110 when the
// command byte at param+8 has bit 2 set, or otherwise clears bits 4 and 7
// from every unit, restores bit 6 (0x40) on the local player's units in the
// selection list and sets bit 4 on the selected unit, then refreshes the
// orders menu through g_game+0x37ebe.
struct Param_0048c7f0 {
    char unknown_0[8];
    unsigned char flags;                // +0x8
};

// FUNCTION: 0x48c7f0
void __stdcall ClickSelectHoverUnit(Param_0048c7f0* param)
{
    Unit* unit = !g_game->hoverUnitId ? 0 : &g_game->units[g_game->hoverUnitId];
    if (unit == 0)
        return;
    if (unit->player != g_game->player)
        return;
    if (!(unit->flags.raw & 0x20))
        return;
    if (unit->buildLeft != 0.0f)
        return;
    if (unit->postTransferHoldoff != 0)
        return;
    if (unit->owner != 0 && !(unit->owner->flags.raw & 0x40000000))
        return;

    if (param->flags & 4) {
        unit->flags.selected = !unit->flags.selected;
        if (unit->flags.selected)
            QueueUnitSpeech(unit, 1, 0);
        g_game->unitIndex = 0;
        g_game->orders.flags |= 0x10;
        return;
    }

    for (Unit* u = g_game->units; u <= g_game->unitsEnd; u++)
        u->flags.raw &= 0xffffff2f;
    PopUntilNamedLayout(0);
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* u = &g_game->units[list[i]];
        if (u->player == g_game->player)
            u->flags.raw = u->flags.raw & 0xffffff7f | 0x40;
    }
    unit->flags.raw |= 0x10;
    QueueUnitSpeech(unit, 1, 0);
    g_game->orders.flags |= 0x10;
}

// FUNCTION: 0x48c9b0
void __stdcall DeselectIfIneligible(Unit* unit)
{
    unsigned int flags = unit->flags.raw;
    if (flags & 0x10) {
        if (!(flags & 0x20) || unit->buildLeft != 0.0f || unit->postTransferHoldoff != 0
            || (unit->owner != 0 && !(unit->owner->flags.raw & 0x40000000))) {
            unit->flags.raw = flags & ~0x10;
            g_game->unitIndex = 0;
            g_game->orders.byte |= 0x10;
        }
    }
}

// Empties the given list, then walks the local player's team list at
// g_game + player*0x14b + 0x1b63 and appends every unit whose bit 4 flag at
// +0x110 is set.
// FUNCTION: 0x48ca20
void __stdcall CollectSelectedUnits(std::vector<Unit*>* list)
{
    list->clear();
    Player* team = &g_game->players[g_game->player];
    for (Unit* u = team->unitsBegin; u <= team->unitsEnd; u++) {
        if (u->flags.selected)
            list->push_back(u);
    }
}

// The flags dword at +0x110 mixes plain masks (0x10000000, 0x4000) with a
// 1-bit bitfield at bit 4: the bitfield test is the only one the original
// emits as a shift (shr ecx,4 / test cl,1) instead of a masked test, so the
// union keeps both spellings available.
// The team array is indexed by an unsigned char, so its stride shows up as
// g_game + player*0x14b + 0x1b63 (0x14b = 330 + 1).
void __stdcall DrawOrderOverlays(Unit* unit, int mask, void* obj,
                           Unit** sel, int flag);

// FUNCTION: 0x48cc30
void __stdcall DrawSelectedUnitOrderOverlays(void* obj, Unit** sel)
{
    Player* team = &g_game->players[g_game->player];
    Unit* sel1unit = !g_game->unitIndex ? 0 : &g_game->units[g_game->unitIndex];
    Unit* sel2unit = !g_game->hoverUnitId ? 0 : &g_game->units[g_game->hoverUnitId];
    Unit* selunit = *sel;
    bool flag = (selunit && selunit->def->ids)
        || (sel1unit && sel1unit->def->ids)
        || (sel2unit && sel2unit->def->ids);
    for (Unit* u = team->unitsBegin; u <= team->unitsEnd; u++) {
        if ((u->flags.raw & 0x10000000) && !(u->flags.raw & 0x4000)) {
            if (u == *sel || u->id == g_game->unitIndex || u->id == g_game->hoverUnitId)
                DrawOrderOverlays(u, 0x1f, obj, sel, 1);
            else if (u->flags.selected)
                DrawOrderOverlays(u, 0x1f, obj, sel, 0);
            else if (flag)
                DrawOrderOverlays(u, 1, obj, sel, 1);
        }
    }
}

// Picks what lies under the view point: in the unit area, the listed unit
// with the lowest value whose box contains the point (branch A); otherwise
// the nearest slot within distance 2 (branch B).
int __stdcall PointInRect(Rect* rect, int x, int y);

static inline int FixMul(int a, int b)
{
    return (int)(((__int64)a * b) >> 16);
}

// FUNCTION: 0x48cd80
unsigned short __stdcall PickUnitUnderCursor(void)
{
    Point* p = &g_game->view;
    unsigned short result = 0;
    if (PointInRect(&g_game->rect, p->x, p->y)) {
        int best = 0x7fff0000;
        unsigned short* ids = g_game->list;
        if (ids == 0)
            return 0;
        // Walk the id list with the pointer itself (i++, ids++), not ids[i].
        for (int i = 0; i < g_game->count; i++, ids++) {
            Unit* u = &g_game->units[*ids];
            if (u->unitDefIndex != 0) {
                if (HitTestUnitScreenHull(u, p)) {
                    UnitType* def = u->def;
                    int v = FixMul(def->field_17a, 0x8000) + def->field_17e;
                    v = FixMul(v, def->field_176);
                    if (v < best) {
                        result = u->id;
                        best = v;
                    }
                }
            }
        }
    } else if (PointInRect(&g_game->rect_142bb, p->x, p->y)) {
        int best = 99999;
        Slot_0048cd80* s = g_game->list2;
        for (int i = g_game->count2; i > 0; i--) {
            // Both coordinates are read through p, dy first.
            int dy = s->y - p->y;
            int dx = s->x - p->x;
            int d = dx * dx + dy * dy;
            if (d < 4 && d < best) {
                best = d;
                result = s->field_0;
            }
            s++;
        }
    }
    return result;
}

// IssueOrderToSelection (0x48cf30) stays in selection_48cf30.cpp: it only
// matches at its old file's symbol count.

// Builds the local player's selected-unit list, drops the unit at
// g_game->units[g_game->hoverUnitId] from it, and returns an order code.
// With no other selected unit it returns 0xf when arg is 1 and that unit is
// finished and valid, else 0x13; otherwise it folds 0x13 with GetOrderCursor
// over the remaining units and returns the minimum.
static inline void Dummy(void) {}

int __stdcall GetOrderCursor(unsigned char type, Unit* unit, Unit* target, int* out);

class IntDynArray {
public:
    int EraseByValue(int value);
};

// FUNCTION: 0x48d220
int __stdcall ResolveCursorModeForSelection(char arg)
{
    Unit* target;
    if (g_game->hoverUnitId != 0)
        target = &g_game->units[g_game->hoverUnitId];
    else
        target = 0;

    std::vector<Unit*> vec;
    vec.clear();

    Player* player = &g_game->players[g_game->player];
    for (Unit* u = player->unitsBegin; u <= player->unitsEnd; u++) {
        if (u->flags.selected)
            // push_back, not insert(end(), u): picks the out-of-line insert overload.
            vec.push_back(u);
    }

    if (target != 0)
        ((IntDynArray*)&vec)->EraseByValue((int)target);

    if (vec.empty()) {
        if (arg == 1 && target != 0
            && target->player == g_game->player
            && target->flags.done
            && target->buildLeft == 0.0f
            && target->postTransferHoldoff == 0
            && (target->owner == 0 || (target->owner->flags.raw & 0x40000000)))
            return 0x0f;
        return 0x13;
    }

    int result = 0x13;
    for (std::vector<Unit*>::iterator it = vec.begin(); it != vec.end(); ++it) {
        int r = GetOrderCursor(g_game->orderMode, *it, target, &g_game->pos);
        if (r < result)
            result = r;
    }

    // Keep these Dummy() calls: they use inline budget so the _Destroy calls stay out of line.
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    return result;
}

// FUNCTION: 0x48d420
Unit* FindNextUnmarkedLocalUnit(void)
{
    Player* player = &g_game->players[g_game->player];
    Unit* u;
    for (u = player->unitsBegin; u <= player->unitsEnd; u++) {
        if (u->unitDefIndex != 0) {
            if (u->flags.state == 0)
                return u;
        }
    }
    for (Unit* p = g_game->units; p <= g_game->unitsEnd; p++) {
        p->flags.state = 0;
    }
    for (u = player->unitsBegin; u <= player->unitsEnd; u++) {
        if (u->unitDefIndex != 0) {
            if (u->flags.state == 0)
                return u;
        }
    }
    return 0;
}

// Picks a unit of the local player, scrolls the map to it and marks it (and
// every unit in g_game->list with the same owner) as selected. When the first
// search fails, the selected flag of every unit is cleared and the search is
// repeated, so search, clear and search form one inlined helper.
static inline Unit* PickUnit(Player* player)
{
    Unit* u = player->unitsBegin;
    while (u <= player->unitsEnd) {
        if (u->unitDefIndex != 0) {
            if (u->flags.state == 0)
                return u;
        }
        u++;
    }
    Unit* q = g_game->units;
    while (q <= g_game->unitsEnd) {
        q->flags.state = 0;
        q++;
    }
    u = player->unitsBegin;
    while (u <= player->unitsEnd) {
        if (u->unitDefIndex != 0) {
            if (u->flags.state == 0)
                return u;
        }
        u++;
    }
    return 0;
}

// FUNCTION: 0x48d4d0
void FocusNextLocalUnit(void)
{
    Unit* u = PickUnit(&g_game->players[g_game->player]);
    if (u == 0)
        return;
    g_game->focusUnitId = u->id;
    CenterCameraOnMapPosition((Vec3*)&u->pos_x, 1);
    CollectVisibleUnitIds();
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* unit = &g_game->units[list[i]];
        if (unit->player == g_game->player)
            unit->flags.state = 1;
    }
    u->flags.state = 1;
}

// Finds, in the current team's unit list, a finished unit (bit 5 of the flags
// dword at +0x110, build fraction at +0x104 down to 0, nothing queued at
// +0xfb, not attached to something that blocks it) whose type name matches
// the owning player's name, then centres the view on it and selects it.
// Layout notes for the neighbours 0x48d4d0 and 0x48d790 (same structs):
// - the 10 team records at g_game+0x1b63 are 0x14b bytes: +0x27 the player
//   (its +0x95 is the index), +0x67 / +0x6b the unit list bounds, +0x6a a
//   Pos for CenterCameraOnMapPosition, +0xa8 a word copied to g_game+0x1436f.
// - a unit is 0x118 bytes: +0x6a Pos, +0x86 a pointer to a struct with a
//   flags dword at +0x110, +0x92 the type (name at +0x20), +0xa6 a type
//   index, +0xfb an int, +0x104 the build fraction, +0x110 the flags dword.
// - the unit array at g_game+0x14357 is indexed by the same 0x118 stride.
void __cdecl ClearCameraFollowState(void);

// FUNCTION: 0x48d630
void __stdcall FocusCommander(int param_1)
{
    Player* team = &g_game->players[g_game->playerIndex];
    char* playerName = g_game->playerNames[team->info->index].name;
    Unit* last = team->unitsEnd;
    for (Unit* u = team->unitsBegin; u <= last; u++) {
        if (u->flags.done && u->buildLeft == 0.0f && u->postTransferHoldoff == 0
            && (u->owner == 0 || u->owner->flags.bit30)) {
            if (strcmp(u->def->name, playerName) == 0) {
                ClearCameraFollowState();
                CenterCameraOnMapPosition((Vec3*)&u->pos_x, 1);
                if (param_1 == 0)
                    return;
                SelectStopOrder();
                for (Unit* v = g_game->units; v <= g_game->unitsEnd; v++) {
                    v->flags.selected = 0;
                    v->flags.bit6 = 0;
                    v->flags.bit7 = 0;
                }
                PopUntilNamedLayout(0);
                u->flags.selected = 1;
                g_game->orders.bit4 = 1;
                return;
            }
        }
    }
}

// The match path in the second loop jumps to the shared `g_game->flags |= flag`
// tail with a goto; MSVC then duplicates that tail into the match block (reloading
// g_game after each store through a pointer), which is the original's layout.

// FUNCTION: 0x48d790
void __stdcall CycleSelection(void)
{
    Unit* found = 0;
    Player* t = &g_game->players[g_game->playerIndex];
    Unit* u = t->unitsBegin;
    Unit* last = t->unitsEnd;

    for (; u <= last; u++) {
        if (u->flags.raw & 0x20) {
            if (u->buildLeft == 0.0f && u->postTransferHoldoff == 0) {
                Unit* owner = u->owner;
                if (owner == 0 || (owner->flags.raw & 0x40000000)) {
                    if (found == 0) {
                        found = u;
                    }
                    if (u->flags.bit4) {
                        for (Unit* q = g_game->units;
                             q <= g_game->unitsEnd; q++) {
                            q->flags.raw &= 0xffffff2f;
                        }
                        PopUntilNamedLayout(0);
                        break;
                    }
                }
            }
        }
    }

    unsigned short flag = 0x10;
    if (found != 0) {
        u++;
        if (u <= t->unitsEnd) {
            do {
                if ((u->flags.raw & 0x20) && u->buildLeft == 0.0f && u->postTransferHoldoff == 0) {
                    Unit* owner = u->owner;
                    if (owner == 0 || (owner->flags.raw & 0x40000000)) {
                        u->flags.raw |= flag;
                        g_game->unitIndex = 0;
                        goto done;
                    }
                }
                u++;
            } while (u <= t->unitsEnd);
        }
        found->flags.raw |= flag;
    }
done:
    g_game->orders.flags |= flag;
}

void __stdcall SetUnitSquad(Unit* unit, int arg);

// FUNCTION: 0x48d920
void __stdcall CreateSquad(int param_1)
{
    Player* player = &g_game->players[g_game->player];
    for (Unit* u = player->unitsBegin; u <= player->unitsEnd; u++) {
        if (u->unitDefIndex != 0) {
            if (u->flags.selected) {
                SetUnitSquad(u, param_1);
            } else if (u->group == param_1) {
                SetUnitSquad(u, 0);
            }
        }
    }
}

static inline int TestBit(UnitTypeSet* set, unsigned short n)
{
    return set->bits[n >> 5] & (1 << (n & 0x1f));
}

// FUNCTION: 0x48d9a0
bool __stdcall SelectSquad(int id, int param_2)
{
    UnitTypeSet* setA = GetCategoryMask("CTRL_F");
    int cnt = 0;
    Player* player = &g_game->players[g_game->player];
    UnitTypeSet* setB = GetCategoryMask("CTRL_F");

    Unit* u;
    Unit* v;
    Unit* w;
    int found;
    // The outer and nested scans each read through their own Player pointer
    // local (q, r); sharing or dropping one changes the addressing.
    Player* q = &g_game->players[g_game->player];
    for (u = q->unitsBegin; u <= q->unitsEnd; u++) {
        if ((u->flags.byte & 0x20) && u->buildLeft == 0.0f && u->postTransferHoldoff == 0
            && (u->owner == 0 || (u->owner->flags.raw & 0x40000000))
            && u->group == id && TestBit(setB, u->unitDefIndex)) {
            {
                Player* r = &g_game->players[g_game->player];
                for (v = r->unitsBegin;
                     v <= r->unitsEnd; v++) {
                    if ((v->flags.raw & 0x20) && v->buildLeft == 0.0f && v->postTransferHoldoff == 0
                        && (v->owner == 0 || (v->owner->flags.raw & 0x40000000))
                        && v->group == id && (v->flags.raw & 0x80000000)) {
                        goto found_match;
                    }
                }
            }
            break;
        }
    }

    found = 0;
    goto select_units;
found_match:
    found = 1;
select_units:
    for (w = player->unitsBegin;
         w <= player->unitsEnd; w++) {
        if ((w->flags.raw & 0x20) && w->buildLeft == 0.0f && w->postTransferHoldoff == 0
            && (w->owner == 0 || (w->owner->flags.raw & 0x40000000))) {
            if (w->group == id) {
                if (found) {
                    if (TestBit(setA, w->unitDefIndex)) {
                        w->flags.raw &= ~0x10;
                    } else {
                        w->flags.raw |= 0x10;
                        cnt++;
                    }
                } else {
                    w->flags.raw |= 0x10;
                    cnt++;
                }
            } else if (param_2 == 0) {
                w->flags.raw &= ~0x10;
            }
        }
    }
    SelectStopOrder();
    g_game->unitIndex = 0;
    g_game->orders.bit4 = 1;
    return cnt > 0;
}

// Searches the local player's unit list for a live unit whose type is in the
// "CTRL_F" category and whose field at +0xac holds the given id. Returns 1 on
// the first match. The unit's own flags at +0x110 are read as a byte here,
// while the owner object's flags at the same offset are read as a dword
// (0x40000000), so both views of the union are used. The guard before the
// loop (u > end) and the test at the bottom are both present in the original.

// FUNCTION: 0x48dc30
int __stdcall SquadHasCtrlFMember(int id)
{
    UnitTypeSet* set = GetCategoryMask("CTRL_F");
    Player* p = &g_game->players[g_game->player];
    Unit* u = p->unitsBegin;
    Unit* end = p->unitsEnd;
    if (u > end)
        return 0;
    while (u <= end) {
        if ((u->flags.byte & 0x20) && u->buildLeft == 0.0f && u->postTransferHoldoff == 0
            && (u->owner == 0 || (u->owner->flags.raw & 0x40000000))
            && u->group == id && TestBit(set, u->unitDefIndex))
            return 1;
        u++;
    }
    return 0;
}

// Scans the local player's unit list for a unit whose +0x110 flags hold 0x20 and
// 0x80000000, whose +0x104 float is 0.0f, whose +0xfb is clear, whose +0x86
// owner (when there is one) has flag 0x40000000 and whose +0xac equals the
// argument. Returns 1 for the first such unit, else 0. The predicate is the
// negation of the one in the matched DeselectIfIneligible, which clears the unit's
// 0x10 flag when exactly this state no longer holds.

// FUNCTION: 0x48dd10
int __stdcall SquadHasReadyMember(int param_1)
{
    Player* player = &g_game->players[g_game->player];
    for (Unit* u = player->unitsBegin; u <= player->unitsEnd; u++) {
        unsigned int flags = u->flags.raw;
        if (flags & 0x20) {
            if (u->buildLeft == 0.0f) {
                if (u->postTransferHoldoff == 0) {
                    Unit* owner = u->owner;
                    if (owner == 0 || (owner->flags.raw & 0x40000000)) {
                        if (u->group == param_1) {
                            if (flags & 0x80000000)
                                return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

// std::vector<Unit*>::insert(iterator _P, const T& _X) from MSVC 5's <vector>
// (the two-argument overload, VECTOR line 146): remembers the offset, calls
// insert(_P, 1, _X), then returns begin() + offset. Element type is Unit*
// (0x406c00.cpp).
typedef std::vector<Unit*> Vec_0048ddc0;
typedef Vec_0048ddc0::iterator (Vec_0048ddc0::*InsertFn_0048ddc0)(
    Vec_0048ddc0::iterator, Unit* const&);

// FUNCTION: 0x48ddc0 ?insert@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAEPAPAUUnit@@PAPAU3@ABQAU3@@Z
InsertFn_0048ddc0 g_insert_0048ddc0 = &Vec_0048ddc0::insert;

struct MissionConditions {
    char unknown_0[0x40];
    int field_40;
    char unknown_44[0x40];
    int field_84;
    int field_88;

    MissionConditions();
};

// FUNCTION: 0x48df90
MissionConditions::MissionConditions()
{
    field_88 = 1;
    field_40 = 0;
    field_84 = 0;
}
