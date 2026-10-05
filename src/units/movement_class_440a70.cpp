// Decompiled by Opus. Names are provisional.
// Refreshes the entries of the table at 0x512358 for an object: entries newer
// than the object's last update if it has a unit, otherwise every entry in use
// (an inlined helper taking the two points by value, so they stay in registers).

struct Point_00440a70 {
    short x;
    short y;
};

struct Class_00440320 {
    int* field_0;                      // +0x0
    char unknown_4[0x1c - 0x4];
    unsigned int field_1c;             // +0x1c
};

class Class_00440830 {
public:
    void FUN_00440830(Point_00440a70 a, Point_00440a70 b);
};

struct Class_00440290 {
    Class_00440320 entries[32];

    static Class_00440290 DAT_00512358;
};

#pragma pack(push, 2)
struct Unit_00440a70 {
    char unknown_0[0x26];
    unsigned int lastTick;             // +0x26
};

struct Struct_00440a70 {
    Unit_00440a70* unit;               // +0x0
    char unknown_4[0x76 - 0x4];
    Point_00440a70 a;                  // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point_00440a70 b;                  // +0x7e
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    unsigned int ticks;                // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

static inline void UpdateAll(Point_00440a70 a, Point_00440a70 b)
{
    for (int i = 0; i < 32; i++) {
        if (Class_00440290::DAT_00512358.entries[i].field_0 != 0) {
            ((Class_00440830*)&Class_00440290::DAT_00512358.entries[i])->FUN_00440830(a, b);
        }
    }
}

// FUNCTION: 0x440a70
void __stdcall FUN_00440a70(Struct_00440a70* p)
{
    if (p->unit != 0) {
        unsigned int last = p->unit->lastTick;
        p->unit->lastTick = g_game->ticks;
        for (int i = 0; i < 32; i++) {
            if (last < Class_00440290::DAT_00512358.entries[i].field_1c) {
                ((Class_00440830*)&Class_00440290::DAT_00512358.entries[i])->FUN_00440830(p->a, p->b);
            }
        }
        return;
    }
    UpdateAll(p->a, p->b);
}
