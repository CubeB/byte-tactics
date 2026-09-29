// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, then by Claude Sonnet 5.5. Names are provisional.
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
// MATCH (206 of 206 bytes; was 87.2%, Claude Sonnet 5.5 #755). The flag update
// was already right (`and edi, 3` masking param_5 in place and `and al, 0xfc`
// clearing the two low bits, thanks to the `(short)` cast on the OR operand). The
// last difference was the first block: the original sign-extends origin.x into eax,
// shifts it there and copies it to ecx (`movsx eax, ax; shl eax, 0x13; mov ecx, eax`)
// after storing the origin copy, which is what writing the offset as a
// multiplication, `origin.x * 0x80000`, gives (the shift `origin.x << 19` sign-extends
// straight into ecx). The declaration-count sweep has only two states (84.2 and 87.2
// percent, neither a match), so compiler state was not involved, and named
// temporaries for the shifted offsets, a flags-carrying helper, a local copy of
// param_5 and reordering the condition did nothing.

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
    c.x = (v.x - origin.x * 0x80000 + 0x80000) >> 20;
    c.y = (v.z - origin.y * 0x80000 + 0x80000) >> 20;
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
