// Decompiled by space-bunny-free. Names are provisional.
// Samples the ground under a unit at its four surrounding terrain
// vertices and stores the resulting pitch (0x68) and roll (0x70) on the
// unit, plus a heading (0x64) from the two side vertices. The 0x11/0x04
// bytes of a heightmap tile are the two half heights of its edge pair;
// the corner heights are bilinearly interpolated.
//
// NOT MATCHED (27.7%, 857 bytes against our 1025). The frame model is
// pinned down and confirmed from the disassembly: locals run from esp+0x10
// to esp+0x8b, with k at +0x10, the hz low nibble at +0x14, a pointer to
// the corner array at +0x18, a pointer to the height array at +0x1c, the
// spilled row and map-info pointers at +0x20 and +0x24, further spills at
// +0x28 and +0x2c, the rotated {x,z} pair at +0x34, then four 8-byte
// corners at +0x3c and four 12-byte height records at +0x5c, with the two
// induction pointers based at &corners[0].z and &heights[0].h. Two things
// are still wrong.
//
// 1. Register allocation. The original parks the unit pointer in edi and
//    runs out of registers: it spills the row pointer, the map pointer, k
//    and both array pointers, then reloads all of them inside the loop
//    (0x48a4ed, 0x48a4f1, 0x48a4f8, 0x48a4fc) with the loop rotated so
//    the reload block is skipped on the first pass (the jmp at 0x48a4eb).
//    Our version keeps them live and hoists row->ids out of the loop, which
//    loses the rotation and is most of the byte difference. This is a
//    pressure problem, not a shape problem: raising the number of live
//    values in the body (the address-taken pair, the four corners and the
//    four height records all exist here already) is the lever, and nothing
//    tried in the time available found the right count.
// 2. The original stores a word at unit+0x70, which is the high half of the
//    16.16 pos.y at +0x6e, so pos.y cannot be an int in the same struct;
//    the field set here is posx at +0x6a, posy_lo at +0x6e, roll at +0x70
//    and posz at +0x72, which reproduces the offsets, but the pitch, roll
//    and heading writes are still one field out in the generated code.
//
// Confirmed from the disassembly, worth keeping:
//  * The local at esp+0x30 is read (0x48a736) and never written anywhere in
//    the function: the original reads an uninitialised local and stores it
//    into height[4][i].spare. A named uninitialised int reproduces the
//    warning and the load; MSVC 5 has not dropped the dead store here.
//  * The 4x4 corner height bilinear interpolation divides by 16 with the
//    cdq/and/add idiom and then shifts by 4, so the source has
//    "(a*f)/16 >> 4", not "a*f >> 8"; MSVC 5 does not fold the two.
//  * The float branch assigns height[4][i].h twice: once with the raw
//    height and once with max(height, seaLevel), through a frame temporary
//    at +0x14 that is then re-read at 0x48a71b and added to the result of
//    FUN_004b7123. Both the ternary and the division by 60 that follows
//    (unsigned magic 0x88888889, shr 5) are reproduced.
//  * The three float tests are one && chain, each branch jumping to the
//    same else block at 0x48a724 (test/je, test/je, test dh,0x40/jne), and
//    hs[4][i].wx is stored before the chain, not inside it.

#pragma pack(push, 1)

struct MapVertex_0048a490 {
    int x;                              // +0x0
    int y;                              // +0x4
    int z;                              // +0x8
};

struct MapRow_0048a490 {
    char unknown_0[0xc];
    unsigned short* ids;                // +0xc
    char unknown_10[0x20 - 0x10];
};

struct MapInfo_0048a490 {
    char unknown_0[0xc];
    int count;                          // +0xc
    char unknown_10[0x24 - 0x10];
    MapVertex_0048a490* verts;           // +0x24
    MapRow_0048a490* rows;               // +0x28
};

struct UnitType_0048a490 {
    char unknown_0[0x192];
    int sight;                          // +0x192
    char unknown_196[0x241 - 0x196];
    unsigned int flags;                 // +0x241
};

struct PlayerRec_0048a490 {
    char unknown_0[0x20];
    int sight;                          // +0x20
    char unknown_24[0x2a - 0x24];
    int age;                            // +0x2a
};

struct Pos2_0048a490 {
    int x;
    int z;
};

struct Corner_0048a490 {
    int wx;
    int h;
    int spare;
};

