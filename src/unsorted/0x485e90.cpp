// Decompiled by space-bunny-free. Names are provisional.
// Initialises a unit: finds its unit type, gives the three 0x1c-byte members
// at +8, +0x24 and +0x40 their vtable, stores the type id at +0xa6, then hands
// the unit to the position/flags setter, the object builder and two more
// methods. When the unit type's +0x22f is 1 the unit also gets a new
// Class_0043dc00 and the type's +0x210 copied to +0x66, the tail of the
// matched FUN_00485e50 (src/unsorted/0x485e50.cpp) inlined.
// The three vtable stores are guarded by `if (unit)` but the +0xa6 store right
// after them, and the new expression at the end, are not: a null unit pointer
// writes to 0xa6, then to 0, and reads 0x92. Kept as the original has it.

class Class_0043dc00;

#pragma pack(push, 1)
struct UnitType_00485e90 {
    char unknown_0[0x210];
    short field_210;                   // +0x210
    char unknown_212[0x22f - 0x212];
    unsigned char field_22f;           // +0x22f
    char unknown_230[0x241 - 0x230];
    unsigned int flags;                // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Pos_00485e90 {
    int x, y, z;
};

struct Class_00481490 {                // 0x1c bytes, vtable 0x4fd6f0
    void* vtable;                      // +0x0
    char unknown_4[0x1c - 0x4];
};

struct Unit_00485e90 {
    Class_0043dc00* obj;               // +0x0
    char unknown_4[0x8 - 0x4];
    Class_00481490 sub_8;              // +0x8
    Class_00481490 sub_24;             // +0x24
    Class_00481490 sub_40;             // +0x40
    char unknown_5c[0x66 - 0x5c];
    short field_66;                    // +0x66
    char unknown_68[0x92 - 0x68];
    UnitType_00485e90* type;           // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                    // +0xa6
};

struct Game_00485e90 {
    char unknown_0[0x1439b];
    UnitType_00485e90* unitTypes;      // +0x1439b
};
#pragma pack(pop)

extern Game_00485e90* g_game;
extern void* DAT_004fd6f0[];

class Class_0043dc00 {
public:
    char unknown_0[0x2f];
    Class_0043dc00(Unit_00485e90* unit);
};

void __stdcall FUN_00485a40(Unit_00485e90* unit, Pos_00485e90 pos, int param_5);
void __stdcall FUN_00485d40(Unit_00485e90* unit);
void __stdcall FUN_0049e070(Unit_00485e90* unit);
void __stdcall FUN_00437840(Unit_00485e90* unit);
void* __cdecl operator new(unsigned int size);

// FUNCTION: 0x485e90
void __stdcall FUN_00485e90(int unitType, Pos_00485e90 pos, int param_5, Unit_00485e90* unit)
{
    UnitType_00485e90* type = &g_game->unitTypes[(unsigned short)unitType];
    if (unit) {
        Class_00481490* sub = (Class_00481490*)((char*)unit + 8);
        for (int i = 0; i < 3; i++) {
            sub->vtable = DAT_004fd6f0;
            sub = (Class_00481490*)((char*)sub + 0x1c);
        }
    }
    unit->field_a6 = (short)unitType;
    FUN_00485a40(unit, pos, param_5);
    FUN_00485d40(unit);
    FUN_0049e070(unit);
    FUN_00437840(unit);
    if (type->field_22f == 1) {
        unit->obj = new Class_0043dc00(unit);
        unit->field_66 = unit->type->field_210;
    }
}
