// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash (issue 3314 retry, 10 min): body reconfirmed at 82.3% / 847 bytes against 844;
// no new shape tried beat it. Left as is, with the diff exactly as described below.
// deepseek-v4.1-flash (issue 2893 retry, 10 min): body left at 82.3% / 847 bytes against 844, still the
// best. This pass tried ~35 more source shapes, all CSE'd or scored lower: feature-block ternaries
// (v_tern1/v_tern2, equal), branch reorder 0xfffe-first (80.6), `mf = 0` init with conditional assigns
// (79.1), a single mapping tail via an fi sentinel (79.3), named `map`/`fc` feature locals (80.6/73.7),
// redundant callee-saved aliases c2/p2/t2 (equal) and a second g2 (75.2), `int f` (81.1), `int cx/cz`
// (81.7), `unsigned short` cx/cz (81.2/81.5), moving cx/cz inside `if (mf)` (77.5), unit0 spellings
// `u->elev + u->type->high`, subtraction, `>` and hi/el/sum temps (equal/81.6/81.3/79.7/69.3), inlining
// cx/cz at both uses (76.9/75.8/63.1), a named `owner` local (63.5), `py` local (76.6), reference g
// (78.0), no-g direct g_game (81.1), direct g_game only in the feature block (75.2). Every spelling of
// the feature block CSEs to the same 847 bytes, so the g-in-edi vs cz-in-edi split is a global
// allocation choice that no source shape tried reaches; the mapping tail stays duplicated.
// deepseek-v4.1-flash (issue 2421 retry, 10 min): current body scores 82.3% / 847 bytes against 844, and
// nothing in this retry beat it, so the body is unchanged. Tried and scored lower or equal: unit0
// operand swap `u->elev + u->type->high` (same bytes, same diff), `int` cx/cz (81.7/82.2), cz declared
// first (80.2), extra `(void)cz` use (equal), two fresh locals gu/gf instead of reusing `g` (equal),
// the whole feature block written with direct `g_game->` (81.1: this DOES put g_game in edi, but it
// changes the feature-block expression shapes and duplicates the mapping tail anyway), direct `g_game->`
// only after the flags test (81.1), `g = g_game;` moved inside the block after the flags test (76.3),
// `g` left uninitialised and assigned (equal), local sum/fresh-elev spellings of the unit0 high test
// (69.3/79.7/81.6), rewriting the feature block with a shared `f` index (equal), a flag + single
// `mapping + f` tail (75.3), and adding `g = g_game;` in the cellX/cellZ store branch (82.0 / 856).
// What still differs is unchanged: g_game is not kept in edi after 0x49b284 (ours copies it to ecx and
// spills to [esp+0x30], and cz takes edi), plus the unit0 elev/high pair is swapped, the mapping tail
// is duplicated instead of merged at 0x49b31b, and the final block reloads g from [esp+0x30].
// deepseek-v4.1-flash (issue 1715): 73.6% -> 81.9% (855 bytes against 844). Three changes, all in the
// source shape, none of them register hints: (1) delete the `int oz = proj->py.i;` local and read
// `proj->py.i` directly in the unit0 test, which removes the hoisted `mov ebp,[esi+8]`; (2) declare
// `Game_0049b090* g = g_game;` and reassign `g = g_game;` immediately before the 0x4000 flags test,
// which is what makes MSVC emit the single `mov edi,[g_game]` reload at 0x49b284 instead of keeping a
// spilled g; (3) replace `MapFeature* mf = 0;` with an uninitialised `mf` and explicit `else mf = 0;`
// in every arm, which moves the `xor` out of the declaration and lands the feature-id load at the
// original's position. Still differs (all ~11 bytes): MSVC homes the new g in ecx and re-spills it to
// [esp+0x30] so the cellZ temp takes edi (`mov ecx,edi` + `mov [esp+0x30],ecx` extra), the unit0
// elev/high pair is swapped (ours ecx=elev,ebp=high; original ebp=elev,ecx=high), and the
// `mapping + f` tail is duplicated instead of merged at 0x49b31b. Ledger has the scored variants.
// GPT-6 retry: <windows.h> improves to 75.2%, not MATCH. Collision and
// feature lookup helper, width and reference variants did not improve the
// baseline. Game/height allocation and feature tail still differ.
// deepseek-v4.1-flash (issue 1400 retry): reconfirmed 73.6% / 842 bytes against
// 844, no variant beat it. Scored with check.py --sym: removing oz, using
// proj->py.i directly twice, drops to 62% / 866 and moves cell to ebp, g to ebx;
// block-local oz in each unit block is 59.2% / 869; oz = pos->y is 53.1% / 878;
// an early `unsigned short u1 = cell->unit1` is 61% / 847. The allocator always
// refuses g_game in edi once proj->py.i is a live value, so the first diff (the
// hoisted `mov ebp,[esi+8]` plus the g spill) is the whole problem; see below.
// deepseek-v4.1-flash retry: still 73.6%, 842 bytes against 844, no variant
// improved on it. Removing the `oz` local (direct proj->py.i in both unit
// blocks), moving it into the unit0 block, reading it through `pos`, swapping
// which block uses it, adding a second pointer local `q = proj`, and moving
// `g = g_game` earlier or later all produced the same 842-byte code (MSVC CSEs
// every spelling of the py load into one hoisted load in ebp). Declaring `g`
// early or dropping the `g` local entirely scored lower (59.3%, 72.8%). So the
// py load hoist and the g spill are one allocator state, not source-shape
// problems, and the notes below about the g-in-edi cause still stand.
// Not matched yet: 73.6%, 842 bytes against 844 (Sonnet 5.5 retry, #1097; was 72.8%). This header was rewritten
// because the previous one still described the 66.0% / 870-byte state, which
// the current body no longer has.
// Retry note: a `Game_0049b090* g = g_game;` local declared just before
// `if (cell->unit0)` (all later g_game-> uses go through it) gave +0.8 points and the
// right size. Still differs as described below: `oz` is hoisted into a register
// (ebp) where the original loads proj->py.i inside each unit block, and g then
// spills to [esp+0x30] and mf takes edi. About 150 variants (oz and g placement,
// block-local y, extra locals for ground and height, declaration order of
// cx/cz/f/mf) never moved it; without oz the cell lands in ebp, with it in ebx.
//
// 0x49b090 is the projectile collision test. It looks up the map cell holding
// the projectile with FUN_004815a0(&proj->pos); if there is none it stores the
// selected projectile's last position and sound, clears the selection, sets the
// dead flag and returns. Otherwise: (1) if the projectile is attached to a unit,
// the 64-bit squared distance is checked against type->radius^2 and the
// projectile is killed on contact; (2) proj->radius is set to
// (cell->radius + cell->ground) / 2; (3) the two unit indices in the cell are
// tested against proj->owner and the height window
// [type->low + elev, type->high + elev]; (4) the map-feature id is resolved,
// including the 0xfffe "read the neighbour cell" case, and if the feature is new
// for this cell the cell coordinates are stored; (5) the 0x8000 and 0x10000 flag
// rules, the g_game->limit ceiling and the netgame check are applied before the
// final kill.
//
// Two fixes moved it from 66.0% / 870 bytes to 72.8% / 852 bytes, and both are
// counter-intuitive, so do not "tidy" either one away:
//
// 1. The prologue register swap IS solved, and the fix is in the position
//    spelling rather than in any register hint. The position must be read
//    through a NAMED `Pos_0049b090* pos` local whose value is used again after
//    the cell lookup (as in `int dx = pos->x - proj->unit->pos.x;`), not read
//    inline as `proj->px.i`. That alone makes the original's own prologue
//    (`lea edi, [esi+4] / push edi`), its entire 64-bit distance block and its
//    radius block come out byte exact, including the interleaved load order
//    (py, px, pz) and the `mov ebp, eax / mov [esp+0x1c], edx` spill sequence.
//    The previous header concluded from a true longest-common-subsequence over
//    instructions that neither ordering wins (165/282 against 168/282). That
//    comparison was sound but it was made before the position was read through
//    a named local, and with the named local the original's order does win.
// 2. The cell register is `ebx`, not `ebp`, and about 30 expression-shape
//    variants (declaration orders, commutativity, control flow, extra locals in
//    other regions) all failed to move it. The only thing that flipped it was a
//    change to the SET of live variables: one extra `int` local, declared just
//    before the `if (cell->unit0)` block and used once inside that block, makes
//    MSVC 5 give the cell ebx instead of ebp. 64.9% before, 72.8% after. This
//    is the reusable lesson: in a register-starved function the choice between
//    ebx and ebp for a long-lived local can be flipped by adding a single live
//    local in a specific region, and the two orderings are otherwise
//    indistinguishable to every spelling sweep.
//
// 3. The flag word at ProjType+0x111 is a bitfield struct, not a plain unsigned
//    int. The original tests bit 14 as `test ah, 0x40` and bit 16 as
//    `test dword ptr [m], 0x10000`, but bit 15 as
//    `mov eax,[m]; shr eax,0xf; test al,1; je`. ONLY a 1-bit bitfield read
//    produces the shift form: `flags & 0x8000` and `(flags >> 15) & 1` both
//    fold to `test ah, 0x80` (probed directly in
//    build/scratch/0x49b090/probe1..3.cpp). The working spelling is a union of
//    the raw word with a bitfield view:
//        union TypeFlags_0049b090 {
//            unsigned int raw;
//            struct { unsigned int low : 15; unsigned int b15 : 1;
//                     unsigned int high : 16; } b;
//        };
//    used as `type->flags.raw & 0x4000`, `type->flags.b.b15`,
//    `type->flags.raw & 0x10000`. Worth about 3 points once the register
//    layout is right.
//
// What still differs, all of it in the last third of the function, and all of it
// traced to one cause: `g_game` is not kept in edi across the unit blocks and
// the feature block. In the original edi holds g_game from 0x49b1ca to
// 0x49b3ba; in the current source the extra local from fix 2 above takes edi
// and g_game is reloaded (`mov eax,[g_game]; mov ecx,[eax+0x14357]`). That one
// cause explains all of:
//   - the duplicated `mapping + f*256` tail, because the original merges both
//     arms into the shared block at 0x49b31b and this build does not;
//   - `mf` living in edi instead of ecx;
//   - `cmp cx, ax` against the original's `cmp dx, ax` at 0x49b3ad;
//   - the `mov [esp+0x30]` reload of g_game at 0x49b3ad and 0x49b3ba.
// Two smaller items alongside it:
//   - `cz` is kept in ax instead of being spilled to [esp+0x30], so the cellZ
//     test is `cmp word ptr [esi+0x5c], ax` where the original has
//     `mov ax,[esi+0x5c]; cmp ax, word ptr [esp+0x30]`.
//   - in the feature block the feature id is zero-extended eagerly
//     (`xor edx,edx` + `and ecx,0xffff`) where the original defers it to
//     `mov dx,[ebx+8]` and an `and edx,0xffff` on the taken path only.
//
// Where to look next, in order:
//   1. The g_game-in-edi problem, which is the highest value. Not yet tried: an
//      extra variable live only in a region where edi is dead (before
//      0x49b1ca or after 0x49b284), two extra variables whose net register cost
//      is zero, or a `Game_0049b090*` local declared so the compiler
//      rematerialises g_game rather than keeping it.
//   2. Forcing the cz spill to [esp+0x30], for instance by giving cz a second
//      use, which should also fix the cellX/cellZ compare shape.
//   3. The reusable harness is in build/scratch/0x49b090/ (gen.py, rg.py,
//      try*.py, probe*.cpp, sweep1.py). It scores a variant in about 0.3 s
//      with `check.py --sym`, which is why this pass used no check.py runs at
//      all.
//
// Suspected original bug, still open and now confirmed in the part that does
// match: 0x49b2c8 to 0x49b2d6 range-checks the map-feature id against
// g_game+0x14253, but the 0xfffe reload path at 0x49b2e7 to 0x49b30f re-tests
// only against 0xfffb and skips the count check, so a feature id read from the
// neighbouring cell indexes g_game->mapping unchecked.

