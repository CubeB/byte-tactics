// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by claude-opus-5-5, deepseek-v4.1-flash retry, second deepseek-v4.1-flash pass. Names are provisional.
//
// Second pass: 63.7 -> 66.6%, and the file is now semantically faithful
// (item 3 below fixes a real bug the first pass had).
//
//  1. The two face loops' first-face prologue must be a FULL if/else that
//     assigns both the face pointer and the index:
//        Face_459c70* face; int fi;
//        if (info->firstFace != -1) { face = info->faces + 1; fi = 1; }
//        else { face = info->faces; fi = 0; }
//     That is what emits the original's `je` + `jmp` diamond at 0x459f4b and
//     0x45a13d (+1.5). Writing it as `face = info->faces; fi = 0; if (...)`
//     drops the `jmp`; doing the same in the THIRD (poly) loop costs 2.5, and
//     that loop wants a named `int skipFirst` test instead.
//  2. tools/permute.py took the file from 64.4 to 66.6 and all of that
//     survived cleaning. Free to delete: the self-assignments it added
//     (`mode = mode;`, `offX = offX;`, `x = x;`, `tmp5 = tmp5;`), both getter
//     helpers, its `int tmp2 = X, n = tmp2` copy chain, the empty `else` after
//     a `continue`, the `(int)` casts, the `(Bitmap_459c70*)bmp` cast, the
//     `if (f->count != 4) {} else {}` inversion, the `do {} while (0)` wrapper
//     (-0.3 if kept, so it is gone), the `if (x == 0) {} else {}` in the tail
//     and its `for (;;) + break` row loop. What is left of its 66.6 is the
//     tail's statement order: `y++;` BEFORE `d += bmp->width;` (+0.2), and
//     `Face_459c70* f = info->faces;` as one declaration. Load-bearing, and kept here under
//     real names because every alternative spelling costs more:
//       * a FUNCTION-SCOPE `float w;` for the division loop (-1.7 if it moves
//         back inside the loop);
//       * `int xs = x >> 16;` before `x = (short)xs << 1;` in the doubled
//         projection arm, and `int y2 = y / 2;` in the z arm (-11 if the x
//         one is inlined);
//       * `int vy = verts[k].y;` in the undoubled arm, and `float t0`/`float
//         t2` around the two `accum[k][c] = t + normal[i].?` updates: these
//         decide which operand MSVC loads first, and the temps give the
//         original's `fld accum / fadd normal` order (each is worth 0.2-0.7);
//       * the second face loop as `while (i < info->faceCount) { ... i++, f++; }`
//         rather than a `for` header (-5.8);
//       * the poly loop's `for (; i < info->faceCount; )` with `i++, f++;` at
//         the bottom, and `int x, y, z;` written as three declarations (-0.3).
//     Two spellings that are NOT free and are deliberately not taken:
//     `int x, y, z;` on one line and merging the second face loop's
//     `unsigned short* idx = f->indices;` back into one declaration (0.3 each).
//  3. SEMANTIC FIX: the two vertex.z arms are NOT interchangeable. In the
//     original the mode!=0 arm doubles y and then halves it again, so both
//     arms compute y16 + bias; the first pass had `y + bias` in the mode!=0
//     arm (2*y16 + bias) and `y/2 + bias` in the mode==0 arm. The correct
//     `if (mode != 0) { z = bias + y/2 } else { z = y + bias }` is in the
//     file; the wrong one scores about 0.3 higher and the permuter keeps
//     proposing it, so do not "fix" it back. The arms differ in C only because
//     a truncating /2 of an odd value is not exact, but with the doubling
//     above, the two paths are.
//  4. `offX`/`offY` and the draw target come from the `bmp` local, which holds
//     exactly what the original's reassigned `bitmap` parameter holds (the
//     shadow in mode 1, the front bitmap in mode 0), so the values are right;
//     only the spelling differs. The original really does overwrite its
//     parameter slot 0x159e8 with the shadow at 0x459d28 and then read the
//     vertex offsets back out of it (0x459df0/0x459dfc); every spelling of
//     that reassignment costs 1.7-2.0 points, so the separate local is kept.
//
// Still differing, all of it register and slot placement with no semantic
// content left:
//  * Frame slots. The original's map is 0x10 shade / 0x14 verts-walk / 0x18
//    verts (reused for firstFace) / 0x1c piece / 0x20 mode / 0x24 info / 0x28
//    src / 0x2c offY / 0x30 n / 0x34 x / 0x38 p / 0x3c offX. MSVC 5.0 does not
//    assign these in declaration order (moving every declaration was inert
//    here), so the map can only be moved by changing the dataflow.
//  * The vertex loop (0x459e08-0x459f38, the largest single loss left). The
//    original has three induction registers: esi = &vertex[k], ebx =
//    &accum[k][1] (stored at +4/0/-4 off that one base) and a third, the
//    verts walk, which it SPILLS to [esp+0x14]; it also spills x to
//    [esp+0x34] and materialises the mode-test zero with `xor ecx,ecx; cmp
//    edx,ecx` so the same ecx can store the three accum zeros. Here MSVC
//    shares one induction between `verts[k]` and `accum[k][2]` (an
//    `accum - verts` delta in a register instead of a third induction), keeps
//    x in a register, and uses `test`/`movl $0`. Giving the accum its own
//    pointer induction (`float* a` walked by `a += 3`) or the vertex walk its
//    own pointer each costs 7-8 points, so the sharing is a net win for MSVC
//    5 and the original's shape is not reachable from this source shape.
//  * The bias helper materialises in ecx where the original uses edx (six
//    instructions at 0x459eb8 and 0x459ee5). Passing the loaded flag word, or
//    a `char*`, to `shade_bias` instead of the list does not move it.
//  * 0x45a320: the original's scratch for the `usePic` bit test is ecx
//    (`mov ecx,eax; shr ecx,1; test cl,1`), ours picks edx. This is MSVC 5's
//    [eax,ecx,edx] temp rotation and no spelling of the three bit tests
//    moves it.
//  * 0x45a29b: the piece bit-2 test is `mov dl,[ecx+0x28]; shr dl,2; test
//    dl,1` in the original and `test byte ptr [ecx+0x28],4` here; `(f >> 2)
//    & 1` folds to the mask test in every spelling tried.
//  * 0x45a1e2: the original re-derives the division loop's bound from
//    `[piece]->info->vertexCount` and keeps firstFace in ebp across the face
//    loops; using the re-read bound costs 7-9 points, so the loop keeps the
//    `n` slot here.
//  * 0x45a419 tail: the original's inner copy loop is entered through
//    `mov edi,eax; dec eax; test edi,edi; je; lea edi,[eax+1]` and reloads
//    `src->height` at the row end; no spelling of `while (x--)`, `for (x = w;
//    x; --x)`, `do {} while (--x)` or a pre-decrement reproduces it.
//  * 0x45a2fd: the original bumps the poly induction before the index one.
//  * 0x459c70-0x459d6a: the original keeps the memset product in ebx and the
//    front bitmap in ebp, and re-reads `useColor` from its argument slot three
//    times instead of holding it in a register.
//
//  5. Two bits of permuter noise in `shade_bias` are load-bearing and are left
//     in on purpose: the `int ret0; ret0 = c ? 125 : 50; return ret0;` chain
//     (-0.4 if written as `return c ? 125 : 50;`). The split `bool c; c = ...;`
//     declaration beside it is free to merge and has been.
//
// Counted check.py runs against this file this pass: 163 (63.7 -> 66.6), of
// which 12 were against this file and the rest --sym runs on scratch variants.
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ddraw.h>

