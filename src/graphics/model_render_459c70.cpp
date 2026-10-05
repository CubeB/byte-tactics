// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by claude-opus-5-5, deepseek-v4.1-flash retry, second deepseek-v4.1-flash pass, finished by Space Bunny Free, finished by claude-opus-5-5, checked by GPT-6. Names are provisional., finished by claude-opus-5-5
// Draws a unit model with per-vertex lighting: projects the vertices, builds
// a normal per face and averages them per vertex, then draws each face with
// a shade from the averaged normal. The anti-aliased path is the same as
// 0x459830's (draw into the doubled shadow bitmap, then downsample).
//
// MATCH (#5138), rewritten from 0x459830's matched source (was 73.5%).
// What it took (each step measured):
//  * Head, piece tests, face draw and downsampling tail are 0x459830's code:
//    `src = bitmap; bitmap = shadow;` last in the shadow branch, the piece
//    flags as positive `list->pieces[p].flags.visible/colored` tests, and
//    each coordinate arm reading `v->x` itself.
//  * `weight[k] = 0;` is a store in the vertex loop, not a memset before
//    it. MSVC turns it into the `rep stosd` itself and puts that after the
//    loop's pointer set-up, so the vertex and accum pointers live across it
//    and lose ecx/edi: that is what puts them in esi and ebx.
//  * There is no `if (n > 0)` and no `n` local: the loop runs to
//    `info->vertexCount` and the division loop re-reads
//    `list->pieces[p].info->vertexCount`. The bitmap offsets are read inside
//    the loop (`vertex[k].x += (short)bitmap->field_4`), as in 0x459830.
//  * The walking pointer is its own read, `Vec3* v =
//    list->pieces[p].vertices;`, after `verts` and before `info`; written
//    `v = verts` MSVC rebases it to `verts + 8`.
//  * The shade is `int s = (k * 3) & 0x1f;` (MSVC makes the k * 3 counter
//    and zeroes it after the loop guard), computed right after the
//    coordinate arms, before `vertex[k].x = x`: s then overlaps x, x gets
//    the original's stack slot and the frame grows to the original's
//    0x159d4 (69.6% -> 97.6%, then 98.7% for the k * 3 form).
//  * The division loop divides by `weight[k]` directly (no float local),
//    and accum[k][2] is summed through a temporary.
//  * g_lightY is declared before g_lightX. The order of those
//    extern declarations decides which product of the lighting sum MSVC
//    loads first (the later-declared constant's goes first); declared in
//    address order, accum[k][1] was loaded before accum[k][0]. (Indexing
//    with `idx[j]` instead of the walking `*idx` also flips it, but then
//    the lighting loop steps its two pointers in the wrong order.) <math.h>
//    matters too: with <string.h> alone this is 98.9%.
//  * The empty `do {} while (0);` in the summing loop emits no code. It ends
//    the basic block after the accum[k][2] load, so the scheduler keeps the
//    store address `lea` after the fadd, as in the original; without it
//    this is 99.8%. `if (0) {}` there matches too, so it is most likely a
//    debug macro that compiles to nothing. Found by tools/permute.py.
#include <math.h>
#include <string.h>

extern char* g_game;
extern float g_lightY;
extern float g_lightX;
extern float g_lightZ;
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
void* __stdcall GetGafSequenceFrame(void* pic);
void* __stdcall GetGafFrame(unsigned short* table, int index);
void __stdcall DownsampleFrame(Bitmap_459c70* dst, Bitmap_459c70* src);
void __stdcall FillShadedPolygon(Bitmap_459c70* surface, void* poly, int count, int flag);
void __stdcall DrawLitTexturedPolygon(Bitmap_459c70* surface, void* pic, void* poly, int flag);

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
    void DrawLitPieces(Bitmap_459c70* bitmap, List_459c70* list, int kind, int useColor);
};

// The 50 or 125 bias the original materialises separately in each arm of the
// doubled-bitmap test, once per projected vertex.
static __inline int shade_bias(List_459c70* list)
{
    bool c = ((*(unsigned int*)(list->owner->field_92 + 0x241) >> 30) & 1) != 0;
    return c ? 125 : 50;
}

