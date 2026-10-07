// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.

#include <stdlib.h>

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

struct Hs_0048a490 {
    int wx;                             // +0x0
    int h;                              // +0x4
    int spare;                          // +0x8
};

struct Unit {
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

struct Game {
    char unknown_0[0x14233];
    int gridW;                          // +0x14233
    int gridH;                          // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    unsigned char* hmaps;               // +0x14287
    char unknown_1428b[0x14377 - 0x1428b];
    MapInfo_0048a490** maps;            // +0x14377
    char unknown_1437b[0x38a47 - 0x1437b];
    int frame;                          // +0x38a47
};

#pragma pack(pop)

extern Game* g_game;

unsigned int GetTicks();
int __cdecl FUN_004b7123(short a, int b);
int __cdecl FUN_004b715a(int x, int y);
void __cdecl FUN_004b7173(unsigned short deg, Pos2_0048a490* p);

#define max(a, b) (((a) > (b)) ? (a) : (b))

// Samples the ground under a unit at its four surrounding terrain vertices and
// stores the resulting pitch (0x68) and roll (0x70) on the unit, plus a heading
// (0x64) from the two side vertices. The 0x11/0x04 bytes of a heightmap tile are
// the two half heights of its edge pair, and the corner heights are bilinearly
// interpolated with a plain / 16. The sea-level block then adds a jitter that
// fades out over 60 frames.
// FUNCTION: 0x48a490
void __stdcall AlignUnitToGround(Unit* u)
{
    MapInfo_0048a490* m = g_game->maps[u->map];
    MapRow_0048a490* row = m->rows + m->count;
    if (m->count < 0)
        return;
    {
        // gw and c0 are declared apart from t, pts, hs and k.
        int gw, c0;
        Pos2_0048a490 t;
        Pos2_0048a490 pts[4];
        Hs_0048a490 hs[4];
        int k;
        // A for loop: a do/while is rotated the other way.
        for (k = 0; k < 4; k++) {
            MapVertex_0048a490* v = m->verts + row->ids[k];
            int vx = v->x;
            // Chained: keeps the store order and the corner-array register.
            pts[k].x = t.x = vx;
            int vz = v->z;
            t.z = vz;
            pts[k].z = vz;
            FUN_004b7173(u->aim, &t);
            // wx before hz, fx before fz, and gz declared before fz.
            int wx = (short)((t.x + u->posx) >> 16);
            int hz = (short)((u->posz - t.z) >> 16);
            int fx = wx & 0xf;
            unsigned gz = (unsigned)hz >> 4;
            int fz = hz & 0xf;
            gw = g_game->gridW;
            unsigned gx = (unsigned)wx >> 4;
            if (gx >= gw - 1)
                return;
            if (gz >= g_game->gridH - 1)
                return;
            unsigned char* tb = g_game->hmaps + (gz * gw + gx) * 13;
            int b0 = tb[4];
            // gridW read again, not gw: keeps the allocation.
            unsigned char* tb1 = tb + g_game->gridW * 13;
            int b1 = tb1[4];
            c0 = tb[0x11];
            int c1 = tb1[0x11];
            int H0 = b0 + ((c0 - b0) * fx) / 16;
            int H1 = b1 + ((c1 - b1) * fx) / 16;
            hs[k].wx = wx;
            if ((u->type->flags & 0x1000) && (u->flags & 0x10000000)
                && !(u->flags & 0x4000)) {
                // Stored through the array, then read back into H.
                hs[k].h = H0 + ((H1 - H0) * fz) / 16;
                int H = hs[k].h;
                int sea = g_game->seaLevel;
                hs[k].h = max(H, sea);
                // short p and `* 2048`, not `<< 11`: keeps the 0x1f mask and 16-bit add.
                short p = (short)(((GetTicks() & 0x1f) + k * 8) * 2048 + u->fix_lo);
                int s = u->type->sight / 2;
                // Owner kept in a local across the 64-bit helper calls.
                PlayerRec_0048a490* o = u->owner;
                int q = o->sight;
                if (q >= s)
                    q = s;
                // The 64-bit part is one expression, not an __int64 local.
                int w = (int)((((__int64)q << 16) / s));
                int mm = 2 - (int)((((__int64)w * 2) >> 16));
                unsigned int n = g_game->frame - o->age;
                // Clamp inline as a ternary, not a separate if.
                mm -= (unsigned int)(mm * (n < 60 ? n : 60)) / 60;
                hs[k].h = FUN_004b7123(p, mm) + hs[k].h;
            } else {
                hs[k].h = H0 + ((H1 - H0) * fz) / 16;
            }
            hs[k].spare = hz;
        }
        // hs[1] read before hs[0]; roll and pitch use (h0+h1)/2, not /2/2.
        int h1 = hs[1].h;
        int h0 = hs[0].h;
        int h2 = hs[2].h;
        int h3 = hs[3].h;
        int a = (h0 + h1) / 2;
        int b = (h2 + h3) / 2;
        u->roll = (a + b) / 2;
        u->pitch = FUN_004b715a(b - a, (short)(abs(pts[0].z - pts[3].z) >> 16));
        u->hdg = FUN_004b715a(h0 - h1, (short)(abs(pts[0].x - pts[1].x) >> 16));
    }
}
