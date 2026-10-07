// Decompiled by Opus, DeepSeek V4.1 Flash, Claude Opus 5.5, Haiku, deepseek-v4.1-flash, GPT-6.1-sol, deepseek-v4.1, space-bunny-free, fledge-alpha-free, GPT-6 and Sonnet. Names are provisional.
// The in-game control panel's order and build menu handling: the order-mode
// state, the build menu tables, the order buttons and the build progress. The
// files of the module's first part (0x419560 to 0x41bcd0) gathered in address
// order; 0x41b2e0 keeps its own file (its locals' symbol ids decide its match).
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

// The copy constructor is only declared: it is never called (the temporary is
// elided), but declaring it makes MSVC build the by-value argument in place in
// the callee's argument slot, which is what the original does.
class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760(const Class_00438760& other);
};

struct Option_00419560;

struct Pair_00419560 {
    int a;
    int b;
};

struct Arg_00419670 {
    char unknown_0[8];
    unsigned int field_8;              // +0x8
};

struct Unit;
struct Player;
struct Owner;
struct Nano_0041b8d0;
struct MenuEntry;
struct Layer;
struct Menu;
struct BuildList_0041ace0;

class UnitResources {
public:
    int RequestEnergyAndMetal(float energy, float metal);
};

struct Nano_0041b8d0 {
    char unknown_0[0x10];
    int field_10;                      // +0x10
};

struct UnitTypeFlagsBits {
    unsigned int unknown_0 : 6;        // +0x241 bits 0-5
    unsigned int flag_6 : 1;           // bit 6
    unsigned int unknown_7 : 4;        // bits 7-10
    unsigned int flag_11 : 1;          // bit 11
    unsigned int unknown_12 : 6;       // bits 12-17
    unsigned int flag_18 : 1;          // bit 18
    unsigned int unknown_19 : 5;       // bits 19-23
    unsigned int flag_24 : 1;          // bit 24
    unsigned int unknown_25 : 7;       // bits 25-31
};

union UnitTypeFlags {
    unsigned int raw;
    UnitTypeFlagsBits bits;
};

struct UnitTypeFlags2 {
    unsigned int canMoveOrder : 1;     // +0x245 bit 0
    unsigned int canFireOrder : 1;     // bit 1
    unsigned int canOnOff : 1;         // bit 2
    unsigned int canStop : 1;          // bit 3
    unsigned int canAttack : 1;        // bit 4
    unsigned int canDefend : 1;        // bit 5
    unsigned int canPatrol : 1;        // bit 6
    unsigned int canMove : 1;          // bit 7
    unsigned int canLoad : 1;          // bit 8
    unsigned int canRepair : 1;        // bit 9
    unsigned int canReclaim : 1;       // bit 10
    unsigned int bit11 : 1;
    unsigned int canCapture : 1;       // bit 12
    unsigned int canCloak : 1;         // bit 13
    unsigned int canBlast : 1;         // bit 14
    unsigned int bits15 : 17;
};

// The game's build type table entry (0x249 bytes), the unit's own type as well
// as a build menu's entry.
struct UnitType {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x111 - 0x40];
    unsigned int field_111;            // +0x111
    char unknown_115[0x14a - 0x115];
    Point origin;                      // +0x14a
    char unknown_14e[0x156 - 0x14e];
    int field_156;                     // +0x156
    char unknown_15a[0x186 - 0x15a];
    float energyCost;                  // +0x186 (TDF buildcostenergy)
    float metalCost;                   // +0x18a (TDF buildcostmetal)
    char unknown_18e[0x1ea - 0x18e];
    int buildTime;                     // +0x1ea
    char unknown_1ee[0x1fa - 0x1ee];
    unsigned int maxHp;                // +0x1fa
    char unknown_1fe[0x21e - 0x1fe];
    short id;                          // +0x21e
    char unknown_220[0x22e - 0x220];
    unsigned char field_22e;           // +0x22e
    unsigned char field_22f;           // +0x22f
    char unknown_230[0x241 - 0x230];
    UnitTypeFlags flags;               // +0x241
    UnitTypeFlags2 flags2;             // +0x245
};

// A GUI layer's entry (0x15b bytes); entry 0 stores the entry count where the
// others keep their text.
struct MenuEntry {
    unsigned char type;                // +0x0
    char unknown_1;
    char name[0x26];                   // +0x2
    unsigned char flags;               // +0x28
    char unknown_29;
    unsigned char field_2a;            // +0x2a
    char unknown_2b[0x60 - 0x2b];
    int index;                         // +0x60
    char unknown_64[0xb4 - 0x64];
    unsigned short shown : 1;          // +0xb4 bit 0
    unsigned short bits_b4 : 15;
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0x138 - 0xb6];       // +0xb6
    } u;
    short state;                       // +0x138
    char unknown_13a[0x13c - 0x13a];
    unsigned short enabled : 1;        // +0x13c bit 0
    unsigned short bits_13c : 15;
    char unknown_13e[0x15b - 0x13e];
};

struct Layer {
    int unknown_0;                     // +0x0
    MenuEntry* entries;                // +0x4
    void (__stdcall* handler)(Menu*);  // +0x8
    int field_c;                       // +0xc
};

struct Menu {
    char unknown_0[0x18];
    Layer* layer;                      // +0x18
    char unknown_1c[0x60 - 0x1c];
    int index;                         // +0x60
    char unknown_64[0xcca - 0x64];
    int field_cca;                     // +0xcca
};

struct Owner {
    Unit* unit;                        // +0x0
    char unknown_4[0x95 - 4];
    unsigned char playerIndex;         // +0x95
};

