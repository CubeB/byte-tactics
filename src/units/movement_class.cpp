// Decompiled by Opus, space-bunny-free, Sonnet, Space Bunny Free, Haiku and deepseek-v4.1-flash. Names are provisional.
// The units/movement_class module: the 32-entry movement class table at
// 0x512358, the passability maps it owns and the helpers that keep them
// current. One Game, one MovementClass, one MovementClassTable and one
// Class_00440500 cover the module's views (Class_00440500 keeps its own
// 32-byte view of MovementClass).
//
// Nothing here uses <windows.h>: its symbols put RefreshPassMap and
// SetPassMapCell in the symbol-id windows they match in (docs/c2-regalloc.md).
#include <windows.h>
#include <string.h>
// Unused by the code, but it puts the pointer first in v[n] in BuildPassMap.
#include <stdio.h>

void __cdecl FUN_004d85a0(void* p);

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

struct MovementClass;
unsigned int __stdcall FUN_0047e1f0(MovementClass* obj, int x, int y);

class TdfRecord {
public:
    int GetFieldInt(const char* name, int def);
};

struct Source_00440340 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

struct Point {
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
    Point a;                  // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point b;                  // +0x7e
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Record_00440af0 {
    Unit_00440af0* unit;               // +0x0
    char unknown_4[0x76 - 0x4];
    Point a;                  // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point b;                  // +0x7e
    char unknown_82[0x110 - 0x82];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

// One terrain cell as 0x440500 reads it.
struct Cell_00440500 {
    unsigned short spot;               // +0x0
    char unknown_2[2];
    unsigned char field_4;
    unsigned char high;                // +0x5
    unsigned char low;                 // +0x6
    unsigned char unknown_7;
    unsigned short feature;            // +0x8
    unsigned char spotY;               // +0xa
    unsigned char spotX;               // +0xb
    unsigned char flags;               // +0xc
};
#pragma pack(pop)

// The 32-byte entry: a movement class with its own 2-bit-per-cell passability
// map. field_0 holds its name; the table below owns 32 of them.
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
    void RefreshPassMap(Point a, Point b);
    void RefreshMovedUnits(Object_00440af0* p);
    void RefreshUnitIfStale(Object_00440af0* p);
};

// The same 32 bytes as MovementClass under the view 0x440500 builds the map
// with (its own field names and BuildPassMap method).
class Class_00440500 {
public:
    int* field_0;                      // +0x0
    short field_4;                     // +0x4, footprint x
    short field_6;                     // +0x6, footprint y
    short field_8;
    short field_a;
    unsigned char field_c;
    unsigned char field_d;
    unsigned char field_e;
    unsigned char field_f;
    unsigned int field_10;             // +0x10, width
    unsigned int field_14;             // +0x14, height
    unsigned int* field_18;            // +0x18, the 2-bit map
    int field_1c;

    void BuildPassMap();
};

int __stdcall GetPassMapCellValue(Class_00440500* obj, Cell_00440500* cell);

// A static data member holding 32 entries. 0x440230 is its compiler-generated
// initialiser (the entry constructor inlined as a loop), and 0x440290 the
// destructor it registers with atexit. MSVC guards the destructor of a static
// data member with a "$S" flag, which is the byte right after the table.
// The destructor is implicit (there is no user-written body to write): its body
// is the compiler's own destruction of the 32 entries, each inlining
// ~MovementClass (FUN_004d85a0 on field_0, then operator delete on cells),
// which is the loop 0x440290 registers with atexit. A user-written loop would
// otherwise run on top of that destruction.
struct MovementClassTable {
    MovementClass entries[32];

