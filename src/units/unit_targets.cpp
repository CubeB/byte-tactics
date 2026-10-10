// Decompiled by Space Bunny Free, space-bunny-free, Opus, Sonnet, GPT-6.1-sol, DeepSeek V4.1 Flash, deepseek-v4.1-flash, deepseek-v4.1 and muse-spark-1.3-free. Names are provisional.
// The unit target and link code: the PropList debug string, the unit's
// weapon target entries (set, clear, query), the links in a unit's owner list
// (head at +0xa2), and the damage and paralyze events.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "cob_script.h"

class Unit;
class UnitRef;
class PathOrderAttach;
class Order;

// One virtual slot, called on the object a link belongs to.
class Listener_004896f0 {
public:
    virtual void Notify(int code);
};

#pragma pack(push, 1)
struct Point_004898b0 {
    short a;                           // +0x0
    short b;                           // +0x2
};

// What an entry's pointer at +0xc leads to.
struct Target_0048a1e0 {
    char unknown_0[0x68];
    int radius;                        // +0x68
    char unknown_6c[0x111 - 0x6c];
    unsigned int field_111;            // +0x111
};

struct Entry_004898b0 {                // 0x1c bytes
    Point_004898b0 point;              // +0x0
    char unknown_4[0xc - 4];
    Target_0048a1e0* target;           // +0xc
    char unknown_10[0x1b - 0x10];
    unsigned char flags;               // +0x1b
};

// The unit's type.
struct UnitType {
    char unknown_0[0x152];
    int count;                         // +0x152, entries of ids
    short* ids;                        // +0x156, the unit ids it can build
    char unknown_15a[0x186 - 0x15a];
    float energyCost;                  // +0x186
    float metalCost;                   // +0x18a
    char unknown_18e[0x192 - 0x18e];
    int sight;                         // +0x192
    char unknown_196[0x19e - 0x196];
    int field_19e;                     // +0x19e
    char unknown_1a2[0x1aa - 0x1a2];
    int f1aa;                          // +0x1aa, damage scale as 16.16 fixed point
    char unknown_1ae[0x1ba - 0x1ae];
    unsigned short field_1ba;          // +0x1ba
    char unknown_1bc[0x1ea - 0x1bc];
    int field_1ea;                     // +0x1ea
    char unknown_1ee[0x1fa - 0x1ee];
    unsigned int maxhp;                // +0x1fa
    char unknown_1fe[0x22f - 0x1fe];
    unsigned char mobile;              // +0x22f, zero means it has no speeds
    char unknown_230[0x241 - 0x230];
    unsigned int f241;                 // +0x241, bits 12 and 26
};

struct Vec3_0048a1e0 {
    int x;
    int y;
    int z;
};

// The object at the start of a unit.
struct Body_0048a1e0 {
    char unknown_0[8];
    Vec3_0048a1e0 offset;              // +0x8
    char unknown_14[0x20 - 0x14];
    int sight;                         // +0x20
    char unknown_24[0x2a - 0x24];
    int age;                           // +0x2a
};

struct Player_0048b090 {
    int active;                        // +0x0
    int id;                            // +0x4
    char unknown_8[0x73 - 0x8];
    unsigned char type;                // +0x73, 1 or 2 for a real player, 3 spreads damage
};