struct Player {
    int active;                        // +0x0
    char unknown_4[0x27 - 4];
    Owner* owner;                      // +0x27
    char unknown_2b[0x67 - 0x2b];
    Unit* units;                       // +0x67
    Unit* unitsEnd;                    // +0x6b
    char unknown_6f[0x73 - 0x6f];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct UnitFlagsBits {
    unsigned int unknown_0 : 4;        // +0x110 bits 0-3
    unsigned int selected : 1;         // bit 4
    unsigned int unknown_5 : 6;        // bits 5-10
    unsigned int cloak : 1;            // bit 11
    unsigned int unknown_12 : 6;       // bits 12-17
    unsigned int moveOrder : 2;        // bits 18-19
    unsigned int fireOrder : 2;        // bits 20-21
    unsigned int buildPage : 1;        // bit 22
    unsigned int page : 3;             // bits 23-25
    unsigned int unknown_26 : 3;       // bits 26-28
    unsigned int flag_29 : 1;          // bit 29
    unsigned int unknown_30 : 2;       // bits 30-31
};

union UnitFlags {
    unsigned int raw;
    UnitFlagsBits bits;
};

struct Unit {
    int field_0;                       // +0x0
    char unknown_4[0x8 - 4];
    unsigned int field_8;              // +0x8
    char unknown_c[0x10 - 0xc];
    void* field_10;                    // +0x10
    char unknown_14[0x1e - 0x14];
    unsigned char field_1e;            // +0x1e
    char unknown_1f[0x86 - 0x1f];
    int field_86;                      // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType* type;                    // +0x92
    Player* player;                    // +0x96
    char unknown_9a[0x9e - 0x9a];
    Nano_0041b8d0* field_9e;           // +0x9e
    char unknown_a2[0xa6 - 0xa2];
    unsigned short field_a6;           // +0xa6
    unsigned short field_a8;           // +0xa8
    char unknown_aa[0xbb - 0xaa];
    unsigned char flags_bb;            // +0xbb
    UnitResources store;               // +0xbc
    char unknown_bd[0xd4 - 0xbd];
    float field_d4;                    // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player* field_ec;                  // +0xec
    char unknown_f0[0xf5 - 0xf0];
    unsigned char field_f5;            // +0xf5
    char unknown_f6[0xff - 0xf6];
    unsigned char field_ff;            // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                   // +0x104
    short hp;                          // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned short onOff : 1;          // +0x10e bit 0
    unsigned short bits_10e : 15;
    UnitFlags flags;                   // +0x110
    char unknown_114[0x118 - 0x114];

