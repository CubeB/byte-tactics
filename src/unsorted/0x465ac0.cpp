// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL: 95.8% (871 bytes against the original's 865). Everything from the
// prologue through the third visibility test is byte exact; all that is left is
// the last test, and it is three separate things (see below).
//
// What the function does: it is a visibility test for one unit. It returns 1 at
// once if the unit's map pointer (field_96) is the map asked about, or 0 if
// flag 2 of the unit's field_10e is set. Otherwise it builds a 16.16 position
// from the unit's position plus the def's offsets, bails out when the position
// is below the game-wide limit, and then asks "is this point visible" four
// times: at p, at p + (def+0x176,0,0), at p + (def+0x176,0,def+0x17e) shifted
// down by def+0x17a on y, and finally at p again. The first three go through
// FUN_00408090 unless the player's flags say to use the explored map instead;
// the fourth uses the shared visibility bit mask for the player.
//
// Two modelling points that the disassembly forces:
//  - The 12-byte local is three ints (16.16), but every test reads only the
//    HIGH word of each, through a cast to a struct of six shorts (the shape
//    already matched in src/unsorted/0x408090.cpp). So the int store and the
//    16-bit sign-extending load are both correct and the low halves are dead.
//  - The explored byte map is spelled THREE different ways in the original, one
//    per call site, and all three are needed for the bytes to line up:
//    site 1 materialises the data pointer (`mov eax,[esi+0x7c]; add ebp,ecx;
//    cmp byte [ebp+eax]`), sites 2 and 3 fold it into the index (`add ebp,
//    [esi+0x7c]; cmp byte [ebp+ecx]`), and site 4 builds a base pointer
//    first (`mov edi,[esi+0x7c]; add edx,edi; cmp byte [edx+ecx]`). Hence
//    IsExplored (Get method), IsExplored2 (plain index) and IsExplored3
//    (pointer local). Using one spelling for all four costs about 8 points.
//
// What still differs, all of it in the last test, and it is two things:
//  1. Both of the last test's exits go through the original's
//     `xor ecx,ecx / test eax,eax / setne cl / mov eax,ecx` (an int -> bool ->
//     int round trip on a value the disassembly shows is a literal 1 or 0), and
//     each of the four exits carries its own copy of it. MSVC 5 must simply fail
//     to fold it, because every other spelling folds. `short c = a ? 1 : 0;`
//     gives the exact four instructions but costs one `movsx eax,ax` per exit
//     (+3 bytes twice, hence 871 rather than 865), and that is the 6 bytes of
//     slack. The seen arm is already right: `if (b) b=1; else b=0;` survives
//     there because IsSeen hands it a value that MSVC cannot track as 0/1 (it
//     arrives through `neg eax / sbb eax,eax / xor ecx,ecx / neg eax`). The
//     explored arm's value arrives as `mov eax, 1` or `xor eax, eax`, which is
//     tracked, so every int spelling folds away (all four exit copies vanish,
//     844 to 850 bytes, 92 to 94 percent).
//  2. The original reloads g_game into ebx, the unit pointer's register, which
//     is dead after `mov eax,[ebx+0x92]`, and does it at the top of the last
//     test's fall-through block (original +0x25f). Ours puts the load one
//     instruction earlier, in the block that ends the call (ours +0x24a, i.e.
//     before `test eax,eax`), so it cannot take ebx, which is still live there,
//     and lands in ebp instead. That is one extra instruction here and the four
//     g_game field reads in the rest of the test name ebp instead of ebx
//     (+0x14281, +0x14273, +0x2a43).
//
// Shapes tried and rejected for 1, all of which fold: `int c = (a != 0);`,
// `int c = 0; if (a) c = 1;`, `int c; if (a) c=1; else c=0;` followed by
// `if (c) c=1; else c=0;`, `if (!a) a=0; else a=1;`, `c = 0; c = (a != 0);`,
// and the self-correction hidden behind an inlined one-argument helper
// (`Norm(1)` is still folded, so a parameter buys nothing). A `bool` local and
// a bool-returning helper spill the flag to a stack slot (`mov byte ptr
// [esp+0x20],cl`); a `char` local rotates the value into al/ecx the wrong way
// round; `unsigned short` gives `movzx` and a 875-byte function; an
// `unsigned value / unsigned b0 : 1` bitfield union collapses the function to
// 812 bytes. Moving the self-correction inside IsExplored3 and returning its
// result directly is the closest of the lot at 845 bytes, but it merges the two
// exits and drops the score to 93.6 percent.
//
// Shapes tried and rejected for 2, none of which changes anything at all (all
// stay at 871 bytes and 95.8 percent): a `Game_00465ac0*` local for the flag
// test, an `unsigned char` local for `g_game->flags`, a `UnitDef*` local for
// the last test only, and both at once. Moving the flag test above
// `p.x -= u->def->f176;` gives 891 bytes, and hoisting `u->def` to a
// function-wide local gives 882, so the order of those two statements is
// load-bearing. Putting the seen arm first (testing `!= 2`) gives 851 bytes and
// 86.9 percent. Note the ebp/ebx difference is NOT a consequence of the extra
// `short` local: the 844-byte folded variant picks ebp in exactly the same
// place.
//
// Other shapes that made things worse: one shared inline helper for all four
// visibility tests (the arms get merged and the function shrinks to about 780
// bytes), computing the cell coordinates once above the flag test for the last
// pair (that merges the two bounds checks), and putting both arms of the last
// test inside one inline function (same).
//
// Addendum by muse-spark-1.3-free (kept the short-ternary version: correct
// value, 871 bytes, 95.8%. About 20 scratch variants under
// build/scratch/0x465ac0/, all scored free via check.py --sym):
//  - `int c = (a == 0); return c;` (scratch u_eq0, u_0eqa) and
//    `int c = (0 < a);` (scratch w_0lta, u_gt0) both give EXACT size 865 at
//    97.6%. MSVC 5 folds `X != 0` to the identity `X` (round trip vanishes,
//    844 bytes) but does not fold `X == 0`, which survives as `test; sete`,
//    nor `X > 0`, which survives as `test; setg`. Neither is the original's
//    `test; setne`. Warning: the `== 0` form is semantically INVERTED
//    (returns 0 when explored); only the `>` forms have correct polarity.
//    A surviving correct-polarity `test; setne` on the branch-materialised
//    1/0 was not found. The seen arm keeps its round trip only because its
//    value arrives untracked via neg/sbb/neg.
//  - Scheduler tries for the ebx/ebp gap (assign-form subtract, swapped flag
//    compare, unsigned char flag local, Game* local, UnitDef* locals) all
//    keep `mov ebp,[g_game]` hoisted above the third-test branch, still
//    95.8%. Keeping UnitDef* live across the third test rotates u out of ebx
//    (scratch t_defacross, 860 bytes but 67.9%). headers.py: nothing helps.
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
    if ((g_game->flags & 2) == 2) {
        int a = IsExplored3(map, (Position_00465ac0*)&p);
        short c = a ? 1 : 0;             // the original's int -> bool -> int
        return c;
    }
    int b = IsSeen(map, (Position_00465ac0*)&p);
    if (b)
        b = 1;
    else
        b = 0;
    return b;
}
