// Decompiled by longcat-2.5-preview-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash retry (#3858): re-baselined at 71.0% / 1050 bytes, all
// four documented wall hunks unchanged (ebx list load hoisted above the
// 0x20000000 test, saved bitmap slot 0x24 vs original 0x20, the missing
// [esp+0x5f18] write-back, and the src/useColor/list register rotation).
// deepseek-v4.1-flash retry (#3631), finished by deepseek-v4.1-flash: 66.8 -> 71.0.
// The lever is the face-block branch layout: the original emits
// `test al,1 / jne <clip>` at 0x459b12 with the 4-vertex pic path as the
// fall-through and FUN_004c1000 out of line at 0x459b9d, so the source is
// `if ((flags & 1) == 0) { if (face->count == 4) { ... FUN_004c8760 ... } }
// else { FUN_004c1000(...); }`, NOT the `if (flags & 1) { clip } else
// if (count == 4) {...}` form (which scores 66.8 and 8 bytes shorter).
// Still differs: bitmap (the parameter, reassigned to this->shadow) stays in
// ebp where the original reloads it from its home [esp+0x5f18] at 0x45999f,
// 0x459b8e, 0x459ba3 and 0x459c03; list is in edx not esi, fi in ebx not
// edi, the vertex-copy pointers edx/edi are swapped, and the spill slots are
// counter 0x20 / src 0x24 (original sr 0x20 / counter 0x24).
// Also tried in this pass and DID NOT help: the tail downsample reading the
// row count from src->height and the row width from src->width (the shapes
// 0x459c1d/0x459c25 read), 57.0; the same with count locals, 63.6; a
// `while (x-- > 0)` inner loop, 66.0; moving `src = bitmap; bitmap = shadow;`
// before the two memsets (so the home store lands at the original 0x4598d3),
// 65.6; Face_459c70::flags as a 1/1/1/29 bitfield and the bit 1/bit 2 tests
// written as shifts, both byte-identical to the masks (66.8 then).
// deepseek-v4.1-flash retry (#2889): 62.0 -> 66.8. The lever is the source
// position of `src = bitmap`. Declaring `Bitmap_459c70* src;` uninitialised
// at function scope and assigning it late in the shadow branch, immediately
// before `bitmap = shadow;`, makes MSVC give useColor ebx (matching the
// original, 0x45985b / 0x459b08) instead of esi. Reads in the shadow setup
// must stay on `bitmap` (so `src = bitmap` can be sunk to the end of the
// branch, and the compiler hoists the src store to 0x4598c0 as in the
// original). Assigning src at the top of the branch (A) is 61.2; moving the
// `bitmap = shadow` store before the memsets (E) is 61.4; dropping the shadow
// local and reassigning bitmap early (K/R) is 55.7/58.6. What still differs:
// list lands in edx (original esi, reloaded at 0x459951), the saved bitmap
// stays in ebp after the branch where the original reloads it from
// [esp+0x5f18] (0x45999f), so the vertex-loop bitmap loads and the face-loop
// induction (fi edi vs ebx) rotate; the src slot is [esp+0x24] and the p
// counter [esp+0x20] (original src 0x20, counter 0x24). Same allocator family
// as sibling 0x458fa0.
// deepseek-v4.1 retry (#2650): the 62.0% wall is still the src/useColor
// register home, and the new experiments say it is not source-reachable from
// this file. `src` and the `bitmap` parameter are copy-propagated into ONE
// variable: writing `bitmap->width` or `bitmap->width` in the save branch gives
// byte-identical output (62.0, 1037 bytes). `Bitmap* src;` uninitialised and
// assigned in the branch, and a separate `sh`/`dst` local for this->shadow,
// both give 61.2 (1047 bytes); taking src's address (address-taken forces a
// memory home) gives 61.0, which shows the enregistration of the bitmap
// value, not the source shape, drives the rotation. An extra throwaway
// `useColor != (int)0x80000000` compare scored 62.7 but did NOT move
// useColor out of esi, so that 0.7 is diff alignment, not progress (not
// kept). Same throwaway inside the p-loop guard: 55.1. Splitting the guard
// into `int c = ...; if (useColor != -1 && useColor != c && owner->f != 0)
// continue;` is byte-identical to the || form (62.0). What still differs:
// original has useColor in ebx (reloaded after the vertex and face arms),
// list in esi (reloaded at the p-loop top), the saved bitmap in [esp+0x20]
// memory only, and the vertex loop reads offX/offY from the [esp+0x5f18]
// slot; ours keeps bitmap/src in ebp, useColor in esi, list in edx, and
// puts the counter at [esp+0x20] and the src spill at [esp+0x24].
// deepseek-v4.1-flash retry (#2532): consolidated the attempts below and
// confirmed the 62.0% wall is one whole-register rotation, not a missing
// source shape. The original keeps list in esi and useColor in ebx (list
// reloaded at the top of the p-loop, useColor reloaded after the vertex loop
// clobbers ebx as offX); ours keeps useColor in esi and spills list, so the
// p-loop, face index, vertex-copy and tail-copy blocks all rotate registers
// (dest/index and edi/edx swap) and the src/counter slots are 0x24/0x20
// instead of 0x20/0x24. New experiments, all worse or equal and none flips
// the rotation: #include <windows.h> 39.3% (and our vertex loop already
// matches without it, unlike sibling 0x458fa0); uninitialised `Bitmap* src;`
// assigned inside the branch, plus a function-scope p, 61.2%; `p` declared
// at function scope before or after src, 62.0%; `while (p-- > 0)` counter
// form, 62.0%. The one lever the guide records for this class (an extra loop
// level / goto label changing loop-nesting register priority, 0x40e160) and
// swapping the src/p declaration order did not move it. This is the same
// unreachable allocator tie that sibling 0x458fa0 documents at 84.1%; treat
// it as compiler state from the original file, not this function's text.
// Partial, 62.0% (real check, deepseek-v4.1 retry). Matched: the
// g_game+0x37f06 shadow flag as an unsigned-short bitfield gives the
// original's `shr dl,1; test dl,1`; the owner shade helper with a bool gives
// `shr edx,0x1e; and dl,1; neg dl; sbb edx,edx`; the vertex loop is a plain
// `for` with the offX/offY reads written as `(short)bitmap->field_4/6` inside
// the body (removed the explicit `if (n > 0)`, whose guard MSVC duplicated);
// and `src = bitmap` initialised at function scope plus the declaration order
// `int mode; Bitmap* src = bitmap;` moved the early bitmap load up to match.
// Remaining: useColor is still live in esi where the original has list/bitmap
// in esi and useColor in ebx, so the p-loop, firstFace, vertex and face blocks
// all rotate their registers (list edx vs esi, fi ebx vs edi, offX/offY
// swapped); the stack slot for info (0x24 vs 0x1c) and src (0x24 vs 0x20)
// differ; the face flags test keeps the clip path as fall-through where the
// original puts it out of line. Tried and did NOT help: nested ifs for the
// shadow condition (drops to 57.9), useColor-tested-first (59.3), a second
// `Bitmap* bmp`/`List* lp` local, an `int uc = useColor` local, and `src`
// declared before `mode` (all 61.2 or less). deepseek-v4.1 retry: the wall
// is src/useColor register allocation. Original stores the saved bitmap to
// a memory slot (mov [esp+0x20],esi at 0x45988f) and reloads it in the tail,
// and useColor lives in ebx (reloaded at 0x459b12 after the vertex loop
// clobbers ebx with offX). Ours always enregisters src: ebp when
// `src = bitmap` is at function scope (baseline), ebx when the assignment
// moves inside the branch (61.2), and ebx again when src is a 1-element
// array or when the branch reads bitmap-> instead of src-> (61.2); giving
// src an initialiser `= 0` drops to 54.5 by shifting the counter slot, and
// writing the face vertex copy as `poly[j] = vertex[face->indices[j]]`
// (which is what the original's edi/edx induction shape looks like) drops
// to 55.3, so the moving idx/q pointers stay. Swapping the src/shadow
// declaration order changes nothing (62.0). useColor ends up in esi in all
// variants, so the p-loop, vertex, face and tail blocks rotate registers.
// The four locals sit at 0x14 mode, 0x18 pieces, 0x1c info, 0x20 src,
// 0x24 counter in the original; ours puts the counter at 0x20 and src
// (spill) at 0x24.
// deepseek-v4.1-flash retry (#3241): hoisting the loop counter `p` to
// function scope before `src` (so both are plain locals in the original's
// declaration order) is byte-identical at 66.8, the two slots still come out
// counter 0x20, src 0x24, so the slot order follows the spill order, not the
// declaration order. Not kept.
#include <string.h>

