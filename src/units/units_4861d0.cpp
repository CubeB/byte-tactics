// Decompiled by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// Creates a unit from a spawn record (the 0x11-byte record the build and
// placement code fills in: player, unit type, unit id, position). It takes the
// unit slot for the record's id out of the unit array, refuses to go on when
// that player has no unit list, clears the slot when it is still in use,
// initialises the unit (the matched FUN_00485e90, inlined), then registers it
// with FUN_0048a870, FUN_0047cc30, FUN_00482ac0 and the list manager and bumps
// the player's counters at +0x144 and +0x140.
//
// The middle of the function is FUN_00485e90 (src/units/units_485e90.cpp)
// inlined by /Ob2: the 12-byte local frame is its by-value position argument,
// and its type argument stays in dx because it is an unsigned short. Written
// out in the body, or with an int type parameter as 0x485e90.cpp has it, the
// type load gets a `xor edx, edx` zero-extension the original does not have.
// 0x485e90 also matches with `unsigned short unitType`.
//
// The player's record is taken once as a pointer for the test at the top and
// indexed again from g_game for the counters at the end, as in 0x486f10 from
// the same file. That keeps only the scaled index (player * 0x14b) alive, in
// the dead first-parameter slot, and puts its multiply before the unit lookup.

struct Unit_004861d0;

struct Class_00481490 {                  // 0x1c bytes, vtable 0x4fd6f0
    void* vtable;                        // +0x0
    char unknown_4[0x1c - 0x4];
};

class Class_0043dc00 {
public:
    char unknown_0[0x2f];
    Class_0043dc00(Unit_004861d0* unit);
};

class Class_00490520 {
public:
    void FUN_00490580(Unit_004861d0* unit);
};

struct Pos_004861d0 {
    int x, y, z;
};

#pragma pack(push, 1)
struct UnitType_004861d0 {               // 0x249 bytes
    char unknown_0[0x210];
    short field_210;                     // +0x210
    char unknown_212[0x22f - 0x212];
    unsigned char field_22f;             // +0x22f
    char unknown_230[0x249 - 0x230];
};

struct Spawn_004861d0 {                  // 0x11 bytes
    unsigned char player;                // +0x0
    unsigned short type;                 // +0x1
    unsigned short id;                   // +0x3
    Pos_004861d0 pos;                    // +0x5
};

struct Unit_004861d0 {                   // 0x118 bytes
    Class_0043dc00* obj;                 // +0x0
    char unknown_4[0x8 - 0x4];
    Class_00481490 sub_8;                // +0x8
    Class_00481490 sub_24;               // +0x24
    Class_00481490 sub_40;               // +0x40
    char unknown_5c[0x66 - 0x5c];
    short field_66;                      // +0x66
    char unknown_68[0x92 - 0x68];
    UnitType_004861d0* type;             // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                      // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

struct Player_004861d0 {                 // 0x14b bytes
    char unknown_0[0x67];
    Unit_004861d0* units_begin;          // +0x67
    char unknown_6b[0x140 - 0x6b];
    int field_140;                       // +0x140
    short field_144;                     // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game_004861d0 {
    char unknown_0[0x1b63];
    Player_004861d0 players[10];         // +0x1b63
    char unknown_2851[0x14357 - 0x2851];
    Unit_004861d0* units;                // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    UnitType_004861d0* unitTypes;        // +0x1439b
    char unknown_1439f[0x391ed - 0x1439f];
    Class_00490520* list;                // +0x391ed
};
#pragma pack(pop)

extern Game_004861d0* g_game;
extern void* DAT_004fd6f0[];

void __stdcall FUN_004864b0(Unit_004861d0* unit, int param_2);
void __stdcall FUN_00485a40(Unit_004861d0* unit, Pos_004861d0 pos, int param_5);
void __stdcall FUN_00485d40(Unit_004861d0* unit);
void __stdcall FUN_0049e070(Unit_004861d0* unit);
void __stdcall FUN_00437840(Unit_004861d0* unit);
void __stdcall FUN_0048a870(Unit_004861d0* unit);
void __stdcall FUN_0047cc30(Unit_004861d0* unit);
void __stdcall FUN_00482ac0(Unit_004861d0* unit);
void* __cdecl operator new(unsigned int size);

// Inlined copy of FUN_00485e90. As there, the three vtable stores are guarded
// by `if (unit)` but the +0xa6 store and the new expression are not.
static inline void __stdcall InitUnit_00485e90(unsigned short unitType, Pos_004861d0 pos,
                                               int param_5, Unit_004861d0* unit)
{
    UnitType_004861d0* type = &g_game->unitTypes[unitType];
    if (unit) {
        Class_00481490* sub = (Class_00481490*)((char*)unit + 8);
        for (int i = 0; i < 3; i++) {
            sub->vtable = DAT_004fd6f0;
            sub = (Class_00481490*)((char*)sub + 0x1c);
        }
    }
    unit->field_a6 = unitType;
    FUN_00485a40(unit, pos, param_5);
    FUN_00485d40(unit);
    FUN_0049e070(unit);
    FUN_00437840(unit);
    if (type->field_22f == 1) {
        unit->obj = new Class_0043dc00(unit);
        unit->field_66 = unit->type->field_210;
    }
}

// FUNCTION: 0x4861d0
Unit_004861d0* __stdcall FUN_004861d0(unsigned char player, Spawn_004861d0* spawn)
{
    Player_004861d0* pl = &g_game->players[player];
    Unit_004861d0* unit;
    if (spawn->id == 0) {
        unit = 0;
    } else {
        unit = &g_game->units[spawn->id];
    }
    if (pl->units_begin == 0) {
        return 0;
    }
    if (unit->field_a6 != 0) {
        FUN_004864b0(unit, 0);
    }
    InitUnit_00485e90(spawn->type, spawn->pos, 0, unit);
    FUN_0048a870(unit);
    FUN_0047cc30(unit);
    FUN_00482ac0(unit);
    g_game->players[player].field_144++;
    g_game->players[player].field_140++;
    g_game->list->FUN_00490580(unit);
    return unit;
}
