// Decompiled by longcat-2.5-preview-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-opus-5-5, checked by GPT-6. Names are provisional.
// GPT-6 retry (#5025): checkall.py confirms the retained 96.5% / 1082-byte
// version. The sibling shape was measured and its stack-slot tradeoff is
// documented below.
//
// Partial, 96.5% at exactly 1082 bytes (issue #4924).
//
// CORRECTNESS FIX (#4924). The tail used to downsample over the doubled
// shadow bitmap's height and width, which writes past src. It now does what
// the original does (0x459c1d/0x459c25/0x459c4c read the saved src, 0x459c40
// steps d by the target bitmap's width):
//     for (int y = 0; y < src->height; y++) {
//         int x = src->width;
//         while (x--) { unsigned int c = *d; *s++ = c; d += 2; }
//         d += bitmap->width;
//     }
// The tail and the head now match the original instruction for instruction.
//
// WHAT MOVED IT (71.2% with the faithful tail, the old file's 78.9% was only
// reachable with the wrong tail):
//  1. The shadow head is two nested ifs with their own `mode = 0` else blocks,
//     and `src = bitmap; bitmap = shadow;` come LAST in the shadow branch, the
//     size reads going through bitmap. That alone gives the original's whole
//     allocation: useColor in ebx, list in esi at the piece loop, and src and
//     the target bitmap in memory (src at [esp+0x20], the bitmap in its
//     parameter slot). Every earlier pass had src/bitmap in ebx/ebp. (With
//     the `bool` flag local and an && chain it stays at 71%.)
//  2. The poly copy indexes the destination, `poly[j] = vertex[*idx]` with
//     `j++, idx++` in the increment: the original's `lea edi, [poly]` sits
//     after the loop guard, i.e. it is a strength-reduced `poly[j]`, not a
//     walking pointer.
//  3. `x = verts->x;` is read once before the mode test (the original's load
//     sits before the `je`; MSVC 5 never hoists it out of the arms).
//  4. `int n = list->pieces[p].info->vertexCount;` (through the piece, not
//     `info`) puts the post-loop reloads in the original's order.
//
// STILL DIFFERING (16 instructions, all local register choice):
//  * the owner load for the field_104 test is eax, the original's ecx;
//  * the vertex preheader loads field_4 first (into ebx) and field_6 second;
//    the original loads field_6 first but still gives field_4 ebx. With
//    explicit offX/offY locals the first ASSIGNED always gets ebx, whatever
//    the declaration order, and they must then sit under an explicit
//    `if (0 < n)` (`n > 0` there costs 18 points);
//  * the mode test uses edx with the x load before it; the original tests
//    mode in eax and loads x into eax after the test (0x459c70's original
//    has the same shape);
//  * the else arm loads z first where the original loads y first. All 36
//    statement orders of the two arms were scored: an arm order that fixes
//    the else arm breaks the mode arm and vice versa.
// Inert here: header sets (tools/headers.py, 256 sets), compiling 0x4597f0
// and 0x459170 first, unsigned char bitfields for the piece flags (identical
// code), a helper for the fixed-point conversion, `* 2` for `<< 1`,
// struct-copying the vertex, field_104 spellings and helpers. Compiling the
// real 0x459200 + 0x4597f0 first (the file order) drops this to 64.5% and
// moves the bitmap into ebp, so the remaining ties are probably decided by
// compiler state from the ~15 functions before this one. tools/permute.py
// (two 15-minute runs, ~25000 candidates) tops out at 97.1%: the same
// residual, scored differently, via a `short` temporary for z in the else
// arm (`short sz = (short)(-verts->z >> 16); ...; z = sz;`). Not kept.
#include <string.h>

extern char* g_game;
struct Bitmap_459c70;

struct Flags_459830 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
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

struct Piece_459c70 {
    PieceInfo_459c70* info;          // +0x00
    char unknown_4[0x22 - 0x04];
    Vec3* vertices;                  // +0x22
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

static __inline int shade_bias(Owner_459c70* owner)
{
    bool c = ((*(unsigned int*)(owner->field_92 + 0x241) >> 30) & 1) != 0;
    return c ? 125 : 50;
}
// FUNCTION: 0x459830
void Class_004581e0::FUN_00459830(Bitmap_459c70* bitmap, List_459c70* list,
    int kind, int useColor)
{
    Vec3 vertex[2000];
    Vec3 poly[25];

    int mode;
    Bitmap_459c70* src;
    if (((Flags_459830*)(g_game + 0x37f06))->b1) {
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
        unsigned char pflags = list->pieces[p].flags;
        if ((pflags & 1) == 0)
            continue;
        if (!(useColor == -1 || useColor == ((pflags >> 1) & 1)
                || list->owner->field_104 != 0.0f))
            continue;

        PieceInfo_459c70* info = list->pieces[p].info;
        int n = list->pieces[p].info->vertexCount;
        Vec3* verts = list->pieces[p].vertices;
        for (int k = 0; k < n; k++) {
            int x;
            int y;
            int z;
            x = verts->x;
            if (mode) {
                x = (short)(x >> 16) << 1;
                y = (short)(verts->y >> 16) << 1;
                z = (short)(-verts->z >> 16) << 1;
            } else {
                x = (short)(x >> 16);
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
            } else {
                FUN_004c1000(bitmap, poly, face->count, face->unknown_0);
            }
        }
    }

    if (((Flags_459830*)(g_game + 0x37f06))->b1) {
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
