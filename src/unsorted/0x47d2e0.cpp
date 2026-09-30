// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1. Names are provisional.
// PARTIAL: 36.8% (best). Rewritten from the disassembly: logic, block order and the second
// inlined terrain-entry lookup (static helper) now follow the original; reading g_game-> fields
// directly instead of through a cached Game* local gained 3.4 points (33.4 -> 36.8).
// Still differs: the frame is 0x20 vs the original 0x2c, so every local slot is 12 bytes off;
// the original spills both LOS temporaries (wx at [esp+0x30], wy at [esp+0x38]) because ebx=0,
// ebp=g_game, esi=origin.x/cols and edi=los are all live across the FUN_00485010 call, while ours
// keeps wx/wy in esi/edi and caches g_game in a register across the call.
// Tried this session: an explicit `int cols = origin.x` local used as the column bound (the
// original keeps exactly that value in its [esp+0x18] slot and reloads esi from it before the
// loop) plus reassigning function-scope x/y across the call, an `int r` truncated with
// `(short)r >> 1` to force the original's shl eax,0x10 idiom, and moving min6/max5 up to the
// ok declaration (that one dropped to 34.1%). None moved the frame or the prologue: ours keeps
// homing unit to ebx with the zero in eax, and leaves x/y in esi/edi instead of spilling them.
// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1. Names are provisional.
#pragma pack(push, 1)

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

struct Los_0047d2e0 {
    char unknown_0[0x7c];
    unsigned char* field_7c;
    unsigned int width;
    unsigned int height;
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

static unsigned char* Terrain_0047d2e0(Cell_0047d2e0* c)
{
    if (c != 0) {
        unsigned short v = c->field_8;
        if (v < 0xfffb) {
            if ((int)v < g_game->field_14253)
                return g_game->field_1426f + v * 0x100;
        } else if (v == 0xfffe) {
            Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
            unsigned short v2 = ref->field_8;
            if (v2 < 0xfffb)
                return g_game->field_1426f + v2 * 0x100;
        }
    }
    return 0;
}

// FUNCTION: 0x47d2e0
int __stdcall FUN_0047d2e0(Unit_0047d2e0* unit, Point cell, short type, Los_0047d2e0* los)
{
    DAT_0051e684 = 0;
    DAT_0051e688 = 0;
    Point origin = unit->origin;
    if (cell.x < 1 || cell.y < 1)
        return 0;
    
    int cols = origin.x;
    if (cell.x + cols >= g_game->width)
        return 0;
    if (cell.y + origin.y >= g_game->height)
        return 0;
    int ok = 1;
    int x = 0;
    int y = 0;
    int row;
    int col;
    if (los != 0) {
        x = (origin.x + cell.x * 2) << 19;
        y = (origin.y + cell.y * 2) << 19;
        int r = FUN_00485010(&cell);
        x = (short)(x >> 16) >> 5;
        y = ((short)(y >> 16) - ((short)r >> 1)) >> 5;
        if ((unsigned)x >= los->width || (unsigned)y >= los->height)
            return 0;
        unsigned int bit = 1 << g_game->player;
        if ((g_game->field_14273[y * los->width + x] & bit) == 0)
            return 0;
        if ((g_game->losFlags & 2) == 2) {
            if ((unsigned)x < los->width && (unsigned)y < los->height
                && los->field_7c[y * los->width + x] != 0)
                ok = 1;
            else
                ok = 0;
        } else {
            ok = (unsigned)x < los->width && (unsigned)y < los->height
                && (g_game->field_14273[y * los->width + x] & bit) != 0;
        }
    }
    unsigned char min6 = 0xff;
    unsigned char max5 = 0;
    unsigned char max5b = 0;
    int found80 = 0;
    int foundFE20 = 0;
    int index = 0;
    Cell_0047d2e0* c = &g_game->cells[cell.y * g_game->width + cell.x];
    for (row = 0; row < origin.y; row++) {
        for (col = 0; col < cols; col++) {
            DAT_0051e688 += c->field_7;
            unsigned char m = unit->mask[index];
            index++;
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
                int rr;
                unsigned short v = (unsigned short)c->field_8;
                if (v == 0xffff) {
                    rr = 0;
                } else if (v < 0xfffb) {
                    if ((int)v < g_game->field_14253)
                        rr = (g_game->field_1426f[v * 0x100 + 0xfe] >> 6) & 1;
                    else
                        rr = 1;
                } else if (v == 0xfffe) {
                    Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
                    unsigned short v2 = (unsigned short)ref->field_8;
                    if (v2 < 0xfffb)
                        rr = (g_game->field_1426f[v2 * 0x100 + 0xfe] >> 6) & 1;
                    else
                        rr = 0;
                } else {
                    rr = 1;
                }
                if (rr != 0)
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
        c += g_game->width - origin.x;
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
