// Decompiled by Opus, space-bunny-free and Sonnet. Names are provisional.

// Nothing here uses <windows.h>: its symbols put RefreshPassMap and
// SetPassMapCell in the symbol-id windows they match in (docs/c2-regalloc.md).
#include <windows.h>

void __cdecl FUN_004d85a0(int*);

class TdfRecord {
public:
    int GetFieldInt(const char* name, int def);
};

struct Source_00440340 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

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

struct MovementClass;
unsigned int __stdcall FUN_0047e1f0(MovementClass* obj, int x, int y);

struct MovementClass {
    int* field_0;                      // +0x00
    short footprintX;                  // +0x04
    short footprintZ;                  // +0x06
    short maxWaterDepth;               // +0x08
    short minWaterDepth;               // +0x0a
    unsigned char maxSlope;            // +0x0c
    unsigned char badSlope;            // +0x0d
    unsigned char maxWaterSlope;       // +0x0e
    unsigned char badWaterSlope;       // +0x0f
    unsigned int width;                // +0x10, of the pass map
    unsigned int height;               // +0x14
    unsigned int* cells;               // +0x18
    unsigned int lastTick;             // +0x1c

    MovementClass();
    ~MovementClass();
    void ReadMoveInfo(Source_00440340* src);
    void ResizePassMap(unsigned int w, unsigned int h);
    void SetPassMapCell(int param_1, int param_2, int param_3);
    void RefreshPassMap(Point_00440af0 a, Point_00440af0 b);
    void RefreshMovedUnits(Object_00440af0* p);
    void RefreshUnitIfStale(Object_00440af0* p);
};

// Out-of-line constructor of the 32-byte entry that 0x440290.cpp declares
// with the same inline body (MovementClass there).
// FUNCTION: 0x4402e0
MovementClass::MovementClass()
{
    field_0 = 0;
    footprintX = 0;
    footprintZ = 0;
    maxWaterDepth = 10000;
    minWaterDepth = -10000;
    maxSlope = 0xff;
    maxWaterSlope = 0xff;
    badSlope = 0xff;
    badWaterSlope = 0xff;
    width = 0;
    height = 0;
    cells = 0;
    lastTick = 0;
}

// Destructor of the 32-byte entry whose out-of-line constructor is 0x4402e0
// (0x440290.cpp inlines both for its array of entries, as MovementClass).
// Its one caller (0x42bf40) constructs a local entry with 0x4402e0 and calls
// this on it (ecx) when the local goes out of scope.
// FUNCTION: 0x440320
MovementClass::~MovementClass()
{
    FUN_004d85a0(field_0);
    operator delete(cells);
}

// Reads the movement fields from a TDF section (the object at src+4) into the
// 32-byte entry whose layout 0x440290.cpp declares as MovementClass. The four
// slope fields are clamped so that each is never larger than its limit.
// FUNCTION: 0x440340
void MovementClass::ReadMoveInfo(Source_00440340* src)
{
    footprintX = src->tdf->GetFieldInt("FootPrintX", 0);
    footprintZ = src->tdf->GetFieldInt("FootPrintZ", 0);
    maxWaterDepth = src->tdf->GetFieldInt("maxwaterdepth", maxWaterDepth);
    minWaterDepth = src->tdf->GetFieldInt("minwaterdepth", minWaterDepth);
    maxSlope = src->tdf->GetFieldInt("maxslope", maxSlope);
    badSlope = src->tdf->GetFieldInt("badslope", maxSlope / 2);
    maxWaterSlope = src->tdf->GetFieldInt("maxwaterslope", maxWaterSlope);
    badWaterSlope = src->tdf->GetFieldInt("badwaterslope", maxWaterSlope / 2);
    if (maxSlope > maxWaterSlope)
        maxSlope = maxWaterSlope;
    if (badSlope > maxSlope)
        badSlope = maxSlope;
    if (badWaterSlope > maxWaterSlope)
        badWaterSlope = maxWaterSlope;
}

