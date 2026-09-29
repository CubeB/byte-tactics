// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 98.3 percent, size exact (865 bytes against the original's 865).
// Everything from the prologue through the third visibility test is byte exact,
// and so is the whole last test's block shape, including the two long
// normalisations `xor ecx,ecx / test eax,eax / setne cl / mov eax,ecx` on both
// exits of the inner explored diamond. The ONLY remaining diff is ONE 6-byte
// instruction, the g_game reload that starts the last region: the original
// keeps it there, in EBX at +0x25f, right after `mov eax,[ebx+0x92]` has killed
// the unit pointer (so EBX is free and preferred) and uses EBX for all three
// g_game field reads of the last test; we preload it one block earlier, into the
// join block after the third test's call, where EBX still holds the unit and the
// last free callee-saved register is EBP. Position and register are one cause,
// not two; see the note at the end of this header.
// (check.py aligns with difflib, so the percentage is far below the 6 bytes
// that actually differ: brief item 17.)
//
// What the function does: it is a visibility test for one unit. It returns 1 at
// once if the unit's map pointer (f96) is the map asked about, or 0 if flag 2 of
// the unit's f10e is set. Otherwise it builds a 16.16 position from the unit's
// position plus the def's offsets, bails out when the position is below the
// game-wide limit, and then asks "is this point visible" four times: at p, at
// p + (def->f176,0,0), at p + (f176,-f17a,f17e), and finally at p again. The
// first three go through FUN_00408090 unless the player's flags say to use the
// explored byte map instead; the fourth uses the shared visibility bit mask.
//
// Two modelling points the disassembly forces:
//  - The 12-byte local is three ints (16.16), but every test reads only the
//    HIGH word of each, through a cast to a struct of six shorts (the shape
//    already matched in src/unsorted/0x408090.cpp).
//  - The explored byte map is spelled FOUR different ways in the original, one
//    per call site, and all four are needed for the bytes to line up: site 1
//    materialises the data pointer (`mov eax,[esi+0x7c]; add ebp,ecx; cmp byte
//    [ebp+eax]`), sites 2 and 3 fold it into the index (`add ebp,[esi+0x7c];
//    cmp byte [ebp+ecx]`), and site 4 builds a base pointer first
//    (`mov edi,[esi+0x7c]; add edx,edi; cmp byte [edx+ecx]`). Hence IsExplored
//    (Get method), IsExplored2 (plain index) and IsExplored3 (pointer local).
//
// What finally fixed the 4-byte normalisation (it was 1-byte, `test al,al`,
// for many attempts): the last test is NOT `if (flags) { a = IsExplored3(...);
// return a ? 1 : 0; } ...` in any spelling. The four exit blocks of the last
// test all end with the SAME long normalisation, including the two exits of
// the IsSeen path, so the normalisation is a single statement AFTER the
// flags test, not part of either arm. Spelling it as one helper,
// IsVisible3, whose two arms return the two inlined tests, and calling it as
// `if (IsVisible3(map, &p)) return 1; return 0;`, gives all four blocks the
// original's form. The width then follows the type of the value being
// normalised, so the helper's arms must return `int` (a `unsigned char` or
// `short` arm gives `test al,al` / `test ax,ax`), and MSVC 5 will only keep
// the normalisation at all if it cannot range-track the value, which is why
// IsExplored3 must keep its branchy `if (contains) { if (d[tx] != 0) return 1; }
// return 0;` shape rather than a branchless `return d[tx] != 0;`.
//
// The g_game register, and everything tried for it. Every one of these still
// compiles to 865 bytes and still puts the load in exactly the same place, one
// block too high, in EBP:
//   - `Game* g = g_game;` inside the helper, or g_game passed to the helper as
//     a parameter; a `GameFlags()` accessor; `unsigned char fl = g_game->flags;`
//     inside the helper (before the test or after the p.x update); `2 == (fl&2)`,
//     `fl & 2`, `!(fl & 1)`; reading the flags through a `Game` member function.
//   - The last test's spelling: `if (...) return 1; return 0;`, the same with
//     `else return 0;`, `return cond ? 1 : 0;`, `return cond & 1;`,
//     `if (!cond) return 0; return 1;`, the condition through a local `int r`
//     (with and without braces), a second pointer local for the position, the
//     whole test (p.x update included) in a helper taking map/pos/u, in a
//     helper taking map/pos/def, the p.x update folded into the call's argument
//     with the comma operator, the p.x update split into `int t = ...; p.x = t;`,
//     `p.x = p.x - u->def->f176;`, `int f176 = u->def->f176;` first,
//     `UnitDef* d = u->def;` first (a `def` local at the top of the function,
//     used by all three updates, costs 21 points), a `def` local for the last
//     update only, and `IsVisible3` with its two arms written as an if/else on
//     the result local (which loses 4 points) or as two `if`s (which collapses
//     the whole function to 756 bytes).
//   - The third test's condition written `!= 0` or through a local `int r3`; the
//     third and fourth tests nested inside `if (!third) { ... } return 1;` (79.6
//     percent, it changes the block layout); braces, `if (1) { }`,
//     `switch (0) { case 0: }`, a `goto`; renaming the helper; a union for `p`.
//   - headers: headers.py tried all 128 sets, <memory.h> ties with no header,
//     and <stdio.h>, <stdlib.h> and <string.h> all cost 13 points.
//   - The last region folded into a helper of its own: tests 2+3+4 in one
//     helper (`return VisibleRest(map, u, &p);`), tests 3+4 in one, and only
//     test 4 in one (which also owns the `p.x -=` update). All three stay at
//     98.3, so the single `return <helper>` at the end is not the lever.
//   - `Game* g = g_game;` (and `g_game->flags` and the mask and the player
//     index) passed in as arguments at the call site, `unsigned short* m` and
//     `int bit` locals inside IsSeen, `int i` for the mask index, the mask
//     index and the player bit split into two arguments, `2 == (flags & 2)`,
//     a `?: ? :` for the two arms, an `else` after the explored arm, braces
//     around the explored arm, the arms written as `if (...) return 1; return
//     0;` (76.0) or as `return X ? 1 : 0;` (84.5 to 94.1), `p.x -= d->f176`
//     with a `UnitDef* d` local, `pp->x -= ...` through a `Pos*` local, a
//     `Unit* uu = u` local, `else` around the whole last region, and the third
//     test plus the last region nested in one `if (!IsVisible2(...)) { ... }`.
//     Every one of them is 98.3 with exactly the same 6-byte diff, except the
//     ones that change the last test's block shape, which all score lower.
//
// WHAT TRIGGERS THE HOIST (the one diagnostic result worth having). Delete the
// IsSeen arm from IsVisible3, so the flags load has a single use and the false
// arm is a bare `return 0`, and the load STAYS at the top of the fourth test's
// block, in exactly the original's position and after exactly the original's
// `mov eax,[ebx+0x92]` (measured in build/scratch/0x465ac0/v2/e_onlyexpl.cpp,
// 71.2 percent overall but the load is where the original has it). Put the
// IsSeen arm back, so the false arm is several basic blocks, and the load
// hoists to the join block again, for every spelling of IsSeen, including one
// whose mask and player index are passed in as arguments so the flags load is
// unambiguously a single-use expression (f_maskarg). So the hoist is not about
// the size of the CSE group, it is about the FALSE ARM BEING MORE THAN ONE
// BLOCK, and the original has a false arm of several blocks too. That is the
// unexplained part, and it is where the next attempt should start: some source
// shape for the last test's second arm has to give the same block count with a
// different internal block order, since the emitted CFG is already identical.
//
// The load is hoisted one block too high, above `test eax,eax / je`, whatever
// the source order of the two statements is, so the fix has to change the region
// itself or the register pressure in it, not the spelling.
//
// SOURCE ORDER IS NOT THE LEVER (measured this session, build/scratch/0x465ac0/
// B_main.cpp and A_ug.cpp, both free scratch scores). Writing the last region's
// opening as `UnitDef* d4 = u->def; Game* g4 = g_game; p.x -= d4->f176;` with
// the flags test spelled `if ((g4->flags & 2) == 2)`, in exactly the order the
// original emits, still hoists the g_game load into the join block, still in
// EBP, and still reads 93.2 (the two extra locals cost the normalisation shape
// too). The same as a helper taking (map, pos, def, g) with the arguments
// evaluated left to right in that order, and the same as a helper taking u and
// making both loads itself, 58.1 each. So the load is NOT placed at its source
// position: LCL extends its live range up to the enclosing statement level (the
// join block) whatever the spelling, and the block it lands in is fixed. The
// only remaining lever is therefore to give the last test's flags test a
// DEEPER statement level of its own, so that the level LCL extends to is the
// fourth test's block and not the join: something that makes `if (flags&2)` a
// statement nested one level below the `if (IsVisible3(...)) return 1;` that
// contains it, rather than its sibling. A nested level with no intervening
// branch is what a `? :` with a side effect in one arm would give, and that is
// the next thing to try. Also tried and still 98.3 with the identical diff, so
// none of these is the lever either: a `? :` for the two arms (94.1, it costs
// the 4-byte normalisation), a `switch (flags & 2)` (86.7), a nested
// `if (IsExplored3(...)) return 1; return 0;` inside the explored arm (88.6),
// `do { ... } while (0)` around the helper's body, `Game* g4 = g_game;` taken
// before the `p.x -=` and passed to the helper as a third argument with IsSeen
// taking it as a fourth, and the flags test itself passed into the helper as
// `(g_game->flags & 2) == 2` so it is evaluated at the call site. The last of
// those is the strongest structural change tried and it produces the same
// hoisted EBP load, which is the clearest evidence yet that the live range is
// extended to the join level before any source-level placement is considered.
//
// WHAT THE REMAINING DIFF ACTUALLY IS (one cause, not two). Only one 6-byte
// instruction is in the wrong place; everything else in the function is byte
// exact, and the size is exact because the load moved rather than changed.
// MSVC 5 preloads the last test's g_game load into the S-block entry, which is
// the join block after the third test's call (the block holding
// `mov di,[esp+0x1a] / mov dx,[esp+0x16] / test eax,eax / je`). The original
// keeps it in the fourth test's own block, in the gap between the `u->def` load
// and the `def->f176` load. The register follows from the block and is not a
// second problem: at the join, ESI is map, EDI is DI and EBX is u, so the only
// callee-saved register left is EBP, and at the start of the fourth test's
// block the def load has just killed u, so EBX is free and wins the ESI, EDI,
// EBX, EBP preference order. Both the register and the position must come from
// one change. Blocking the preload by occupying EBP in the join is not possible
// without emitting an instruction the original does not have: the only values
// the original keeps across the call are p.y and p.z, and it re-loads exactly
// their low words into EDX and EDI.
//
// More attempts by deepseek-v4.1-flash, all 98.3 with the identical 6-byte diff
// (load still at the join, in EBP), so none of these is the lever either:
// inline last test with nested ifs (76.0, block shape changes); inline with an
// `int r` result local (94.1); ternary helper `return (cond ? A : B) != 0;`
// (94.1, collapses the 4-byte normalisation); ternary helper returning raw
// (94.1); nested-if helper normalised (94.1); helper with `int r = 0` and two
// ifs (63.6); `Game* g = g_game;` local inside the helper; `unsigned char fl =
// g_game->flags;` local; flags test passed in as a third argument; g_game
// pointer passed in as a third argument; `while(1){...break;}`; `for(;;)`;
// `do{}while(0)`; comma-operator condition `(p.x -= f176, IsVisible3(...))`;
// comma inside the call's argument; a differently named identical helper;
// renaming IsVisible3 to IsVisible5; `int v = IsVisible3(...)` then `if (v)`;
// `IsVisible3(...) == 1`; the two arms with braces/`if (1)`; a `goto explored`
// layout (98.3); splitting the fourth test as an explored-if followed by a
// seen-if (94.7); `(g_game->flags & 2) != 0` (94.6, the cmp/and shape changes);
// `<stdio.h>`, `<stdlib.h>`, `<string.h>` (all cost points, headers.py already
// covers this); and unused-declaration compiler state: 1 to 2048 unused
// prototypes, 64/256 unused extern ints and 64 unused structs. The compiler
// state arm is active (256 prototypes drops the function to 863 bytes, 85.5),
// but no count moves the load. The only thing not tried is a genuinely
// different block ORDER for the second arm, which is what the header above
// argues for.
//
// Strongest new diagnostic: passing g_game itself as a THIRD ARGUMENT to the
// helper, `if (IsVisible3(map, &p, g_game))`, so the load is a call-site
// argument evaluated in $L302 after the je, STILL compiles to the hoisted
// `mov ebp, [g_game]` at the join (98.3, build/scratch/0x465ac0/s1_gparam.cpp).
// A local pointer for the position (`Position_00465ac0* pp = &p;`) and putting
// the p.x update plus the whole test in one helper taking u both score 58.1.
// So no source position for the load survives; the block it lands in is chosen
// before source placement is considered, as this header already concluded.
// One last family tried: the test as an inline method of a member sub-object,
// the 0x463610 trick: `map->IsVisible3(&p)`, `u->IsVisible4(map, &p)`,
// `map->IsVisible5(u, &p)` and the flags read through a `g_game->ExploredMode()`
// method. The first and last are still 98.3 with the identical hoist; the other
// two change the frame (844 and 870 bytes). So the method boundary does not
// block this hoist either (build/scratch/0x465ac0/gen_batch6.py).
//
// Confirmed again by space-bunny-free, free scratch scores on top of the list
// above. Every one of these is the same 98.3 with the same 6-byte diff, so none
// of them is the lever either: the last region in an `else` block; `do{}while(0)`
// around it; an `int r` result local; a `Position*` local for `&p`;
// `if (!IsVisible3(...)) return 0; return 1;`; `return IsVisible3(...) ? 1 : 0;`;
// a brace-delimited block around the last two statements; `Game* g` as the first
// statement of IsVisible3; g_game as a parameter of IsSeen; a `unsigned short*`
// local for the visibility mask; and `1 << (g_game->playerIndex & 0x1f)`, which
// is 868 bytes (the mask is NOT folded away for a field read, three bytes over).
// Swapping the source order of the two updates before the third test (`p.y -=`
// before `p.z +=`) is 96.5 with the same diff plus two more bytes, so the
// emitted order there is not a lever either.
#pragma pack(push, 1)
struct MapSize_00465ac0 {
    unsigned int width;
    unsigned int height;
    int Contains(unsigned int tx, unsigned int ty) { return tx < width && ty < height; }
};
struct ByteMap_00465ac0 {
    unsigned char* data;
    MapSize_00465ac0 size;
    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};