class Unit {
public:
    Body_0048a1e0* body;               // +0x0
    Entry_004898b0 entries[3];         // +0x4
    char unknown_58[0x5c - 0x58];
    Order* effect;                     // +0x5c
    char unknown_60[0x64 - 0x60];
    unsigned short hdg;                // +0x64
    unsigned short aim;                // +0x66
    unsigned short pitch;              // +0x68
    struct {
        int x;                         // +0x6a
        union {
            int y;                     // +0x6e, 16.16
            struct {
                unsigned short y_fraction;
                short roll;            // +0x70
            };
        };
        int z;                         // +0x72
    } pos;
    char unknown_76[0x92 - 0x76];
    UnitType* def;                     // +0x92
    Player_0048b090* player;           // +0x96
    CobScript* script;                 // +0x9a
    char unknown_9e[0xa2 - 0x9e];
    union {
        UnitRef* head;                 // +0xa2, the links of this unit's list
        PathOrderAttach* link;
    };
    unsigned short map;                // +0xa6
    unsigned short id;                 // +0xa8
    unsigned short fix_lo;             // +0xaa
    char unknown_ac[0xb8 - 0xac];
    unsigned short killCount;           // +0xb8, armour: divided by 5, capped at 5
    char unknown_ba[0xf0 - 0xba];
    void* last;                        // +0xf0
    char unknown_f4;
    unsigned char kind;                // +0xf5
    char unknown_f6[0xff - 0xf6];
    unsigned char teamId;              // +0xff
    char unknown_100[0x108 - 0x100];
    short hp;                          // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char state;               // +0x10e
    char unknown_10f;
    union {
        unsigned int flags;            // +0x110
        struct {
            char unknown_110;
            unsigned int field_111;    // +0x111
        };
    };
    char unknown_115[0x118 - 0x115];

    unsigned char ChooseWeapon();
};

struct MapInfo_0048a490;

struct Game {
    char unknown_0[0x2a42];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x14233 - 0x2a43];
    int mapWidthTiles;                 // +0x14233
    int mapHeightTiles;                // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    unsigned char* heightMap;          // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x14377 - 0x1435b];
    MapInfo_0048a490** unitModels;     // +0x14377
    char unknown_1437b[0x38a47 - 0x1437b];
    int gameTick;                      // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
char* __stdcall Translate(const char* text);
int GetTickRate();

// A helper, not inline or a float local: the product is spilled before GetTickRate.
static inline float spd(int v)
{
    return v * 1.52587890625e-05f;
}

// FUNCTION: 0x489280
char* __stdcall MakePropList(UnitType* obj)
{
    char* buf = (char*)GameAllocIgnoreTag("PropList", 0xc0);
    memset(buf, 0, 0xc0);
    char* p = buf;

    // Every append advances with `p += strlen(p) + 1`, not by wsprintfA's result.
    wsprintfA(p, "\n");
    p += strlen(p) + 1;

    wsprintfA(p, "%d", (int)obj->energyCost);
    p += strlen(p) + 1;

    wsprintfA(p, "%d", (int)obj->metalCost);
    p += strlen(p) + 1;

    wsprintfA(p, "%d", obj->field_1ea);
    p += strlen(p) + 1;

    wsprintfA(p, "\n");
    p += strlen(p) + 1;

    if (obj->mobile) {
        sprintf(p, "%.1f %s ", (double)GetTickRate() * spd(obj->sight) * 0.4,
                Translate("m/s"));
        p += strlen(p) + 1;

        sprintf(p, "%.2f %s", (double)GetTickRate() * spd(obj->field_19e) * 0.4,
                Translate("m/s/s"));
        p += strlen(p) + 1;

        sprintf(p, "%.0f %s",
                (double)GetTickRate() * obj->field_1ba * 0.0054931640625,
                Translate("deg/s"));
    } else {
        sprintf(p, "%s", Translate("N/A"));
        p += strlen(p) + 1;

        sprintf(p, "%s", Translate("N/A"));
        p += strlen(p) + 1;

        sprintf(p, "%s", Translate("N/A"));
    }

    return buf;
}

// Returns 1 when the unit's type lists `id` in its table of shorts.
// FUNCTION: 0x4894f0
int __stdcall UnitCanBuild(Unit* unit, short id)
{
    for (int i = 0; i < unit->def->count; i++) {
        if (unit->def->ids[i] == id)
            return 1;
    }
    return 0;
}

#include "unit_ref.h"