// FUNCTION: 0x440470
void MovementClass::ResizePassMap(unsigned int w, unsigned int h)
{
    width = w;
    height = h;
    unsigned int n = ((h + 15) >> 4) * w;
    delete cells;
    if (n != 0) {
        cells = new unsigned int[n];
    } else {
        cells = 0;
    }
}

// FUNCTION: 0x4404c0
void MovementClass::SetPassMapCell(int param_1, int param_2, int param_3)
{
    int shift = (param_2 & 0xf) << 1;
    int row = (param_2 >> 4) * width + param_1;
    unsigned int* p = cells + row;
    *p = (param_3 << shift) | (~(3 << shift) & *p);
}

// Writes a rectangle of 2-bit cells into the transposed bitmap whose dword at
// column x of row band (y>>4) holds 16 cells stacked down the column.
//
// The key to the original's 5 stack homes and its 0x14 frame is that the
// two-bit cell mask is written as `~(3 << shift)` in the source, with only
// the `3 << shift` part in a local (`m`) and the `~` written where the mask
// is used. That leaves an extra NOT node in the expression tree, and MSVC 5
// then keeps `~m` in ebp with a stack home: it stores it in the outer loop
// body and reloads it at the top of the inner loop, exactly like the original
//   mov [esp+0x18], ebp / jmp body / mov ebp, [esp+0x18]
// With the whole mask as one local (`mask = ~(3 << shift)`) the NOT is folded
// away, the mask is promoted straight into ebp with no home, and the frame
// drops back to 0xc with the `v << shift` scheduled first, so the masked old
// value is never live at the same time and never needs a home either. That
// version is 71.2%.
// FUNCTION: 0x440830
void MovementClass::RefreshPassMap(Point_00440af0 a, Point_00440af0 b)
{
    int left = a.x - footprintX;
    int top = a.y - footprintZ;
    int right = a.x + b.x + 1;
    int bottom = a.y + b.y + 1;
    if (left < 0) {
        left = 0;
    }
    if (top < 0) {
        top = 0;
    }
    if (right > width) {
        right = width;
    }
    if (bottom > height) {
        bottom = height;
    }
    if (left < right && top < bottom) {
        for (int y = top; y < bottom; y++) {
            for (int x = left; x < right; x++) {
                unsigned int v = FUN_0047e1f0(this, x, y);
                unsigned int m = 3 << ((y & 0xf) * 2);
                unsigned int* p = &cells[(y >> 4) * width + x];
                *p = (*p & ~m) | (v << ((y & 0xf) * 2));
            }
        }
    }
}

// Refreshes one entry of a pathfinder's dirty list for the object at p
// (see 0x440a70 / 0x440be0): every unit in the unit table that moved in the
// window since the last refresh gets the new dirty rectangle drawn.
// FUNCTION: 0x440af0
void MovementClass::RefreshMovedUnits(Object_00440af0* p)
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
        ((MovementClass*)this)->RefreshPassMap(p->a, p->b);
    }
    if (start != old) {
        for (Record_00440af0* r = &g_game->units[1]; r <= g_game->units_end; r++) {
            if ((r->flags & 0x10000000) != 0 && r->unit != 0) {
                unsigned int t = r->unit->lastTick;
                if (t >= old && t < start) {
                    ((MovementClass*)this)->RefreshPassMap(r->a, r->b);
                }
            }
        }
    }
    p->unit->lastTick = prev;
}

// One entry of the table at 0x512358 (see 0x440a70): refreshes it for an
// object whose unit was last updated before the entry changed.
// FUNCTION: 0x440be0
void MovementClass::RefreshUnitIfStale(Object_00440af0* p)
{
    if (p->unit->lastTick < lastTick) {
        ((MovementClass*)this)->RefreshPassMap(p->a, p->b);
    }
}