struct Map_00465ac0 {
    char unknown_0[0x7c];
    ByteMap_00465ac0 explored;           // +0x7c
};
struct Game_00465ac0 {
    char unknown_0[0x2a43];
    unsigned char playerIndex;           // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;      // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char limitY;                // +0x1427f
    char unknown_14280;
    unsigned char flags;                 // +0x14281
};
struct UnitDef_00465ac0 {
    char unknown_0[0x15e];
    int f15e;
    char unknown_162[0x166 - 0x162];
    int f166;
    char unknown_16a[0x16e - 0x16a];
    int f16e;
    char unknown_172[0x176 - 0x172];
    int f176;
    int f17a;
    int f17e;
};
struct Vec3_00465ac0 {
    int x, y, z;
};
struct Unit_00465ac0 {
    char unknown_0[0x6a];
    Vec3_00465ac0 pos;                   // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_00465ac0* def;               // +0x92
    Map_00465ac0* f96;                   // +0x96
    char unknown_9a[0x10e - 0x9a];
    unsigned char f10e;                  // +0x10e
    char unknown_10f[0x110 - 0x10f];
    unsigned int f110;                   // +0x110
};
struct Position_00465ac0 {              // 16.16 fixed point; only high words read
    short xFrac;
    short x;                            // +0x2
    short yFrac;
    short y;                            // +0x6
    short zFrac;
    short z;                            // +0xa
};
struct Pos_00465ac0 {                   // 16.16 fixed point
    int x, y, z;
};
#pragma pack(pop)

