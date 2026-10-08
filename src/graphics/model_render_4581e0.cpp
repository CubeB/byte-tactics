// Decompiled by Opus, space-bunny-free, longcat-2.5-preview-free, GPT-6, GPT-6.1-sol, deepseek-v4.1, deepseek-v4.1-flash and claude-opus-5-5. Names are provisional.
// The object-picture builder's lit rasteriser: projects the vertices, builds
// a normal per face and draws each face with a shade from the averaged normal
// (0x459c70). It stays in its own file: the merged model_render.cpp cannot
// place it at the symbol count its registers need.
// Keep <stdio.h>, <stdlib.h> and <math.h>: without them operand orders change.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern char* g_game;
// g_lightY is declared before g_lightX: the order decides which product of
// DrawLitPieces' lighting sum MSVC loads first.
extern float g_lightY;
extern float g_lightX;
extern float g_lightZ;
extern const float DAT_004fd4cc;
struct Bitmap_459c70;

// A 16.16 fixed-point position.
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

Vec3f __stdcall VectorFromToInt(Vec3 a, Vec3 b);
Vec3f __stdcall CrossProduct(Vec3f a, Vec3f b);
Vec3f __stdcall NormalizeVector(Vec3f v);
void* __stdcall GetGafSequenceFrame(void* pic);
void* __stdcall GetGafFrame(unsigned short* table, int index);
void __stdcall DownsampleFrame(Bitmap_459c70* dst, Bitmap_459c70* src);
void __stdcall FillFlatPolygon(Bitmap_459c70* surface, void* poly, int count, int flag);
void __stdcall DrawTexturedPolygon(Bitmap_459c70* surface, void* pic, void* poly, int flag);
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
    char unknown_96[0xff - 0x96];
    unsigned char kind;              // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                 // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int field_110;          // +0x110
    unsigned char field_114;         // +0x114
};

struct FaceFlags_459c70 {
    union {
        unsigned int raw;
        struct {
            unsigned int textured : 1;
            unsigned int usePic : 1;
            unsigned int shaded : 1;
            unsigned int rest : 29;
        } bits;
    };
};

struct Face_459c70 {
    int unknown_0;                   // +0x00
    int count;                       // +0x04
    int unknown_8;                   // +0x08
    unsigned short* indices;         // +0x0c
    void* pic;                       // +0x10
    int unknown_14;                  // +0x14
    unsigned short* color;           // +0x18
    FaceFlags_459c70 flags;          // +0x1c
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
    Vec3* vertices;                  // +0x22
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

struct UnitTable {
    char unknown_0[0x10];
    Bitmap_459c70* shadow;           // +0x10

    void MeasureModel(int* width, int* height, int* originX, int* originY, List_459c70* model, Vec3* offset);
    int BuildObjectPicture(List_459c70* list, int param_2, int param_3);
    void DrawPieces(Bitmap_459c70* bitmap, List_459c70* list, int kind, int useColor);
    void DrawLitPieces(Bitmap_459c70* bitmap, List_459c70* list, int kind, int useColor);
};

static __inline int shade_bias(Owner_459c70* owner)
{
    bool c = ((*(unsigned int*)(owner->field_92 + 0x241) >> 30) & 1) != 0;
    return c ? 125 : 50;
}

// The 50 or 125 bias the original materialises separately in each arm of the
// doubled-bitmap test, once per projected vertex.
static __inline int shade_bias(List_459c70* list)
{
    bool c = ((*(unsigned int*)(list->owner->field_92 + 0x241) >> 30) & 1) != 0;
    return c ? 125 : 50;
}

// Draws a unit model with per-vertex lighting: projects the vertices, builds
// a normal per face and averages them per vertex, then draws each face with
// a shade from the averaged normal. The anti-aliased path is the same as
// 0x459830's (draw into the doubled shadow bitmap, then downsample).
//
// Must stay the first function in the file: compiled later, a lea in the
// summing loop moves above the fadd.
// FUNCTION: 0x459c70
void UnitTable::DrawLitPieces(Bitmap_459c70* bitmap, List_459c70* list,
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
            // Last in the shadow branch.
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
                // Own read of the vertices, between verts and info: not v = verts.
                Vec3* v = list->pieces[p].vertices;
                info = list->pieces[p].info;
                // No n local or count guard: the loop runs to info->vertexCount.
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
                    // Computed right after the coordinate arms, before vertex[k].x = x:
                    // sets the frame size.
                    int s = (k * 3) & 0x1f;
                    vertex[k].x = x;
                    vertex[k].y = z - (y >> 1);
                    if (mode) vertex[k].z = y/2 + shade_bias(list);
                    else vertex[k].z = y + shade_bias(list);
                    vertex[k].shade = s;
                    vertex[k].x += (short)bitmap->field_4;
                    vertex[k].y += (short)bitmap->field_6;
                    accum[k][0] = accum[k][1] = accum[k][2] = 0.0f;
                    // A store here, not a memset before the loop.
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
                        Vec3f a = VectorFromToInt(p0, p1);
                        Vec3f b = VectorFromToInt(p0, p2);
                        Vec3f c = CrossProduct(a, b);
                        normal[fi] = c;
                        c = NormalizeVector(c);
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
                        // The empty do-while ends the block: keeps the store lea after the fadd.
                        float t2 = accum[*idx][2];
                        do {} while (0);    // emits no code; needed for the match
                        accum[*idx][2] = t2 + normal[fi].z;
                        weight[*idx]++;
                    }
                }

                // Divides by weight[k] directly, re-reading the vertex count.
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
                    if (!face->flags.bits.textured) {
                        if (face->count == 4) {
                            void* pic;
                            if (face->flags.bits.usePic) {
                                if (face->flags.bits.shaded) {
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

// Only AllocBitmap and AllocTwoPlaneBitmap are called here; the other
// members' symbol ids put MeasureModel in the window it matches in.
class CMemoryCache {
public:
    int InitCache(unsigned int size);
    void FreeCache();
    int AllocHandle(void** handle, int size);
    int AllocBitmap(Bitmap_459c70** handle, int w, int h);
    int AllocTwoPlaneBitmap(Bitmap_459c70** handle, int w, int h);
    void FlushCache();
    void ReleaseHandle(int handle);
};
