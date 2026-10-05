// Decompiled by longcat-2.5-preview-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-opus-5-5, checked by GPT-6, finished by claude-opus-5-5. Names are provisional.
// Draws a unit model's pieces into a bitmap. With anti-aliasing on and the
// unit flagged, it draws into the doubled shadow bitmap instead and then
// downsamples that back into the caller's bitmap.
//
// MATCH (#5138). The last two changes, on top of #4924's head and tail:
//  * The piece flags are an `unsigned short` bitfield tested with positive
//    nested ifs (`if (list->pieces[p].flags.visible) { if (useColor == -1 ||
//    useColor == list->pieces[p].flags.colored || ...) { ... } }`), as in
//    0x459c70. Reading the info and vertices through a `piece` pointer
//    instead biases the walking pointer to +0x28 (98.2%).
//    The two bitfields are two loads to the global optimiser, and the code
//    generator reuses the `al` it already holds for the second, which costs
//    one more scratch register in the eax/ecx/edx rotation than a `pflags`
//    local or a mask test. That one step put the owner load at the field_104
//    test in ecx and the bitmap offsets' loads in the original's order.
//  * Each arm of the doubled-bitmap test reads `verts->x` itself. MSVC
//    hoists the identical first load of both arms above the `je` (after the
//    mode test), which is where the original has it; a separate
//    `x = verts->x;` before the test put the mode test in edx and loaded z
//    first in the else arm. (The earlier note that MSVC never hoists it was
//    measured with the rotation one step off.)
#include <string.h>

extern char* g_game;
struct Bitmap_459c70;

struct Flags_37f06 {
    unsigned short damagebars : 1;
    unsigned short antiAlias : 1;
    unsigned short rest : 14;
};

struct FaceFlags_459830 {
    union {
        unsigned int raw;
        struct {
            unsigned int a : 1;
            unsigned int b : 1;
            unsigned int c : 1;
            unsigned int rest : 29;
        } bits;
    };
};

struct Vec3 { int x; int y; int z; };

void* __stdcall GetGafSequenceFrame(void* pic);
void* __stdcall GetGafFrame(unsigned short* table, int index);
void __stdcall DownsampleFrame(Bitmap_459c70* dst, Bitmap_459c70* src);
void __stdcall FillFlatPolygon(Bitmap_459c70* surface, void* poly, int count, int flag);
void __stdcall DrawTexturedPolygon(Bitmap_459c70* surface, void* pic, void* poly, int flag);

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
    FaceFlags_459830 flags;          // +0x1c
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

struct Class_004581e0 {
    char unknown_0[0x10];
    Bitmap_459c70* shadow;           // +0x10
    void DrawPieces(Bitmap_459c70* bitmap, List_459c70* list, int kind, int useColor);
};

static __inline int shade_bias(Owner_459c70* owner)
{
    bool c = ((*(unsigned int*)(owner->field_92 + 0x241) >> 30) & 1) != 0;
    return c ? 125 : 50;
}

// FUNCTION: 0x459830
void Class_004581e0::DrawPieces(Bitmap_459c70* bitmap, List_459c70* list,
    int kind, int useColor)
{
    Vec3 vertex[2000];
    Vec3 poly[25];

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
                PieceInfo_459c70* info = list->pieces[p].info;
                int n = list->pieces[p].info->vertexCount;
                Vec3* verts = list->pieces[p].vertices;
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
                    if (mode) vertex[k].z = y/2 + shade_bias(list->owner);
                    else vertex[k].z = y + shade_bias(list->owner);
                    vertex[k].x += (short)bitmap->field_4;
                    vertex[k].y += (short)bitmap->field_6;
                    verts++;
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
                    for (int j = 0; j < face->count; j++, idx++) {
                        poly[j] = vertex[*idx];
                    }
                    FaceFlags_459830 fflags = face->flags;
                    if (!fflags.bits.a) {
                        if (face->count == 4) {
                            void* pic;
                            if (fflags.bits.b) {
                                if (fflags.bits.c) {
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
                            DrawTexturedPolygon(bitmap, pic, poly, 0);
                        }
                    } else {
                        FillFlatPolygon(bitmap, poly, face->count, face->unknown_0);
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