extern Game_00465ac0* g_game;

int __stdcall FUN_00408090(Map_00465ac0* map, Position_00465ac0* pos);

// the player's explored byte map, inlined where the game flags ask for it
static inline int IsExplored(Map_00465ac0* map, Position_00465ac0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty)) {
        if (map->explored.Get(tx, ty) != 0)
            return 1;
    }
    return 0;
}
static inline int IsSeen(Map_00465ac0* map, Position_00465ac0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (!map->explored.size.Contains(tx, ty))
        return 0;
    return (g_game->visibilityMask[map->explored.size.width * ty + tx] &
            (1 << g_game->playerIndex)) != 0;
}

static inline int IsExplored2(Map_00465ac0* map, Position_00465ac0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty)) {
        if (map->explored.data[map->explored.size.width * ty + tx] != 0)
            return 1;
    }
    return 0;
}
static inline int IsVisible(Map_00465ac0* map, Position_00465ac0* pos)
{
    if ((g_game->flags & 2) == 2)
        return IsExplored(map, pos);
    return FUN_00408090(map, pos);
}
static inline int IsVisible2(Map_00465ac0* map, Position_00465ac0* pos)
{
    if ((g_game->flags & 2) == 2)
        return IsExplored2(map, pos);
    return FUN_00408090(map, pos);
}
static inline int IsExplored3(Map_00465ac0* map, Position_00465ac0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty)) {
        unsigned char* d = map->explored.data + map->explored.size.width * ty;
        if (d[tx] != 0)
            return 1;
    }
    return 0;
}


static inline int IsVisible3(Map_00465ac0* map, Position_00465ac0* pos)
{
    if ((g_game->flags & 2) == 2)
        return IsExplored3(map, pos);
    return IsSeen(map, pos);
}
// FUNCTION: 0x465ac0
int __stdcall FUN_00465ac0(Map_00465ac0* map, Unit_00465ac0* u)
{
    if (u->f96 == map)
        return 1;
    if (u->f10e & 4)
        return 0;
    Pos_00465ac0 p;
    p.x = u->def->f15e + u->pos.x;
    p.y = u->def->f16e + u->pos.y;
    p.z = u->def->f166 + u->pos.z;
    if (!(u->f110 & 0x200)) {
        if (p.y < (g_game->limitY << 16))
            return 0;
    }
    if (IsVisible(map, (Position_00465ac0*)&p))
        return 1;
    p.x += u->def->f176;
    if (IsVisible2(map, (Position_00465ac0*)&p))
        return 1;
    p.z += u->def->f17e;
    p.y -= u->def->f17a;
    if (IsVisible2(map, (Position_00465ac0*)&p))
        return 1;
    p.x -= u->def->f176;
    if (IsVisible3(map, (Position_00465ac0*)&p))
        return 1;
    return 0;
}
