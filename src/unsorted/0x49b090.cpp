// Decompiled by space-bunny-free. Names are provisional.
// Not matched yet, 66.0% (870 bytes against 844). Every block is structurally
// right (the whole prologue, both unit tests, the feature resolution and the
// four exits are in the original's order and each of them is the right test);
// what is left is one allocator state. Grouped by block:
//
// - ALL BLOCKS (prologue on): the original computes `lea edi, [esi+4]` for the
//   position pointer and keeps the CELL in ebx; here the position pointer is in
//   ebx and the cell in edi. Every later difference in the disassembly
//   (0x49b111 `mov ecx, [edi+4]`, 0x49b1ae the cell reload, 0x49b1c7 the
//   cell read, 0x49b1ea the units base off edi) follows from that one swap.
//   Adding a live local to demote a variable one step, or removing one to
//   promote it, is the guide's item 2; I tried an extra live int, two extra
//   shorts, a `g_game` local, a `proj->unit` local, a dead-store pair, an
//   inline accessor for the position, the unit position through a reference,
//   and three static __inline helpers for the differences. All of them score
//   66.0% or lower (the g_game local drops to 52.2%, a `unit` local to 59.6%),
//   so none of them moves this particular pair.
// - The swap is NOT free to fix, and the two orderings have a measurable cost
//   either way (build/scratch/0x49b090/{v2,lcs}.cpp and .py): reading the three
//   differences through the `pos` POINTER instead of through `proj->px.i` is
//   what makes the original's own prologue (`lea edi, [esi+4] / push edi`), its
//   whole 64-bit distance block and its radius block come out byte exact, but
//   it moves the cell one step further down the callee-saved order (ebx ->
//   ebp) and only the cell register then differs. A true LCS over instructions
//   (build/scratch/0x49b090/lcs.py) says 165/282 for that against 168/282 for
//   the version kept here, and check.py's difflib number says 64.9% against
//   66.0%, so the two metrics agree: neither ordering wins. So the original
//   must be doing a THIRD thing, and the most likely candidate is that its
//   distance block reads the position through a pointer that is not the call
//   argument (a `&proj->pos` recomputed inside the block, or the argument
//   itself with the block reading a second copy), which would give the cell
//   the third callee-saved register while still coding the loads off one
//   register.
// - Searched for that third thing this run and did not find it
//   (build/scratch/0x49b090/{w1,w2,w3,x1,x2}.cpp, all scored with --sym, so
//   none of them cost a real check.py run):
//     w1 871 bytes 63.2%  pos-pointer reads, differences declared y,x,z
//     w2 871 bytes 62.5%  pos-pointer reads, differences declared y,z,x
//     w3 871 bytes 63.2%  as w1 plus a local `Pos* up = &proj->unit->pos`
//     x1 863 bytes 64.9%  v2 (x,y,z) plus a `Game_0049b090* g = g_game` local
//     x2 862 bytes 64.9%  the version kept here plus the same g_game local
//   So the declaration order of the three differences is a real lever and
//   x,y,z (the order kept here) is the best of the three, and a `g_game`
//   local is worth 8 bytes of code without moving the score: it lets the two
//   feature-index arms share ONE `mapping + f * 256` (the original's
//   0x49b31b join, with 0x49b2d6 `jl` and 0x49b30f `jb` both entering it)
//   instead of emitting the `shl edx,8 / add` pair twice, but it cannot get
//   g_game into a register while the cell still occupies edi, so the shared
//   tail still reloads the global and the win cancels. The 8 bytes is real
//   though: x2 is 862 against the 870 kept here, all of it the duplicate tail.
// - The reason the whole rest of the function is stuck is now clear and worth
//   recording: the original keeps a `Game*` in a CALLEE-SAVED register across
//   0x49b1ca to 0x49b3ba (`mov eax,[edi+0x14253]`, `[edi+0x14233]`,
//   `[edi+0x1426f]`, `[edi+0x1427f]`, `[edi+0x391e9]`, `[edi+0x14357]`), which
//   only fits if the position pointer has already died in the distance block
//   and freed edi. So the prologue swap is upstream of the g_game local, and
//   getting both right is one problem, not two.
// - Distance block: the original computes the differences in the order y, z, x
//   and keeps x in ebp; x, y, z order scores 66.0% and y, z, x 65.3%, so the
//   x-first order here is already the better of the two even though it still
//   loads in a different sequence.
// - Feature block: the original materialises the resolved map-feature pointer
//   in ecx and tests it with `test ecx, ecx`, and it puts the `shl edx, 8`
//   (index * 256) after the count test. This version materialises it in edx
//   and hoists the shift. `mf = g_game->mapping + f` with a 0x100-byte element
//   is what gives the shift, and f is already an unsigned short, so the
//   `shl edx, 8` shape follows; what does not follow is that MSVC keeps it in
//   ecx.
// - The `f2` reload path (0x49b2e7) also differs in where g_game comes from
//   (ecx held in the original, reloaded from memory here).
//
// Layout facts established by probe (build/scratch/0x49b090/probe.cpp):
// - the projectile's position is a union of three ints at +0x4 and three shorts
//   at +0x6, +0xa and +0xe, the shorts being the map-cell coordinates the code
//   divides by 16. Only a union reproduces both views, and `#pragma pack(1)`
//   is required for it.
// - the cell is 13 bytes (the `lea [ecx+ecx*2]` / `lea [ecx+eax*4]` pair is
//   13 * n), with the two unit indices at +0 and +2, radius and ground at +5
//   and +6, the feature id at +8, and the two cell offsets at +0xa and +0xb.
// - the unit array stride is 0x118, with owner at +0xff, type at +0x92 and
//   elevation at +0x6e; the unit's own position is at +0x4, not +0.
// - the mapping at g_game+0x1426f is an array of 0x100-byte entries indexed
//   directly by the feature id (the count check at +0x14253 is separate), and
//   the entry's height is at +0xfa.
// - the 64-bit distance is the three `(int)(((__int64)v * v) >> 32)` high
//   products, the standard idiom in this codebase (see 0x40b0d0, 0x401e00),
//   summed in the order x, y, z.
//
// Suspected original bug:
// - 0x49b2c8..0x49b2d6 compares the feature id against g_game+0x14253 and
//   takes the null path when it is out of range, but the 0xfffe reload path
//   (0x49b2e7..0x49b30f) re-tests only against 0xfffb and skips the count
//   check entirely, so a feature id from the neighbouring cell can index the
//   mapping array unchecked. Same reader, two guards, one of them missing.
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

