// Decompiled by deepseek-v4.1-flash. Names are provisional.
// One tick of a projectile (slot of DAT_00511df0, see 0x421170/0x420f30).
// obj is the 0x30-byte header FUN_00481140 builds; inner (+0x2c) holds the
// position at +0x16 and the three rotation shorts at +0x10. The timer at
// +0x20 counts down. Above sea level the projectile moves and FUN_00485140
// (map height under the point) decides whether it hit the ground: on a hit it
// damps the velocity and, when the ground is above +0x20000, stops.
#pragma pack(push, 1)

struct Inner_004213b0 {
    char unknown_0[0x10];
    short f10;                         // +0x10
    short f12;                         // +0x12
    short f14;                         // +0x14
    int f16;                           // +0x16
    int f1a;                           // +0x1a
    int f1e;                           // +0x1e
    char unknown_22[0xa];
};

struct Obj_004213b0 {
    void* p0;                          // +0x00
    int p4;                            // +0x04
    short s8;                          // +0x08
    short sA;                          // +0x0a
    short sC;                          // +0x0c
    short sE;                          // +0x0e
    short s10;                         // +0x10
    short s12;                         // +0x12
    int f14;                           // +0x14
    int f18;                           // +0x18
    int f1c;                           // +0x1c
    int f20;                           // +0x20
    int f24;                           // +0x24
    unsigned int b0 : 1;               // +0x28 bit 0
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;               // +0x28 bit 3
    unsigned int b4 : 1;               // +0x28 bit 4
    unsigned int rest : 27;
    Inner_004213b0* inner;             // +0x2c
};

struct Net_004213b0 {
    char unknown_0[0xd44];
    int f_d44;                         // +0xd44
    int f_d48;                         // +0xd48
};

struct Game_004213b0 {
    char unknown_0[0x14263];
    int gravity;                       // +0x14263
    char unknown_14267[0x1427f - 0x14267];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x147eb - 0x14280];
    void* src1;                        // +0x147eb
    void* src2;                        // +0x147ef
    char unknown_147f3[0x147f7 - 0x147f3];
    void* src3;                        // +0x147f7
    char unknown_147fb[0x391e9 - 0x147fb];
    Net_004213b0* net;                 // +0x391e9
};

struct Pos_004213b0 {
    int x;
    int y;
    int z;
};
#pragma pack(pop)

extern Game_004213b0* g_game;

int __stdcall FUN_00485140(Pos_004213b0* pos);
void __stdcall FUN_00420a30(Pos_004213b0* pos, void* src, int index, int flag);

// FUNCTION: 0x4213b0
int __stdcall FUN_004213b0(Obj_004213b0* obj)
{
    int n = obj->f20;
    Inner_004213b0* inner = obj->inner;
    obj->f20 = n - 1;
    if (n == 0)
        return 0;

    Pos_004213b0 pos;
    if (inner->f1a <= (int)((unsigned)g_game->seaLevel << 16)) {
        if (obj->b4 && g_game->net->f_d48 == 0) {
            void* src;
            if (g_game->net->f_d44 != 0)
                src = g_game->src2;
            else
                src = g_game->src1;
            pos.x = inner->f16;
            pos.y = inner->f1a;
            pos.z = inner->f1e;
            FUN_00420a30(&pos, src, -1, 1);
            return 0;
        }
        return 0;
    }

    int vy = obj->f18;
    pos.x = inner->f16;
    pos.y = inner->f1a;
    pos.z = inner->f1e;
    int limit = FUN_00485140(&pos) << 16;
    if (vy + inner->f1a <= limit) {
        int ny = -(vy >> 1);
        int nx = obj->f14 >> 1;
        int nz = obj->f1c >> 1;
        obj->f18 = ny;
        obj->f14 = nx;
        obj->f1c = nz;
        if (ny < 0x20000) {
            if (obj->b4) {
                pos.x = inner->f16;
                pos.y = inner->f1a;
                pos.z = inner->f1e;
                FUN_00420a30(&pos, g_game->src3, 0, 0);
            }
            return 0;
        }
    }

    inner->f16 += obj->f14;
    inner->f1a += obj->f18;
    inner->f1e += obj->f1c;
    inner->f10 += obj->sC;
    inner->f12 += obj->s10;
    inner->f14 += obj->s8;
    if (obj->b3)
        obj->f18 -= g_game->gravity;
    return 1;
}