#include <windows.h>

// deepseek-v4.1-flash (issue 4021 retry, 10 min): body kept at 82.3% / 847
// bytes. One more shape tried: arm2 of the feature map reusing the same `f`
// variable (`f = (cell - n)->feature;`) instead of a separate f2, hoping VC5
// would merge the two mapping tails at 0x49b31b; it is byte-neutral at 82.3% /
// 847 (same diff), so the merge is still out of reach. The residual is
// unchanged: g_game is not kept in edi after 0x49b284 (ours copies it to ecx
// and spills to [esp+0x30]), the unit0 elev/high pair is swapped, the mapping
// tail is duplicated instead of merged at 0x49b31b, and the final block
// reloads g from [esp+0x30].
#pragma pack(push, 1)

struct Pos_0049b090 {
    int x;
    int y;
    int z;
};

// 13 bytes, the stride the cell arithmetic at +0x6e walks with `n * 13`.
struct Cell_0049b090 {
    unsigned short unit0;             // +0x0
    unsigned short unit1;             // +0x2
    unsigned char height;             // +0x4
    unsigned char radius;             // +0x5
    unsigned char ground;             // +0x6
    unsigned char unknown_7;          // +0x7
    unsigned short feature;           // +0x8
    unsigned char offY;               // +0xa
    unsigned char offX;               // +0xb
    unsigned char unknown_c;          // +0xc
};

