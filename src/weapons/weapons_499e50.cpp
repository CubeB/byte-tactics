// Decompiled by Opus. Names are provisional.

struct Vec3_499e50 {
    int x;
    int y;
    int z;
};

struct UnitType_499e50 {
    char unknown_0[0xfe];
    short value;                 // +0xfe
};

struct Unit_499e50 {
    UnitType_499e50* type;       // +0
    Vec3_499e50 pos;             // +4
    char unknown_10[0x69 - 0x10];
    unsigned char flags;         // +0x69
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x142f7];
    Unit_499e50* tracked;        // +0x142f7
    char unknown_142fb[0x1433f - 0x142fb];
    Vec3_499e50 trackedPos;      // +0x1433f
    short trackedValue;          // +0x1434b
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x499e50
void __stdcall FUN_00499e50(Unit_499e50* unit)
{
    if (unit == g_game->tracked) {
        g_game->trackedPos = g_game->tracked->pos;
        g_game->trackedValue = unit->type->value;
        g_game->tracked = 0;
    }
    unit->flags |= 2;
}
