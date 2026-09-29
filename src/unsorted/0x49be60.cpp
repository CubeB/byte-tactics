// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL. 0x49be60 (2272 bytes) is the projectile render pass: it walks the
// 300-entry projectile array (count g_game+0x141f3, base g_game+0x141f7,
// stride 0x6b) and draws every live projectile (p+0x60 counter == 0) that is
// visible to the local player, dispatching on the shot-type byte type+0x10c
// (0..7). The visibility test mirrors 0x481930/0x482c20: with viewFlags bit 2
// set it indexes the per-player line-of-sight grid (PlayerInfo+0x7c ptr,
// +0x80 width, +0x84 height), otherwise it calls FUN_00408090.
//
// This first pass transcribes the full control flow, the branch structure and
// every field offset used. What is NOT right yet and is the next step:
//   - register allocation everywhere (the original keeps g_game in edi and
//     frame0 in [esp+0x14]; MSVC gives different roles here);
//   - the exact local layout: the original's frame is 0x68 with cur/prev
//     Vec3s at +0x48/+0x54, random offsets at +0x60/+0x64/+0x68 and the
//     kind-1 rect at +0x30; the sp Vec3 of kinds 1/3/4/6 sits at +0x48;
//   - kind 7 (the multi-segment trail) is approximated: its inner loop draws
//     a segment from the previous point to the current point plus a per-axis
//     rand()*11/0x8000-5 jitter (rand() returns 0..0x7fff), (short)(nSeg>>16)
//     segments, two outer passes.
// Every offset and callee argument order below is believed correct.
//
// Layout facts:
//   Projectile stride 0x6b. type at +0x0; pos Vec3 (16.16) at +0x4; start
//   Vec3 at +0x10; short field_34/+0x36/+0x38 (the kind 1/6 sprite rect);
//   int field_42 (spawn time) +0x42; int field_46 +0x46; short field_5e +0x5e
//   (sprite half height); short counter +0x60; short field_64 +0x64;
//   ushort flags +0x69.
//   Type: ptr field_74 +0x74 (sprite object, its +0x30 is a second frame);
//   ushort field_e6 +0xe6 (kind 5 animation divisor); byte field_10c +0x10c
//   shot kind; byte field_10d +0x10d palette index; byte field_10e +0x10e
//   second palette index; dword flags +0x111 (bit 21 tested in kind 1).
//   Game: palette bytes +0xdcb; projectiles +0x141f3/+0x141f7; viewFlags byte
//   +0x14281 bit 2; shorts scrollX +0x1431f, scrollY +0x14323; gaf pointers
//   +0x147bb/+0x147bf/+0x147c3/+0x147c7/+0x147cb (kind 4), +0x147f3 (kind 5),
//   +0x1480f (the shared sprite, frame 0); ptr +0x1ab9b (kind 2);
//   localPlayer byte +0x2a43; player array base +0x1b63 stride 0x14b;
//   region +0x37e27 (kind 2); time int +0x38a47.
#include <math.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Vec3_0049be60 {
    int x;
    int y;
    int z;
};

struct Type_0049be60 {
    char unknown_0[0x74];
    void* field_74;                    // +0x74
    char unknown_78[0xe6 - 0x78];
    unsigned short field_e6;           // +0xe6
    char unknown_e8[0x10c - 0xe8];
    unsigned char field_10c;           // +0x10c
    unsigned char field_10d;           // +0x10d
    unsigned char field_10e;           // +0x10e
    char unknown_10f[0x111 - 0x10f];
    unsigned int flags;                // +0x111
};

struct Sprite_0049be60 {
    char unknown_0[0x30];
    void* field_30;                    // +0x30
};

struct Proj_0049be60 {
    Type_0049be60* type;               // +0x0
    Vec3_0049be60 pos;                 // +0x4
    Vec3_0049be60 start;               // +0x10
    char unknown_1c[0x34 - 0x1c];
    short field_34;                    // +0x34
    short field_36;                    // +0x36
    short field_38;                    // +0x38
    char unknown_3a[0x42 - 0x3a];
    int field_42;                      // +0x42
    int field_46;                      // +0x46
    char unknown_4a[0x5e - 0x4a];
    short field_5e;                    // +0x5e
    short counter;                     // +0x60
    char unknown_62[0x64 - 0x62];
    short field_64;                    // +0x64
    char unknown_66[0x69 - 0x66];
    unsigned short flags;              // +0x69
};