    static MovementClassTable g_movementClasses;
};

// The pathfinder's dirty-list view of the unit object 0x440a70 refreshes from
// (0x440af0 / 0x440be0 declare the same bytes as Object_00440af0).
#pragma pack(push, 2)
struct Struct_00440a70 {
    Unit_00440af0* unit;               // +0x0
    char unknown_4[0x76 - 0x4];
    Point a;                  // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point b;                  // +0x7e
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PlayerInfo_00440c10 {
    char unknown_0[0x96];
    unsigned char slot;                // +0x96
};

struct Player_00440c10 {               // 0x14b bytes
    int active;                        // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00440c10* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char side;                // +0x146
    char unknown_147[0x14b - 0x147];
};

struct PlayerData_00440cd0 {
    char unknown_0[0xa7];
    unsigned char field_a7;            // +0xa7
    unsigned char field_a8;            // +0xa8
    unsigned int field_a9;             // +0xa9
};

struct Player_00440cd0 {
    char unknown_0[0x27];
    PlayerData_00440cd0* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};
#pragma pack(pop)

class Mission {
public:
    int FUN_004358f0();
    unsigned int ComputeMapChecksum();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1b63];
    union {
        Player_00440c10 players[10];   // +0x1b63
        Player_00440cd0 players2[10];  // +0x1b63
    };
    char unknown_2851[0x14233 - 0x2851];
    unsigned int width;                // +0x14233
    unsigned int height;               // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00440500* cells;              // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    Record_00440af0* units;            // +0x14357
    Record_00440af0* units_end;        // +0x1435b
    char unknown_1435f[0x38a47 - 0x1435f];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x38d73 - 0x38a4b];
    unsigned char progress;            // +0x38d73
    char unknown_38d74[0x391e9 - 0x38d74];
    Mission* field_391e9;              // +0x391e9
};
#pragma pack(pop)

struct Rect_00440ca0 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    int d;                             // +0xc
};

#pragma pack(push, 1)
struct Dst_00440ca0 {
    char unknown_0[0x99];
    Rect_00440ca0 rect;                // +0x99
};
#pragma pack(pop)

struct Src_00440ca0 {
    int unknown_0;
    Rect_00440ca0 rect;                // +0x4
};

extern Game* g_game;
extern char DAT_00512370[];
extern char DAT_00512770[];

// FUNCTION: 0x440230 _$E6
// FUNCTION: 0x440290 _$E3
MovementClassTable MovementClassTable::g_movementClasses;

// Out-of-line constructor of the 32-byte entry that the table's compiler-
// generated initialiser (0x440230) declares with the same inline body.
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
// (the table's initialiser inlines both for its array of entries). Its one
// caller (0x42bf40) constructs a local entry with 0x4402e0 and calls this on
// it (ecx) when the local goes out of scope.
// FUNCTION: 0x440320
MovementClass::~MovementClass()
{
    FUN_004d85a0(field_0);
    operator delete(cells);
}

// Reads the movement fields from a TDF section (the object at src+4) into the
// 32-byte entry. The four slope fields are clamped so that each is never larger
// than its limit.
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

