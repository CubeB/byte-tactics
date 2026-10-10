// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by claude-sonnet-5-5, finished by claude-opus-5-5. Names are provisional.
// The projectile render pass: it walks the 300-entry projectile array (count g_game+0x141f3, base
// g_game+0x141f7, stride 0x6b) and draws every live projectile (counter at
// +0x60 is 0) that the local player can see, by the shot kind at type+0x10c
// (0..7). The visibility test mirrors 0x481930/0x482c20: with map-flags bit 2
// set it reads the player's explored-cell grid (PlayerInfo+0x7c data, +0x80
// width, +0x84 height), otherwise it calls IsPointVisible.
//  - Kind 0's two line arms each make both DrawLine calls (colour2, then
//    colour1).
//  - Kinds 1, 3 and 6 pass a 3-short angle struct (the projectile's +0x34);
//    kind 3 passes its own `rot3`, which is never written (docs/bugs.md).
//  - Kind 7's dx/dy/dz are one Vec3 `d`, and the three 16.16 steps are written
//    back into it; the 64-bit divisor is its own `__int64 n64`. The jitter is
//    64-bit, `(rand() * 11) / 0x8000 - 5`, added to the high shorts of pt.
//  - Kind 1's rect is copied from +0x34 as one dword, then rect[1] += 0x8000.
//  - The LOS test goes through small inline members (ByteMap::Get,
//    MapSize::Contains) with an explicit visible = 1 / else 0.
//  - Kind 4's frame divisor is `(time - p->spawnTick) % *(unsigned short*)gaf`
//    (signed); its switch is a real jump table, cases 0..4.
// Layout facts:
//   Projectile stride 0x6b. type at +0x0; pos Vec3 (16.16) at +0x4; start
//   Vec3 at +0x10; shorts +0x34/+0x36/+0x38 (angles); int spawnTick (spawn
//   time) +0x42; int time +0x46; short groundHeightAvg +0x5e; short counter
//   +0x60; short propellerSpin +0x64; ushort flags +0x69.
//   Type: ptr field_74 +0x74 (sprite object, its +0x30 is a second frame);
//   ushort field_e6 +0xe6 (kind 5 animation divisor); byte field_10c +0x10c
//   shot kind; byte field_10d +0x10d palette index; byte field_10e +0x10e
//   second palette index; dword flags +0x111 (bit 21 tested in kind 1).
//   Game: palette bytes +0xdcb; projectiles +0x141f3/+0x141f7; map flags word
//   +0x14281 bit 2; ints scrollX +0x1431f, scrollY +0x14323; gaf pointers
//   +0x147bb/+0x147bf/+0x147c3/+0x147c7/+0x147cb (kind 4), +0x147f3 (kind 5),
//   +0x1480f (the shared sprite, frame 0); ptr +0x1ab9b (kind 2);
//   playerIndex byte +0x2a43; player array base +0x1b63 stride 0x14b;
//   region +0x37e27 (kind 2); gameTick int +0x38a47.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
// <minmax.h> is needed: kind 0 then computes abs(x1 - x2) before abs(y1 - y2).
#include <minmax.h>

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
    unsigned int flags_0 : 21;         // +0x111
    unsigned int flag_21 : 1;
    unsigned int flags_2 : 10;
};

struct Sprite_0049be60 {
    char unknown_0[0x30];
    void* child;                       // +0x30
};

struct Angles_0049be60 {
    short x;
    short y;
    short z;
};

struct Proj_0049be60 {
    Type_0049be60* type;               // +0x0
    Vec3_0049be60 pos;                 // +0x4
    Vec3_0049be60 start;               // +0x10
    char unknown_1c[0x34 - 0x1c];
    Angles_0049be60 angles;            // +0x34
    char unknown_3a[0x42 - 0x3a];
    int spawnTick;                     // +0x42
    int time;                          // +0x46
    char unknown_4a[0x5e - 0x4a];
    short groundHeightAvg;             // +0x5e
    short counter;                     // +0x60
    char unknown_62[0x64 - 0x62];
    short propellerSpin;               // +0x64
    char unknown_66[0x69 - 0x66];
    unsigned short flags;              // +0x69
};