extern char* g_game;
extern float DAT_005065f8;
extern float DAT_005065fc;
extern float DAT_00506600;
extern const float DAT_004fd4cc;
struct Bitmap_459c70;

struct Vec3 { int x; int y; int z; };

struct Flags_37f06 {
    unsigned short damagebars : 1;
    unsigned short antiAlias : 1;
    unsigned short shadows : 1;
    unsigned short vehicleShadows : 1;
    unsigned short featureShadows : 1;
    unsigned short shading : 1;
    unsigned short ditheredFog : 1;
    unsigned short unused7 : 1;
    unsigned short switchAlt : 1;
};

struct Vec3f { float x; float y; float z; };

Vec3f __stdcall FUN_004b6f00(Vec3 a, Vec3 b);
Vec3f __stdcall FUN_004b6f70(Vec3f a, Vec3f b);
Vec3f __stdcall FUN_004b6ff0(Vec3f v);
void* __stdcall FUN_004b7ee0(void* pic);
void* __stdcall FUN_004b7f30(unsigned short* table, int index);
void __stdcall FUN_004b95a0(Bitmap_459c70* dst, Bitmap_459c70* src);
void __stdcall FUN_004c0c70(Bitmap_459c70* surface, void* poly, int count, int flag);
void __stdcall FUN_004c8bb0(Bitmap_459c70* surface, void* pic, void* poly, int flag);

struct Bitmap_459c70 {
    unsigned short width;            // +0x00
    unsigned short height;           // +0x02
    short field_4;                   // +0x04
    short field_6;                   // +0x06
    char colorKey;                   // +0x08
    char unknown_9[7];
    char* data;                      // +0x10
    char* data2;                     // +0x14
};

#pragma pack(push, 1)
struct Owner_459c70 {
    char unknown_0[0x92];
    char* field_92;                  // +0x92
    char unknown_96[0x104 - 0x96];
    float field_104;                 // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int field_110;          // +0x110
    unsigned char field_114;         // +0x114
};