// Looks an entry up by name in the 32-entry table that the table's definition
// owns (the class declarations are shared with it; field_0 holds the name).
// FUNCTION: 0x440420
MovementClass* __stdcall FindMovementClass(char* name)
{
    for (int i = 0; i < 32; i++) {
        if (MovementClassTable::g_movementClasses.entries[i].field_0 != 0
            && _strcmpi((char*)MovementClassTable::g_movementClasses.entries[i].field_0, name) == 0) {
            return &MovementClassTable::g_movementClasses.entries[i];
        }
    }
    return 0;
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

static inline void setcell(unsigned int* q, int sh, unsigned int val)
{
    *q = (*q & ~(3 << sh)) | (val << sh);
}

// Builds the 2-bit-per-cell passability map (field_18) for one footprint.
// Pass 1 walks the map row by row, filling one scratch row with the raw cost
// of every cell and marking 1 on the outer edge of the usable (3) area; pass 2
// reads the map back column by column and marks 1 on the inner edge. Each pass
// works on a scratch row whose two bytes in front and field_4/field_6 cells
// past the end are zeroed.
// FUNCTION: 0x440500
void Class_00440500::BuildPassMap()
{
    unsigned char b;
    unsigned char* p;
    unsigned char* buf;
    int n;
    unsigned char* v;
    unsigned int size;
    if (field_10 + field_4 > field_6 + field_14)
        size = field_10 + field_4;
    else
        size = field_6 + field_14;

    buf = (unsigned char*)operator new(size + 3);
    v = buf + 2;

    // Braces keep the two int j apart: MSVC 5 leaks a for-init variable into the enclosing scope.
    {
    for (int j = 0; j < field_14; j++) {
        Cell_00440500* c = &g_game->cells[j * field_10];
        for (n = 0; n < field_10; n++, c++)
            v[n] = (unsigned char)GetPassMapCellValue(this, c);
        v[-2] = 0;
        v[-1] = 0;
        for (n = 0; n <= field_4; n++)
            (v + n)[field_10] = 0;
        b = 0;
        p = v;
        n = 0;
        for (; n < field_10; n++, p++) {
            int e = n + field_4 - 1;
            if (v[e] <= b) {
                b = v[e];
            } else if (p[-1] <= b) {
                b = p[0];
                int k;
                for (k = n + 1; k <= e; k++)
                    if (b >= v[k]) b = v[k];
            }
            if (b == 3 && (p[-1] < 3 || v[e + 1] < 3))
                setcell(&field_18[(j >> 4) * field_10 + n], (j & 0xf) * 2, 1);
            else
                setcell(&field_18[(j >> 4) * field_10 + n], (j & 0xf) * 2, b);
        }
    }
    }

    {
    for (int j = 0; j < field_10; j++) {
        for (n = 0; n < field_14; n++)
            v[n] = (unsigned char)((field_18[(n >> 4) * field_10 + j]
                                    >> ((n & 0xf) * 2)) & 3);
        v[-2] = 0;
        v[-1] = 0;
        for (n = 0; n <= field_6; n++)
            (v + n)[field_14] = 0;
        b = 0;
        n = 0;
        for (; n < field_14; n++) {
            int e = n + field_6 - 1;
            if (v[e] <= b) {
                b = v[e];
            } else if (v[n - 1] <= b) {
                b = v[n];
                int k;
                for (k = n + 1; k <= e; k++)
                    if (b >= v[k]) b = v[k];
            }
            if (b == 3 && (v[n - 1] < 3 || v[e + 1] < 3))
                setcell(&field_18[(n >> 4) * field_10 + j], (n & 0xf) * 2, 1);
            else
                setcell(&field_18[(n >> 4) * field_10 + j], (n & 0xf) * 2, b);
        }
    }
    }

    operator delete(buf);
}

// Writes a rectangle of 2-bit cells into the transposed bitmap whose dword at
// column x of row band (y>>4) holds 16 cells stacked down the column.
// FUNCTION: 0x440830
void MovementClass::RefreshPassMap(Point a, Point b)
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
                // m holds only 3 << shift; the ~ stays at the use (a whole-mask local changes the frame).
                unsigned int m = 3 << ((y & 0xf) * 2);
                unsigned int* p = &cells[(y >> 4) * width + x];
                *p = (*p & ~m) | (v << ((y & 0xf) * 2));
            }
        }
    }
}

// FUNCTION: 0x440930
void NopAfterFeatureEnum(void)
{
}

// Rebuilds every in-use entry of the 32-entry table at 0x512358 for the
// current map size and tracks the progress percentage in g_game+0x38d73.
// FUNCTION: 0x440940
void BuildAllPassMaps(void)
{
    int count = 0;
    unsigned int n;
    int p = (int)&MovementClassTable::g_movementClasses;
    do {
        if (*(int*)p != 0) {
            count++;
        }
        p += 0x20;
    } while (p < (int)&MovementClassTable::g_movementClasses + 0x400);

    int progress = 100;
    unsigned int* q = (unsigned int*)DAT_00512370;
    do {
        if (q[-6] != 0) {
            unsigned int h = g_game->height;
            unsigned int w = g_game->width;
            q[-2] = w;
            q[-1] = h;
            // Size from the stored entry fields q[-1], q[-2], not the w/h locals.
            n = ((unsigned)(q[-1] + 0xf) >> 4) * q[-2];
            operator delete((void*)q[0]);
            if (n != 0) {
                q[0] = (unsigned int)operator new(n * 4);
            } else {
                q[0] = 0;
            }
            ((Class_00440500*)(q - 6))->BuildPassMap();
            progress += 100;
            g_game->progress = progress / count;
        }
        q += 8;
    } while ((int)q < (int)DAT_00512770);

    g_game->progress = 100;
}

static inline void FreeA(int p)
{
    FUN_004d85a0(*(void**)(p - 0x18));
    *(void**)(p - 0x18) = 0;
}