struct MapSize_0049be60 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_0049be60 {
    unsigned char* data;               // +0x0
    MapSize_0049be60 size;             // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct PlayerInfo_0049be60 {
    char unknown_0[0x7c];
    ByteMap_0049be60 explored;         // +0x7c
};

struct Game {
    char unknown_0[0xdcb];
    unsigned char palette[0x2a];       // +0xdcb
    char unknown_df5[0x2a43 - 0xdf5];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x141f3 - 0x2a44];
    int projectileCount;               // +0x141f3
    Proj_0049be60* projectiles;        // +0x141f7
    char unknown_141fb[0x14281 - 0x141fb];
    unsigned short mapFlags;           // +0x14281
    char unknown_14283[0x1431f - 0x14283];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x147bb - 0x14327];
    void* cannonShellSeq;              // +0x147bb
    void* plasmaSmSeq;                 // +0x147bf
    void* plasmaMdSeq;                 // +0x147c3
    void* ultraShellSeq;               // +0x147c7
    void* plasmaSmSeq2;                // +0x147cb
    char unknown_147cf[0x147f3 - 0x147cf];
    void* flameStreamSeq;              // +0x147f3
    char unknown_147f7[0x1480f - 0x147f7];
    void* shadowSeq;                   // +0x1480f
    char unknown_14813[0x1ab9b - 0x14813];
    void* explosionLensFrame;          // +0x1ab9b
    char unknown_1ab9f[0x37e27 - 0x1ab9f];
    char field_37e27[0x38a47 - 0x37e27];
    int gameTick;                      // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

void* __stdcall GetGafFrame(void* gaf, int frame);
void __stdcall DrawFrame(void* dest, void* src, int x, int y);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);
void __stdcall DrawModel3doProjected(void* dest, Vec3_0049be60* pos, void* sprite, void* rect);
int __stdcall PointInRect(void* region, int x, int y);
void __stdcall DrawLens(void* dest, void* src, int x, int y);
void __stdcall DrawLine(void* dest, int x1, int y1, int x2, int y2, unsigned int color);
int __stdcall GetGafFrameCount(void* gaf);
int __stdcall IsPointVisible(PlayerInfo_0049be60* pi, Vec3_0049be60* pos);


// AllocProjectile (matched in its own file), which comes before this function in
// the original file. It must be defined above: with no earlier function the
// two kind 0 line arms stop sharing their tail.
Proj_0049be60* AllocProjectile()
{
    Proj_0049be60* p = 0;
    if (g_game->projectileCount < 300) {
        p = &g_game->projectiles[g_game->projectileCount++];
        p->flags &= ~2;
        *(int*)((char*)p + 0x4e) = 0;
    }
    return p;
}