// FUNCTION: 0x489540
void UnitRef::LinkToUnit(Unit* o)
{
    if (o != 0 && o->map != 0) {
        owner = o;
        next = o->head;
        o->head = this;
        return;
    }
    owner = 0;
    next = 0;
}

// Unlinks the link from its owner's list (head at +0xa2); the counterpart of
// 0x489540, which links it in.
// FUNCTION: 0x489580
void UnitRef::UnlinkFromUnit()
{
    if (owner != 0) {
        UnitRef** link = &owner->head;
        while (*link != this)
            link = &(*link)->next;
        *link = next;
        owner = 0;
        next = 0;
    }
}

class PathOrderAttach {
public:
    Unit* owner;                       // +0x4
    PathOrderAttach* next;             // +0x8
    Listener_004896f0* value;          // +0xc

    PathOrderAttach(Unit* o, int v);
    virtual ~PathOrderAttach()
    {
        if (owner != 0) {
            PathOrderAttach** pp = &owner->link;
            while (*pp != this)
                pp = &(*pp)->next;
            *pp = next;
            owner = 0;
            next = 0;
        }
    }
    void SetUnit(Unit* o);
};

// A link that registers itself in its owner's list (head at +0xa2) when the
// owner is set and its flag at +0xa6 is non-zero. The only vtable slot is
// the scalar deleting destructor (0x489600), which unlinks it again.
// FUNCTION: 0x4895c0
PathOrderAttach::PathOrderAttach(Unit* o, int v)
    : value((Listener_004896f0*)v)
{
    if (o != 0 && o->map != 0) {
        owner = o;
        next = o->link;
        o->link = this;
        return;
    }
    owner = 0;
    next = 0;
}

// The compiler-generated scalar deleting destructor of PathOrderAttach
// (vtable 0x4fd754, constructor 0x4895c0). The destructor (out of line at
// 0x489650) is inlined here: it unlinks the object from its owner's list
// (head at +0xa2) and clears the link.
// FUNCTION: 0x489600 ??_GPathOrderAttach@@UAEPAXI@Z

// The out-of-line destructor of PathOrderAttach (vtable 0x4fd754): unlinks
// the object from its owner's list (head at +0xa2) and clears the link.
// Callers already call it as UnitRef::Unlink, so it is written as that
// method, which runs the real destructor non-virtually.
// FUNCTION: 0x489650
void UnitRef::Unlink()
{
    ((PathOrderAttach*)this)->PathOrderAttach::~PathOrderAttach();
}

// Moves the link to a new owner: unlinks it from the current owner's list
// (as the destructor 0x489650 does), then links it into `o`'s list (as the
// constructor 0x4895c0 does) when `o` is set and its flag is non-zero.
// FUNCTION: 0x489690
void PathOrderAttach::SetUnit(Unit* o)
{
    if (owner != 0) {
        PathOrderAttach** pp = &owner->link;
        while (*pp != this)
            pp = &(*pp)->next;
        *pp = next;
        owner = 0;
        next = 0;
    }
    if (o != 0 && o->map != 0) {
        owner = o;
        next = o->link;
        o->link = this;
        return;
    }
    owner = 0;
    next = 0;
}

// FUNCTION: 0x4896f0
void UnitRef::ClearRef(void)
{
    Unit* saved = owner;
    if (listener)
        listener->Notify(8);
    if (owner == saved && owner) {
        UnitRef** pp = &owner->head;
        while (*pp != this)
            pp = &(*pp)->next;
        *pp = next;
        owner = 0;
        next = 0;
    }
}

// Detaches every node from an owner's list (head at +0xa2): for each node
// calls listener slot 0 with 8, then unlinks it from the list when its owner
// did not change during the notification.
// FUNCTION: 0x489740
void __stdcall ClearUnitRefs(Unit* obj)
{
    for (UnitRef* n = obj->head; n != 0; n = obj->head) {
        Unit* saved = n->owner;
        if (n->listener)
            n->listener->Notify(8);
        if (n->owner == saved && n->owner) {
            UnitRef** pp = &n->owner->head;
            while (*pp != n)
                pp = &(*pp)->next;
            *pp = n->next;
            n->owner = 0;
            n->next = 0;
        }
    }
}

