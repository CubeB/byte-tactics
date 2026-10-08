// Decompiled by Sonnet 5.5, finished by space-bunny-free. Names are provisional.
// Creates a unit of a given type for a player: refuses when the type is not
// buildable or the player already has its limit of that type, takes the
// requested unit slot (or the first free one) from the player's unit list,
// initialises it (InitUnit inlined) and registers it.
// Must stay: without this header the 2-bit store at +0x110 grows by an instruction.
#include <windows.h>

struct Unit;

struct Class_00481490 {                  // 0x1c bytes, vtable 0x4fd6f0
    void* vtable;                        // +0x0
    char unknown_4[0x1c - 0x4];
};

class UnitMotion {
public:
    char unknown_0[0x2f];
    UnitMotion(Unit* unit);
};

class MissionConditions {
public:
    void NotifyUnitCreated(Unit* unit);
};

struct Pos_00485f50 {
    int x, y, z;
};

#pragma pack(push, 1)
struct UnitType_00485f50 {               // 0x249 bytes
    char unknown_0[0x15a];
    int limit;                           // +0x15a
    char unknown_15e[0x210 - 0x15e];
    short field_210;                     // +0x210
    char unknown_212[0x22f - 0x212];
    unsigned char field_22f;             // +0x22f
    char unknown_230[0x241 - 0x230];
    union {
        unsigned int flags;              // +0x241
        struct {
            unsigned int lo : 18;
            unsigned int bit18 : 1;
            unsigned int mid : 5;
            unsigned int bit24 : 1;
            unsigned int hi : 7;
        };
    };
    char unknown_245[0x249 - 0x245];
};

struct Unit {                            // 0x118 bytes
    UnitMotion* obj;                     // +0x0
    char unknown_4[0x8 - 0x4];
    Class_00481490 sub_8;                // +0x8
    Class_00481490 sub_24;               // +0x24
    Class_00481490 sub_40;               // +0x40
    char unknown_5c[0x66 - 0x5c];
    short field_66;                      // +0x66
    char unknown_68[0x92 - 0x68];
    UnitType_00485f50* type;             // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;             // +0xa6
    char unknown_a8[0xf5 - 0xa8];
    unsigned char field_f5;              // +0xf5
    char unknown_f6[0x110 - 0xf6];
    unsigned int mode : 2;               // +0x110
    unsigned int unknown_bits : 12;
    unsigned int bit14 : 1;
    unsigned int unknown_bits2 : 17;
    char unknown_114[0x118 - 0x114];
    void SetStateBits(int a, int b);
};

struct Player_00485f50 {                 // 0x14b bytes
    char unknown_0[0x67];
    Unit* units_begin;                   // +0x67
    Unit* units_end;                     // +0x6b
    char unknown_6f[0x140 - 0x6f];
    int field_140;                       // +0x140
    unsigned short field_144;            // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00485f50 players[10];         // +0x1b63
    char unknown_2851[0x14357 - 0x2851];
    Unit* units;                         // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    UnitType_00485f50* unitTypes;        // +0x1439b
    char unknown_1439f[0x391ed - 0x1439f];
    MissionConditions* list;             // +0x391ed
};
#pragma pack(pop)

extern Game* g_game;
extern void* DAT_004fd6f0[];

void __stdcall InitUnitFromType(Unit* unit, Pos_00485f50 pos, int param_5);
void __stdcall InitUnitScript(Unit* unit);
void __stdcall InitUnitWeaponSlots(Unit* unit);
void __stdcall UpdateMetalExtraction(Unit* unit);
void __stdcall UpdateUnitHeight(Unit* unit);
void __stdcall AddUnitToMap(Unit* unit);
void __stdcall SendNewUnit(Unit* unit);
void __stdcall FUN_004560c0(Unit* a, Unit* b);
void __stdcall RevealNewUnit(Unit* unit);
void* __cdecl operator new(unsigned int size);

static inline void __stdcall InitUnit_00485e90(unsigned short unitType, Pos_00485f50 pos,
                                               int param_5, Unit* unit)
{
    UnitType_00485f50* type = &g_game->unitTypes[unitType];
    if (unit) {
        Class_00481490* sub = (Class_00481490*)((char*)unit + 8);
        for (int i = 0; i < 3; i++) {
            sub->vtable = DAT_004fd6f0;
            sub = (Class_00481490*)((char*)sub + 0x1c);
        }
    }
    unit->field_a6 = unitType;
    InitUnitFromType(unit, pos, param_5);
    InitUnitScript(unit);
    InitUnitWeaponSlots(unit);
    UpdateMetalExtraction(unit);
    if (type->field_22f == 1) {
        unit->obj = new UnitMotion(unit);
        unit->field_66 = unit->type->field_210;
    }
}

// FUNCTION: 0x485f50
Unit* __stdcall CreateUnit(unsigned char player, unsigned short typeId, Pos_00485f50 pos,
                                      int param_5, int mode, unsigned short id)
{
    int off = player * 0x14b;
    Player_00485f50* pl = (Player_00485f50*)((char*)g_game + off + 0x1b63);
    if (typeId == 0)
        return 0;
    UnitType_00485f50* type = &g_game->unitTypes[typeId];
    if (!(type->flags & 0x800000))
        return 0;
    if (type->limit != -1) {
        int count = 0;
        for (Unit* u = pl->units_begin; u <= pl->units_end; u++) {
            if (u->field_a6 == typeId)
                count++;
        }
        if (count >= type->limit)
            return 0;
    }
    Unit* unit;
    for (unit = pl->units_begin; unit <= pl->units_end; unit++) {
        if (id != 0) {
            unit = &g_game->units[id];
            if (unit < pl->units_begin || unit > pl->units_end)
                return 0;
        }
        if (unit->field_a6 == 0)
            goto found;
        if (id != 0)
            return 0;
    }
    return 0;
found:
    InitUnit_00485e90(typeId, pos, param_5, unit);
    unit->mode = mode;
    UpdateUnitHeight(unit);
    AddUnitToMap(unit);
    SendNewUnit(unit);
    if (param_5) {
        if (type->field_22f == 0)
            FUN_004560c0(unit, unit);
        if (unit->type->bit18)
            ((Unit*)unit)->SetStateBits(1, 1);
        if (unit->type->bit24) {
            unit->field_f5 = 7;
            unit->bit14 = 1;
        }
    }
    RevealNewUnit(unit);
    // Array subscript, not a byte-offset cast: it fixes the SIB operand order.
    g_game->players[player].field_144++;
    g_game->players[player].field_140++;
    g_game->list->NotifyUnitCreated(unit);
    return unit;
}
