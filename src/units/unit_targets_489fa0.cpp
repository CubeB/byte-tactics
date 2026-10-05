// Decompiled by space-bunny-free. Names are provisional.
// Queues a "paralyze" effect on a unit that is finished being built, not
// flagged as an old (0x4000) unit, and whose owner is an active human or
// computer player (types 1 and 2) whose unit definition does not refuse
// effects (0x4000000 at +0x241). A paralyze effect already on the unit
// (the pointer at +0x5c) has its remaining time (field_36) extended instead
// of a second effect being queued.

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
    Class_0043a1f0(Class_00438760 type, void* owner, void* pos, int a, int b, int c);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Player_00489fa0 {
    int active;                        // +0x0
    char unknown_4[0x73 - 4];
    unsigned char type;                // +0x73
};

struct UnitDef_00489fa0 {
    char unknown_0[0x241];
    unsigned int flags;                // +0x241
};

struct Unit {
    char unknown_0[0x5c];
    Class_0043a1f0* effect;            // +0x5c
    char unknown_60[0x92 - 0x60];
    UnitDef_00489fa0* def;             // +0x92
    Player_00489fa0* owner;            // +0x96
    char unknown_9a[0x110 - 0x9a];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

void __stdcall AppendOrder(Unit* owner, Class_0043a1f0* node);

// FUNCTION: 0x489fa0
void __stdcall ParalyzeUnit(Unit* unit, int ticks)
{
    if (!(unit->flags & 0x10000000))
        return;
    if (unit->flags & 0x4000)
        return;
    Player_00489fa0* owner = unit->owner;
    if (!owner->active)
        return;
    if (owner->type == 1 || owner->type == 2) {
        if (!(unit->def->flags & 0x4000000)) {
            Class_00438760 kind("paralyze");
            Class_0043a1f0* effect = unit->effect;
            if (effect && effect->kind == kind.index) {
                effect->field_36 += ticks;
                return;
            }
            AppendOrder(unit, new Class_0043a1f0(kind, 0, 0, ticks, 0, 0));
        }
    }
}