// Walks a linked list at +0xa2 and calls slot 0 of each node's object.
// FUNCTION: 0x4897b0
void __stdcall NotifyUnitRefs(Unit* obj, int event)
{
    for (UnitRef* n = obj->head; n != 0; n = n->next) {
        if (n->listener != 0)
            n->listener->Notify(event);
    }
}

// Tests bit 1 of three flag bytes 0x1c apart. A single exit through an
// unsigned char local keeps the final AND byte-sized (`mov dl, 2` hoisted,
// `and al, dl`); separate returns widen it to `mov edx, 2` / `and eax, edx`.
// FUNCTION: 0x4897e0
unsigned char Unit::ChooseWeapon()
{
    unsigned char result;
    if (entries[0].flags & 2)
        result = 0;
    else if (entries[1].flags & 2)
        result = 1;
    else
        result = entries[2].flags & 2;
    return result;
}

#pragma pack(push, 1)
struct Dmg_00489bb0 {              // 9 bytes, the record ApplyUnitDamage takes
    unsigned char kind;            // +0x0, 11 here, never read by that callee
    short team_target;             // +0x1
    short team_source;             // +0x3
    short amount;                  // +0x5
    unsigned char extra;           // +0x7
    unsigned char type;            // +0x8, the damage type
};
#pragma pack(pop)

void __stdcall ApplyUnitDamage(Dmg_00489bb0* dmg);
void __stdcall BroadcastPacket(int who, Dmg_00489bb0* dmg, int size);
int __cdecl GetLocalDpid(void);

// Works out how much damage one unit does to another and hands the result to
// ApplyUnitDamage as a 9 byte record. Damage type 10 skips the whole calculation
// and passes the amount through; any other type scales the amount by the
// target's 16.16 damage scale and then takes off armour, four percent per
// point of (armour / 5), capped at five points.
// Afterwards the damage is spread to every unit of the target's kind, but only
// when the target's kind is flagged (active set, type 3) and the type is not 11.
// Every caller found fills the low byte, so the record's
// byte +7 is zero on all of them; see the note in the bug list.
// FUNCTION: 0x489bb0
void __stdcall DamageUnit(Unit* source, Unit* target, int amount, int type, unsigned short extra)
{
    int dmg;
    if (type != 10) {
        if ((target->state & 2) && amount < 0x7530)
            amount = (int)(((__int64)target->def->f1aa * amount) >> 0x10);
        int armour = target->killCount / 5;
        if (armour > 5)
            armour = 5;
        dmg = (25 - armour) * amount * 4 / 100;
    } else {
        dmg = amount;
    }
    Dmg_00489bb0 d;
    d.kind = 11;
    d.team_target = !target ? 0 : target->id;
    d.team_source = !source ? 0 : source->id;
    d.amount = dmg;
    // extra is a 16-bit value of which only the high byte reaches the record.
    d.extra = (unsigned char)(extra >> 8);
    d.type = type;
    ApplyUnitDamage(&d);
    if (target->player->active != 0 && target->player->type == 3 && type != 11) {
        // The call is written in both arms: a ternary argument merges the tails.
        if (source)
            BroadcastPacket(source->player->id, &d, 9);
        else
            BroadcastPacket(GetLocalDpid(), &d, 9);
    }
}

#include "../orders/mission_type.h"

#include "../orders/order.h"

#pragma pack(push, 1)
struct Event_00489ce0 {
    unsigned char type;                // +0x0, unused here
    unsigned short attacker;           // +0x1
    unsigned short target;             // +0x3
    unsigned short amount;             // +0x5
    unsigned char param_7;             // +0x7
    unsigned char kind;                // +0x8
};
#pragma pack(pop)