    void SetStateBits(int a, int b);
};

struct OrdersBits {
    unsigned short unknown_0 : 1;      // +0x37ebe bit 0
    unsigned short refresh : 1;        // bit 1
    unsigned short unknown_2 : 5;      // bits 2-6
    unsigned short next : 1;           // bit 7
    unsigned short prev : 1;           // bit 8
    unsigned short orders : 1;         // bit 9
    unsigned short build : 1;          // bit 10
    unsigned short unknown_11 : 1;     // bit 11
    unsigned short fireOrder : 3;      // bits 12-14
    unsigned short unknown_15 : 1;     // bit 15
    unsigned short moveOrder : 3;      // +0x37ec0 bits 0-2
    unsigned short cloak : 2;          // bits 3-4
    unsigned short onOff : 2;          // bits 5-6
    unsigned short canMove : 1;        // bit 7
    unsigned short canStop : 1;        // bit 8
    unsigned short canAttack : 1;      // bit 9
    unsigned short canDefend : 1;      // bit 10
    unsigned short canPatrol : 1;      // bit 11
    unsigned short canLoad : 1;        // bit 12
    unsigned short canReclaim : 1;     // bit 13
    unsigned short canCapture : 1;     // bit 14
    unsigned short canRepair : 1;      // bit 15
    unsigned short canBlast : 1;       // +0x37ec2 bit 0
    unsigned short unknown_1 : 15;
};

union Orders {
    unsigned short raw;
    OrdersBits bits;
};

struct BitFlags_004197d0 {
    unsigned char b0:1;
    unsigned char b1:1;
    unsigned char b2:1;
    unsigned char b3:1;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char b6:1;
    unsigned char b7:1;
};

union Flags_004197d0 {
    unsigned char value;
    BitFlags_004197d0 bits;
};

// A unit type's extra build menu entry (0x25 bytes).
struct BuildEntry_0041ace0 {
    short typeId;                      // +0x00
    unsigned char page;                // +0x02
    unsigned char slot;                // +0x03
    char name[0x21];                   // +0x04
};

struct BuildList_0041ace0 {            // 0xbd bytes
    int count;                         // +0x00
    BuildEntry_0041ace0 entries[5];    // +0x04
};

// One player's side name (0x232 bytes); 0x419560 reads the dword at +0x224.
struct SideName {
    char name[0x224];                  // +0x0
    int field_224;                     // +0x224
    char unknown_228[0x232 - 0x228];
};

struct Game {
    char unknown_0[0x519];
    Menu menu;                         // +0x519
    char unknown_11e7[0x1b63 - 0x11e7];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    char unknown_2a44[0x2c76 - 0x2a44];
    char orders[0x2c92 - 0x2c76];      // +0x2c76
    int field_2c92;                    // +0x2c92
    int field_2c96;                    // +0x2c96
    int field_2c9a;                    // +0x2c9a
    int field_2c9e;                    // +0x2c9e
    int field_2ca2;                    // +0x2ca2
    int field_2ca6;                    // +0x2ca6
    Vec3 pos;                          // +0x2caa
    char unknown_2cb6[0x2cc3 - 0x2cb6];
    unsigned char field_2cc3;          // +0x2cc3
    unsigned short field_2cc4;         // +0x2cc4
    Flags_004197d0 flags;              // +0x2cc6
    char unknown_2cc7[0x14357 - 0x2cc7];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    UnitType* buildTypes;              // +0x1439b
    char unknown_1439f[0x37e9c - 0x1439f];
    unsigned short unitIndex;          // +0x37e9c
    unsigned short field_37e9e;        // +0x37e9e
    char unknown_37ea0[0x37ebe - 0x37ea0];
    Orders orderState;                 // +0x37ebe
    char unknown_37ec4[0x37eee - 0x37ec4];
    int difficulty;                    // +0x37eee (a guess)
    char unknown_37ef2[0x37f5b - 0x37ef2];
    SideName sideNames[8];             // +0x37f5b
    char unknown_390eb[0x391c7 - 0x390eb];
    int buildListCount;                // +0x391c7
    BuildList_0041ace0* buildLists;    // +0x391cb
};

#pragma pack(pop)

extern Game* g_game;

extern int DAT_00511bc0;
extern int DAT_00511bc4;
extern int DAT_00511bc8;
extern int DAT_00511c20;
extern int DAT_00511c34;
extern int DAT_00511c48;
extern int DAT_00511c50;
extern Pair_00419560 DAT_00511a60[44];
extern Pair_00419560 DAT_00511c60[44];
extern Option_00419560 DAT_00501d38;
extern Option_00419560 DAT_00501f48;
extern Option_00419560 DAT_00501fd0;

void __stdcall RegisterCommands(Option_00419560* option);
void __stdcall SetDefaultCommandHandler(void (__stdcall* callback)(int), int param_2);
void __stdcall FUN_00417890(int param_1);

void __stdcall GetGadgetName(MenuEntry* entries, char* name, int index);
void __stdcall FUN_0043afc0(Class_00438760 kind, int remove, Unit* owner,
                            int id, Vec3* pos, int param_6, int param_7);
int __stdcall CanBuildAt(UnitType* type, Point cell, int a, Player* player);
int GetBuildSiteHeight(void);
int __stdcall GetFootprintHeight(UnitType* unit, Point cell);
MenuEntry* __stdcall FindGadgetOrNull(MenuEntry* entries, char* name);
unsigned short __stdcall FindUnitTypeId(char* name);
int __stdcall FUN_00439d80(void* owner, int index);
void __stdcall FUN_0049fa90(void* obj);
int __stdcall FindGadgetIndexBySubstring(int value, char* name);
int __stdcall FindGadgetIndexBySubstring(MenuEntry* entries, char* name);
void __stdcall SetGadgetStatus(Menu* menu, int index, int value);
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall FUN_0043b0b0(Class_00438760 kind, Unit* unit, int id, int count);
void __stdcall IssueOrderToSelection(void* a, int b, Class_00438760 kind,
                                     int d, int e, int f);
void __stdcall FUN_004a1200(Menu* menu, int index, int value);
void __stdcall FUN_004a03f0(Menu* menu, int index, char value);
void __stdcall RenderLayer(Menu* menu, int value);
void __stdcall FUN_004a0570(Menu* menu, char* name, int param_3);
void __stdcall FUN_004ab0a0(Menu* menu);
int __stdcall HandleOrdersPanelClick(Menu* menu, MenuEntry* entries);
int __stdcall HandleOrderButtonClick(Menu* menu, MenuEntry* entries);
int __stdcall IsKeyDown(int key);
int __stdcall FUN_004ab6b0(Menu* menu);
void __stdcall QueueBuildOrder(char* name, Unit* unit, int count);
void __stdcall RefreshBuildCountTexts(Menu* menu, Unit* unit);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall HAPI_FileLengthByName(char* path);
Layer* __stdcall LoadGuiLayer(Menu* menu, const char* name, int flags);
void __stdcall HandleBuildPanelClick(Menu* menu);
void __stdcall RefreshOrderButtons(Unit* unit);
int __stdcall IsScreenNamed(Menu* menu, const char* name);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall FUN_004560c0(Unit* obj, Unit* target);
void __stdcall DamageUnit(Unit* obj, Unit* unit, int n, int kind, int flag);
void __stdcall FinishConstruction(Unit* builder, Unit* unit);
int __stdcall AddBuildProgress(Unit* builder, Unit* unit, float amount);

static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

static inline void CellToWorld(Point origin, Point c, Vec3* v)
{
    v->x = (origin.x + c.x * 2) << 19;
    v->z = (origin.y + c.y * 2) << 19;
}

// FUNCTION: 0x419560
void FUN_00419560()
{
    DAT_00511c20 = g_game->sideNames[4].field_224;
    DAT_00511bc0 = 0;
    DAT_00511bc4 = 0;
    // The second field of the second table is cleared through a walking
    // pointer: the original keeps a separate induction pointer for it.
    Pair_00419560* p = DAT_00511c60;
    for (int i = 0; i < 44; i++) {
        DAT_00511a60[i].a = 0;
        DAT_00511a60[i].b = 0;
        DAT_00511c60[i].a = 0;
        p->b = 0;
        p++;
    }
    DAT_00511c34 = 0;
    DAT_00511bc8 = 0;
    DAT_00511c48 = 0;
    DAT_00511c50 = 0;
    RegisterCommands(&DAT_00501d38);
    RegisterCommands(&DAT_00501f48);
    RegisterCommands(&DAT_00501fd0);
    SetDefaultCommandHandler(FUN_00417890, 4);
}

// Copies the 32-character name of entry `index` of the table at
// g_game+0x1439b (0x249-byte entries) into `dest`. <windows.h> fixes the
// operand order of the address lea (tools/headers.py).
// FUNCTION: 0x4195f0
void __stdcall CopyMenuEntryName(char* dest, unsigned short index)
{
    strncpy(dest, g_game->buildTypes[index].name, 0x20);
    dest[0x1f] = 0;
}

// Returns whether the name of menu entry `index` contains `text`.
// FUNCTION: 0x419630
int __stdcall MenuEntryNameContains(MenuEntry* entries, char* text, int index)
{
    char name[32];
    GetGadgetName(entries, name, index);
    return strstr(name, text) != 0;
}

// Snaps the position at +0x2caa to the centre of its cell for the unit
// type selected at +0x2cc4, then passes it to FUN_0043afc0 as a
// "MOBILEBUILD" (def flag bit 11 clear) or "VTOL_MOBILEBUILD" (bit 11 set)
// order for each of the local player's units with flag 0x10 whose def has
// flag 0x40. Bit 2 of the argument's field_8 is passed through.
// FUNCTION: 0x419670
void __stdcall IssueMobileBuildOrders(Arg_00419670* arg)
{
    unsigned int remove = (arg->field_8 >> 2) & 1;
    unsigned short index = g_game->field_2cc4;
    UnitType* def = &g_game->buildTypes[index];
    Vec3 pos = g_game->pos;
    // origin read once into a local: passing def->origin to each helper changes the frame.
    Point origin = def->origin;
    Point cell = WorldToCell(pos, origin);
    CellToWorld(origin, cell, &pos);
    pos.y = g_game->field_2c96 << 16;

    unsigned char team = g_game->localPlayer;
    Player* p = &g_game->players[team];
    for (Unit* u = p->units; u <= p->unitsEnd; u++) {
        if ((u->flags.raw & 0x10) && (u->type->flags.raw & 0x40)) {
            if (!(u->type->flags.raw & 0x800)) {
                FUN_0043afc0("MOBILEBUILD", remove, u, 0, &pos, index, 0);
            } else {
                UnitTypeFlagsBits flags = u->type->flags.bits;
                if (flags.flag_11) {
                    FUN_0043afc0("VTOL_MOBILEBUILD", remove, u, 0, &pos, index, 0);
                }
            }
        }
    }
}

// FUNCTION: 0x4197d0
int FUN_004197d0(void)
{
    UnitType* item = &g_game->buildTypes[g_game->field_2cc4];
    Point cell = WorldToCell(g_game->pos, item->origin);
    g_game->field_2c92 = cell.x << 4;
    g_game->field_2c9a = cell.y << 4;
    Point origin = item->origin;
    g_game->field_2c9e = (origin.x << 4) + g_game->field_2c92;
    g_game->field_2ca6 = (origin.y << 4) + g_game->field_2c9a;
    g_game->flags.bits.b6 = CanBuildAt(item, cell, 0, &g_game->players[g_game->localPlayer]);
    unsigned char r;
    if (g_game->flags.value & 0x40)
        r = GetBuildSiteHeight();
    else
        r = GetFootprintHeight(item, cell);
    g_game->field_2c96 = r;
    g_game->field_2ca2 = r;
    return (g_game->flags.value >> 6) & 1;
}

// Finds the GUI entry named after list entry `index` of the game's 0x249-byte
// table at +0x1439b and sets its text at +0xb6 to "+<n>", or clears it
// when n is 0.
// FUNCTION: 0x419940
void __stdcall SetBuildCountText(Menu* obj, unsigned short index, int n)
{
    MenuEntry* e = FindGadgetOrNull(obj->layer->entries, g_game->buildTypes[index].name);
    if (e) {
        if (n)
            sprintf(e->u.text, "+%d", n);
        else
            e->u.text[0] = 0;
    }
}

// Refreshes the text of the menu entries owned by the unit's owner. The menu
// keeps a table of 0x15b-byte entries whose first entry stores the entry count
// as a short at +0xb6 (the same offset entry 1..n use for their text). Entries
// whose byte at +0 is 1 have either a name at +2 looked up with FindUnitTypeId
// (printing "+<amount>") or show the unit's own count at +0x1e. Entry 0 holds
// only the count, so the loop starts at entry 1.
// FUNCTION: 0x4199b0
void __stdcall RefreshBuildCountTexts(Menu* menu, Unit* unit)
{
    MenuEntry* e = menu->layer->entries;
    // Read as an int so it is sign-extended once.
    int count = e->u.count;
    e++;
    for (int i = 1; i < count + 1; i++) {
        char* text = e->u.text;
        if (e->type == 1) {
            if (e->field_2a & 4) {
                unsigned short v = FindUnitTypeId(e->name);
                if (v != 0) {
                    int r = FUN_00439d80(unit, v);
                    if (r != 0)
                        sprintf(text, "+%d", r);
                    else
                        text[0] = 0;
                }
            } else if (e->field_2a & 8) {
                int n = unit->field_1e;
                int r = FUN_00439d80(unit, 0);
                text[0] = 0;
                if (n != 0)
                    sprintf(text, "%d", n);
                if (r != 0)
                    sprintf(text + strlen(text), " +%d", r);
            }
        }
        // Incremented after the reads, not before.
        e++;
    }
    FUN_0049fa90(&g_game->menu);
}

// Sets the "ONOFF" menu entry from bit 0 of the unit's flags at +0x10e
// (compare 0x495860).
// FUNCTION: 0x419ac0
void __stdcall UpdateOnOffButton(Unit* unit)
{
    int index = FindGadgetIndexBySubstring((int)g_game->menu.layer->entries, "ONOFF");
    if (index != -1) {
        SetGadgetStatus(&g_game->menu, index, unit->onOff);
    }
}

// FUNCTION: 0x419b00
void __stdcall QueueBuildOrder(char* name, Unit* unit, int count)
{
    if (unit->field_ff == g_game->field_2a43) {
        if (count > 0)
            PlaySoundByName("addbuild", 0);
        else
            PlaySoundByName("subbuild", 0);
    }
    if (strstr(name, "MAKENUKE") != 0 || strstr(name, "MAKEANTI") != 0) {
        FUN_0043b0b0(Class_00438760("BUILDWEAPON"), unit, 0, count);
        return;
    }
    unsigned short id = FindUnitTypeId(name);
    if (id == 0)
        return;
    int mobile = unit->field_0;
    const char* kind = mobile ? "MOBILEBUILD" : "BUILDINGBUILD";
    FUN_0043b0b0(Class_00438760(kind), unit, id, count);
}

// FUNCTION: 0x419bc0
void __stdcall FUN_00419bc0(unsigned char param_1)
{
    g_game->field_2cc3 = param_1;
    g_game->flags.value = g_game->flags.value & 0xf7;
}

// Handles a click on an order button: finds which order the button's name
// contains and selects that order mode (FUN_00419bc0 inlined), plays the
// "immediateorders" or "specialorders" sound and returns 1; returns 0 when the
// name is no order. STOP issues the stop order at once.
static inline void SetOrderMode(unsigned char mode)
{
    g_game->field_2cc3 = mode;
    g_game->flags.value = g_game->flags.value & 0xf7;
}

// FUNCTION: 0x419be0
int __stdcall HandleOrderButtonClick(MenuEntry* button, MenuEntry* entries)
{
    char name[32];
    void* orders = g_game->orders;
    // Uninitialised unless the entry is type 1, as in the original: a `: button` fallback changes the code.
    MenuEntry* e;
    if (entries[button->index].type == 1) e = &entries[button->index];

    GetGadgetName(entries, name, button->index);
    if (strstr(name, "MOVE")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(2);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "STOP")) {
        SetOrderMode(1);
        IssueOrderToSelection(orders, 0, "STOP", 0, 0, 0);
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "ATTACK")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(3);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "BLAST")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(4);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "DEFEND")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(7);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "REPAIR")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(8);
        }
        PlaySoundByName("specialorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "PATROL")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(9);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "RECLAIM")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(0xc);
        }
        PlaySoundByName("specialorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "CAPTURE")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(0xd);
        }
        PlaySoundByName("specialorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "UNLOAD")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(5);
        }
        PlaySoundByName("specialorders", 0);
        return 1;
    }
    GetGadgetName(entries, name, button->index);
    if (strstr(name, "LOAD")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(6);
        }
        PlaySoundByName("immediateorders", 0);
        return 1;
    }
    return 0;
}