struct PlayerInfo_0049be60 {
    char unknown_0[0x7c];
    unsigned char* los;                // +0x7c
    int losWidth;                      // +0x80
    int losHeight;                     // +0x84
};

struct Game_0049be60 {
    char unknown_0[0xdcb];
    unsigned char palette[0x2a];       // +0xdcb
    char unknown_df5[0x2a43 - 0xdf5];
    unsigned char localPlayer;         // +0x2a43
    char unknown_2a44[0x141f3 - 0x2a44];
    int projectileCount;               // +0x141f3
    Proj_0049be60* projectiles;        // +0x141f7
    char unknown_141fb[0x14281 - 0x141fb];
    unsigned char viewFlags;           // +0x14281
    char unknown_14282[0x1431f - 0x14282];
    short scrollX;                     // +0x1431f
    short scrollY;                     // +0x14323
    char unknown_14325[0x147bb - 0x14325];
    void* gaf_147bb;                   // +0x147bb
    void* gaf_147bf;                   // +0x147bf
    void* gaf_147c3;                   // +0x147c3
    void* gaf_147c7;                   // +0x147c7
    void* gaf_147cb;                   // +0x147cb
    char unknown_147cf[0x147f3 - 0x147cf];
    void* gaf_147f3;                   // +0x147f3
    char unknown_147f7[0x1480f - 0x147f7];
    void* gaf_1480f;                   // +0x1480f
    char unknown_14813[0x1ab9b - 0x14813];
    void* field_1ab9b;                 // +0x1ab9b
    char unknown_1ab9f[0x37e27 - 0x1ab9f];
    char field_37e27[0x38a47 - 0x37e27];
    int time;                          // +0x38a47
};
#pragma pack(pop)

extern Game_0049be60* g_game;

void* __stdcall FUN_004b7f30(void* gaf, int frame);
void __stdcall FUN_004b7f90(void* dest, void* src, int x, int y);
void __stdcall FUN_004b8500(void* dest, void* src, int x, int y);
void __stdcall FUN_0046bae0(void* dest, Vec3_0049be60* pos, void* sprite, void* rect);
int __stdcall FUN_004b6720(void* region, int x, int y);
void __stdcall FUN_004b9360(void* dest, void* src, int x, int y);
void __stdcall FUN_004be950(void* dest, int x1, int y1, int x2, int y2, unsigned int color);
int __stdcall FUN_004b7f60(void* gaf);
int __stdcall FUN_00408090(PlayerInfo_0049be60* pi, Vec3_0049be60* pos);

static int Abs_0049be60(int v)
{
    return (v ^ (v >> 31)) - (v >> 31);
}