extern char s_paralyze_00508d80[];
extern char s_HitByWeapon_00508d74[];
extern char s_TakeDamage_00508d68[];

void __stdcall AppendOrder(Unit* owner, Order* node);
void __stdcall MarkRecentlyDamaged(Unit* unit);
void __stdcall ReactToAttack(Unit* target, Unit* attacker, int amount);
void __stdcall AddCdActivitySample(int flag);
int __cdecl FUN_004b7123(unsigned short idx, int scale);
int __cdecl FUN_004b70ef(short idx, int scale);

// Applies one damage/heal event record (a small struct on the caller's stack:
// byte type at +0, two unit ids at +1 and +3, an amount at +5, a byte at +7
// and the event kind at +8) to the unit named by the first id.
//
// The heal branch repeats the sum inside the clamp ternary. With a separate
// `int v = ...` local MSVC puts the second addend in the accumulator (hp in
// edx, amount in eax); giving the ternary its own copy of `unit->hp +
// ev->amount` makes the original put hp in eax and the amount in edx.
//
// FUNCTION: 0x489ce0
void __stdcall ApplyUnitDamage(Event_00489ce0* ev)
{
    Unit* unit = ev->attacker == 0 ? 0 : &g_game->units[ev->attacker];
    Unit* target = ev->target == 0 ? 0 : &g_game->units[ev->target];

    if (unit == 0)
        return;
    if (!(unit->flags & 0x10000000))
        return;
    if (unit->flags & 0x4000)
        return;

    if (ev->kind == 10) {
        unit->hp = (short)((unsigned int)(unit->hp + ev->amount) < unit->def->maxhp
                           ? unit->hp + ev->amount : unit->def->maxhp);
        return;
    }

    MarkRecentlyDamaged(unit);

    if (ev->kind != 11)
        ReactToAttack(target, unit, ev->amount);

    unit->kind = ev->kind;

    if (target) {
        unsigned char c = target->teamId;
        unit->unknown_f4 = c;
        unit->last = target;
        if (target->teamId == g_game->localPlayer || unit->teamId == g_game->localPlayer)
            AddCdActivitySample(1);
    }

    if (ev->kind == 2) {
        int ticks = ev->amount;
        if (unit->flags & 0x10000000) {
            if (!(unit->flags & 0x4000)) {
                Player_0048b090* owner = unit->player;
                if (owner->active && (owner->type == 1 || owner->type == 2)) {
                    if (!(unit->def->f241 & 0x4000000)) {
                        MissionType kind(s_paralyze_00508d80);
                        Order* e = unit->effect;
                        if (e && e->kind == kind) {
                            e->ticks += ticks;
                            return;
                        }
                        AppendOrder(unit, new Order(kind, 0, 0, ticks, 0, 0));
                        return;
                    }
                }
            }
        }
    } else {
        unit->hp -= ev->amount;
        if (unit->hp <= 0) {
            if (unit->player->active && (unit->player->type == 1 || unit->player->type == 2)) {
                unit->flags |= 0x4000;
                return;
            }
            unit->hp = 0;
        }
        if (ev->kind == 1) {
            int a = FUN_004b7123(ev->param_7 << 8, 400);
            int b = FUN_004b70ef(ev->param_7 << 8, 400);
            unit->script->StartScriptWithArgs(s_HitByWeapon_00508d74, 0, 0, 2, a, b, 0, 0);
            int pct = unit->hp * 100 / unit->def->maxhp;
            if (pct < 0)
                pct = 0;
            if (pct > 100)
                pct = 100;
            unit->script->StartScriptWithArgs(s_TakeDamage_00508d68, 0, 0, 1, pct, 0, 0, 0);
        }
    }
}

