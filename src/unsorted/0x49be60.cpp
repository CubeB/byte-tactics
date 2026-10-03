// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by claude-sonnet-5-5, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, #4983, 2026-10-03; was 87.5%). The projectile render
// pass: it walks the 300-entry projectile array (count g_game+0x141f3, base
// g_game+0x141f7, stride 0x6b) and draws every live projectile (counter at
// +0x60 is 0) that the local player can see, by the shot kind at type+0x10c
// (0..7). The visibility test mirrors 0x481930/0x482c20: with viewFlags bit 2
// set it reads the player's explored-cell grid (PlayerInfo+0x7c data, +0x80
// width, +0x84 height), otherwise it calls FUN_00408090.
// What decided the last 10 points, each measured with check.py:
//  - FUN_0049b6e0 is defined above this function. With no earlier function in
//    the file, MSVC never cross-jumps kind 0's two line arms (the y-major arm
//    gets its own copy after the loop, 2308 bytes); with any function before
//    it, the arms share their tail exactly as in the original (87.5% to
//    95.9%). 0x49b6e0 and 0x49b720 precede this function in the exe. Defining
//    0x49b720 too (with one Game struct shared by both) also gives 97.8% and
//    leaves 0x49b720 a MATCH; writing it with cast macros instead breaks the
//    tail merge again (78.1%).
//  - <minmax.h> (tools/headers.py): kind 0 then computes abs(x1 - x2) before
//    abs(y1 - y2), as the original does (95.9% to 96.6%).
//  - Kind 7's segment start `pt` lives in eax/ecx/edx on the loop entry, and
//    which component gets which register follows the components' reference
//    weights (equal weights: first component first). The original gives z
//    edx, then x ecx and y eax, so z carries one more reference than x and y:
//    the unused `int z = pt.z;` before `prev = pt;` emits no code and is
//    that reference. Without it the plain copy gets x edx, y ecx, z eax
//    (96.6%); copying the fields as z, x, y gets the registers but stores z
//    first (97.8%). A dead `prev.z = pt.z;` before the copy, or
//    `pt.z = start->z;` before `pt = *start;`, matches the same way; a second
//    use after the rand() calls makes z live across calls and moves it to esi.
// Earlier passes (still accurate):
//  - Kind 0's two line arms each make both FUN_004be950 calls (colour2, then
//    colour1); one shared colour1 call after the arms swaps the colour slots.
//  - Kinds 1, 3 and 6 pass a 3-short angle struct (the projectile's +0x34);
//    kind 3 passes its own `rot3`, which is never written (docs/bugs.md).
//  - Kind 7's dx/dy/dz are one Vec3 `d`, and the three 16.16 steps are written
//    back into it; the 64-bit divisor is its own `__int64 n64`. The jitter is
//    64-bit, `(rand() * 11) / 0x8000 - 5`, added to the high shorts of pt.
//  - Kind 1's rect is copied from +0x34 as one dword, then rect[1] += 0x8000.
//  - The LOS test goes through small inline members (ByteMap::Get,
//    MapSize::Contains) with an explicit visible = 1 / else 0.
//  - Kind 4's frame divisor is `(time - p->field_42) % *(unsigned short*)gaf`
//    (signed); its switch is a real jump table, cases 0..4.
// Layout facts:
//   Projectile stride 0x6b. type at +0x0; pos Vec3 (16.16) at +0x4; start
//   Vec3 at +0x10; shorts +0x34/+0x36/+0x38 (angles); int field_42 (spawn
//   time) +0x42; int field_46 +0x46; short field_5e +0x5e (sprite half
//   height); short counter +0x60; short field_64 +0x64; ushort flags +0x69.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
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
    void* field_30;                    // +0x30
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
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x147bb - 0x14327];
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


// FUN_0049b6e0 (matched in its own file), which comes before this function in
// the original file; see the note at the top.
Proj_0049be60* FUN_0049b6e0()
{
    Proj_0049be60* p = 0;
    if (g_game->projectileCount < 300) {
        p = &g_game->projectiles[g_game->projectileCount++];
        p->flags &= ~2;
        *(int*)((char*)p + 0x4e) = 0;
    }
    return p;
}

