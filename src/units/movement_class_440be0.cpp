// Decompiled by Opus. Names are provisional.
// One entry of the table at 0x512358 (see 0x440a70): refreshes it for an
// object whose unit was last updated before the entry changed.

struct Point_00440be0 {
    short x;
    short y;
};

#pragma pack(push, 2)
struct Unit_00440be0 {
    char unknown_0[0x26];
    unsigned int lastTick;             // +0x26
};

struct Struct_00440be0 {
    Unit_00440be0* unit;               // +0x0
    char unknown_4[0x76 - 0x4];
    Point_00440be0 a;                  // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point_00440be0 b;                  // +0x7e
};
#pragma pack(pop)

class MovementClass {
public:
    int* field_0;                      // +0x0
    char unknown_4[0x1c - 0x4];
    unsigned int field_1c;             // +0x1c

    void RefreshUnitIfStale(Struct_00440be0* p);
    void RefreshPassMap(Point_00440be0 a, Point_00440be0 b);
};

// FUNCTION: 0x440be0
void MovementClass::RefreshUnitIfStale(Struct_00440be0* p)
{
    if (p->unit->lastTick < field_1c) {
        ((MovementClass*)this)->RefreshPassMap(p->a, p->b);
    }
}