// Refreshes the order buttons of the unit menu: the BUILD/ORDERS page toggle
// for the selected unit, the multi-state CLOAK, ONOFF, MOVEORD and FIREORD
// buttons from the selection's combined order state, and disables every order
// button the selection cannot use.
// FUNCTION: 0x41a120
void __stdcall RefreshOrderButtons(Unit* unit)
{
    Menu* menu = &g_game->menu;
    int layer = (int)g_game->menu.layer->entries;
    int index;

    index = FindGadgetIndexBySubstring(layer, "BUILD");
    if (index != -1) {
        if (unit && unit->type->field_22e) {
            SetGadgetStatus(menu, index, unit->flags.bits.buildPage);
        } else {
            FUN_004a1200(menu, index, 1);
        }
    }
    index = FindGadgetIndexBySubstring(layer, "ORDERS");
    if (index != -1) {
        if (unit && unit->type->field_22e) {
            SetGadgetStatus(menu, index, !unit->flags.bits.buildPage);
        } else {
            FUN_004a1200(menu, index, 1);
        }
    }
    index = FindGadgetIndexBySubstring(layer, "CLOAK");
    if (index != -1) {
        if (g_game->orderState.bits.cloak == 3) {
            FUN_004a1200(menu, index, 1);
        } else {
            SetGadgetStatus(menu, index, g_game->orderState.bits.cloak);
        }
    }
    index = FindGadgetIndexBySubstring(layer, "ONOFF");
    if (index != -1) {
        if (g_game->orderState.bits.onOff == 3) {
            FUN_004a1200(menu, index, 1);
        } else {
            SetGadgetStatus(menu, index, g_game->orderState.bits.onOff);
        }
    }
    index = FindGadgetIndexBySubstring(layer, "MOVEORD");
    if (index != -1) {
        if (g_game->orderState.bits.moveOrder == 4) {
            FUN_004a1200(menu, index, 1);
        } else {
            SetGadgetStatus(menu, index, g_game->orderState.bits.moveOrder);
        }
    }
    index = FindGadgetIndexBySubstring(layer, "FIREORD");
    if (index != -1) {
        if (g_game->orderState.bits.fireOrder == 4) {
            FUN_004a1200(menu, index, 1);
        } else {
            SetGadgetStatus(menu, index, g_game->orderState.bits.fireOrder);
        }
    }
    if (!g_game->orderState.bits.canMove) {
        index = FindGadgetIndexBySubstring(layer, "MOVE");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orderState.bits.canStop) {
        index = FindGadgetIndexBySubstring(layer, "STOP");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orderState.bits.canAttack) {
        index = FindGadgetIndexBySubstring(layer, "ATTACK");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orderState.bits.canDefend) {
        index = FindGadgetIndexBySubstring(layer, "DEFEND");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orderState.bits.canPatrol) {
        index = FindGadgetIndexBySubstring(layer, "PATROL");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orderState.bits.canReclaim) {
        index = FindGadgetIndexBySubstring(layer, "RECLAIM");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orderState.bits.canRepair) {
        index = FindGadgetIndexBySubstring(layer, "REPAIR");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orderState.bits.canCapture) {
        index = FindGadgetIndexBySubstring(layer, "CAPTURE");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orderState.bits.canLoad) {
        index = FindGadgetIndexBySubstring(layer, "LOAD");
        if (index != -1) {
            FUN_004a03f0(menu, index, 0);
        }
        index = FindGadgetIndexBySubstring(layer, "UNLOAD");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
        if (!g_game->orderState.bits.canBlast) {
            index = FindGadgetIndexBySubstring(layer, "BLAST");
            if (index != -1) {
                FUN_004a1200(menu, index, 1);
            }
        }
    } else {
        index = FindGadgetIndexBySubstring(layer, "BLAST");
        if (index != -1) {
            FUN_004a03f0(menu, index, 0);
        }
    }
}