// FUNCTION: 0x459c70
void Class_004581e0::DrawLitPieces(Bitmap_459c70* bitmap, List_459c70* list,
    int kind, int useColor)
{
    PieceInfo_459c70* info;
    Poly_459c70 poly[25];
    float accum[2000][3];
    Vec3f normal[2000];
    int weight[2000];
    Poly_459c70 vertex[2000];

    int mode;
    Bitmap_459c70* src;
    if (((Flags_37f06*)(g_game + 0x37f06))->antiAlias) {
        if ((list->owner->field_110 & 0x20000000) != 0 && useColor != 0) {
            Bitmap_459c70* shadow = this->shadow;
            mode = 1;
            shadow->width = (unsigned short)(bitmap->width << 1);
            shadow->height = (unsigned short)(bitmap->height << 1);
            shadow->unknown_9[0] = 0;
            shadow->colorKey = 1;
            shadow->field_4 = (short)(bitmap->field_4 << 1);
            shadow->field_6 = (short)(bitmap->field_6 << 1);
            memset(shadow->data2, 0, shadow->width * shadow->height);
            memset(shadow->data, 1, shadow->width * shadow->height);
            src = bitmap;
            bitmap = shadow;
        } else {
            mode = 0;
        }
    } else {
        mode = 0;
    }

    for (int p = list->count - 1; p >= 0; p--) {
        if (list->pieces[p].flags.visible) {
            if (useColor == -1 || useColor == list->pieces[p].flags.colored
                    || list->owner->field_104 != 0.0f) {
                Vec3* verts = list->pieces[p].vertices;
                Vec3* v = list->pieces[p].vertices;
                info = list->pieces[p].info;
                for (int k = 0; k < info->vertexCount; k++, v++) {
                    int x;
                    int y;
                    int z;
                    if (mode) {
                        x = (short)(v->x >> 16) << 1;
                        y = (short)(v->y >> 16) << 1;
                        z = (short)(-v->z >> 16) << 1;
                    } else {
                        x = (short)(v->x >> 16);
                        y = (short)(v->y >> 16);
                        z = (short)(-v->z >> 16);
                    }
                    int s = (k * 3) & 0x1f;
                    vertex[k].x = x;
                    vertex[k].y = z - (y >> 1);
                    if (mode) vertex[k].z = y/2 + shade_bias(list);
                    else vertex[k].z = y + shade_bias(list);
                    vertex[k].shade = s;
                    vertex[k].x += (short)bitmap->field_4;
                    vertex[k].y += (short)bitmap->field_6;
                    accum[k][0] = accum[k][1] = accum[k][2] = 0.0f;
                    weight[k] = 0;
                }

                Face_459c70* face;
                int fi;
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

                if (info->firstFace != -1) {
                    face = info->faces + 1;
                    fi = 1;
                } else {
                    face = info->faces;
                    fi = 0;
                }
                for (; fi < info->faceCount; fi++, face++) {
                    unsigned short* idx = face->indices;
                    for (int j = 0; j < face->count; j++, idx++) {
                        accum[*idx][0] += normal[fi].x;
                        accum[*idx][1] += normal[fi].y;
                        float t2 = accum[*idx][2];
                        do {} while (0);    // no code: see the notes at the top
                        accum[*idx][2] = t2 + normal[fi].z;
                        weight[*idx]++;
                    }
                }

                for (k = 0; k < list->pieces[p].info->vertexCount; k++) {
                    if (weight[k] != 0) {
                        accum[k][0] /= weight[k];
                        accum[k][1] /= weight[k];
                        accum[k][2] /= weight[k];
                    }
                }

                if (info->firstFace != -1) {
                    face = info->faces + 1;
                    fi = 1;
                } else {
                    face = info->faces;
                    fi = 0;
                }
                for (; fi < info->faceCount; fi++, face++) {
                    unsigned short* idx = face->indices;
                    for (int j = 0; j < face->count; j++, idx++) {
                        poly[j] = vertex[*idx];
                        if (list->pieces[p].flags.lit) {
                            float light = accum[*idx][0] * g_lightX + accum[*idx][1] * g_lightY;
                            light += accum[*idx][2] * g_lightZ;
                            poly[j].shade = 0x1f & ((int)(DAT_004fd4cc * light));
                        } else {
                            poly[j].shade = 0xf;
                        }
                    }
                    if (!face->flags.textured) {
                        if (face->count == 4) {
                            void* pic;
                            if (face->flags.usePic) {
                                if (face->flags.shaded) {
                                    int unit = *(int*)(g_game + 0x1b8a + kind * 0x14b);
                                    pic = GetGafFrame(face->color,
                                        *(unsigned char*)(unit + 0x96));
                                } else if (useColor) {
                                    pic = GetGafFrame(face->color, 0);
                                } else {
                                    pic = GetGafSequenceFrame(&face->pic);
                                }
                            } else {
                                pic = face->pic;
                            }
                            DrawLitTexturedPolygon(bitmap, pic, poly, 0);
                        }
                    } else {
                        FillShadedPolygon(bitmap, poly, face->count, face->unknown_0);
                    }
                }
            }
        }
    }

    if (((Flags_37f06*)(g_game + 0x37f06))->antiAlias) {
        if (mode != 0) {
            DownsampleFrame(bitmap, src);
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
