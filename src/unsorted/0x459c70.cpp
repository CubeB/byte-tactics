// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6. Names are provisional.
// Partial, 34.5%: Sampling direction and mutable lighting-vector reads corrected.
// Frame-slot rotation, per-piece registers and x87 scheduling still differ.
#include <map>
#include <memory.h>
#include <ddraw.h>

extern char* g_game;
extern float DAT_005065f8;
extern float DAT_005065fc;
extern float DAT_00506600;
extern const float DAT_004fd4cc;
struct Bitmap_459c70;

struct Vec3 { int x; int y; int z; };
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

struct Face_459c70 {
    int unknown_0;                   // +0x00
    int count;                       // +0x04
    int unknown_8;                   // +0x08
    unsigned short* indices;         // +0x0c
    void* pic;                       // +0x10
    int unknown_14;                  // +0x14
    unsigned short* color;           // +0x18
    unsigned int flags;              // +0x1c
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
    return (((*(unsigned int*)((char*)list->owner->field_92 + 0x241) >> 30) & 1)
        != 0) ? 125 : 50;
}

// FUNCTION: 0x459c70
void Class_004581e0::FUN_00459c70(Bitmap_459c70* bitmap, List_459c70* list,
    int kind, int useColor)
{
    Poly_459c70 poly[25];
    float accum[2000][3];
    float normal[2000][3];
    int weight[2000];
    Poly_459c70 vertex[2000];

    Bitmap_459c70* src;
    int mode;
    if (((*(unsigned char*)(g_game + 0x37f06) >> 1) & 1) != 0
        && (list->owner->field_110 & 0x20000000) != 0
        && useColor != 0) {
        mode = 1;
        src = bitmap;
        Bitmap_459c70* shadow = this->shadow;
        shadow->width = (unsigned short)(src->width << 1);
        shadow->height = (unsigned short)(src->height << 1);
        shadow->unknown_9[0] = 0;
        shadow->colorKey = 1;
        shadow->field_4 = (short)(src->field_4 << 1);
        shadow->field_6 = (short)(src->field_6 << 1);
        memset(shadow->data2, 0, shadow->width * shadow->height);
        memset(shadow->data, 1, shadow->width * shadow->height);
        bitmap = shadow;
    } else {
        src = bitmap;
        mode = 0;
    }

    for (int p = list->count - 1; p >= 0; p--) {
        Piece_459c70* piece = &list->pieces[p];
        unsigned char pflags = piece->flags;
        if ((pflags & 1) == 0)
            continue;
        if (!(useColor == -1 || useColor == ((pflags >> 1) & 1)
                || list->owner->field_104 != 0.0f))
            continue;

        PieceInfo_459c70* info = piece->info;
        Vec3* verts = piece->vertices;
        int n = info->vertexCount;
        int shade = 0;
        if (n > 0) {
            int offX = (short)bitmap->field_4;
            int offY = (short)bitmap->field_6;
            for (int z1 = 0; z1 < n; z1++)
                weight[z1] = 0;
            for (int k = 0; k < n; k++) {
                int x;
                int y;
                int z;
                if (mode) {
                    x = (short)(verts->x >> 16) << 1;
                    y = (short)(verts->y >> 16) << 1;
                    z = (short)(-verts->z >> 16) << 1;
                    vertex[k].x = x;
                    vertex[k].y = z - (y >> 1);
                    vertex[k].z = (y / 2) + shade_bias(list);
                } else {
                    x = (short)(verts->x >> 16);
                    y = (short)(verts->y >> 16);
                    z = (short)(-verts->z >> 16);
                    vertex[k].x = x;
                    vertex[k].y = z - (y >> 1);
                    vertex[k].z = y + shade_bias(list);
                }
                vertex[k].shade = shade & 0x1f;
                vertex[k].x += offX;
                vertex[k].y += offY;
                accum[k][0] = 0.0f;
                accum[k][1] = 0.0f;
                accum[k][2] = 0.0f;
                shade += 3;
                verts++;
            }
        }

        int skip = (info->firstFace != -1) ? 1 : 0;
        for (int fi = skip; fi < info->faceCount; fi++) {
            Face_459c70* face = info->faces + fi;
            unsigned short* idx = face->indices;
            if (idx[0] == idx[1] || idx[1] == idx[2] || idx[2] == idx[0]) {
                normal[fi][0] = 0.0f;
                normal[fi][1] = 1.0f;
                normal[fi][2] = 0.0f;
            } else {
                Vec3* pv = piece->vertices;
                Vec3 p0 = pv[idx[1]];
                Vec3 p1 = pv[idx[0]];
                Vec3 p2 = pv[idx[2]];
                Vec3f a = FUN_004b6f00(p0, p1);
                Vec3f b = FUN_004b6f00(p0, p2);
                Vec3f c = FUN_004b6f70(a, b);
                Vec3f d = FUN_004b6ff0(c);
                normal[fi][0] = d.x;
                normal[fi][1] = d.y;
                normal[fi][2] = d.z;
            }
        }

        {
            Face_459c70* f = info->faces;
            int i = 0;
            if (info->firstFace != -1) {
                f++;
                i = 1;
            }
            for (; i < info->faceCount; i++, f++) {
                unsigned short* idx = f->indices;
                for (int j = 0; j < f->count; j++) {
                    int k = idx[j];
                    accum[k][0] += normal[i][0];
                    accum[k][1] += normal[i][1];
                    accum[k][2] += normal[i][2];
                    weight[k]++;
                }
            }
        }

        for (int q1 = 0; q1 < n; q1++) {
            if (weight[q1] != 0) {
                float w = (float)weight[q1];
                accum[q1][0] /= w;
                accum[q1][1] /= w;
                accum[q1][2] /= w;
            }
        }

        {
            Face_459c70* f = info->faces;
            int i = 0;
            if (info->firstFace != -1) {
                f++;
                i = 1;
            }
            for (; i < info->faceCount; i++, f++) {
                int cnt = f->count;
                unsigned short* idx = f->indices;
                for (int j = 0; j < cnt; j++) {
                    int k = idx[j];
                    poly[j] = vertex[k];
                    if ((piece->flags & 4) != 0) {
                        float v = accum[k][0] * DAT_005065f8 + accum[k][1] * DAT_005065fc
                            + accum[k][2] * DAT_00506600;
                        poly[j].shade = ((int)(v * DAT_004fd4cc)) & 0x1f;
                    } else {
                        poly[j].shade = 0xf;
                    }
                }
                unsigned int fflags = f->flags;
                if ((fflags & 1) != 0) {
                    FUN_004c0c70(bitmap, poly, f->count, f->unknown_0);
                } else if (f->count == 4) {
                    void* pic;
                    if ((fflags & 2) != 0) {
                        if ((fflags & 4) != 0) {
                            int unit = *(int*)(g_game + 0x1b8a + kind * 0x14b);
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
            }
        }
    }

    if (((*(unsigned char*)(g_game + 0x37f06) >> 1) & 1) != 0 && mode != 0) {
        FUN_004b95a0(bitmap, src);
        char* s = src->data2;
        if (s != 0) {
            char* d = bitmap->data2;
            for (int y = 0; y < src->height; y++) {
                for (unsigned int x = src->width; x != 0; --x) {
                    *s++ = *d;
                    d += 2;
                }
                d += bitmap->width;
            }
        }
    }
}