struct ProjType_0049b090 {
    char unknown_0[0xd6];
    unsigned short radius;             // +0xd6
    char unknown_d8[0xfe - 0xd8];
    unsigned short sound;              // +0xfe
    char unknown_100[0x111 - 0x100];
    unsigned int flags;                // +0x111
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
        int dx = proj->px.i - proj->unit->pos.x;
        int dy = proj->py.i - proj->unit->pos.y;
        int dz = proj->pz.i - proj->unit->pos.z;
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
    if (type->flags & 0x4000)
        return;
    {
        short cx = proj->px.s.hi / 16;
        short cz = proj->pz.s.hi / 16;
        unsigned short f = cell->feature;
        MapFeature_0049b090* mf = 0;
        if (f < 0xfffb) {
            if (f < g_game->featureCount)
                mf = g_game->mapping + f;
        } else if (f == 0xfffe) {
            int n = g_game->width * cell->offY + cell->offX;
            unsigned short f2 = (cell - n)->feature;
            if (f2 < 0xfffb)
                mf = g_game->mapping + f2;
        }
        if (mf) {
            if (mf->height + cell->ground <= proj->py.s.hi)
                mf = 0;
            else if (proj->cellX == cx && proj->cellZ == cz)
                mf = 0;
            else {
                proj->cellX = cx;
                proj->cellZ = cz;
            }
        }
        if (mf)
            FUN_00499eb0(proj, 0);
    }
    if (cell->ground > proj->py.s.hi) {
        if (type->flags & 0x8000) {
            proj->field_20 = -(proj->field_20 >> 2);
            return;
        }
    } else if (type->flags & 0x10000) {
        return;
    } else if (proj->py.s.hi >= g_game->limit) {
        return;
    } else if (((Net_0049b090*)g_game->net)->field_d48) {
        return;
    }
    FUN_00499eb0(proj, 0);
}