// Queues a "paralyze" effect on a unit that is finished being built, not
// flagged as an old (0x4000) unit, and whose owner is an active human or
// computer player (types 1 and 2) whose unit definition does not refuse
// effects (0x4000000 at +0x241). A paralyze effect already on the unit
// (the pointer at +0x5c) has its remaining time (ticks) extended instead
// of a second effect being queued.
// FUNCTION: 0x489fa0
void __stdcall ParalyzeUnit(Unit* unit, int ticks)
{
    if (!(unit->flags & 0x10000000))
        return;
    if (unit->flags & 0x4000)
        return;
    Player_0048b090* owner = unit->player;
    if (!owner->active)
        return;
    if (owner->type == 1 || owner->type == 2) {
        if (!(unit->def->f241 & 0x4000000)) {
            MissionType kind("paralyze");
            Order* effect = unit->effect;
            if (effect && effect->kind == kind) {
                effect->ticks += ticks;
                return;
            }
            AppendOrder(unit, new Order(kind, 0, 0, ticks, 0, 0));
        }
    }
}

// FUNCTION: 0x48a060
void __stdcall SetWeaponTargetUnit(char* param_1, char* param_2, int param_3)
{
    char* elem = param_1 + param_3 * 0x1c;
    *(unsigned short*)(elem + 4) = *(unsigned short*)(param_2 + 0xa8);
    *(unsigned short*)(elem + 6) = 0x8000;
    *(unsigned short*)(param_1 + 0xba) &= 0x83ff;
}

// Sibling of 0x48a060: stores the high words of a fixed-point position's x
// and z into entry param_3 (0x1c bytes each), keeping z away from 0x8000,
// then clears bits 10-14 of the flags at +0xba.
// FUNCTION: 0x48a0a0
void __stdcall SetWeaponTargetPos(char* param_1, int* param_2, int param_3)
{
    short* elem = (short*)(param_1 + param_3 * 0x1c + 4);
    elem[0] = (short)(param_2[0] >> 16);
    elem[1] = (short)(param_2[2] >> 16);
    if (elem[1] == (short)0x8000) {
        elem[1] = (short)0x8001;
    }
    *(unsigned short*)(param_1 + 0xba) &= 0x83ff;
}

// Clears target entry `index` (the same reset as ResetWeaponTarget) unless it is
// already clear, then tells the unit's script "StartBuilding" and
// "TargetCleared".
// FUNCTION: 0x48a0f0
void __stdcall ClearWeaponTarget(Unit* unit, int index)
{
    Point_004898b0* p = &unit->entries[index].point;
    if (p->a != 0 || p->b != (short)0x8000) {
        p->a = 0;
        p->b = (short)0x8000;
        unit->script->FindScript("StartBuilding");
        unit->script->StartScriptWithArgs("TargetCleared", 0, 0, 1, index, 0, 0, 0);
    }
}

// FUNCTION: 0x48a160
void __stdcall ResetWeaponTarget(Unit* obj, int index)
{
    Point_004898b0* p = &obj->entries[index].point;
    p->a = 0;
    p->b = (short)0x8000;
}

// Returns the unit a target entry (see 0x48a160) points at, or 0 when the
// entry is not a unit target.
// FUNCTION: 0x48a190
Unit* __stdcall GetWeaponTargetUnit(Unit* obj, int index)
{
    Point_004898b0* p = &obj->entries[index].point;
    if (p->b != (short)0x8000) {
        return 0;
    }
    if (p->a == 0) {
        return 0;
    }
    return &g_game->units[p->a];
}

int __stdcall GetGroundHeight(Vec3_0048a1e0* pos);
void __stdcall GetSweetSpot(Unit* def, int pos);

// The intact tail block: writing the three adds in their natural x,y,z order
// makes MSVC sink the first product to its use, and z,y,x order orders the
// adds z,y,x. Passing the three products through a static helper that
// returns the vector by value makes all three writes land in memory before the
// adds are read, and the original's x,y,z schedule falls out.
static Vec3_0048a1e0 offset_0048a1e0(Body_0048a1e0* m, __int64 s)
{
    Vec3_0048a1e0 d;
    d.x = (int)(((__int64)m->offset.x * s) >> 16);
    d.y = (int)(((__int64)m->offset.y * s) >> 16);
    d.z = (int)(((__int64)m->offset.z * s) >> 16);
    return d;
}