struct FaceFlags_459c70 {
    unsigned int textured : 1;
    unsigned int usePic : 1;
    unsigned int shaded : 1;
    unsigned int rest : 29;
};

struct Face_459c70 {
    int unknown_0;                   // +0x00
    int count;                       // +0x04
    int unknown_8;                   // +0x08
    unsigned short* indices;         // +0x0c
    void* pic;                       // +0x10
    int unknown_14;                  // +0x14
    unsigned short* color;           // +0x18
    FaceFlags_459c70 flags;         // +0x1c
};

struct PieceInfo_459c70 {
    char unknown_0[4];
    int vertexCount;                 // +0x04
    int faceCount;                   // +0x08
    int firstFace;                   // +0x0c
    char unknown_10[8];
    unsigned short* color;           // +0x18
    char unknown_1c[0xc];
    Face_459c70* faces;              // +0x28
};

struct Piece_459c70 {
    PieceInfo_459c70* info;          // +0x00
    char unknown_4[0x22 - 0x04];
    Vec3* vertices;         // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned char flags;             // +0x28
    char unknown_29[0x36 - 0x29];
};

struct List_459c70 {
    int count;                       // +0x00
    char unknown_4[8];
    Owner_459c70* owner;             // +0x0c
    Bitmap_459c70* bitmap;           // +0x10
    char unknown_14[0x22 - 0x14];
    Piece_459c70 pieces[1];          // +0x22
};
#pragma pack(pop)

struct Poly_459c70 { int x; int y; int z; int shade; };

struct Class_004581e0 {
    char unknown_0[0x10];
    Bitmap_459c70* shadow;           // +0x10
    void FUN_00459c70(Bitmap_459c70* bitmap, List_459c70* list, int kind, int useColor);
};

// The 50 or 125 bias the original materialises separately in each arm of the
// doubled-bitmap test, once per projected vertex.
static __inline int shade_bias(List_459c70* list)
{
    bool c = 0 != ((*(unsigned int*)((char*)list->owner->field_92 + 0x241) >> 30) & 1);
    int ret0;
    ret0 = c ? 125 : 50;
    return ret0;
}



