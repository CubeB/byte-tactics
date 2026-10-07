// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// A live unit that changed its type (the linked unit name at g_game+0x37f5f
// matches the unit type's name) recomputes a "Killed" kill count and sends the
// 0xb byte command record at +0xa it builds here to its own player. param_2 is
// the command kind: 7 means a spy / non-kill path, 4/5/9 or a positive
// field_108 skip the recount, otherwise the kills are a percentage of the
// type's field_1fa.
#include <string.h>

#pragma pack(push, 1)
struct Owner_4864b0 {
    char unknown_0[0x95];
    unsigned char playerIndex;         // +0x95
};

struct Link_4864b0 {
    int active;                        // +0x00
    int field_4;                       // +0x04
    char unknown_8[0x27 - 0x8];
    Owner_4864b0* owner;               // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;               // +0x73
    char unknown_74[0x149 - 0x74];
    unsigned short field_149;          // +0x149
};

struct Info_4864b0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x1fa - 0x40];
    unsigned int field_1fa;            // +0x1fa
};

class CobScript {
public:
    int QueryScript(char* name, int* a, int* b, int c, int d);
};

struct Unit {
    char unknown_0[0x92];
    Info_4864b0* info;                 // +0x92
    Link_4864b0* link;                 // +0x96
    CobScript* field_9a;               // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    unsigned short field_a8;           // +0xa8
    char unknown_aa[0xf0 - 0xaa];
    Unit* field_f0;                    // +0xf0
    unsigned char field_f4;            // +0xf4
    char unknown_f5[0xf7 - 0xf5];
    unsigned char field_f7;            // +0xf7
    char unknown_f8[0xff - 0xf8];
    unsigned char field_ff;            // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                   // +0x104
    short field_108;                   // +0x108
    char unknown_10a[0x110 - 0x10a];
    unsigned int flags;                // +0x110
};

struct Name_4864b0 {
    char name[0x232];                  // +0x0
};

struct Game {
    char unknown_0[0x37ef6];
    int field_37ef6;                   // +0x37ef6
    char unknown_37efa[0x37f5f - 0x37efa];
    Name_4864b0 players[10];           // +0x37f5f
};

struct Cmd_4864b0 {
    unsigned char type;                // +0x0
    unsigned short field_1;            // +0x1
    int field_3;                       // +0x3
    unsigned short field_7;            // +0x7
    signed char field_9;               // +0x9
    // 4-bit bitfields, not a plain char: they give the XOR read-modify-writes.
    unsigned char count : 4;           // +0xa
    unsigned char kind : 4;            // +0xa
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall GetSlotDpid(unsigned char index);
int __stdcall BroadcastPacket(int id, unsigned char* packet, int size);
void __stdcall ApplyUnitDeath(unsigned char* cmd, int param);
void __stdcall FUN_00491d70(int param);
void __stdcall KillPlayerUnits(unsigned char player);

// FUNCTION: 0x4864b0
void __stdcall KillUnit(Unit* unit, int param_2)
{
    if ((unit->flags & 0x10000000) != 0) {
        int same = _strcmpi(g_game->players[unit->link->owner->playerIndex].name,
                            unit->info->name) == 0;
        if (same) {
            unit->link->field_149 &= 0xfffe;
        }
        int amount;
        int flag;
        if (param_2 == 7) {
            flag = 1;
            amount = 0;
        } else if (param_2 == 4 || param_2 == 5 || param_2 == 9 || unit->field_108 > 0) {
            amount = 0;
            flag = 0;
        } else {
            amount = ((int)(unit->field_108 * -100 / unit->info->field_1fa) + unit->field_f7) / 2;
            if (amount < 1)
                amount = 1;
            if (amount > 100)
                amount = 100;
            unit->field_9a->QueryScript("Killed", &amount, &flag, 0, 0);
        }
        if (unit->field_104 != 0.0f) {
            flag = 0;
        }
        Cmd_4864b0 cmd;
        // field_1 before field_9: sets the register roles.
        cmd.field_1 = unit->field_a8;
        cmd.field_9 = amount;
        cmd.type = 0xc;
        cmd.count = flag;
        cmd.kind = param_2;
        cmd.field_3 = GetSlotDpid(unit->field_f4);
        if (unit->field_f0 == 0)
            cmd.field_7 = 0;
        else
            cmd.field_7 = unit->field_f0->field_a8;
        if (unit->link->active != 0 &&
            (unit->link->state == 1 || unit->link->state == 2)) {
            BroadcastPacket(unit->link->field_4, (unsigned char*)&cmd, 0xb);
        }
        ApplyUnitDeath((unsigned char*)&cmd, 1);
        if (same && g_game->field_37ef6 != 0 && unit->link->active != 0 &&
            (unit->link->state == 1 || unit->link->state == 2)) {
            FUN_00491d70(1);
            KillPlayerUnits(unit->field_ff);
        }
    }
}
