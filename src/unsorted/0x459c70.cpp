// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by claude-opus-5-5, deepseek-v4.1-flash retry, second deepseek-v4.1-flash pass, finished by Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
//
// Partial, 73.5% (2057 bytes against 2047). Issue #4863 took it from 66.6%.
//
// ISSUE #4924 (no code change, about 150 variants scored). The sibling
// 0x459830 went from 71% to 96.5% on its head form (nested ifs, `src =
// bitmap; bitmap = shadow;` LAST), but that form is the original 0x459830's
// allocation (src and the bitmap in memory), the opposite of this original
// (bitmap in esi, src in ebp, list in edi). Here it gives 68.5%; every head
// order and target spelling (35 combinations: && chain or nested, src/bitmap
// assigned top, after the field writes or last, reads through src or bitmap,
// writes through shadow or bitmap, either memset product order) is 67 to 69%,
// and a `dst` local for the target is 59%. A diagnostic that adds one
// throwaway piece-loop-level read of bitmap and one of src (`if
// (bitmap->width == 12345 || src->width == 54321) continue;`) produces exactly
// the original's allocation (ebp src, esi bitmap, edi list), so the original
// gives both more weight at that level than this source does; the missing
// uses were not found. Also tried and worse or equal: `continue`-style piece
// tests (72.4), the 0x459830 poly-copy spelling `j++, idx++` with `*idx`
// (64.2), a walking `Vec3* v` for the vertex loop (56 to 60: MSVC rebases it
// to `verts + 8`), `n` read through the piece (72.2), the divide loop bounded
// by `piece->info->vertexCount` (54.6), offX/offY declaration orders
// (identical), 0x459830's current body compiled first (73.2). Hoisting the
// offsets into the loop (`vertex[k].x += (short)bitmap->field_4;`) scores
// 73.7 but moves the two loads after the memset, where the original has them
// before it, so it is not kept.
//
// What the earlier passes recorded and is still true: the face/normal loop
// (0x459f42-0x45a12d) and the draw-call block match instruction for
// instruction, and the bias helper, the first-face if/else diamonds and the
// function-scope `float w` are load-bearing. Several earlier notes were wrong
// and are gone: both draw calls DO take the target bitmap as their first
// argument (the old reading forgot that each `push` moves esp, so
// [esp+0x159f4] after three pushes is the 0x159e8 bitmap slot, not useColor).
//
// WHAT CHANGED IN #4863 (each step measured, all of it faithful source):
//  1. The target bitmap really is the reassigned parameter: `src = bitmap;
//     bitmap = shadow;` in the shadow branch and nothing for src on the
//     other path. The original's else block loads src from its never-written
//     home ([esp+0x28]) into ebp, which is only possible when src is assigned
//     in the shadow branch alone. With the `bmp` local gone the frame map and
//     the 2047-byte size of the original came back.
//  2. Piece flags are an `unsigned short` bitfield (visible, colored, lit at
//     bits 0..2). That is what gives `test byte ptr [m], 1` and then a fresh
//     `mov dl, [m]; shr edx, 1; and edx, 1` for the colour test, and the
//     poly loop's `mov dl, [m]; shr dl, 2; test dl, 1` for `lit`. An unsigned
//     char field folds both to `test byte ptr [m], mask`.
//  3. The piece tests are nested ifs, not `continue`s, and the shadow-branch
//     condition is `if (antiAlias) { if (owner && useColor) {...} else
//     mode = 0; } else mode = 0;` (no goto).
//  4. In the shadow branch `src = bitmap; bitmap = shadow;` come first and
//     the size reads go through src.
//  5. `int s = 0x1f & shade;` computed before the z arms: the original has
//     `and edi, 0x1f` ahead of the mode branch and stores it after the
//     merge. The accum zeros are one chained assignment (one zero register
//     stored three times, right to left).
//  6. The tail is `for (y...) { int x = src->width; while (x--) { unsigned
//     int c = *d; *s++ = c; d += 2; } d += bitmap->width; }`: the int
//     temporary is what gives `xor eax, eax; inc edx; mov al, [ecx]; ...;
//     mov [edx-1], al`, and `while (x--)` gives `mov edi, eax; dec eax; test
//     edi, edi; je; lea edi, [eax+1]`.
//  7. A combinatorial sweep (build/scratch/0x459200/combo.py with the choice
//     groups in g_c70*.py, about 4000 compiles) picked the rest: `int xs` /
//     `int vy` / `int y2` temporaries in the vertex loop, the t0/t2 temps in
//     the accumulate loop, the zero-first form of the poly-loop prologue and
//     `vertex[k].y = vertex[k].y + offY`. Each of these is worth 0.3 to 10
//     points only in combination with the others; on their own most are
//     neutral, so treat them as one choice.
//
// STILL DIFFERING (all register allocation, measured):
//  * Function level: the original keeps the target bitmap in esi and src in
//    ebp across the piece loop and the tail, and list in edi at loop level.
//    Here src gets esi and the bitmap is only loaded where used. Depending on
//    the tail's shape MSVC instead promotes useColor; no source construct
//    tried (helpers, comparison spellings, list->pieces[p] forms, inline
//    helpers) moved useColor below list without a throwaway use, and a
//    throwaway extra `list` reference does flip it (diagnostic only).
//  * The piece pointer induction is biased to &piece->flags (+0x28) where the
//    original's is the struct base; every spelling of the piece accesses
//    compiled byte-identically.
//  * Vertex loop (largest residual, about 60 of 92 lines): the original walks
//    the vertices with a separate unbiased pointer in edx (home [esp+0x14]),
//    keeps x in memory and the zero for the mode compare and the accum stores
//    in ecx. MSVC 5 rebases every pointer walk it can prove to be an
//    induction (`lea reg, [verts+8]`), so `v++` walks, `Vec3 p = *v++` and
//    `Vec3* p = v++` all lost 2 to 15 points against the shared verts[k]
//    induction used here.
//  * The division loop: the original re-reads `piece->info->vertexCount` as
//    its bound; spelling that frees n's slot, shrinks the frame by 4 and
//    costs 17 points.
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