// Click handler for the unit orders panel: cycles the move order, fire order,
// activation or cloak setting named by the clicked entry, sends the matching
// order and updates the button. Returns 0 when the entry is none of these.
// Inlined copy of MenuEntryNameContains.
static inline int Contains(MenuEntry* entries, char* text, int index)
{
    char name[32];
    GetGadgetName(entries, name, index);
    return strstr(name, text) != 0;
}

// FUNCTION: 0x41a490
int __stdcall HandleOrdersPanelClick(Menu* menu, MenuEntry* entries)
{
    Unit* unit = &g_game->units[g_game->unitIndex];
    void* orders = g_game->orders;
    int index = menu->index;

    if (Contains(entries, "MOVEORD", index)) {
        switch (g_game->orderState.bits.moveOrder) {
        case 0:
            IssueOrderToSelection(orders, 0, "STANDING_MOVEORDER", 0, 1, 0);
            g_game->orderState.bits.moveOrder = 1;
            break;
        case 1:
            IssueOrderToSelection(orders, 0, "STANDING_MOVEORDER", 0, 2, 0);
            g_game->orderState.bits.moveOrder = 2;
            break;
        case 2:
        case 3:
            IssueOrderToSelection(orders, 0, "STANDING_MOVEORDER", 0, 0, 0);
            g_game->orderState.bits.moveOrder = 0;
            break;
        }
        PlaySoundByName("setmoveorders", 0);
        SetGadgetStatus(&g_game->menu, index, g_game->orderState.bits.moveOrder);
    } else if (Contains(entries, "FIREORD", index)) {
        switch (g_game->orderState.bits.fireOrder) {
        case 0:
            IssueOrderToSelection(orders, 0, "STANDING_FIREORDER", 0, 1, 0);
            g_game->orderState.bits.fireOrder = 1;
            break;
        case 1:
            IssueOrderToSelection(orders, 0, "STANDING_FIREORDER", 0, 2, 0);
            g_game->orderState.bits.fireOrder = 2;
            break;
        case 2:
        case 3:
            IssueOrderToSelection(orders, 0, "STANDING_FIREORDER", 0, 0, 0);
            g_game->orderState.bits.fireOrder = 0;
            break;
        }
        PlaySoundByName("setfireorders", 0);
        SetGadgetStatus(&g_game->menu, index, g_game->orderState.bits.fireOrder);
    } else if (Contains(entries, "STATUS", index) || Contains(entries, "ONOFF", index)) {
        switch (g_game->orderState.bits.onOff) {
        case 0:
            IssueOrderToSelection(orders, 0, "ACTIVATE", 0, 0, 0);
            g_game->orderState.bits.onOff = 1;
            break;
        case 1:
            IssueOrderToSelection(orders, 0, "DEACTIVATE", 0, 0, 0);
            g_game->orderState.bits.onOff = 0;
            break;
        case 2:
            IssueOrderToSelection(orders, 0, "ACTIVATE", 0, 0, 0);
            g_game->orderState.bits.onOff = 1;
            break;
        }
        PlaySoundByName("specialorders", 0);
        SetGadgetStatus(&g_game->menu, index, g_game->orderState.bits.onOff);
    } else if (Contains(entries, "CLOAK", index)) {
        if (g_game->orderState.bits.cloak) {
            IssueOrderToSelection(orders, 0, "CLOAK_OFF", 0, 0, 0);
            g_game->orderState.bits.cloak = 0;
        } else {
            IssueOrderToSelection(orders, 0, "CLOAK_ON", 0, 0, 0);
            g_game->orderState.bits.cloak = 1;
        }
        PlaySoundByName("specialorders", 0);
        SetGadgetStatus(&g_game->menu, index, g_game->orderState.bits.cloak);
    } else {
        return 0;
    }
    RefreshOrderButtons(g_game->unitIndex ? unit : 0);
    RenderLayer(&g_game->menu, 0x40);
    return 1;
}

