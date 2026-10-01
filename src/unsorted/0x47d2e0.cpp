// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, retried by Sonnet 5.5, retried by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// PARTIAL 80.7% (1339 of 1339 bytes, exact size; was 70.6% at 1337 bytes). What moved it:
//  * The ground height is NOT pos.y. The original stores `FUN_00485010(&cell) << 16`
//    into the dead `los` home slot [esp+0x4c] and reads `movsx word [esp+0x4e]`, so it
//    is a separate `Fix` local declared beside the Pos and both inline helpers take it
//    as a third `Fix*` argument. With the height inside Pos the helpers keep it there
//    and neither the frame layout nor the store sequence can match.
//  * Both inline helpers take the player bit as a fourth argument. That stops MSVC
//    from re-deriving `1 << g_game->player` inside IsSeen, which is what put the
//    g_game reload and the vis multiply in the wrong block (69.3% -> 80.2%).
//  * The helpers read the position through a six-short `Position` cast
//    (`(Position_0047d2e0*)&pos`), the shape matched in 0x408090 and 0x465ac0,
//    instead of `pos->x.p.hi` (80.2% -> 80.7%).
// Earlier passes, still true:
//  * `los` is the neighbours' Map: `explored` = ByteMap {data, MapSize{width, height}}
//    with MapSize::Contains and ByteMap::Get (see 0x4658e0, 0x465ac0, 0x408090).
//  * The loop's `rr`/Terrain tests are early-return inline helpers (Blocked_,
//    Terrain_), which reproduces the `xor ecx,ecx; jmp join` ladders exactly.
//  * The second visibility test is NOT folded to ok = 1 when it goes through an
//    inline helper that re-derives tx/ty from `&pos` (IsSeen_/IsExplored_) while the
//    first test is written by hand (Contains, then `(vis[w*y+x] & bit) == 0`). A
//    helper taking (los, x, y) is folded again, and so is any hand-written copy.
//  * `int ok` declared at the very top (assigned before `if (los)`), and `bit`
//    declared BEFORE the Contains test, each worth several points through register
//    allocation only.
// What is left is ONE cause: the esi/edi choice in the prologue. The original takes
// `esi` for origin.x (spilled to [esp+0x18]) and `edi` for `los`; ours takes `edi` for
// origin.x ([esp+0x14]) and `esi` for `los`, so every later use of the two is
// mirrored, `los` has to be reloaded from [esp+0x4c] in the explored arm, and the
// loop's mask index lands in esi instead of edi. The register the 16-bit cell.y temp
// takes decides it: the original emits `mov di, word [esp+0x46]`, we emit
// `mov si, word [esp+0x46]`. Tried and flat at 80.7% or worse: `origin.x + cell.x` /
// `cols + cell.x` / `cell.x + cols` operand orders, `int width0` cached before or
// after `cols`, `int rows` / `int oy` / `int cy` locals, the guard as two `if`s, as
// one `||`, as an inline OutOfBounds helper, the two bounds tests swapped, `cols`
// materialised before the cell guard, origin read after the guard, the map width
// cached in a local `w` (with and without passing it to the helpers), the first
// visibility test folded into its own inline helper, `los` and `unit` cached in
// locals, a second `Game*` local for the cell index, `CellAt()` for the index, the
// helpers re-deriving the bit (69.3%) or taking it as a pointer (80.2% unchanged),
// the explored arm's Contains as a nested `if` (h6) or an early `return 0` (h7),
// braces on the visibility arms, and the mask-index spellings.
// Still open once that flips: the explored arm's own `mov [esp+0x10], 0` block (we
// share one store with the seen arm), `movsx eax,[esi+0x46]; imul eax,[ebp+0x14233]`
// against our width-into-eax form, and the col latch order.
#pragma pack(push, 1)

union Fix_0047d2e0 {
    int v;
    struct { short lo; short hi; } p;
};

struct Point {

    short x;
    short y;
};

struct Cell_0047d2e0 {
    short field_0;
    char unknown_2[0x5 - 0x2];
    unsigned char field_5;
    unsigned char field_6;
    unsigned char field_7;
    short field_8;
    unsigned char field_a;
    unsigned char field_b;
    unsigned char field_c;
};

struct Unit_0047d2e0 {
    char unknown_0[0x14a];
    Point origin;
    unsigned char* mask;
    char unknown_152[0x1be - 0x152];
    short field_1be;
    short field_1c0;
    char unknown_1c2[0x228 - 0x1c2];
    unsigned char field_228;
    char unknown_229[0x22c - 0x229];
    unsigned char field_22c;
};

struct MapSize_0047d2e0 {
    unsigned int width;
    unsigned int height;
    int Contains(unsigned int tx, unsigned int ty) { return tx < width && ty < height; }
};