extern char* g_game;
struct Bitmap_459c70;

struct Flags_459830 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short rest : 14;
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
    bool shadow = ((Flags_459830*)(g_game + 0x37f06))->b1;
    Bitmap_459c70* src;
    if (shadow
        && (list->owner->field_110 & 0x20000000) != 0
        && useColor != 0) {
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

    for (int p = list->count - 1; p >= 0; p--) {
        unsigned char pflags = list->pieces[p].flags;
        if ((pflags & 1) == 0)
            continue;
        if (!(useColor == -1 || useColor == ((pflags >> 1) & 1)
                || list->owner->field_104 != 0.0f))
            continue;

        PieceInfo_459c70* info = list->pieces[p].info;
        Vec3* verts = list->pieces[p].vertices;
        int n = info->vertexCount;
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
            if ((fflags & 1) == 0) {
                if (face->count == 4) {
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
            } else {
                FUN_004c1000(bitmap, poly, face->count, face->unknown_0);
            }
        }
    }

    if (((Flags_459830*)(g_game + 0x37f06))->b1) {
      if (mode != 0) {
        FUN_004b95a0(bitmap, src);
        char* s = src->data2;
        if (s != 0) {
            char* d = bitmap->data2;
            for (int y = 0; y < bitmap->height; y++) {
                for (unsigned int x = bitmap->width; x != 0; --x) {
                    *s++ = *d;
                    d += 2;
                }
                d += bitmap->width;
            }
        }
    }
}
    }