// The mapping is an array of these, indexed as `mapping[f * 256]`.
struct MapFeature_0049b090 {
    char unknown_0[0xfa];
    unsigned char height;             // +0xfa
    char unknown_fb[0x100 - 0xfb];
};

union TypeFlags_0049b090 {
    unsigned int raw;
    struct {
        unsigned int low : 15;
        unsigned int b15 : 1;
        unsigned int high : 16;
    } b;
};

struct ProjType_0049b090 {
    char unknown_0[0xd6];
    unsigned short radius;             // +0xd6
    char unknown_d8[0xfe - 0xd8];
    unsigned short sound;              // +0xfe
    char unknown_100[0x111 - 0x100];
    TypeFlags_0049b090 flags;          // +0x111
};

struct UnitType_0049b090 {
    char unknown_0[0x162];
    int low;                           // +0x162
    char unknown_166[8];
    int high;                          // +0x16e
};

struct Unit_0049b090 {
    char unknown_0[4];
    Pos_0049b090 pos;                  // +0x4
    char unknown_10[0x6e - 0x10];
    int elev;                          // +0x6e
    char unknown_72[0x92 - 0x72];
    UnitType_0049b090* type;           // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char owner;               // +0xff
    char unknown_100[0x118 - 0x100];
};

union Flags_0049b090 {
    unsigned char value;
    struct {
        unsigned char b0 : 1;
        unsigned char dead : 1;
        unsigned char rest : 6;
    } bits;
};

