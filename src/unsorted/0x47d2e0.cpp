// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, retried by Sonnet 5.5, retried by deepseek-v4.1-flash, finished by claude-sonnet-5-5. Names are provisional.
// PARTIAL 70.6% (1337 of 1339 bytes; was 45.6% at 1237 bytes). What moved it:
//  * `los` is the neighbours' Map: `explored` = ByteMap {data, MapSize{width, height}}
//    with MapSize::Contains and ByteMap::Get (see 0x4658e0, 0x465ac0, 0x408090), and
//    the position is a 12-byte 16.16 fixed-point Pos {Fix x, y, z} (Fix = union of an
//    int and two shorts). Pos is address-taken-like memory, so wx/wz are stored as
//    dwords and read back with `movsx word [esp+0x32]/[0x3a]`, and the frame is the
//    original 0x2c with no width cache and no dummy locals.
//  * The loop's `rr`/Terrain tests are early-return inline helpers (Blocked_,
//    Terrain_), which reproduces the `xor ecx,ecx; jmp join` ladders exactly.
//  * The second visibility test is NOT folded to ok = 1 when it goes through an
//    inline helper that re-derives tx/ty from `&pos` (IsSeen_/IsExplored_) while the
//    first test is written by hand (Contains, then `(vis[w*y+x] & bit) == 0`). A
//    helper taking (los, x, y) is folded again, and so is any hand-written copy.
//  * `int ok` declared at the very top (assigned before `if (los)`), and `bit`
//    declared BEFORE the Contains test, each worth several points through register
//    allocation only.
// Still differs: (1) the prologue allocation (original: g_game in ebp, origin.x in esi;
// ours the reverse for the first compare), (2) the original keeps y (h << 16) as a
// spilled temp in the dead `los` home slot [esp+0x4c], ours puts it in pos.y at
// [esp+0x34] (the helpers need y inside Pos), (3) `bit`/vis/width are computed before
// the bounds jumps here, after them in the original, (4) arm 2 stores ok = 0 with a
// mov instead of `xor eax,eax; jmp` into one shared store, (5) the cell.y * width
// operand order after the LOS block and the col latch order.
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

static inline int IsExplored_0047d2e0(Los_0047d2e0* los, Pos_0047d2e0* pos)
{
    int tx = pos->x.p.hi >> 5;
    int ty = (pos->z.p.hi - (pos->y.p.hi >> 1)) >> 5;
    if (los->explored.size.Contains(tx, ty) && los->explored.Get(tx, ty) != 0)
        return 1;
    return 0;
}

static inline int IsSeen_0047d2e0(Los_0047d2e0* los, Pos_0047d2e0* pos)
{
    int tx = pos->x.p.hi >> 5;
    int ty = (pos->z.p.hi - (pos->y.p.hi >> 1)) >> 5;
    if (!los->explored.size.Contains(tx, ty))
        return 0;
    return (g_game->field_14273[los->explored.size.width * ty + tx] &
            (1 << g_game->player)) != 0;
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
        pos.x.v = (origin.x + cell.x * 2) << 19;
        pos.z.v = (origin.y + cell.y * 2) << 19;
        pos.y.v = FUN_00485010(&cell) << 16;
        x = pos.x.p.hi >> 5;
        y = (pos.z.p.hi - (pos.y.p.hi >> 1)) >> 5;

        unsigned int bit = 1 << g_game->player;
        if (!los->explored.size.Contains(x, y))
            return 0;

        if ((g_game->field_14273[los->explored.size.width * y + x] & bit) == 0)
            return 0;

        if ((g_game->losFlags & 2) == 2)
            ok = IsExplored_0047d2e0(los, &pos);
        else
            ok = IsSeen_0047d2e0(los, &pos);

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