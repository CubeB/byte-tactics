// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
// Applies one damage/heal event record (a small struct on the caller's stack:
// byte type at +0, two unit ids at +1 and +3, an amount at +5, a byte at +7
// and the event kind at +8) to the unit named by the first id.

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[4];
    unsigned char kind;                // +0x4
    char unknown_5[0x36 - 0x5];
    int field_36;                      // +0x36
    char unknown_3a[0x56 - 0x3a];
    Class_0043a1f0(Class_00438760 k, void* owner, void* pos, int a, int b, int c);
};
#pragma pack(pop)

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4, int param_5,
                    int param_6, int param_7, int param_8);
};

#pragma pack(push, 1)
struct Player_00489ce0 {
    int active;                        // +0x0
    char unknown_4[0x73 - 4];
    unsigned char type;                // +0x73
};

struct UnitDef_00489ce0 {
    char unknown_0[0x1fa];
    unsigned int maxhp;                // +0x1fa
    char unknown_1fe[0x241 - 0x1fe];
    unsigned int flags;                // +0x241
};

struct Unit {
    char unknown_0[0x5c];
    Class_0043a1f0* effect;            // +0x5c
    char unknown_60[0x92 - 0x60];
    UnitDef_00489ce0* def;             // +0x92
    Player_00489ce0* owner;            // +0x96
    CobScript* anims;                  // +0x9a
    char unknown_9e[0xf0 - 0x9e];
    void* last;                       // +0xf0
    char unknown_f4;
    unsigned char kind;                // +0xf5
    char unknown_f6[0xff - 0xf6];
    unsigned char teamId;              // +0xff
    char unknown_100[0x108 - 0x100];
    short hp;                          // +0x108
    char unknown_10a[0x110 - 0x10a];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Event_00489ce0 {
    unsigned char type;                // +0x0, unused here
    unsigned short attacker;           // +0x1
    unsigned short target;             // +0x3
    unsigned short amount;             // +0x5
    unsigned char param_7;             // +0x7
    unsigned char kind;                // +0x8
};

struct Game {
    char unknown_0[0x2a42];
    unsigned char teamId;              // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit* units;                       // +0x14357
};
#pragma pack(pop)

extern Game* g_game;
extern char s_paralyze_00508d80[];
extern char s_HitByWeapon_00508d74[];
extern char s_TakeDamage_00508d68[];

void __stdcall AppendOrder(Unit* owner, Class_0043a1f0* node);
void __stdcall FUN_00467950(Unit* unit);
void __stdcall FUN_00406f80(Unit* target, Unit* attacker, int amount);
void __stdcall FUN_00494ff0(int flag);
int __cdecl FUN_004b7123(unsigned short idx, int scale);
int __cdecl FUN_004b70ef(short idx, int scale);

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

    FUN_00467950(unit);

    if (ev->kind != 11)
        FUN_00406f80(target, unit, ev->amount);

    unit->kind = ev->kind;

    if (target) {
        unsigned char c = target->teamId;
        unit->unknown_f4 = c;
        unit->last = target;
        if (target->teamId == g_game->teamId || unit->teamId == g_game->teamId)
            FUN_00494ff0(1);
    }

    if (ev->kind == 2) {
        int ticks = ev->amount;
        if (unit->flags & 0x10000000) {
            if (!(unit->flags & 0x4000)) {
                Player_00489ce0* owner = unit->owner;
                if (owner->active && (owner->type == 1 || owner->type == 2)) {
                    if (!(unit->def->flags & 0x4000000)) {
                        Class_00438760 kind(s_paralyze_00508d80);
                        Class_0043a1f0* e = unit->effect;
                        if (e && e->kind == kind.index) {
                            e->field_36 += ticks;
                            return;
                        }
                        AppendOrder(unit, new Class_0043a1f0(kind, 0, 0, ticks, 0, 0));
                        return;
                    }
                }
            }
        }
    } else {
        unit->hp -= ev->amount;
        if (unit->hp <= 0) {
            if (unit->owner->active && (unit->owner->type == 1 || unit->owner->type == 2)) {
                unit->flags |= 0x4000;
                return;
            }
            unit->hp = 0;
        }
        if (ev->kind == 1) {
            int a = FUN_004b7123(ev->param_7 << 8, 400);
            int b = FUN_004b70ef(ev->param_7 << 8, 400);
            unit->anims->StartScriptWithArgs(s_HitByWeapon_00508d74, 0, 0, 2, a, b, 0, 0);
            int pct = unit->hp * 100 / unit->def->maxhp;
            if (pct < 0)
                pct = 0;
            if (pct > 100)
                pct = 100;
            unit->anims->StartScriptWithArgs(s_TakeDamage_00508d68, 0, 0, 1, pct, 0, 0, 0);
        }
    }
}