struct ByteMap_0047d2e0 {
    unsigned char* data;
    MapSize_0047d2e0 size;
    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct Los_0047d2e0 {
    char unknown_0[0x7c];
    ByteMap_0047d2e0 explored;
};

struct Game_0047d2e0 {
    char unknown_0[0x2a43];
    unsigned char player;
    char unknown_2a44[0x14233 - 0x2a44];
    int width;
    int height;
    char unknown_1423b[0x14253 - 0x1423b];
    int field_14253;
    char unknown_14257[0x1426f - 0x14257];
    unsigned char* field_1426f;
    unsigned short* field_14273;
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;
    char unknown_14280[0x14281 - 0x14280];
    unsigned char losFlags;
    char unknown_14282[0x14287 - 0x14282];
    Cell_0047d2e0* cells;
};
#pragma pack(pop)

extern Game_0047d2e0* g_game;
extern int DAT_0051e684;
extern int DAT_0051e688;

int __stdcall FUN_00485010(Point* p);

struct Pos_0047d2e0 {
    Fix_0047d2e0 x, y, z;
};

struct Position_0047d2e0 {              // 16.16 fixed point, only high words read
    short xFrac;
    short x;
    short yFrac;
    short y;
    short zFrac;
    short z;
};

static inline int IsExplored_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fix_0047d2e0* hgt, unsigned int bit)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    if (los->explored.size.Contains(tx, ty) && los->explored.Get(tx, ty) != 0)
        return 1;
    return 0;
}

static inline int IsSeen_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fix_0047d2e0* hgt, unsigned int bit)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    if (!los->explored.size.Contains(tx, ty))
        return 0;
    return (g_game->field_14273[los->explored.size.width * ty + tx] & bit) != 0;
}

static int Blocked_0047d2e0(Cell_0047d2e0* c)
{
    unsigned short v = c->field_8;
    if (v == 0xffff)
        return 0;
    if (v < 0xfffb) {
        if ((int)v >= g_game->field_14253)
            return 1;
        return (g_game->field_1426f[v * 0x100 + 0xfe] >> 6) & 1;
    }
    if (v != 0xfffe)
        return 1;
    Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return (g_game->field_1426f[v2 * 0x100 + 0xfe] >> 6) & 1;
}

static unsigned char* Terrain_0047d2e0(
Cell_0047d2e0* c)
{
    if (c == 0)
        return 0;
    unsigned short v = c->field_8;
    if (v < 0xfffb) {
        if ((int)v >= g_game->field_14253)
            return 0;
        return g_game->field_1426f + v * 0x100;
    }
    if (v != 0xfffe)
        return 0;
    Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return g_game->field_1426f + v2 * 0x100;
}

// FUNCTION: 0x47d2e0
int __stdcall FUN_0047d2e0(Unit_0047d2e0* unit, Point cell, short type, Los_0047d2e0* los)
{
    int ok;
    DAT_0051e684 = 0;
    DAT_0051e688 = 0;
    Point origin = unit->origin;
    if (cell.x < 1 || cell.y < 1)
        return 0;
    int cols = origin.x;
    int width0 = g_game->width;
    if (cell.x + cols >= width0)
        return 0;
    if (cell.y + origin.y >= g_game->height)
        return 0;
    int x;
    int y;
    ok = 1;
    if (los != 0) {
        Pos_0047d2e0 pos;
        Fix_0047d2e0 hgt;
        pos.x.v = (origin.x + cell.x * 2) << 19;
        pos.z.v = (origin.y + cell.y * 2) << 19;
        hgt.v = FUN_00485010(&cell) << 16;
        x = pos.x.p.hi >> 5;
        y = (pos.z.p.hi - (hgt.p.hi >> 1)) >> 5;

        unsigned int bit = 1 << g_game->player;
        if (!los->explored.size.Contains(x, y))
            return 0;

        if ((g_game->field_14273[los->explored.size.width * y + x] & bit) == 0)
            return 0;

        if ((g_game->losFlags & 2) == 2)
            ok = IsExplored_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt, bit);
        else
            ok = IsSeen_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt, bit);

    }
    unsigned char min6 = 0xff;
    unsigned char max5 = 0;
    unsigned char max5b = 0;
    int found80 = 0;
    int foundFE20 = 0;
    int index = 0;
    Cell_0047d2e0* c = &g_game->cells[cell.y * g_game->width + cell.x];
    int row;
    int col;
    for (row = 0; row < origin.y; row++) {
        for (col = 0; col < cols; col++) {
            DAT_0051e688 += c->field_7;
            int m = unit->mask[index++];
            if (m & 8) {
                if (c->field_6 < min6)
                    min6 = c->field_6;
                if (c->field_5 > max5)
                    max5 = c->field_5;
            }
            if ((m & 0x10) && c->field_5 > max5b)
                max5b = c->field_5;
            if ((m & 1) && (c->field_c & 2) && ok)
                return 0;
            if ((m & 6) && c->field_0 != 0 && c->field_0 != type && ok)
                return 0;
            if (m & 0x20) {
                if (Blocked_0047d2e0(c) != 0)
                    return 0;
            }
            if (m & 0x40) {
                unsigned char* e = Terrain_0047d2e0(c);
                if (e != 0 && (e[0xff] & 2))
                    return 0;
            }
            if (m & 0x80) {
                found80 = 1;
                unsigned char* e = Terrain_0047d2e0(c);
                if (e != 0 && (e[0xfe] & 0x20))
                    foundFE20 = 1;
            }
            c++;
        }
        c += g_game->width - cols;

    }
    if (found80 && !foundFE20)
        return 0;
    unsigned char r;
    if (max5 < min6) {
        r = g_game->seaLevel - unit->field_22c;
    } else {
        if (max5 - min6 > unit->field_228)
            return 0;
        r = min6;
    }
    if (max5b > r)
        return 0;
    if (min6 < g_game->seaLevel - unit->field_1be)
        return 0;
    if ((max5 > max5b ? max5 : max5b) > g_game->seaLevel - unit->field_1c0)
        return 0;
    DAT_0051e684 = r;
    return 1;
}