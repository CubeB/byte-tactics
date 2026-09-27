// Decompiled by space-bunny-free. Names are provisional.
// Creates a unit from a spawn record (the 0x11-byte record the build and
// placement code fills in: player, unit type, unit id, position). It takes the
// unit slot for the record's id out of the unit array, refuses to go on when
// that player holds no unit, resets the unit's three 0x1c-byte sub-objects,
// hands FUN_00485a40 the record's position and type, builds the 0x2f-byte
// object when the unit type asks for it, then registers the unit with the two
// list managers and bumps the player's counters at +0x144 and +0x140.
//
// NOT MATCHING yet (best 62.9%, everything else in the function matches):
//   1. The player index. The original keeps g_game in ebx and the byte offset
//      of the player record (index * 0x14b) in edx, storing that offset in the
//      dead first-parameter slot and addressing [g_game + off + 0x1bca],
//      [g_game + off + 0x1ca7] and [g_game + off + 0x1ca3]. Any spelling I
//      tried makes MSVC materialise the whole address instead (g_game + p in
//      esi, the address in ebx) or re-read the parameter and store the offset
//      in the second-parameter slot. Writing the three accesses out longhand
//      gives the right [base + index + disp] shape but the wrong slot and the
//      multiply lands after the branch rather than before it.
//   2. The three position words. The original loads x into ebp, spills y and z
//      to the saved ebp and ebx slots, and keeps the unit type in dx; mine
//      loads z and y first and re-loads the type after the vtable loop.
//   3. Because of (2) the two counter updates also use a full pointer rather
//      than the reloaded offset, so they read [pl + 0x144] instead of
//      [g_game + off + 0x1ca7].
// The frame shape, the if/else on the record id, the vtable loop, the
// by-value 12-byte argument block for FUN_00485a40, the Class_0043dc00
// allocation (as in 0x485e50) and both returns all match.
//
// Two further experiments from the orchestrator, both on scratch copies, which
// show the remaining difference is the *scheduling* of the player offset rather
// than its form. Writing the three accesses longhand, with no `pl` local at
// all (`g_game->players[(unsigned char)player].first` and so on), does produce
// the original's `base + index + disp` addressing, but MSVC then rematerialises
// the index at each use instead of hoisting it: it spills the *unscaled* index
// to the dead parameter slot and only multiplies at the first use, which puts
// the 331*player chain after the `spawn->id` branch rather than before it
// (62.9%, 389 of 395 bytes). Hoisting the index into a local declared before
// the branch (`int idx = (unsigned char)player;` with `g_game->players[idx]`
// at each use) is worse again at 58.8%: the multiply is not hoisted at all.
// So MSVC 5 will not keep the scaled offset in a register across that branch
// from any spelling tried here, while the original does.
//
// The original reuses the saved ebp and ebx slots for the y and z arguments
// and re-reads them for the call, and the epilogue pops those slots, so it
// returns with ebx and ebp holding y and z instead of their old values. That
// is reproduced here by the same spill, and is a bug in the original.

struct Unit_004861d0;

struct Class_00481490 {
    void** vptr;                        // +0x00
    char unknown_4[0x1c - 4];
};

class Class_0043dc00 {
public:
    char unknown_0[0x2f];
    Class_0043dc00(Unit_004861d0* unit);
};

class Class_00490520 {
public:
    void FUN_00490580(void* param_1);
};

struct Vec3_004861d0 {
    int x, y, z;
};

#pragma pack(push, 1)
struct BuildType_004861d0 {              // 0x249 bytes
    char unknown_0[0x210];
    short field_210;                     // +0x210
    char unknown_212[0x22f - 0x212];
    unsigned char field_22f;             // +0x22f
    char unknown_230[0x249 - 0x230];
};

struct Spawn_004861d0 {
    unsigned char player;                // +0x00
    unsigned short type;                 // +0x01
    unsigned short id;                   // +0x03
    int x;                               // +0x05
    int y;                               // +0x09
    int z;                               // +0x0d
};

struct Unit_004861d0 {                  // 0x118 bytes
    Class_0043dc00* obj;                 // +0x00
    char unknown_4[0x8 - 0x4];
    Class_00481490 subs[3];              // +0x08
    char unknown_5c[0x66 - 0x5c];
    short field_66;                      // +0x66
    char unknown_68[0x92 - 0x68];
    BuildType_004861d0* type;            // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                      // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

struct Player_004861d0 {                // 0x14b bytes
    char unknown_0[0x67];
    int first;                           // +0x67
    char unknown_6b[0x140 - 0x6b];
    int count;                           // +0x140
    short count2;                        // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game_004861d0 {
    char unknown_0[0x1b63];
    Player_004861d0 players[10];         // +0x1b63
    char unknown_2851[0x14357 - 0x2851];
    Unit_004861d0* units;                // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    BuildType_004861d0* buildTypes;      // +0x1439b
    char unknown_1439f[0x391ed - 0x1439f];
    Class_00490520* list;                // +0x391ed
};
#pragma pack(pop)

extern Game_004861d0* g_game;
extern void* DAT_004fd6f0[];

void __stdcall FUN_004864b0(void* unit, int param_2);
void __stdcall FUN_00485a40(Unit_004861d0* unit, Vec3_004861d0 pos, int flag);
void __stdcall FUN_00485d40(Unit_004861d0* unit);
void __stdcall FUN_0049e070(Unit_004861d0* unit);
void __stdcall FUN_00437840(Unit_004861d0* unit);
void __stdcall FUN_0048a870(Unit_004861d0* unit);
void __stdcall FUN_0047cc30(Unit_004861d0* unit);
void __stdcall FUN_00482ac0(Unit_004861d0* unit);

// FUNCTION: 0x4861d0
Unit_004861d0* __stdcall FUN_004861d0(int player, Spawn_004861d0* spawn)
{
    Player_004861d0* pl = &g_game->players[(unsigned char)player];
    Unit_004861d0* unit;
    if (spawn->id == 0) {
        unit = 0;
    } else {
        unit = &g_game->units[spawn->id];
    }
    if (pl->first == 0) {
        return 0;
    }
    if (unit->field_a6 != 0) {
        FUN_004864b0(unit, 0);
    }
    Vec3_004861d0 pos;
    pos.x = spawn->x;
    pos.y = spawn->y;
    pos.z = spawn->z;
    BuildType_004861d0* buildType = &g_game->buildTypes[spawn->type];
    if (unit != 0) {
        for (int i = 0; i < 3; i++) {
            unit->subs[i].vptr = DAT_004fd6f0;
        }
    }
    unit->field_a6 = spawn->type;
    FUN_00485a40(unit, pos, 0);
    FUN_00485d40(unit);
    FUN_0049e070(unit);
    FUN_00437840(unit);
    if (buildType->field_22f == 1) {
        unit->obj = new Class_0043dc00(unit);
        unit->field_66 = unit->type->field_210;
    }
    FUN_0048a870(unit);
    FUN_0047cc30(unit);
    FUN_00482ac0(unit);
    pl->count2++;
    pl->count++;
    g_game->list->FUN_00490580(unit);
    return unit;
}
