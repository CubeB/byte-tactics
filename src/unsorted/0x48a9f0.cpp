// Decompiled by space-bunny-free. Names are provisional.
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
// Still at 85.7% (tools/check.py), two differences left, both in the
// register/scheduler choices rather than in the statements:
//
// 1. First block. The original loads param_5 (mov edi, [esp+0x28]) before the
//    two movsx of the origin and puts the movsx of origin.y (read back from
//    the copied Point) above the movsx of origin.x; ours hoists the origin.x
//    movsx above the copy of the origin and loads param_5 after the cell.y
//    shift. Every spelling tried gives our order: the WorldToCell helper with
//    the origin first, a local Point origin, the position by value, by const
//    reference or via an out-parameter helper, a Point(int,int) constructor,
//    the cell.y expression first, two short locals, a SetTeam inline helper and
//    both compare operand orders. It looks like scheduler state left by
//    earlier functions in the original file.
//
// 2. `and al, 0xfc` (original, a byte) against `and eax, 0xfc` (ours). The
//    original's OR operand has to be a char-typed expression, which is what
//    lets MSVC narrow the clear to a byte: `(unsigned char)(param_5 & 3)`, a
//    two-bit bitfield in a union, or an `unsigned char` parameter of an inline
//    helper each reproduce `and al, 0xfc`, but every one of them also swaps the
//    register allocation (pos.x lands in edi and param_5 in ebx instead of the
//    other way round), which loses more than the narrowing gains. The `0xfc`
//    mask below is the spelling that gives the and/or form at all: with
//    `& ~3` MSVC picks the xor/and/xor lowering of the same statement. Note
//    that 0xfc also clears bits 2..7 of the flags, unlike `& ~3`; that is what
//    the original binary does (and, with a char operand, that is what the
//    narrowed `and al, 0xfc` would mean anyway).

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
        unit->flags = (unit->flags & 0xfc) | (param_5 & 3);
        FUN_0047cc30(unit);
        FUN_004827b0(unit);
    }
    unit->flags |= 0x10000;
    return unit->flags;
}