struct PieceFlags_459c70 {
    unsigned short visible : 1;
    unsigned short colored : 1;
    unsigned short lit : 1;
    unsigned short rest : 13;
};

struct Piece_459c70 {
    PieceInfo_459c70* info;          // +0x00
    char unknown_4[0x22 - 0x04];
    Vec3* vertices;         // +0x22
    char unknown_26[0x28 - 0x26];
    PieceFlags_459c70 flags;         // +0x28
    char unknown_2a[0x36 - 0x2a];
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
    bool c = ((*(unsigned int*)(list->owner->field_92 + 0x241) >> 30) & 1) != 0;
    return c ? 125 : 50;
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

    Bitmap_459c70* src;
    int mode;
    float w;
    Piece_459c70* piece;
    Vec3* verts;

    if (((Flags_37f06*)(g_game + 0x37f06))->antiAlias) {
        if ((list->owner->field_110 & 0x20000000) != 0 && useColor != 0) {
            Bitmap_459c70* shadow = this->shadow;
            mode = 1;
            src = bitmap;
            bitmap = shadow;
            shadow->width = (unsigned short)(src->width << 1);
            shadow->height = (unsigned short)(src->height << 1);
            shadow->unknown_9[0] = 0;
            shadow->colorKey = 1;
            shadow->field_4 = (short)(src->field_4 << 1);
            shadow->field_6 = (short)(src->field_6 << 1);
            memset(shadow->data2, 0, shadow->height * shadow->width);
            memset(shadow->data, 1, shadow->width * shadow->height);
        } else {
            mode = 0;
        }
    } else {
        mode = 0;
    }

    for (int p = list->count - 1; p >= 0; p--) {
        piece = &list->pieces[p];
        if (piece->flags.visible) {
            if (useColor == -1 || useColor == piece->flags.colored
                    || list->owner->field_104 != 0.0f) {
                verts = piece->vertices;
                info = piece->info;
                int n = info->vertexCount;
                if (0 < n) {
                    int k;
                    int offY = (short)bitmap->field_6;
                    int shade = 0;
                    int offX = (short)bitmap->field_4;
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
                        int s = 0x1f & shade;
                        if (mode != 0) {
                            int y2 = y / 2;
                            vertex[k].z = shade_bias(list) + y2;
                        } else {
                            vertex[k].z = y + shade_bias(list);
                        }
                        vertex[k].shade = s;
                        vertex[k].x += offX;
                        vertex[k].y = vertex[k].y + offY;
                        shade += 3;
                        accum[k][0] = accum[k][1] = accum[k][2] = 0.0f;
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
                        f = info->faces + 1;
                        i = 1;
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

                for (int q1 = 0; q1 < n; q1++) {
                    if (weight[q1] != 0) {
                        w = (float)weight[q1];
                        accum[q1][0] /= w;
                        accum[q1][1] /= w;
                        accum[q1][2] /= w;
                    }
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
                            if (piece->flags.lit) {
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
                                FUN_004c8bb0(bitmap, pic, poly, 0);
                            }
                        } else {
                            FUN_004c0c70(bitmap, poly, f->count, f->unknown_0);
                        }
                        i++, f++;
                    }
                }
            }
        }
    }

    if (((Flags_37f06*)(0x37f06 + g_game))->antiAlias) {
        if (mode != 0) {
            FUN_004b95a0(bitmap, src);
            unsigned char* s = (unsigned char*)src->data2;
            if (s != 0) {
                unsigned char* d = (unsigned char*)bitmap->data2;
                for (int y = 0; y < src->height; y++) {
                    int x = src->width;
                    while (x--) {
                        unsigned int c = *d;
                        *s++ = c;
                        d += 2;
                    }
                    d += bitmap->width;
                }
            }
        }
    }
}