// Builds the "<unit name>PREV" / "<unit name>NEXT" gadget names for the
// skeleton of the unit's owner and pushes them into the GUI.
// FUNCTION: 0x41a920
void __stdcall SetPrevNextGadgetNames(Unit* unit)
{
    char buf[256];
    if (unit->type->field_22e < 2) {
        sprintf(buf, "%sPREV", g_game->sideNames[unit->player->owner->playerIndex].name);
        FUN_004a0570(&g_game->menu, buf, 0);
        sprintf(buf, "%sNEXT", g_game->sideNames[unit->player->owner->playerIndex].name);
        FUN_004a0570(&g_game->menu, buf, 0);
    }
}

// Click handler of the unit build/orders panel (installed by 0x41ace0 and
// 0x41b0f0): PREV/NEXT/ORDERS/BUILD buttons set request flags, a unit-type
// entry whose type has field_22f == 0 switches to mode 0xe with that type,
// the orders buttons go to HandleOrdersPanelClick, and any other entry adds or removes
// build queue entries (5 at a time when IsKeyDown(0xf9) is set).
// FUNCTION: 0x41aa00
void __stdcall HandleBuildPanelClick(Menu* menu)
{
    if (menu->index != -1) {
        MenuEntry* entries = menu->layer->entries;
        // char[17]: a [20] buffer is placed above the name buffer.
        char idName[17];
        GetGadgetName(entries, idName, menu->index);
        idName[16] = 0;
        unsigned short id = FindUnitTypeId(idName);
        Unit* unit = &g_game->units[g_game->unitIndex];
        char name[32];
        GetGadgetName(entries, name, menu->index);
        if (strstr(name, "PREV")) {
            g_game->orderState.bits.prev = 1;
        } else if (strstr(name, "NEXT")) {
            g_game->orderState.bits.next = 1;
        } else if (strstr(name, "ORDERS")) {
            g_game->orderState.bits.orders = 1;
            PlaySoundByName("ordersbutton", 0);
        } else if (strstr(name, "BUILD")) {
            g_game->orderState.bits.build = 1;
            PlaySoundByName("buildbutton", 0);
        } else if (id != 0 && g_game->buildTypes[id].field_22f == 0) {
            g_game->field_2cc3 = 0xe;
            g_game->field_2cc4 = id;
            PlaySoundByName("addbuild", 0);
        } else if (!HandleOrdersPanelClick(menu, entries) && !HandleOrderButtonClick(menu, entries)) {
            // Own nested if: inside the && chain the bit test is compiled differently.
            if (unit->flags.bits.selected) {
                char text[256];
                GetGadgetName(entries, text, menu->index);
                if (IsKeyDown(0xf9)) {
                    if (FUN_004ab6b0(menu) == 1)
                        QueueBuildOrder(text, unit, 5);
                    else
                        QueueBuildOrder(text, unit, -5);
                } else {
                    if (FUN_004ab6b0(menu) == 1)
                        QueueBuildOrder(text, unit, 1);
                    else
                        QueueBuildOrder(text, unit, -1);
                }
                if (unit->flags.bits.flag_29
                    || (((UnitType*)unit->field_10)->field_111 & 0x10000000))
                    RefreshBuildCountTexts(menu, unit);
                FUN_0049fa90(&g_game->menu);
            }
        }
        // One call after the chain, not one per branch.
        FUN_004ab0a0(menu);
    }
}