// FUNCTION: 0x48a1e0
int __stdcall GetWeaponTargetPos(Unit* unit, Vec3_0048a1e0* pos, int index)
{
    Entry_004898b0* e = &unit->entries[index];
    if (e->point.b != (short)0x8000) {
        pos->x = e->point.a << 16;
        pos->z = e->point.b << 16;
        pos->y = max(GetGroundHeight(pos), g_game->seaLevel) << 16;
        return 1;
    }
    if (e->point.a == 0) {
        return 0;
    }
    Unit* def = &g_game->units[e->point.a];
    if (def->map == 0) {
        Entry_004898b0* f = &unit->entries[index];
        if (f->point.a != 0 || f->point.b != (short)0x8000) {
            e->point.a = 0;
            e->point.b = (short)0x8000;
            unit->script->FindScript("StartBuilding");
            unit->script->StartScriptWithArgs("TargetCleared", 0, 0, 1, index, 0, 0, 0);
        }
        return 0;
    }
    GetSweetSpot(def, (int)pos);
    if ((e->flags & 2) && !(e->target->field_111 & 0x2000000) && def->body != 0
        && unit->killCount > 5 && e->target->radius != 0) {
        Vec3_0048a1e0 d;
        d.x = unit->pos.x - pos->x;
        d.y = unit->pos.y - pos->y;
        d.z = unit->pos.z - pos->z;
        int dist = (int)sqrt((double)d.x * d.x + (double)d.y * d.y + (double)d.z * d.z);
        int scale = (int)(((__int64)dist << 16) / e->target->radius);
        scale = (int)((scale * (__int64)0xcccc) >> 16);
        __int64 s = scale;
        d = offset_0048a1e0(def->body, s);
        pos->x = pos->x + d.x;
        pos->y = pos->y + d.y;
        pos->z = pos->z + d.z;
    }
    return 1;
}

// FUNCTION: 0x48a440
int __stdcall IsTargetingUnit(Unit* list, Unit* obj)
{
    for (int i = 0; i < 3; i++) {
        if (list->entries[i].point.b == -0x8000 && list->entries[i].point.a == obj->id) {
            return 1;
        }
    }
    return 0;
}

#pragma pack(push, 1)
struct MapVertex_0048a490 {
    int x;                              // +0x0
    int y;                              // +0x4
    int z;                              // +0x8
};

struct MapRow_0048a490 {
    char unknown_0[0xc];
    unsigned short* ids;                // +0xc
    char unknown_10[0x20 - 0x10];
};

struct MapInfo_0048a490 {
    char unknown_0[0xc];
    int count;                          // +0xc
    char unknown_10[0x24 - 0x10];
    MapVertex_0048a490* verts;           // +0x24
    MapRow_0048a490* rows;               // +0x28
};

struct Pos2_0048a490 {
    int x;
    int z;
};

struct Hs_0048a490 {
    int wx;                             // +0x0
    int h;                              // +0x4
    int spare;                          // +0x8
};
#pragma pack(pop)

unsigned int GetTicks();
int __cdecl FUN_004b715a(int x, int y);
void __cdecl FUN_004b7173(unsigned short deg, Pos2_0048a490* p);

