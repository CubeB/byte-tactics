// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL first pass. Build-site scan: walks the unit's footprint (origin at
// Unit+0x14a, mask at +0x14e) over the map cells and accumulates
//   min(cell+6) over mask bit 3, max(cell+5) over mask bit 3,
//   max(cell+5) over mask bit 4,
// then range-checks the floors against the type's draft/limits at +0x228,
// +0x22c, +0x1be, +0x1c0. arg1 is a Point (cell), arg2 a type id compared
// against cell->field_0, arg3 a PlayerInfo los view (los +0x7c, w +0x80,
// h +0x84). g_game width +0x14233, height +0x14237, type table +0x1426f,
// 16-bit player bitmask +0x14273, sea level +0x1427f, los mode +0x14281,
// cells +0x14287, local player +0x2a43. Cell stride 0xd.
//
// Still differs (23.4%, 1209 vs 1339 bytes): frame is 0x24 vs 0x2c (8 bytes
// short) and the whole register allocation is wrong from the prologue on. The
// original reads arg0/arg1 into eax/ecx before the pushes, zeroes both DATs
// with ebx, keeps g_game in ebp (not edi), and keeps the "is there a los
// footprint" player bit and several counters in memory slots we dropped into
// registers. Our version also hoisted the DAT zeroing before the pushes and
// used a different outer/inner loop register set. No source-level lever tried
// yet beyond this first structural translation.
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

// FUNCTION: 0x47d2e0
int __stdcall FUN_0047d2e0(Unit_0047d2e0* unit, Point cell, short type, Los_0047d2e0* los)
{
    DAT_0051e684 = 0;
    DAT_0051e688 = 0;
    Point origin = unit->origin;
    if (cell.x < 1 || cell.y < 1)
        return 0;
    if (cell.x + origin.x >= g_game->width || cell.y + origin.y >= g_game->height)
        return 0;
    int ok = 1;
    if (los != 0) {
        int wx = (origin.x + cell.x * 2) << 19;
        int wy = (origin.y + cell.y * 2) << 19;
        int r = FUN_00485010(&cell);
        int x = ((short)wx) >> 5;
        int y = (((short)wy) - ((short)r >> 1)) >> 5;
        if ((unsigned)x >= los->width || (unsigned)y >= los->height)
            return 0;
        unsigned int bit = 1 << (g_game->player & 0x1f);
        unsigned short m = g_game->field_14273[y * los->width + x];
        if ((m & bit) == 0)
            return 0;
        if ((g_game->losFlags & 2) == 2) {
            if ((unsigned)x < los->width && (unsigned)y < los->height
                && los->field_7c[y * los->width + x] != 0)
                ok = 1;
            else
                ok = 0;
        } else {
            if ((unsigned)x < los->width && (unsigned)y < los->height)
                ok = (g_game->field_14273[y * los->width + x] & bit) != 0;
            else
                ok = 0;
        }
    }
    unsigned char min6 = 0xff;
    unsigned char max5 = 0;
    int found80 = 0;
    int foundFE20 = 0;
    unsigned char max5b = 0;
    int index = 0;
    Cell_0047d2e0* c = &g_game->cells[(cell.y * g_game->width + cell.x)];
    for (int row = 0; row < origin.y; row++) {
        for (int col = 0; col < origin.x; col++) {
            DAT_0051e688 += c->field_7;
            unsigned char m = unit->mask[index];
            index++;
            if (m & 8) {
                if (c->field_6 < min6)
                    min6 = c->field_6;
                if (c->field_5 > max5)
                    max5 = c->field_5;
            }
            if (m & 0x10) {
                if (c->field_5 > max5b)
                    max5b = c->field_5;
            }
            if ((m & 1) && (c->field_c & 2) && ok)
                return 0;
            if ((m & 6) && c->field_0 != 0 && c->field_0 != type && ok)
                return 0;
            if (m & 0x20) {
                unsigned short v = (unsigned short)c->field_8;
                int rr;
                if (v == 0xffff)
                    rr = 0;
                else if (v >= 0xfffb) {
                    if (v == 0xfffe) {
                        Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
                        unsigned short v2 = (unsigned short)ref->field_8;
                        if (v2 >= 0xfffb)
                            rr = 0;
                        else
                            rr = (g_game->field_1426f[v2 * 0x100 + 0xfe] & 0x40) >> 6;
                    } else {
                        rr = 1;
                    }
                } else if ((int)v < g_game->field_14253) {
                    rr = (g_game->field_1426f[v * 0x100 + 0xfe] & 0x40) >> 6;
                } else {
                    rr = 1;
                }
                if (rr != 0)
                    return 0;
            }
            if (m & 0x40) {
                unsigned char* e = 0;
                if (c != 0) {
                    unsigned short v = (unsigned short)c->field_8;
                    if (v < 0xfffb) {
                        if ((int)v < g_game->field_14253)
                            e = g_game->field_1426f + v * 0x100;
                    } else if (v == 0xfffe) {
                        Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
                        unsigned short v2 = (unsigned short)ref->field_8;
                        if (v2 < 0xfffb)
                            e = g_game->field_1426f + v2 * 0x100;
                    }
                }
                if (e != 0 && (e[0xff] & 2))
                    return 0;
            }
            if (m & 0x80) {
                found80 = 1;
                unsigned char* e = 0;
                if (c != 0) {
                    unsigned short v = (unsigned short)c->field_8;
                    if (v < 0xfffb) {
                        if ((int)v < g_game->field_14253)
                            e = g_game->field_1426f + v * 0x100;
                    } else if (v == 0xfffe) {
                        Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
                        unsigned short v2 = (unsigned short)ref->field_8;
                        if (v2 < 0xfffb)
                            e = g_game->field_1426f + v2 * 0x100;
                    }
                }
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
        if ((int)(unsigned)unit->field_228 < (int)(max5 - min6))
            return 0;
        r = min6;
    }
    if (max5b > r)
        return 0;
    if ((int)min6 < (int)(g_game->seaLevel - unit->field_1be))
        return 0;
    unsigned char mx = max5 > max5b ? max5 : max5b;
    if ((int)mx > (int)(g_game->seaLevel - unit->field_1c0))
        return 0;
    DAT_0051e684 = r;
    return 1;
}
