// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Sibling of 0x43b1f0: appends command objects (Class_0043a1f0, 0x56 bytes)
// to the unit's list. FUN_0043f0e0 (relation 8, then 2) picks the kinds; the
// unit's flags at +0x110 gate the three cases (bits 18-19 equal to 0, 0x40000
// or 0x80000), and when bit 18 alone is set a second, positional command is
// made at the unit's own position before the one aimed at the target.
// The two-command cases differ only in the last constructor argument: the
// first reads a signed short at def+0x202, the second an unsigned short at
// def+0x214. The list insertion is the body of 0x43acb0, inlined five times;
// the second command's two copies in the 0x40000 and 0x80000 cases write the
// link and the flag merge once and the earlier loop jumps forward to it, which
// needs the explicit `return 1;` in each case (a single trailing return makes
// MSVC keep two store blocks). See 0x43b1f0.cpp.

#pragma pack(push, 1)

struct Unit;
class Class_0043a1f0;

// The command kind, one byte wide, but not a POD type: it is returned through
// a hidden pointer placed in a dead argument slot.
class Class_00438760 {
public:
    unsigned char index;

    Class_00438760() {}
};

struct Vec3 {
    short x, y, z;
};

class Class_0043a1f0 {
public:
    char unknown_0[0xe];
    Unit* unit;                                   // +0xe
    char unknown_12[0x2e - 0x12];
    short field_2e;                               // +0x2e
    short field_30;                               // +0x30
    char unknown_32[0x42 - 0x32];
    unsigned int flags;                           // +0x42
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;                         // +0x4a
    char unknown_4e[0x56 - 0x4e];

    Class_0043a1f0(Class_00438760 kind, Unit* owner, Vec3* pos, int a, int b, int c);
};

struct UnitDef {
    char unknown_0[0x202];
    short field_202;                              // +0x202
    char unknown_204[0x214 - 0x204];
    unsigned short field_214;                     // +0x214
};

struct Unit {
    char unknown_0[0x5c];
    Class_0043a1f0* list;                         // +0x5c
    Class_0043a1f0* list2;                        // +0x60, for commands with flag 0x40000
    char unknown_64[0x6a - 0x64];
    Vec3 pos;                                     // +0x6a, only pos.y is read here
    short field_70;                               // +0x70
    short field_72;                               // +0x72
    short field_74;                               // +0x74
    char unknown_76[0x92 - 0x76];
    UnitDef* def;                                 // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags;                           // +0x110
};
#pragma pack(pop)

void* __cdecl operator new(unsigned int size);

Class_00438760 __stdcall FUN_0043f0e0(unsigned char relation, Unit* unit, Unit* target, Vec3* pos);

// Puts `cmd` in front of `before` in the list its own flag 0x40000 picks, and
// copies bit 0x4000 of the command it displaces.
static inline void Insert(Unit* unit, Class_0043a1f0* cmd, Class_0043a1f0* before)
{
    unsigned int which = cmd->flags & 0x40000;
    Class_0043a1f0* last = which ? unit->list2 : unit->list;
    Class_0043a1f0** link = which ? &unit->list2 : &unit->list;
    while (*link != before) {
        link = &(*link)->next;
    }
    *link = cmd;
    cmd->unit = unit;
    cmd->next = before;
    if (before != 0) {
        cmd->flags |= before->flags & 0x4000;
    }
}

// FUNCTION: 0x43b400
int __stdcall FUN_0043b400(Unit* unit, Unit* target, int param_3)
{
    Class_00438760 kind = FUN_0043f0e0(8, unit, target, 0);
    if (!kind.index)
        return 0;
    unsigned int f = unit->flags & 0xc0000;
    if (f == 0 && !param_3) {
        Class_00438760 kind2 = FUN_0043f0e0(2, unit, 0, &unit->pos);
        Class_0043a1f0* cmd = new Class_0043a1f0(kind2, 0, &unit->pos, 0, 0, 0);
        Insert(unit, cmd, (cmd->flags & 0x40000) ? unit->list2 : unit->list);
        Class_0043a1f0* cmd2 = new Class_0043a1f0(kind, target, 0, 0, 0, unit->def->field_202);
        cmd2->field_2e = unit->pos.y;
        cmd2->field_30 = unit->field_74;
        Insert(unit, cmd2, (cmd2->flags & 0x40000) ? unit->list2 : unit->list);
        return 1;
    } else if (f == 0x40000 && !param_3) {
        Class_00438760 kind2 = FUN_0043f0e0(2, unit, 0, &unit->pos);
        Class_0043a1f0* cmd = new Class_0043a1f0(kind2, 0, &unit->pos, 0, 0, 0);
        Insert(unit, cmd, (cmd->flags & 0x40000) ? unit->list2 : unit->list);
        Class_0043a1f0* cmd2 = new Class_0043a1f0(kind, target, 0, 0, 0, unit->def->field_214);
        cmd2->field_2e = unit->pos.y;
        cmd2->field_30 = unit->field_74;
        Insert(unit, cmd2, (cmd2->flags & 0x40000) ? unit->list2 : unit->list);
        return 1;
    } else if (f == 0x80000 && !param_3) {
        Class_0043a1f0* cmd3 = new Class_0043a1f0(kind, target, 0, 0, 0, 0);
        Insert(unit, cmd3, (cmd3->flags & 0x40000) ? unit->list2 : unit->list);
        return 1;
    } else {
        return 0;
    }
}