struct Unit_0048a490 {
    PlayerRec_0048a490* owner;           // +0x0
    char unknown_4[0x64 - 4];
    unsigned short hdg;                 // +0x64
    unsigned short aim;                 // +0x66
    unsigned short pitch;               // +0x68
    int posx;                           // +0x6a
    unsigned short posy_lo;             // +0x6e
    unsigned short roll;                // +0x70
    int posz;                           // +0x72
    char unknown_76[0x92 - 0x76];
    UnitType_0048a490* type;            // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short map;                 // +0xa6
    char unknown_a8[0xaa - 0xa8];
    unsigned short fix_lo;              // +0xaa
    char unknown_ac[0x110 - 0xac];
    unsigned int flags;                 // +0x110
};

struct Game_0048a490 {
    char unknown_0[0x14233];
    int gridW;                          // +0x14233
    int gridH;                          // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14377 - 0x14280];
    MapInfo_0048a490** maps;            // +0x14377
    char unknown_1437b[0x38a47 - 0x1437b];
    int frame;                          // +0x38a47
};

#pragma pack(pop)

extern Game_0048a490* g_game;

unsigned int FUN_004b6340();
int FUN_004b7123(int a, int b);
int FUN_004b715a(int x, int z);
void FUN_004b7173(unsigned short deg, Pos2_0048a490* p);

__int64 _alldiv(__int64 a, __int64 b);
__int64 _allmul(__int64 a, __int64 b);
__int64 _allshr(__int64 a, int b);
__int64 _allshl(__int64 a, int b);

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

// FUNCTION: 0x48a490
void __stdcall FUN_0048a490(Unit_0048a490* u)
{
    MapInfo_0048a490* m = g_game->maps[u->map];
    MapRow_0048a490* row = m->rows + m->count;
    if (m->count >= 0) {
        Pos2_0048a490 t;
        Pos2_0048a490 pts[4];
        Corner_0048a490 hs[4];
        int junk;
        int k = 0;
        do {
            MapVertex_0048a490* v = m->verts + row->ids[k];
            t.x = v->x;
            pts[k].x = v->x;
            t.z = v->z;
            pts[k].z = v->z;
            FUN_004b7173(u->aim, &t);
            int wx = (short)((t.x + u->posx) >> 16);
            int hz = (short)((u->posz - t.z) >> 16);
            t.z = hz;
            int fz = wx & 0xf;
            int fx = hz & 0xf;
            int gx = (unsigned)wx >> 4;
            int gz = (unsigned)hz >> 4;
            int gw = g_game->gridW;
            if (gx >= gw - 1)
                return;
            if (gz >= g_game->gridH - 1)
                return;
            unsigned char* tb = (unsigned char*)g_game->unknown_14280 + (gz * gw + gx) * 13;
            unsigned char* tb1 = tb + gw * 13;
            int b0 = tb[4];
            int b1 = tb1[4];
            int c0 = tb[0x11];
            int c1 = tb1[0x11];
            int H0 = b0 + (((c0 - b0) * fz) / 16 >> 4);
            int H1 = b1 + (((c1 - b1) * fz) / 16 >> 4);
            hs[k].wx = wx;
            int hres;
            if ((u->type->flags & 0x1000) && (u->flags & 0x10000000)
                && !(u->flags & 0x4000)) {
                int H = H0 + (((H1 - H0) * fx) / 16 >> 4);
                hs[k].h = H;
                hs[k].h = max(H, g_game->seaLevel);
                int p = (((FUN_004b6340() & 0x1f) + k * 8) << 11) + (short)u->fix_lo;
                int q = u->type->sight;
                if (q < 0)
                    q = -q;
                q = q / 2;
                int s = min((int)u->owner->sight, q);
                __int64 l = _alldiv(_allshl(s, 0x10), q);
                l = _allmul(l, 2);
                l = _allshr(l, 0x10);
                int mm = 2 - (int)l;
                int n = min(g_game->frame - u->owner->age, 60);
                mm -= (unsigned)(mm * n) / 60;
                hres = FUN_004b7123(p, mm) + hs[k].h;
            } else {
                hres = H0 + (((H1 - H0) * fx) / 16 >> 4);
            }
            hs[k].h = hres;
            hs[k].spare = junk;
        } while (++k < 4);
        int h0 = hs[0].h;
        int h1 = hs[1].h;
        int h2 = hs[2].h;
        int h3 = hs[3].h;
        u->roll = ((h0 + h1) / 2 / 2 + (h2 + h3) / 2 / 2) / 2 / 2;
        u->pitch = FUN_004b715a((h2 + h3) / 2 / 2 - (h0 + h1) / 2 / 2,
                                ((pts[0].z - pts[3].z) / 2) >> 16);
        u->hdg = FUN_004b715a(h0 - h1, ((pts[1].x - pts[1].z) / 2) >> 16);
    }
}