// FUNCTION: 0x49be60
void __stdcall FUN_0049be60(void* surface)
{
    Game_0049be60* g = g_game;
    int time = g->time;
    void* frame0 = FUN_004b7f30(g->gaf_1480f, 0);
    int index = 0;
    if (g->projectileCount <= 0)
        return;
    int offset = 0;
    do {
        Proj_0049be60* p = (Proj_0049be60*)((char*)g->projectiles + offset);
        if (p->counter == 0) {
            unsigned char player = g->localPlayer;
            char* pb = (char*)g + 0x1b63 + 0x14b * player;
            int visible;
            if ((g->viewFlags & 2) == 2) {
                int col = (int)*(short*)((char*)&p->pos + 2) >> 5;
                int row = ((int)*(short*)((char*)&p->pos + 10)
                           - ((int)*(short*)((char*)&p->pos + 6) >> 1)) >> 5;
                PlayerInfo_0049be60* pi = (PlayerInfo_0049be60*)pb;
                if ((unsigned)col >= (unsigned)pi->losWidth
                    || (unsigned)row >= (unsigned)pi->losHeight)
                    visible = 0;
                else
                    visible = pi->los[row * pi->losWidth + col] != 0;
            } else {
                visible = FUN_00408090((PlayerInfo_0049be60*)pb, &p->pos);
            }
            if (visible) {
                Type_0049be60* type = p->type;
                if (type->field_10c == 0) {
                    unsigned int color1 = g->palette[type->field_10d];
                    unsigned int color2 = g->palette[type->field_10e];
                    int x1 = (int)*(short*)((char*)&p->pos + 2) - g->scrollX + 0x80;
                    int y1 = ((int)*(short*)((char*)&p->pos + 10)
                              - ((int)*(short*)((char*)&p->pos + 6) >> 1))
                             - g->scrollY + 0x20;
                    int x2 = (int)*(short*)((char*)&p->start + 2) - g->scrollX + 0x80;
                    int y2 = ((int)*(short*)((char*)&p->start + 10)
                              - ((int)*(short*)((char*)&p->start + 6) >> 1))
                             - g->scrollY + 0x20;
                    if (type->field_10e == 0) {
                        FUN_004be950(surface, x1, y1, x2, y2, color1);
                    } else if (Abs_0049be60(x1 - x2) > Abs_0049be60(y1 - y2)) {
                        if (x1 > x2) {
                            int t = x1; x1 = x2; x2 = t;
                            t = y1; y1 = y2; y2 = t;
                        }
                        FUN_004be950(surface, x1, y1 - 1, x2, y2 - 1, color2);
                        FUN_004be950(surface, x1, y1, x2, y2, color1);
                    } else {
                        if (y1 > y2) {
                            int t = x1; x1 = x2; x2 = t;
                            t = y1; y1 = y2; y2 = t;
                        }
                        FUN_004be950(surface, x1 - 1, y1, x2 + 1, y2, color2);
                        FUN_004be950(surface, x1, y1, x2, y2, color1);
                    }
                } else if (type->field_10c == 1) {
                    Vec3_0049be60 sp;
                    sp.x = p->pos.x - (g->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g->scrollY << 16);
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    int sy = (int)*(short*)((char*)&sp + 6) - ((int)p->field_5e >> 1) + 0x20;
                    FUN_004b8500(surface, frame0, sx, sy);
                    short rect[4];
                    rect[0] = p->field_34;
                    rect[1] = (short)(p->field_36 + 0x8000);
                    rect[2] = (short)(p->field_38 + 0x8000);
                    rect[3] = 0;
                    FUN_0046bae0(surface, &sp, type->field_74, rect);
                    Sprite_0049be60* s = (Sprite_0049be60*)type->field_74;
                    if (s->field_30 != 0 && time < p->field_46) {
                        if ((type->flags >> 0x15) & 1) {
                            rect[0] = p->field_64;
                            FUN_0046bae0(surface, &sp, s->field_30, rect);
                        } else {
                            FUN_0046bae0(surface, &sp, s->field_30, rect);
                        }
                    }
                } else if (type->field_10c == 2) {
                    int sx = (int)*(short*)((char*)&p->pos + 2) - g->scrollX + 0x80;
                    int sy = ((int)*(short*)((char*)&p->pos + 10)
                              - ((int)*(short*)((char*)&p->pos + 6) >> 1))
                             - g->scrollY + 0x20;
                    if (FUN_004b6720((void*)g->field_37e27, sx, sy) == 0)
                        return;
                    FUN_004b9360(surface, g->field_1ab9b, sx, sy);
                } else if (type->field_10c == 3) {
                    Vec3_0049be60 sp;
                    sp.x = p->pos.x - (g->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g->scrollY << 16);
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    int sy = (int)*(short*)((char*)&sp + 6) - ((int)p->field_5e >> 1) + 0x20;
                    FUN_004b8500(surface, frame0, sx, sy);
                    short clip[4];
                    FUN_0046bae0(surface, &sp, type->field_74, clip);
                } else if (type->field_10c == 4) {
                    if (type->field_10d < 0xff) {
                        Vec3_0049be60 sp;
                        sp.x = p->pos.x - (g->scrollX << 16);
                        sp.y = p->pos.y;
                        sp.z = p->pos.z - (g->scrollY << 16);
                        int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                        int sy = ((int)*(short*)((char*)&sp + 10)
                                  - ((int)*(short*)((char*)&sp + 6) >> 1)) + 0x20;
                        FUN_004b8500(surface, frame0, sx, sy);
                        void* gaf = 0;
                        switch (type->field_10d) {
                        case 0: gaf = g->gaf_147bb; break;
                        case 1: gaf = g->gaf_147bf; break;
                        case 2: gaf = g->gaf_147c3; break;
                        case 3: gaf = g->gaf_147c7; break;
                        case 4: gaf = g->gaf_147cb; break;
                        }
                        if (gaf != 0) {
                            int n = *(unsigned short*)gaf;
                            void* fs = FUN_004b7f30(gaf, (time - p->field_42) % n);
                            FUN_004b7f90(surface, fs, sx, sy);
                        }
                    }
                } else if (type->field_10c == 5) {
                    int sx = (int)*(short*)((char*)&p->pos + 2) - g->scrollX + 0x80;
                    int sy = ((int)*(short*)((char*)&p->pos + 10)
                              - ((int)*(short*)((char*)&p->pos + 6) >> 1))
                             - g->scrollY + 0x20;
                    void* gaf = g->gaf_147f3;
                    int n = FUN_004b7f60(gaf);
                    int fr = n - ((p->field_46 - time) * n) / (int)type->field_e6;
                    if (fr >= 0 && fr < n) {
                        void* fs = FUN_004b7f30(gaf, fr);
                        FUN_004b8500(surface, fs, sx, sy);
                    }
                } else if (type->field_10c == 6) {
                    Vec3_0049be60 sp;
                    sp.x = p->pos.x - (g->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g->scrollY << 16);
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    int sy = (int)*(short*)((char*)&sp + 6) - ((int)p->field_5e >> 1) + 0x20;
                    FUN_004b8500(surface, frame0, sx, sy);
                    FUN_0046bae0(surface, &sp, type->field_74, &p->field_34);
                } else if (type->field_10c == 7) {
                    unsigned int color = g->palette[type->field_10d];
                    int dx = p->pos.x - p->start.x;
                    int dy = p->pos.y - p->start.y;
                    int dz = p->pos.z - p->start.z;
                    int d = (int)sqrt((double)(dx * dx + dy * dy + dz * dz));
                    int nSeg = (int)(((__int64)d << 16) / 0x50000);
                    if (nSeg != 0) {
                        int stepX = (int)(((__int64)dx << 16) / (__int64)nSeg);
                        int stepY = (int)(((__int64)dy << 16) / (__int64)nSeg);
                        int stepZ = (int)(((__int64)dz << 16) / (__int64)nSeg);
                        int outer = 2;
                        do {
                            Vec3_0049be60 cur = p->start;
                            short n = (short)(nSeg >> 16);
                            if (n > 0) {
                                int i = n;
                                do {
                                    Vec3_0049be60 prev = cur;
                                    cur.x += stepX;
                                    cur.y += stepY;
                                    cur.z += stepZ;
                                    int ox = (rand() * 11) / 0x8000 - 5;
                                    int oy = (rand() * 11) / 0x8000 - 5;
                                    int oz = (rand() * 11) / 0x8000 - 5;
                                    FUN_004be950(surface,
                                        (int)*(short*)((char*)&prev + 2) - g->scrollX + 0x80,
                                        ((int)*(short*)((char*)&prev + 10)
                                         - ((int)*(short*)((char*)&prev + 6) >> 1))
                                        - g->scrollY + 0x20,
                                        (int)*(short*)((char*)&cur + 2) + ox - g->scrollX + 0x80,
                                        (((int)*(short*)((char*)&cur + 10) + oz)
                                         - (((int)*(short*)((char*)&cur + 6) + oy) >> 1))
                                        - g->scrollY + 0x20,
                                        color);
                                    i--;
                                } while (i != 0);
                            }
                            outer--;
                        } while (outer != 0);
                    }
                }
            }
        }
        index++;
        offset += 0x6b;
    } while (index < g->projectileCount);
}