// Samples the ground under a unit at its four surrounding terrain vertices and
// stores the resulting pitch (0x68) and roll (0x70) on the unit, plus a heading
// (0x64) from the two side vertices. The 0x11/0x04 bytes of a heightmap tile are
// the two half heights of its edge pair, and the corner heights are bilinearly
// interpolated with a plain / 16. The sea-level block then adds a jitter that
// fades out over 60 frames.
// FUNCTION: 0x48a490
void __stdcall AlignUnitToGround(Unit* u)
{
    MapInfo_0048a490* m = g_game->unitModels[u->map];
    MapRow_0048a490* row = m->rows + m->count;
    if (m->count < 0)
        return;
    {
        // gw and c0 are declared apart from t, pts, hs and k.
        int gw, c0;
        Pos2_0048a490 t;
        Pos2_0048a490 pts[4];
        Hs_0048a490 hs[4];
        int k;
        // A for loop: a do/while is rotated the other way.
        for (k = 0; k < 4; k++) {
            MapVertex_0048a490* v = m->verts + row->ids[k];
            int vx = v->x;
            // Chained: keeps the store order and the corner-array register.
            pts[k].x = t.x = vx;
            int vz = v->z;
            t.z = vz;
            pts[k].z = vz;
            FUN_004b7173(u->aim, &t);
            // wx before hz, fx before fz, and gz declared before fz.
            int wx = (short)((t.x + u->pos.x) >> 16);
            int hz = (short)((u->pos.z - t.z) >> 16);
            int fx = wx & 0xf;
            unsigned gz = (unsigned)hz >> 4;
            int fz = hz & 0xf;
            gw = g_game->mapWidthTiles;
            unsigned gx = (unsigned)wx >> 4;
            if (gx >= gw - 1)
                return;
            if (gz >= g_game->mapHeightTiles - 1)
                return;
            unsigned char* tb = g_game->heightMap + (gz * gw + gx) * 13;
            int b0 = tb[4];
            // mapWidthTiles read again, not gw: keeps the allocation.
            unsigned char* tb1 = tb + g_game->mapWidthTiles * 13;
            int b1 = tb1[4];
            c0 = tb[0x11];
            int c1 = tb1[0x11];
            int H0 = b0 + ((c0 - b0) * fx) / 16;
            int H1 = b1 + ((c1 - b1) * fx) / 16;
            hs[k].wx = wx;
            if ((u->def->f241 & 0x1000) && (u->flags & 0x10000000)
                && !(u->flags & 0x4000)) {
                // Stored through the array, then read back into H.
                hs[k].h = H0 + ((H1 - H0) * fz) / 16;
                int H = hs[k].h;
                int sea = g_game->seaLevel;
                hs[k].h = max(H, sea);
                // short p and `* 2048`, not `<< 11`: keeps the 0x1f mask and 16-bit add.
                short p = (short)(((GetTicks() & 0x1f) + k * 8) * 2048 + u->fix_lo);
                int s = u->def->sight / 2;
                // Owner kept in a local across the 64-bit helper calls.
                Body_0048a1e0* o = u->body;
                int q = o->sight;
                if (q >= s)
                    q = s;
                // The 64-bit part is one expression, not an __int64 local.
                int w = (int)((((__int64)q << 16) / s));
                int mm = 2 - (int)((((__int64)w * 2) >> 16));
                unsigned int n = g_game->gameTick - o->age;
                // Clamp inline as a ternary, not a separate if.
                mm -= (unsigned int)(mm * (n < 60 ? n : 60)) / 60;
                hs[k].h = FUN_004b7123(p, mm) + hs[k].h;
            } else {
                hs[k].h = H0 + ((H1 - H0) * fz) / 16;
            }
            hs[k].spare = hz;
        }
        // hs[1] read before hs[0]; roll and pitch use (h0+h1)/2, not /2/2.
        int h1 = hs[1].h;
        int h0 = hs[0].h;
        int h2 = hs[2].h;
        int h3 = hs[3].h;
        int a = (h0 + h1) / 2;
        int b = (h2 + h3) / 2;
        u->pos.roll = (a + b) / 2;
        u->pitch = FUN_004b715a(b - a, (short)(abs(pts[0].z - pts[3].z) >> 16));
        u->hdg = FUN_004b715a(h0 - h1, (short)(abs(pts[0].x - pts[1].x) >> 16));
    }
}
