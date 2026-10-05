// Decompiled by Opus. Names are provisional.
// Snaps a 16.16 fixed-point world position to the centre of its grid cell
// (cells are 16 units, relative to the unit's origin in 8-unit steps), then
// sets the height from the cell. The two conversions are inlined helpers that
// take the origin and the position by value; the origin must be the last
// parameter of WorldToCell so that it is read before the position.

struct Point_0047ddc0 {
    short x;
    short y;
};

struct Vec3_0047ddc0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Unit_0047ddc0 {
    char unknown_0[0x14a];
    Point_0047ddc0 origin;             // +0x14a
    char unknown_14e[0x22f - 0x14e];
    char field_22f;                    // +0x22f
};
#pragma pack(pop)

int __stdcall GetFootprintHeight(Unit_0047ddc0* unit, Point_0047ddc0 cell);

static inline Point_0047ddc0 WorldToCell(Vec3_0047ddc0 v, Point_0047ddc0 origin)
{
    Point_0047ddc0 c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

static inline void CellToWorld(Point_0047ddc0 origin, Point_0047ddc0 c, Vec3_0047ddc0* v)
{
    v->x = (origin.x + c.x * 2) << 19;
    v->z = (origin.y + c.y * 2) << 19;
}

// FUNCTION: 0x47ddc0
void __stdcall FUN_0047ddc0(Unit_0047ddc0* unit, Vec3_0047ddc0* pos)
{
    if (unit->field_22f)
        return;
    Point_0047ddc0 cell = WorldToCell(*pos, unit->origin);
    CellToWorld(unit->origin, cell, pos);
    pos->y = GetFootprintHeight(unit, cell) << 16;
}
