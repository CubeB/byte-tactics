// Decompiled by Space Bunny Free. Names are provisional.
// Refreshes one entry of a pathfinder's dirty list for the object at p
// (see 0x440a70 / 0x440be0): every unit in the unit table that moved in the
// window since the last refresh gets the new dirty rectangle drawn.

struct Point_00440af0 {
    short x;
    short y;
};

#pragma pack(push, 2)
struct Unit_00440af0 {
    char unknown_0[0x26];
    unsigned int lastTick;             // +0x26
};

struct Object_00440af0 {
    Unit_00440af0* unit;               // +0x0
    char unknown_4[0x76 - 0x4];
    Point_00440af0 a;                  // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point_00440af0 b;                  // +0x7e
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Record_00440af0 {
    Unit_00440af0* unit;               // +0x0
    char unknown_4[0x76 - 0x4];
    Point_00440af0 a;                  // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point_00440af0 b;                  // +0x7e
    char unknown_82[0x110 - 0x82];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game {
    char unknown_0[0x14357];
    Record_00440af0* units;            // +0x14357
    Record_00440af0* units_end;        // +0x1435b
    char unknown_1435f[0x38a47 - 0x1435f];
    unsigned int ticks;                // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class Class_00440830 {
public:
    void FUN_00440830(Point_00440af0 a, Point_00440af0 b);
};

class Class_00440af0 {
public:
    void* field_0;                     // +0x0
    char unknown_4[0x1c - 0x4];
    unsigned int lastTick;             // +0x1c

    void FUN_00440af0(Object_00440af0* p);
};

// FUNCTION: 0x440af0
void Class_00440af0::FUN_00440af0(Object_00440af0* p)
{
    unsigned int now = g_game->ticks;
    unsigned int old = lastTick;
    if (now <= 0x1e) {
        now = 0x1e;
    }
    unsigned int start = now - 0x1e;
    lastTick = start;
    unsigned int prev = p->unit->lastTick;
    p->unit->lastTick = g_game->ticks;
    if (prev < old) {
        ((Class_00440830*)this)->FUN_00440830(p->a, p->b);
    }
    if (start != old) {
        for (Record_00440af0* r = &g_game->units[1]; r <= g_game->units_end; r++) {
            if ((r->flags & 0x10000000) != 0 && r->unit != 0) {
                unsigned int t = r->unit->lastTick;
                if (t >= old && t < start) {
                    ((Class_00440830*)this)->FUN_00440830(r->a, r->b);
                }
            }
        }
    }
    p->unit->lastTick = prev;
}