// The world position is three ints at +0x4 and, in the same bytes, three shorts
// at +0x6, +0xa and +0xe: the map-cell coordinates. A union is the only way to
// get the two views, and it is the shorts the code reaches for the cell.
union WordPair_0049b090 {
    int i;
    struct {
        char lo[2];
        short hi;
    } s;
};

struct Proj_0049b090 {
    ProjType_0049b090* type;           // +0x0
    WordPair_0049b090 px;              // +0x4 (short at +0x6)
    WordPair_0049b090 py;              // +0x8 (short at +0xa)
    WordPair_0049b090 pz;              // +0xc (short at +0xe)
    char unknown_10[0x20 - 0x10];
    int field_20;                      // +0x20
    char unknown_24[0x56 - 0x24];
    Unit_0049b090* unit;               // +0x56
    short cellX;                       // +0x5a
    short cellZ;                       // +0x5c
    short radius;                      // +0x5e
    char unknown_60[0x66 - 0x60];
    unsigned char owner;               // +0x66
    char unknown_67[2];
    Flags_0049b090 flags;              // +0x69
};

struct Game_0049b090 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    int featureCount;                  // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    MapFeature_0049b090* mapping;      // +0x1426f
    char unknown_14273[0x1427f - 0x14273];
    unsigned char limit;               // +0x1427f
    char unknown_14280[0x142f7 - 0x14280];
    Proj_0049b090* selected;           // +0x142f7
    char unknown_142fb[0x1433f - 0x142fb];
    Pos_0049b090 lastPos;              // +0x1433f
    unsigned short lastSound;          // +0x1434b
    char unknown_1434d[0x14357 - 0x1434d];
    Unit_0049b090* units;              // +0x14357
    char unknown_1435b[0x391e9 - 0x1435b];
    void* net;                         // +0x391e9
};

