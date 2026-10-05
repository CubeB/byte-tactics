// Decompiled by Opus. Names are provisional.
// Returns the unit a target entry (see 0x48a160) points at, or 0 when the
// entry is not a unit target.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x118];
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
};
#pragma pack(pop)

extern Game* g_game;

struct Point_0048a190 {
    short a;                           // +0x0
    short b;                           // +0x2
};

struct Entry_0048a190 {
    Point_0048a190 point;              // +0x0
    char unknown_4[0x1c - 4];
};

struct Class_0048a190 {
    int unknown_0;
    Entry_0048a190 entries[1];         // +0x4
};

// FUNCTION: 0x48a190
Unit* __stdcall GetWeaponTargetUnit(Class_0048a190* obj, int index)
{
    Point_0048a190* p = &obj->entries[index].point;
    if (p->b != (short)0x8000) {
        return 0;
    }
    if (p->a == 0) {
        return 0;
    }
    return &g_game->units[p->a];
}