// FUNCTION: 0x49be60
void __stdcall FUN_0049be60(void* surface)
{
    Type_0049be60* type;
    int fr;
    Vec3_0049be60 sp;
    Vec3_0049be60 pt;
    Vec3_0049be60 prev;
    Vec3_0049be60 d;
    __int64 n64;
    int time = g_game->time;
    void* frame0 = FUN_004b7f30(g_game->gaf_1480f, 0);
    int index = 0;
    int visible;
    if (g_game->projectileCount <= 0)
        return;
    int offset = 0;
    while (1) {
        Proj_0049be60* p = (Proj_0049be60*)((char*)g_game->projectiles + offset);
        if (p->counter == 0) {
            unsigned char player = g_game->localPlayer;
            char* pb = (char*)g_game + 0x1b63 + 0x14b * player;
            Vec3_0049be60* pos = &p->pos;
            if ((g_game->viewFlags & 2) == 2) {
                int col = (int)*(short*)((char*)pos + 2) >> 5;
                PlayerInfo_0049be60* pi = (PlayerInfo_0049be60*)pb;
                int row = ((int)*(short*)((char*)pos + 10)
                           - ((int)*(short*)((char*)pos + 6) >> 1)) >> 5;
                if (pi->explored.size.Contains(col, row) && pi->explored.Get(col, row))
                    visible = 1;
                else
                    visible = 0;
            } else {
                visible = FUN_00408090((PlayerInfo_0049be60*)pb, pos);
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
                        if (abs(x1 - x2) > abs(y1 - y2)) {
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
                    } else {
                        FUN_004be950(surface, x1, y1, x2, y2, color1);
                    }
                } else if (type->field_10c == 1) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sx = 0x80 + (int)*(short*)((char*)&sp + 2);
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->field_5e >> 1) + 0x20;
                    FUN_004b8500(surface, frame0, sx, sy);
                    Angles_0049be60 rot = p->angles;
                    rot.y += 0x8000;
                    rot.z += 0x8000;
                    FUN_0046bae0(surface, &sp, type->field_74, &rot);
                    Sprite_0049be60* s = (Sprite_0049be60*)type->field_74;
                    if (0 != s->field_30 && p->field_46 > time) {
                        if (type->flag_21) {
                            rot.x = p->field_64;
                            FUN_0046bae0(surface, &sp, s->field_30, &rot);
                        } else {
                            FUN_0046bae0(surface, &sp, s->field_30, &rot);
                        }
                    }
                } else if (type->field_10c == 2) {
                    int sx = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    int sy = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    if (FUN_004b6720(g_game->field_37e27, sx, sy) == 0)
                        return;
                    FUN_004b9360(surface, g_game->field_1ab9b, sx, sy);
                } else if (type->field_10c == 3) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->field_5e >> 1) + 0x20;
                    FUN_004b8500(surface, frame0, sx, sy);
                    Angles_0049be60 rot3;
                    FUN_0046bae0(surface, &sp, type->field_74, &rot3);
                } else if (type->field_10c == 4) {
                    if (type->field_10d < 0xff) {
                        void* gaf = 0;
                        sp.x = pos->x - (g_game->scrollX << 16);
                        sp.y = p->pos.y;
                        sp.z = p->pos.z - (g_game->scrollY << 16);
                        FUN_004b8500(surface, frame0,
                                     (int)*(short*)((char*)&sp + 2) + 0x80,
                                     (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->field_5e >> 1) + 0x20);
                        int sy = ((int)*(short*)((char*)&sp + 10)
                                  - ((int)*(short*)((char*)&sp + 6) >> 1)) + 0x20;
                        int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                        switch (type->field_10d) {
                        case 0: gaf = g_game->gaf_147bb; break;
                        case 1: gaf = g_game->gaf_147bf; break;
                        case 2: gaf = g_game->gaf_147c3; break;
                        case 3: gaf = g_game->gaf_147c7; break;
                        case 4: gaf = g_game->gaf_147cb; break;
                        }
                        if (gaf) {
                            int n = *(unsigned short*)gaf;
                            FUN_004b7f90(surface, FUN_004b7f30(gaf, (time - p->field_42) % n), sx, sy);
                        }
                    }
                } else if (type->field_10c == 5) {
                    int sx = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    int sy = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    void* gaf = g_game->gaf_147f3;
                    int n = FUN_004b7f60(gaf);
                    fr = n - ((p->field_46 - time) * n) / (int)type->field_e6;
                    if (fr >= 0 && fr < n) {
                        FUN_004b8500(surface, FUN_004b7f30(gaf, fr), sx, sy);
                    }
                } else if (type->field_10c == 6) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->field_5e >> 1) + 0x20;
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    FUN_004b8500(surface, frame0, sx, sy);
                    FUN_0046bae0(surface, &sp, type->field_74, &p->angles);
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
                                        int z = pt.z;  // unused, see the note at the top
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
                                        FUN_004be950(surface,
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