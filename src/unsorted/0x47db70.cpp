// Decompiled by Space Bunny Free. Names are provisional.
//
// PARTIAL: 51.5%. The semantics are recovered and most of the body is right,
// but the prologue is wrong and it costs the whole first block. What differs:
//
//   - the original has a five-dword frame (`sub esp, 0x14`); ours has four
//     (`sub esp, 0x10`). The original spills the unit's origin dword to a real
//     local slot at [esp+0x10] between the two negative tests, and MSVC here
//     coalesces the origin copy into the unit parameter's slot instead, which
//     also shifts all four later local slots down by one dword;
//   - the original keeps `cell` in edx and `unit` in ebp; ours keeps `unit` in
//     edx, so every argument reference in the bounds checks is off by a
//     register as well as an offset.
//
// Ruled out for the frame (all leave `sub esp, 0x10`): the origin as two
// adjacent `short` locals, which is what should force a dword slot and a dword
// spill; a single `short` for the origin's y; a Point rebuilt from two short
// reads; the origin copy moved below the bounds tests and read from the unit
// directly; the origin copy taken from a `static inline Point OriginOf(unit)`
// helper, both as an initialiser and as a separate assignment; an extra unused
// int local; and a sweep of thirty header sets (windows.h, memory.h, string.h,
// stdio.h, stdlib.h, math.h, windowsx.h, objbase.h, ole2.h, d3d.h, ddraw.h,
// dsound.h, excpt.h, setjmp.h, time.h, wchar.h, limits.h and ten pairs), none
// of which changes the frame at all. So the missing dword is not reachable by
// a header here, unlike 0x4399f0 and 0x490080.
//
// Already correct and worth keeping: `Feature` is exactly 0x100 bytes with
// `unsigned char` bitfields (an `unsigned int` unit makes it 0x103 and shifts
// seaLevel), `Cell` needs its padding byte at +7 with feature at +8 for the
// 13-byte stride, `SteepCell` needs no `else` chain with the table lookup
// duplicated in both arms so the two `mov eax,1; jmp join` exits and the table
// falling into the join come out right, and the four height and stride locals
// must be declared first and assigned in the order tolerance, minHeight,
// maxHeight, stride.
//
// Suspected original bugs, with evidence:
//   - 0x47dbb6 and 0x47dbf7: the map index is `(cell.x + cell.y) * width +
//     cell.x` and the row stride is `width - cell.y`, so columns are counted
//     with `cell.y` and the height check uses `cell.y + fp.y`, while the first
//     row starts at `cell.x + cell.y`. That reads as `y * width` mistyped as
//     `(x + y) * width` with `fp.x` dropped, since the footprint then ends up
//     `cell.y` wide rather than `fp.x` wide.
//   - 0x47dd05: the owner test compares the cell's owner field at +0 against
//     the raw low 16 bits of the second argument rather than against
//     `other->field_0`, while 0x47dd48 does dereference that argument at
//     [eax+0x229]. Seven of the eight call sites pass 0 here, so the fallback
//     would dereference null; it is normally unreachable only because the
//     tolerance byte is large.
#pragma pack(push, 1)

struct Point_0047db70 {
    short x;
    short y;
};

struct Cell_0047db70 {
    unsigned short field_0;             // +0x0
    char unknown_2[3];
    unsigned char field_5;              // +0x5
    unsigned char field_6;              // +0x6
    char unknown_7;
    unsigned short feature;             // +0x8
    unsigned char offsetY;              // +0xa
    unsigned char offsetX;              // +0xb
    char unknown_c;
};

struct Feature_0047db70 {
    char name[0xfe];
    unsigned char unknown_6 : 6;
    unsigned char steep : 1;            // +0xfe, bit 6
    char unknown_ff[0x100 - 0xff];
};

struct Unit_0047db70 {
    char unknown_0[0x14a];
    Point_0047db70 origin;              // +0x14a, footprint in map cells
    char unknown_14e[0x1be - 0x14e];
    short field_1be;                    // +0x1be
    short field_1c0;                    // +0x1c0
    char unknown_1c2[0x228 - 0x1c2];
    unsigned char field_228;            // +0x228
    unsigned char field_229;            // +0x229
    char unknown_22a[0x22f - 0x22a];
    unsigned char field_22f;            // +0x22f
};

struct Game_0047db70 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14253 - 0x1423b];
    int nameCount;                      // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature_0047db70* names;            // +0x1426f
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell_0047db70* cells;               // +0x14287
};
#pragma pack(pop)

extern Game_0047db70* g_game;

int __stdcall FUN_0047d2e0(Unit_0047db70* unit, Point_0047db70 cell, int unused1, int unused2);

// Returns non-zero when the cell's ground does not take a unit: water, a cliff
// edge, or a feature type whose name entry has the "steep" bit set.
static inline int SteepCell(Cell_0047db70* c)
{
    unsigned short f = c->feature;
    if (f == 0xffff)
        return 0;
    if (f < 0xfffb) {
        if (f >= g_game->nameCount)
            return 1;
        return g_game->names[f].steep;
    }
    if (f != 0xfffe)
        return 1;
    c = c - (c->offsetY * g_game->width + c->offsetX);
    f = c->feature;
    if (f >= 0xfffb)
        return 0;
    return g_game->names[f].steep;
}

// FUNCTION: 0x47db70
int __stdcall FUN_0047db70(Unit_0047db70* unit, Unit_0047db70* other, Point_0047db70 cell, int flags)
{
    Point_0047db70 fp = unit->origin;
    if (cell.x < 0 || cell.y < 0)
        return flags == 2;
    int y = cell.y;
    if (y + cell.x >= g_game->width)
        return flags == 2;
    if (y + fp.y >= g_game->height)
        return flags == 2;
    if (!unit->field_22f)
        return FUN_0047d2e0(unit, cell, 0, 0);
    Cell_0047db70* c = &g_game->cells[(y + cell.x) * g_game->width + cell.x];
    int minHeight, maxHeight, stride;
    unsigned char tolerance = unit->field_228;
    minHeight = g_game->seaLevel - unit->field_1be;
    maxHeight = g_game->seaLevel - unit->field_1c0;
    stride = g_game->width - y;
    if (flags != 1)
        return 1;
    for (int row = 0; row < fp.y; row++) {
        for (int col = 0; col < y; col++, c++) {
            if (SteepCell(c))
                return 0;
            if (c->field_0 != 0 && c->field_0 != (unsigned short)other)
                return 0;
            if (c->field_6 < minHeight)
                return 0;
            if (c->field_5 > maxHeight)
                return 0;
            if (c->field_5 - c->field_6 > tolerance) {
                if (c->field_6 >= g_game->seaLevel)
                    return 0;
                if (c->field_5 - c->field_6 > other->field_229)
                    return 0;
            }
        }
        c += stride;
    }
    return 1;
}
