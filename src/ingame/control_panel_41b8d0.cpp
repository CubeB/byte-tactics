// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Finished-construction notification: marks the target as no longer being
// built (clears its build progress and sets flag 0x2000), updates the
// BUILDER.GUI panel or starts the builder's nanolathe, syncs two unit-type
// flag bits and the selection flag.
// The flag read-modify-write is written in place (not through a local) so the
// compiler reuses the OR result for the 0x20000000 test, keeping it in ecx
// while the player pointer is loaded into eax.

struct Unit;

#pragma pack(push, 1)
struct UnitType_0041b8d0 {
    char unknown_0[0x156];
    int field_156;                      // +0x156
    char unknown_15a[0x241 - 0x15a];
    unsigned int bits_0 : 18;
    unsigned int flag_18 : 1;           // +0x241 bit 18
    unsigned int bits_19 : 5;
    unsigned int flag_24 : 1;           // +0x241 bit 24
    unsigned int rest : 7;
};

struct Player_0041b8d0 {
    int active;                         // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                 // +0x73
};

struct Nano_0041b8d0 {
    char unknown_0[0x10];
    int field_10;                       // +0x10
};

struct Unit_0041b8d0 {
    char unknown_0[0x86];
    int field_86;                       // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType_0041b8d0* type;            // +0x92
    Player_0041b8d0* player;            // +0x96
    char unknown_9a[0x9e - 0x9a];
    Nano_0041b8d0* field_9e;            // +0x9e
    char unknown_a2[0xa8 - 0xa2];
    unsigned short id;                  // +0xa8
    char unknown_aa[0xf5 - 0xaa];
    unsigned char field_f5;             // +0xf5
    char unknown_f6[0x104 - 0xf6];
    int field_104;                      // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                 // +0x110
};

struct Menu_0041b8d0 {
    char unknown_0[0x18];
    void* entry;                        // +0x18
    char unknown_1c[0xcca - 0x1c];
    int field_cca;                      // +0xcca
};

struct Game {
    char unknown_0[0x519];
    Menu_0041b8d0 menu;                 // +0x519
    char unknown_535[0x37e9c - 0x519 - sizeof(Menu_0041b8d0)];
    unsigned short unitIndex;           // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags;               // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_004ab060(void* obj, const char* name);
void __stdcall FUN_0049fa90(void* obj);
void __stdcall FUN_004199b0(void* a, void* b);
void __stdcall AttachUnitToPiece(Unit_0041b8d0* unit, Unit_0041b8d0* target, char p3, char p4);
void __stdcall FUN_004560c0(Unit_0041b8d0* obj, Unit_0041b8d0* target);

class Class_0048b090 {
public:
    void SetStateBits(int a, int b);
};

// FUNCTION: 0x41b8d0
void __stdcall FUN_0041b8d0(Unit_0041b8d0* unit, Unit_0041b8d0* target)
{
    if (unit && (unit->flags & 0x10000000) && unit->type->field_156 != 0
        && target && (target->flags & 0x10000000)) {
        target->field_9e->field_10 = 0;
        target->field_104 = 0;
        target->flags |= 0x2000;
        if (target->player->active != 0
            && (target->player->type == 1 || target->player->type == 2)) {
            if (target->flags & 0x20000000) {
                if (FUN_004ab060(&g_game->menu, "BUILDER.GUI"))
                    FUN_0049fa90(&g_game->menu);
            } else {
                if (target->field_86 != 0)
                    AttachUnitToPiece(target, 0, -1, 1);
            }
        }
        if (target->type->flag_18)
            ((Class_0048b090*)target)->SetStateBits(1, 1);
        if (g_game->unitIndex == unit->id)
            FUN_004199b0(&g_game->menu, unit);
        if (target->type->flag_24) {
            target->field_f5 = 7;
            target->flags |= 0x4000;
        }
        if (target->player->active != 0
            && (target->player->type == 1 || target->player->type == 2))
            FUN_004560c0(unit, target);
        if ((unit->flags & 0x10) || (target->flags & 0x10))
            g_game->flags |= 0x10;
    }
}