struct Net_0049b090 {
    char unknown_0[0xd48];
    int field_d48;
};
#pragma pack(pop)

extern Game_0049b090* g_game;

Cell_0049b090* __stdcall FUN_004815a0(Pos_0049b090* pos);
void __stdcall FUN_00499eb0(Proj_0049b090* proj, Unit_0049b090* unit);

// The second argument (the type) is the first stack dword: the prologue's
// spill to [esp+0x30] overwrites arg1, and both later reloads of the type
// read [esp+0x2c]. `pos` stays in edi across the cell lookup, which is what
// makes the original load the three differences through it.
// FUNCTION: 0x49b090
void __stdcall FUN_0049b090(ProjType_0049b090* type, Proj_0049b090* proj)
{
    Pos_0049b090* pos = (Pos_0049b090*)&proj->px;
    Cell_0049b090* cell = FUN_004815a0(pos);

    if (!cell) {
        if (proj == g_game->selected) {
            g_game->lastPos = *(Pos_0049b090*)&g_game->selected->px;
            g_game->lastSound = proj->type->sound;
            g_game->selected = 0;
        }
        proj->flags.bits.dead = 1;
        return;
    }
    if (proj->unit) {
        int dx = pos->x - proj->unit->pos.x;
        int dy = pos->y - proj->unit->pos.y;
        int dz = pos->z - proj->unit->pos.z;
        int r = proj->type->radius;
        int d = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dy * dy) >> 32)
            + (int)(((__int64)dz * dz) >> 32);
        if (d < r * r)
            FUN_00499eb0(proj, 0);
    }
    proj->radius = (cell->radius + cell->ground) / 2;
    if (cell->unit0) {
        Unit_0049b090* u = &g_game->units[cell->unit0];
        if (u->owner != proj->owner && proj->py.i < u->type->high + u->elev) {
            FUN_00499eb0(proj, u);
            return;
        }
    }
    if (cell->unit1) {
        Unit_0049b090* u = &g_game->units[cell->unit1];
        if (u->owner != proj->owner) {
            if (proj->py.i >= u->type->low + u->elev
                && proj->py.i <= u->type->high + u->elev) {
                FUN_00499eb0(proj, u);
                return;
            }
        }
    }
    if (type->flags.raw & 0x4000)
        return;
    {
        short cx = proj->px.s.hi / 16;
        short cz = proj->pz.s.hi / 16;
        unsigned short f = cell->feature;
        MapFeature_0049b090* mf;
        if (f < 0xfffb) {
            if (f < g_game->featureCount)
                mf = g_game->mapping + f;
            else
                mf = 0;
        } else if (f == 0xfffe) {
            int n = g_game->width * cell->offY + cell->offX;
            int f2 = (cell - n)->feature;
            if (f2 < 0xfffb)
                mf = g_game->mapping + f2;
            else
                mf = 0;
        } else {
            mf = 0;
        }
        if (mf) {
            if (mf->height + cell->ground > proj->py.s.hi) {
                if (proj->cellX == cx && proj->cellZ == cz)
                    mf = 0;
                else {
                    proj->cellX = cx;
                    proj->cellZ = cz;
                }
            } else {
                mf = 0;
            }
        }
        if (mf) {
            FUN_00499eb0(proj, 0);
            return;
        }
    }
    if (cell->ground > proj->py.s.hi) {
        if (type->flags.b.b15) {
            proj->field_20 = -(proj->field_20 >> 2);
            return;
        }
    } else if (type->flags.raw & 0x10000) {
        return;
    } else if (proj->py.s.hi >= g_game->limit) {
        return;
    } else if (((Net_0049b090*)g_game->net)->field_d48) {
        return;
    }
    FUN_00499eb0(proj, 0);
}