static inline void FreeC(int p)
{
    void* c = *(void**)p;
    *(void**)(p - 8) = 0;
    *(void**)(p - 4) = 0;
    operator delete(c);
    *(void**)p = 0;
}

// FUNCTION: 0x440a00
void FreeMovementClasses(void)
{
    int p = (int)DAT_00512370;
    do {
        FreeA(p);
        FreeC(p);
        p += 0x20;
    } while (p < (int)DAT_00512770);
}

// Refreshes every entry in use of the table at 0x512358 (the loop that 0x440a70
// inlines as its UpdateAll helper).
// FUNCTION: 0x440a40
void __stdcall RefreshAllPassMaps(Point a, Point b)
{
    for (int i = 0; i < 32; i++) {
        if (MovementClassTable::g_movementClasses.entries[i].field_0 != 0) {
            ((MovementClass*)&MovementClassTable::g_movementClasses.entries[i])->RefreshPassMap(a, b);
        }
    }
}

static inline void UpdateAll(Point a, Point b)
{
    for (int i = 0; i < 32; i++) {
        if (MovementClassTable::g_movementClasses.entries[i].field_0 != 0) {
            ((MovementClass*)&MovementClassTable::g_movementClasses.entries[i])->RefreshPassMap(a, b);
        }
    }
}

// Refreshes the entries of the table at 0x512358 for an object: entries newer
// than the object's last update if it has a unit, otherwise every entry in use
// (an inlined helper taking the two points by value, so they stay in registers).
// FUNCTION: 0x440a70
void __stdcall RefreshPassMapsForUnit(Struct_00440a70* p)
{
    if (p->unit != 0) {
        unsigned int last = p->unit->lastTick;
        p->unit->lastTick = g_game->ticks;
        for (int i = 0; i < 32; i++) {
            if (last < MovementClassTable::g_movementClasses.entries[i].lastTick) {
                ((MovementClass*)&MovementClassTable::g_movementClasses.entries[i])->RefreshPassMap(p->a, p->b);
            }
        }
        return;
    }
    UpdateAll(p->a, p->b);
}

// Refreshes one entry of a pathfinder's dirty list for the object at p (see
// 0x440a70 / 0x440be0): every unit in the unit table that moved in the window
// since the last refresh gets the new dirty rectangle drawn.
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
        this->RefreshPassMap(p->a, p->b);
    }
    if (start != old) {
        for (Record_00440af0* r = &g_game->units[1]; r <= g_game->units_end; r++) {
            if ((r->flags & 0x10000000) != 0 && r->unit != 0) {
                unsigned int t = r->unit->lastTick;
                if (t >= old && t < start) {
                    this->RefreshPassMap(r->a, r->b);
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
        this->RefreshPassMap(p->a, p->b);
    }
}

// Returns the lowest of ten slot numbers (0..9) not taken by any active player
// of type 1-3 (side 10 excluded), or 0 when all are taken.
// FUNCTION: 0x440c10
int FindUnusedLogo()
{
    int used[10];
    memset(used, 0, sizeof(used));
    for (int i = 0; i < 10; i++) {
        Player_00440c10* p = &g_game->players[i];
        if (p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->side != 10)
            used[p->info->slot < 9 ? p->info->slot : 9] = 1;
    }
    int result = 0;
    for (int j = 0; j < 10; j++) {
        if (!used[j]) {
            result = j;
            break;
        }
    }
    return result;
}

// FUNCTION: 0x440ca0
void __stdcall CopyPathLockFields(Dst_00440ca0* dst, Src_00440ca0* src)
{
    dst->rect = src->rect;
}

unsigned char FindHostSlot();

// FUNCTION: 0x440cd0
int CheckMapCrc()
{
    if (!g_game->field_391e9->FUN_004358f0()) {
        return 0;
    }
    unsigned char me = FindHostSlot();
    PlayerData_00440cd0* data = 0;
    int check = 0;
    if (me != 10) {
        data = g_game->players2[me].data;
        if (data->field_a7 >= 2 || (data->field_a7 == 1 && data->field_a8 >= 2)) {
            check = 1;
        }
    }
    if (!check) {
        return 1;
    }
    return ((Mission*)g_game->field_391e9)->ComputeMapChecksum() == data->field_a9;
}