static inline MenuEntry* Entries(MenuEntry* t)
{
    return (MenuEntry*)((char*)t + 2);
}

// FUNCTION: 0x41ac90
void __stdcall FUN_0041ac90(Menu* obj)
{
    MenuEntry* t = obj->layer->entries;
    int n = t->u.count;
    for (int i = 0; i < n; i++) {
        if (Entries(t)[i].flags & 4) {
            FUN_004a1200(obj, i, FindUnitTypeId((char*)&Entries(t)[i]) == 0);
        }
    }
}

// Inlined copy of SetPrevNextGadgetNames.
static inline void SetPrevNext(Unit* unit)
{
    char buf[256];
    if (unit->type->field_22e < 2) {
        sprintf(buf, "%sPREV", g_game->sideNames[unit->player->owner->playerIndex].name);
        FUN_004a0570(&g_game->menu, buf, 0);
        sprintf(buf, "%sNEXT", g_game->sideNames[unit->player->owner->playerIndex].name);
        FUN_004a0570(&g_game->menu, buf, 0);
    }
}

// Inlined copy of FUN_0041ac90.
static inline void UpdateCounts(Menu* menu)
{
    MenuEntry* entry = menu->layer->entries;
    int n = entry->u.count;
    int i = 0;
    if (n <= 0)
        return;
    entry = (MenuEntry*)((char*)entry + 2);
    do {
        if (entry[i].flags & 4)
            FUN_004a1200(menu, i, FindUnitTypeId((char*)&entry[i]) == 0);
    } while (++i < n);
}

// Keep the table pointer in entry form while reading its count. The guarded
// do/while places the header advance after the empty-list test.
// FUNCTION: 0x41ace0
void __stdcall OpenBuildMenuGui(Unit* unit, char* guiName, int page)
{
    if (unit->field_104 == 0.0f) {
        Player* player = &g_game->players[g_game->localPlayer];
        int found = 0;
        char path[256];
        char name[256];
        BuildDataPath(path, "guis", guiName, "GUI");
        if (HAPI_FileLengthByName(path) == 0)
            sprintf(name, "%sDL", g_game->sideNames[player->owner->playerIndex].name);
        else
            strcpy(name, guiName);
        Layer* layer = LoadGuiLayer(&g_game->menu, name, 0);
        if (layer != 0) {
            layer->handler = HandleBuildPanelClick;
            layer->field_c = 0;
            for (int i = 0; i < g_game->buildListCount; i++) {
                for (int j = 0; j < g_game->buildLists[i].count; j++) {
                    BuildList_0041ace0* lists = g_game->buildLists;
                    // The entry in a local: the address lea's base and index order follows it.
                    BuildEntry_0041ace0* entry = &lists[i].entries[j];
                    if (unit->type->id == entry->typeId
                        && entry->page - 1 == page) {
                        char* src = entry->name;
                        MenuEntry* e = &layer->entries[entry->slot + 4];
                        e->enabled = 0;
                        e->field_2a = 4;
                        strcpy(e->name, src);
                        found = 1;
                        e->shown = 1;
                    }
                }
            }
            if (found) {
                RenderLayer(&g_game->menu, 2);
                RenderLayer(&g_game->menu, 1);
            }
            SetPrevNext(unit);
            RefreshOrderButtons(unit);
            RefreshBuildCountTexts(&g_game->menu, unit);
            if (unit->flags.raw & 0x20000000) {
                int index = FindGadgetIndexBySubstring(g_game->menu.layer->entries, "ONOFF");
                if (index != -1)
                    SetGadgetStatus(&g_game->menu, index, unit->onOff);
            }
            if (page != 0)
                UpdateCounts(&g_game->menu);
            RenderLayer(&g_game->menu, 0x40);
            g_game->unitIndex = unit->field_a8;
            g_game->field_37e9e = unit->field_a6;
        }
    }
}

// Opens the side-specific "GEN.GUI" dialog (ARMGEN.GUI / COREGEN.GUI) with
// HandleBuildPanelClick as its handler and remembers the selected unit's two ids.
// FUNCTION: 0x41b0f0
void __stdcall OpenGeneratorDialog(Unit* unit)
{
    char name[256];
    sprintf(name, "%sGEN.GUI",
            g_game->sideNames[g_game->players[g_game->localPlayer].owner->playerIndex].name);
    Layer* gadget = LoadGuiLayer(&g_game->menu, name, 0);
    if (gadget != 0) {
        gadget->handler = HandleBuildPanelClick;
        gadget->field_c = 0;
        RefreshOrderButtons(unit);
        RenderLayer(&g_game->menu, 0x40);
        if (unit != 0) {
            g_game->unitIndex = unit->field_a8;
            g_game->field_37e9e = unit->field_a6;
        } else {
            g_game->unitIndex = 0;
            g_game->field_37e9e = 0;
        }
    }
}