// FUNCTION: 0x459c70
void Class_004581e0::FUN_00459c70(Bitmap_459c70* bitmap, List_459c70* list,
    int kind, int useColor)
{
    PieceInfo_459c70* info;
    Poly_459c70 poly[25];
    float accum[2000][3];
    Vec3f normal[2000];
    int weight[2000];
    Poly_459c70 vertex[2000];

    Bitmap_459c70* shadow;
    Bitmap_459c70* src;
    Bitmap_459c70* bmp;
    int mode;
    int offX;
    float w;
    char* s;
    char* d;
    Piece_459c70* piece;
    Vec3* verts;
    unsigned char pflags;

    if (((Flags_37f06*)(g_game + 0x37f06))->antiAlias) {
        if ((list->owner->field_110 & 0x20000000) != 0) {
            if (useColor != 0) {
                shadow = this->shadow;
                mode = 1;
                src = bitmap;
                shadow->width = (unsigned short)(bitmap->width << 1);
                shadow->height = (unsigned short)(bitmap->height << 1);
                shadow->unknown_9[0] = 0;
                shadow->colorKey = 1;
                shadow->field_4 = (short)(bitmap->field_4 << 1);
                shadow->field_6 = (short)(bitmap->field_6 << 1);
                memset(shadow->data2, 0, shadow->height * shadow->width);
                memset(shadow->data, 1, shadow->width * shadow->height);
                bmp = shadow;
                goto haveMode;
            }
        }
    }
    mode = 0;
    bmp = bitmap;
    src = bitmap;
haveMode:
    for (int p = list->count - 1; p >= 0; p--) {
        pflags = list->pieces[p].flags;
        piece = &list->pieces[p];
        if (!(list->pieces[p].flags & 1))
            continue;
        if (!(useColor == -1 || useColor == ((pflags >> 1) & 1)
                || list->owner->field_104 != 0.0f))
            continue;
        verts = piece->vertices;
        info = piece->info;
        int n = info->vertexCount;
        if (0 < n) {
            int k;
            int offY = (short)bmp->field_6;
            int shade = 0;
            int offX = (short)bmp->field_4;
            memset(weight, 0, n * sizeof(int));
            for (k = 0; k < n; k++) {
                int y;
                int z;
                int x;

                x = verts[k].x;
                if (mode != 0) {
                    int xs = x >> 16;
                    y = (short)(verts[k].y >> 16) << 1;
                    x = (short)xs << 1;
                    z = (short)(-verts[k].z >> 16) << 1;
                } else {
                    int vy = verts[k].y;
                    y = (short)(vy >> 16);
                    x = (short)(x >> 16);
                    z = (short)(-verts[k].z >> 16);
                }
                vertex[k].x = x;
                vertex[k].y = z - (y >> 1);
                if (mode != 0) {
                    int y2 = y / 2;
                    vertex[k].z = shade_bias(list) + y2;
                } else {
                    vertex[k].z = y + shade_bias(list);
                }
                vertex[k].shade = 0x1f & shade;
                vertex[k].x += offX;
                vertex[k].y = vertex[k].y + offY;
                shade += 3;
                accum[k][0] = 0.0f;
                accum[k][1] = 0.0f;
                accum[k][2] = 0.0f;
            }
        }

        {
            int fi;
            Face_459c70* face;
            if (info->firstFace != -1) {
                face = info->faces + 1;
                fi = 1;
            } else {
                face = info->faces;
                fi = 0;
            }
            for (; fi < info->faceCount; fi++, face++) {
                unsigned short* idx = face->indices;
                if (!(idx[0] == idx[1] || idx[1] == idx[2] || idx[2] == idx[0])) {
                    Vec3 p0 = verts[idx[1]];
                    Vec3 p1 = verts[idx[0]];
                    Vec3 p2 = verts[idx[2]];
                    Vec3f a = FUN_004b6f00(p0, p1);
                    Vec3f b = FUN_004b6f00(p0, p2);
                    Vec3f c = FUN_004b6f70(a, b);
                    normal[fi] = c;
                    c = FUN_004b6ff0(c);
                    normal[fi] = c;
                } else {
                    normal[fi].x = 0.0f;
                    normal[fi].y = 1.0f;
                    normal[fi].z = 0.0f;
                }
            }
        }

        {
            int i;
            Face_459c70* f;
            if (info->firstFace != -1) {
                i = 1;
                f = info->faces + 1;
            } else {
                f = info->faces;
                i = 0;
            }
            while (i < info->faceCount) {
                    unsigned short* idx;
                    int j;
                    idx = f->indices;
                    for (j = 0; j < f->count; j++) {
                        int k = idx[j];
                        float t0 = accum[k][0];
                        accum[k][0] = t0 + normal[i].x;
                        accum[k][1] += normal[i].y;
                        float t2 = accum[k][2];
                        accum[k][2] = t2 + normal[i].z;
                        weight[k]++;
                    }
                    i++, f++;
                }
        }

        int q1 = 0;
        while (n > q1) {
            if (weight[q1] != 0) {
                w = (float)weight[q1];
                    accum[q1][0] /= w;
            accum[q1][1] /= w;
            accum[q1][2] /= w;
            }
            q1++;
        }

        {
            int i;
            Face_459c70* f = info->faces;
            i = 0;
            if (info->firstFace != -1) {
                f = f + 1;
                i = 1;
            }
            for (; i < info->faceCount; ) {
                int j;
                unsigned short* idx;
                idx = f->indices;
                for (j = 0; j < f->count; j = j + 1) {
                    poly[j] = vertex[idx[j]];
                    if ((piece->flags & 4) != 0) {
                        float v = accum[idx[j]][0] * DAT_005065f8 + accum[idx[j]][1] * DAT_005065fc;
                        v += accum[idx[j]][2] * DAT_00506600;
                        poly[j].shade = 0x1f & ((int)(DAT_004fd4cc * v));
                    } else {
                        poly[j].shade = 0xf;
                    }
                }
                if (f->flags.textured == 0) {
                    if (f->count == 4) {
                        void* pic;
                        if (f->flags.usePic) {
                            if (f->flags.shaded) {
                                int unit = *(int*)((0x1b8a + g_game) + (kind * 0x14b));
                                pic = FUN_004b7f30(f->color,
                                    *(unsigned char*)(unit + 0x96));
                            } else if (useColor) {
                                pic = FUN_004b7f30(f->color, 0);
                            } else {
                                pic = FUN_004b7ee0(&f->pic);
                            }
                        } else {
                            pic = f->pic;
                        }
                        FUN_004c8bb0(bmp, pic, poly, 0);
                    }
                } else {
                    FUN_004c0c70(bmp, poly, f->count, f->unknown_0);
                }
                i++, f++;
            }
        }
    }

    if (((Flags_37f06*)(0x37f06 + g_game))->antiAlias) {
        if (mode != 0) {
            FUN_004b95a0(bmp, src);
            s = src->data2;
            if (s != 0) {
                int y = 0;
                d = bmp->data2;
                while (y < src->height) {
                    int x = src->width;
                    if (x != 0) {
                        do {
                            *s++ = *d;
                            d = d + 2;
                        } while (--x);
                    }
                    y++;
                    d += bmp->width;
                }
            }
        }
    }
}