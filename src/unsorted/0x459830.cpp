// Decompiled by longcat-2.5-preview-free, finished by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// Partial, 51.1%: `else` branch is only `mode = 0;` (no src store), `src` assigned
// only in the shadow arm, and the shift/mask bit tests corrected.
// Remaining differences: MSVC folds ((flag>>1)&1) to `test dl,2` where the
// original keeps `shr dl,1; test dl,1`; the same fold turns the shade bias
// `(v>>30)&1` into `and esi,0x40000000`; the vertex-projection loop keeps n in
// a stack slot instead of ebp and swaps offX/offY register roles; the piece
// pointer base uses 0x44 instead of 0x22; the face-clip and bitmap-copy loops
// use different registers throughout.
#include <string.h>

extern char* g_game;
struct Bitmap_459c70;

struct Vec3 { int x; int y; int z; };

void* __stdcall FUN_004b7ee0(void* pic);
void* __stdcall FUN_004b7f30(unsigned short* table, int index);
void __stdcall FUN_004b95a0(Bitmap_459c70* dst, Bitmap_459c70* src);
void __stdcall FUN_004c1000(Bitmap_459c70* surface, void* poly, int count, int flag);
void __stdcall FUN_004c8760(Bitmap_459c70* surface, void* pic, void* poly, int flag);

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

struct Class_004581e0 {
    char unknown_0[0x10];
    Bitmap_459c70* shadow;           // +0x10
    void FUN_00459830(Bitmap_459c70* bitmap, List_459c70* list, int kind, int useColor);
};

static __inline int shade_bias(List_459c70* list) { return ((*(unsigned int*)(list->owner->field_92+0x241)>>30)&1)?125:50; }
// FUNCTION: 0x459830
void Class_004581e0::FUN_00459830(Bitmap_459c70* bitmap, List_459c70* list,
    int kind, int useColor)
{
    Vec3 vertex[2000];
    Vec3 poly[25];

    Bitmap_459c70* src;
    int mode;
    if (((*(unsigned char*)(g_game + 0x37f06) >> 1) & 1) != 0
        && (list->owner->field_110 & 0x20000000) != 0
        && useColor != 0) {
        Bitmap_459c70* shadow = this->shadow;
        mode = 1;
        src = bitmap;
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
        if (n > 0) {
            int offY = (short)bitmap->field_6;
            int offX = (short)bitmap->field_4;
            for (int k = 0; k < n; k++) {
                int x;
                int y;
                int z;
                if (mode) {
                    x = (short)(verts->x >> 16) << 1;
                    y = (short)(verts->y >> 16) << 1;
                    z = (short)(-verts->z >> 16) << 1;
                } else {
                    x = (short)(verts->x >> 16);
                    y = (short)(verts->y >> 16);
                    z = (short)(-verts->z >> 16);
                }
                vertex[k].x = x;
                vertex[k].y = z - (y >> 1);
                if (mode) vertex[k].z = y/2 + shade_bias(list);
                else vertex[k].z = y + shade_bias(list);
                vertex[k].x += offX;
                vertex[k].y += offY;
                verts++;
            }
        }

        Face_459c70* face = info->faces;
        int fi = 0;
        if (info->firstFace != -1) {
            face++;
            fi = 1;
        }
        for (; fi < info->faceCount; fi++, face++) {
            unsigned short* idx = face->indices;
            Vec3* q=poly;
            for (int j = 0; j < face->count; j++, q++, idx++) {
                *q = vertex[*idx];
            }
            unsigned int fflags = face->flags;
            if ((fflags & 1) != 0) {
                FUN_004c1000(bitmap, poly, face->count, face->unknown_0);
            } else if (face->count == 4) {
                void* pic;
                if ((fflags & 2) != 0) {
                    if ((fflags & 4) != 0) {
                        int unit = *(int*)(g_game + 0x1b8a + kind * 0x14b);
                        pic = FUN_004b7f30(face->color,
                            *(unsigned char*)(unit + 0x96));
                    } else if (useColor) {
                        pic = FUN_004b7f30(face->color, 0);
                    } else {
                        pic = FUN_004b7ee0(&face->pic);
                    }
                } else {
                    pic = face->pic;
                }
                FUN_004c8760(bitmap, pic, poly, 0);
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