// FUNCTION: 0x41b200
int __stdcall FUN_0041b200(Unit* obj)
{
    if (obj->flags.bits.buildPage) {
        return obj->flags.bits.page;
    }
    return 0;
}

// Builds "<name><n>.GUI" for entry `index` of the table at g_game+0x1439b
// (0x249-byte entries) into `dest`. <windows.h> fixes the operand order of
// the address lea (tools/headers.py).
// FUNCTION: 0x41b230
void __stdcall BuildEntryGuiName(char* dest, unsigned short index, int n)
{
    char name[256];
    strncpy(name, g_game->buildTypes[index].name, 0x20);
    name[0x1f] = 0;
    sprintf(dest, "%s%d.GUI", name, n);
}

// FUNCTION: 0x41b2a0
Unit* FUN_0041b2a0()
{
    unsigned short index = g_game->unitIndex;
    if (index) {
        Unit* unit = &g_game->units[index];
        if (unit->field_a6)
            return unit;
    }
    return 0;
}

// Finished-construction notification: marks the target as no longer being
// built (clears its build progress and sets flag 0x2000), updates the
// BUILDER.GUI panel or starts the builder's nanolathe, syncs two unit-type
// flag bits and the selection flag.
// FUNCTION: 0x41b8d0
void __stdcall FinishConstruction(Unit* unit, Unit* target)
{
    if (unit && (unit->flags.raw & 0x10000000) && unit->type->field_156 != 0
        && target && (target->flags.raw & 0x10000000)) {
        target->field_9e->field_10 = 0;
        target->field_104 = 0;
        // In place, not through a local: the OR result is reused for the 0x20000000 test.
        target->flags.raw |= 0x2000;
        if (target->player->active != 0
            && (target->player->type == 1 || target->player->type == 2)) {
            if (target->flags.raw & 0x20000000) {
                if (IsScreenNamed(&g_game->menu, "BUILDER.GUI"))
                    FUN_0049fa90(&g_game->menu);
            } else {
                if (target->field_86 != 0)
                    AttachUnitToPiece(target, 0, -1, 1);
            }
        }
        if (target->type->flags.bits.flag_18)
            target->SetStateBits(1, 1);
        if (g_game->unitIndex == unit->field_a8)
            RefreshBuildCountTexts(&g_game->menu, unit);
        if (target->type->flags.bits.flag_24) {
            target->field_f5 = 7;
            target->flags.raw |= 0x4000;
        }
        if (target->player->active != 0
            && (target->player->type == 1 || target->player->type == 2))
            FUN_004560c0(unit, target);
        if ((unit->flags.raw & 0x10) || (target->flags.raw & 0x10))
            g_game->orderState.raw |= 0x10;
    }
}

// Adds build progress to a unit under construction: `amount` build points
// (negative when it is being taken apart) move the remaining fraction at
// +0x104 towards 0 (done) or 1 (nothing built), clamped to [0, 1]. Building
// charges the builder's resource store (RequestEnergyAndMetal, energy first) for the
// energy and metal share of the step and only proceeds when the store accepts it.
// Unbuilding adds the metal share to the unit's float at +0xd4 (times 0.5 or
// 0.7 for a type 2 player when g_game+0x37eee is 0 or 1) and destroys the
// unit through DamageUnit once nothing is left. Either way the unit's hit
// points follow the progress, and a finished unit goes to FinishConstruction.
// FUNCTION: 0x41ba60
int __stdcall AddBuildProgress(Unit* builder, Unit* unit, float amount)
{
    int result = 0;
    if (unit->field_104 == 0.0f)
        return 0;
    if (amount >= 0.0f)
        unit->flags_bb |= 0x80;
    if (amount == 0.0f)
        return 0;
    UnitType* type = unit->type;
    float prev = unit->field_104;
    float next = min(max(prev - amount / type->buildTime, 0.0f), 1.0f);
    float step = prev - next;
    float energyCharge = type->energyCost * step;
    float metalCharge = type->metalCost * step;
    int hp = (int)(prev * type->maxHp) - (int)(next * type->maxHp);
    if (amount < 0.0f) {
        float refund = -metalCharge;
        // Through a float&: writing unit->field_d4 directly moves the hp load and compare.
        float& store = unit->field_d4;
        if (unit->field_ec->active != 0 && unit->field_ec->type == 2) {
            switch (g_game->difficulty) {
            case 0:
                store += refund * 0.5;
                break;
            case 1:
                store += refund * 0.7;
                break;
            default:
                store += refund;
                break;
            }
        } else {
            store += refund;
        }
        // hp is set before remaining; the other order also changes the build path.
        unit->hp = max(hp + unit->hp, 0);
        unit->field_104 = next;
        unit->flags.raw |= 0x2000;
        if (next >= 1.0f)
            DamageUnit(unit, unit, 30000, 9, 0);
    } else if (builder->store.RequestEnergyAndMetal(energyCharge, metalCharge)) {
        unit->hp = min(hp + unit->hp, unit->type->maxHp);
        unit->field_104 = next;
        unit->flags.raw |= 0x2000;
        result = 1;
    }
    if (unit->field_104 == 0.0f)
        FinishConstruction(builder, unit);
    return result;
}

// FUNCTION: 0x41bcd0
void __stdcall FUN_0041bcd0(Unit* param_1, int param_2)
{
    float val = -((float)(param_1->type->buildTime * param_2) / param_1->type->energyCost);
    AddBuildProgress(param_1, param_1, val);
}
