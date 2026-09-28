// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Moves a unit to a new position. The cell (the two shorts at +0x76) and the
// two low bits of the flags at +0x110 only change when the new position falls
// in a different cell (the fixed-point WorldToCell of the unit type's origin
// at +0x7e) or the low flag bits differ. Then the unit is detached
// (FUN_0047d0e0), the cell and the flags are updated, and it is put back into
// the map (FUN_0047cc30) with its path redone (FUN_004827b0). Either way flag
// 0x10000 (the "position changed" bit FUN_0048a870 tests) is set, and the new
// flags value is returned. The callers (0x406aa0, 0x43d730) pass the position
// as a Vec3 by value and 1 as the last argument.
//
// Still partial (87.2%): the flag update now matches (`and edi, 3` masking
// param_5 in place and `and al, 0xfc` clearing the two low bits, thanks to the
// `(short)` cast on the OR operand, which stops MSVC from lowering the pair as
// the xor/and/xor bit insert and does not need a byte-addressable register the
// way an `unsigned char` cast does). The one difference left is the first
// block: the original reuses `eax` for the sign extend of origin.x
// (`movsx eax, ax; shl eax, 0x13; mov ecx, eax`), which forces the origin copy
// into its stack slot first, while MSVC here sign-extends straight into `ecx`
// (`movsx ecx, ax; ... shl ecx, 0x13`) and keeps the copy after it; param_5's
// load (`mov edi, [esp+0x28]`) consequently lands later than in the original.

#pragma pack(push, 1)
struct Point_0048a9f0 {
    short x;
    short y;
};

struct Pos_0048a9f0 {
    int x;
    int y;
    int z;
};

struct Unit_0048a9f0 {
    char unknown_0[0x6a];
    Pos_0048a9f0 pos;             // +0x6a
    Point_0048a9f0 cell;          // +0x76
    char unknown_7a[4];
    Point_0048a9f0 origin;        // +0x7e
    char unknown_82[0x110 - 0x82];
    unsigned int flags;           // +0x110
};
#pragma pack(pop)

void __stdcall FUN_0047d0e0(Unit_0048a9f0* unit);
void __stdcall FUN_0047cc30(Unit_0048a9f0* unit);
void __stdcall FUN_004827b0(Unit_0048a9f0* unit);

static inline Point_0048a9f0 WorldToCell(Pos_0048a9f0 v, Point_0048a9f0 origin)
{
    Point_0048a9f0 c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

// FUNCTION: 0x48a9f0
int __stdcall FUN_0048a9f0(Unit_0048a9f0* unit, Pos_0048a9f0 pos, int param_5)
{
    Point_0048a9f0 cell = WorldToCell(pos, unit->origin);
    if (cell.x == unit->cell.x && cell.y == unit->cell.y && param_5 == (unit->flags & 3)) {
        unit->pos = pos;
    } else {
        FUN_0047d0e0(unit);
        unit->pos = pos;
        unit->cell = cell;
        unit->flags = (unit->flags & 0xfffffffc) | (short)(param_5 & 3);
        FUN_0047cc30(unit);
        FUN_004827b0(unit);
    }
    unit->flags |= 0x10000;
    return unit->flags;
}