// Stays in its own file: its palette and scroll register plan follows this
// file's symbol ids, which the joined weapons.cpp moves.
// FUNCTION: 0x49be60
void __stdcall DrawProjectiles(void* surface)
{
    Type_0049be60* type;
    int fr;
    Vec3_0049be60 sp;
    Vec3_0049be60 pt;
    Vec3_0049be60 prev;
    Vec3_0049be60 d;
    __int64 n64;
    int time = g_game->gameTick;
    void* frame0 = GetGafFrame(g_game->shadowSeq, 0);
    int index = 0;
    int visible;
    if (g_game->projectileCount <= 0)
        return;
    int offset = 0;
    while (1) {
        Proj_0049be60* p = (Proj_0049be60*)((char*)g_game->projectiles + offset);
        if (p->counter == 0) {
            unsigned char player = g_game->playerIndex;
            char* pb = (char*)g_game + 0x1b63 + 0x14b * player;
            Vec3_0049be60* pos = &p->pos;
            if ((g_game->mapFlags & 2) == 2) {
                int col = (int)*(short*)((char*)pos + 2) >> 5;
                PlayerInfo_0049be60* pi = (PlayerInfo_0049be60*)pb;
                int row = ((int)*(short*)((char*)pos + 10)
                           - ((int)*(short*)((char*)pos + 6) >> 1)) >> 5;
                if (pi->explored.size.Contains(col, row) && pi->explored.Get(col, row))
                    visible = 1;
                else
                    visible = 0;
            } else {
                visible = IsPointVisible((PlayerInfo_0049be60*)pb, pos);
            }
            if (visible) {
                type = p->type;
                if (type->field_10c == 0) {
                    unsigned int color1 = g_game->palette[type->field_10d];
                    unsigned int color2 = g_game->palette[type->field_10e];
                    int x1;
                    int y1;
                    int x2;
                    int y2;
                    x1 = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    y1 = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    x2 = (int)*(short*)((char*)&p->start + 2) - (short)g_game->scrollX + 0x80;
                    y2 = ((int)*(short*)((char*)&p->start + 10)
                              - ((int)*(short*)((char*)&p->start + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    if (type->field_10e != 0) {
                        // Both calls stay in each arm: one shared colour1 call swaps the colour slots.
                        if (abs(x1 - x2) > abs(y1 - y2)) {
                            if (x1 > x2) {
                                int t = x1; x1 = x2; x2 = t;
                                t = y1; y1 = y2; y2 = t;
                            }
                            DrawLine(surface, x1, y1 - 1, x2, y2 - 1, color2);
                            DrawLine(surface, x1, y1, x2, y2, color1);
                        } else {
                            if (y1 > y2) {
                                int t = x1; x1 = x2; x2 = t;
                                t = y1; y1 = y2; y2 = t;
                            }
                            DrawLine(surface, x1 - 1, y1, x2 + 1, y2, color2);
                            DrawLine(surface, x1, y1, x2, y2, color1);
                        }
                    } else {
                        DrawLine(surface, x1, y1, x2, y2, color1);
                    }
                } else if (type->field_10c == 1) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sx = 0x80 + (int)*(short*)((char*)&sp + 2);
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->groundHeightAvg >> 1) + 0x20;
                    DrawFrameBlended(surface, frame0, sx, sy);
                    Angles_0049be60 rot = p->angles;
                    rot.y += 0x8000;
                    rot.z += 0x8000;
                    DrawModel3doProjected(surface, &sp, type->field_74, &rot);
                    Sprite_0049be60* s = (Sprite_0049be60*)type->field_74;
                    if (0 != s->child && p->time > time) {
                        if (type->flag_21) {
                            rot.x = p->propellerSpin;
                            DrawModel3doProjected(surface, &sp, s->child, &rot);
                        } else {
                            DrawModel3doProjected(surface, &sp, s->child, &rot);
                        }
                    }
                } else if (type->field_10c == 2) {
                    int sx = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    int sy = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    if (PointInRect(g_game->field_37e27, sx, sy) == 0)
                        return;
                    DrawLens(surface, g_game->explosionLensFrame, sx, sy);
                } else if (type->field_10c == 3) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->groundHeightAvg >> 1) + 0x20;
                    DrawFrameBlended(surface, frame0, sx, sy);
                    Angles_0049be60 rot3;
                    DrawModel3doProjected(surface, &sp, type->field_74, &rot3);
                } else if (type->field_10c == 4) {
                    if (type->field_10d < 0xff) {
                        void* gaf = 0;
                        sp.x = pos->x - (g_game->scrollX << 16);
                        sp.y = p->pos.y;
                        sp.z = p->pos.z - (g_game->scrollY << 16);
                        DrawFrameBlended(surface, frame0,
                                     (int)*(short*)((char*)&sp + 2) + 0x80,
                                     (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->groundHeightAvg >> 1) + 0x20);
                        int sy = ((int)*(short*)((char*)&sp + 10)
                                  - ((int)*(short*)((char*)&sp + 6) >> 1)) + 0x20;
                        int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                        switch (type->field_10d) {
                        case 0: gaf = g_game->cannonShellSeq; break;
                        case 1: gaf = g_game->plasmaSmSeq; break;
                        case 2: gaf = g_game->plasmaMdSeq; break;
                        case 3: gaf = g_game->ultraShellSeq; break;
                        case 4: gaf = g_game->plasmaSmSeq2; break;
                        }
                        if (gaf) {
                            int n = *(unsigned short*)gaf;
                            DrawFrame(surface, GetGafFrame(gaf, (time - p->spawnTick) % n), sx, sy);
                        }
                    }
                } else if (type->field_10c == 5) {
                    int sx = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    int sy = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    void* gaf = g_game->flameStreamSeq;
                    int n = GetGafFrameCount(gaf);
                    fr = n - ((p->time - time) * n) / (int)type->field_e6;
                    if (fr >= 0 && fr < n) {
                        DrawFrameBlended(surface, GetGafFrame(gaf, fr), sx, sy);
                    }
                } else if (type->field_10c == 6) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->groundHeightAvg >> 1) + 0x20;
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    DrawFrameBlended(surface, frame0, sx, sy);
                    DrawModel3doProjected(surface, &sp, type->field_74, &p->angles);
                } else if (type->field_10c == 7) {
                    unsigned int color = g_game->palette[type->field_10d];
                    Vec3_0049be60* start = &p->start;
                    d.x = pos->x - start->x;
                    d.y = pos->y - start->y;
                    d.z = pos->z - start->z;
                    union { int i; short s[2]; } nSeg;
                    nSeg.i = (int)(((__int64)((int)sqrt(d.x * (double)d.x + (double)d.y * d.y + (double)d.z * d.z)) << 16) / 0x50000);
                    if (0 != nSeg.i) {
                        n64 = nSeg.i;
                        d.x = (int)(((__int64)d.x << 16) / n64);
                        d.y = (int)(((__int64)d.y << 16) / n64);
                        d.z = (int)(((__int64)d.z << 16) / n64);
                        for (int outer = 0; outer < 2; outer++) {
                            pt = *start;
                            sp = *start;
                            {
                                short n = nSeg.s[1];
                                if (n > 0) {
                                    int i = n;
                                    do {
                                        // The unused z stays: it gives pt.z the extra reference
                                        // that sets the register assignment.
                                        int z = pt.z;  // unused; needed for the match
                                        prev = pt;
                                        sp.x += d.x;
                                        sp.y += d.y;
                                        sp.z += d.z;
                                        pt = sp;
                                        *(short*)((char*)&pt + 2) +=
                                            (short)((int)(((__int64)rand() * 11) / 0x8000) - 5);
                                        *(short*)((char*)&pt + 6) +=
                                            (short)((int)(((__int64)rand() * 11) / 0x8000) - 5);
                                        *(short*)((char*)&pt + 10) +=
                                            (short)((int)(((__int64)rand() * 11) / 0x8000) - 5);
                                        DrawLine(surface,
                                            (int)*(short*)(2 + (char*)&prev) - (short)g_game->scrollX + 0x80,
                                            ((int)*(short*)(10 + (char*)&prev)
                                             - ((int)*(short*)((char*)&prev + 6) >> 1))
                                            - (short)g_game->scrollY + 0x20,
                                            (int)*(short*)((char*)&pt + 2) - (short)g_game->scrollX + 0x80,
                                            0x20 + (((int)*(short*)((char*)&pt + 10)
                                             - ((int)*(short*)(6 + (char*)&pt) >> 1))
                                            - (short)g_game->scrollY),
                                            color);
                                        i = i - 1;
                                    } while (i != 0);
                                }
                            }
                        }
                    }
                }
            }
        }
        index = index + 1;
        offset += 0x6b;
        if (index >= g_game->projectileCount)
            break;
    }
}