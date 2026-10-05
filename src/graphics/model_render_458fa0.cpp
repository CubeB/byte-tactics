// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free, verified by GPT-6., retried by Claude Opus 5.5, matched by claude-opus-5-5. Names are provisional.
// MATCH (#5333), from 95.2%. Measured from the natural copy loop
// (`for (k = 0; k < f->count; k++, ip++)`, 84.1%, view in ebx):
//  1. The first-face choice assigns `f` in both arms (`f = info->faces + 1`
//     and `f = info->faces`) instead of `f = info->faces;` before the `if`
//     and `f++` in it. MSVC hoists the common load above the branch, so the
//     code is the same, but C2 counts a read of `info` in each arm: after
//     the copy-loop and vertex-loop pressure splits, info's piece and view's
//     piece reach the colouring at 232 each (info's was 160 against view's
//     240), info's goes first and takes ebx, and view is left with ebp, as
//     in the original. 84.1 -> 99.3. This replaces the old
//     `bool NoMorePoints()` guard, which got view out of ebx through C2's
//     byte-register check at the cost of two extra instructions.
//  2. The last difference was the reload order after the copy loop
//     (original faceno, view, info; ours view, faceno, info). C2 reloads
//     split pieces in the order of their candidate ids. Reading the flags
//     byte into `unsigned char flags` and an empty `do {} while (0);` after
//     it (most likely a debug macro that compiled to nothing; `if (0) {}`
//     and `while (0) {}` work too, `;` and `{}` do not) put them in the
//     original's order; each alone stays at 99.3%. Found by tools/permute.py
//     (do_while0 plus a byte temporary for the flags) and minimised by hand.
//     A byte colour parameter (`unsigned char` here and in the FUN_004c0820
//     prototype) also matches without these two, but 0x4c0820's own notes
//     found `int color` better in its definition, so it is not used here.
//     With an int colour and neither change, none of these moved the reload:
//     a local copy of the colour, declaration order or scope of faceno, f,
//     ip, k, j, v and info, the face loop's latch order or a `while` form,
//     a named `f->count + 1`, single-use locals in either loop, and any
//     count of unused externs (before or after the function).
// The symbol count matters with a period of 128: 0 to 57 unused externs
// after <windows.h> match, about 60 to 122 give 81.4% (453 B), and so on.
// No header, the lean <windows.h>, <math.h> or <vector> land in the bad
// half; <windows.h> alone (or with <ddraw.h>, <memory.h> or
// stdlib/string/stdio) in the good one.
#include <windows.h>

struct Vertex_0045a610 {
    int x;
    int y;
    int z;
};

struct View_0045a610 {
    short field_0;                   // width
    short field_2;                   // height
    short field_4;                   // x origin, added to every vertex x
    short field_6;                   // y origin, added to every vertex y
};

struct Face_00458fa0 {
    int unknown_0;
    int count;                       // +0x4 number of points in the face
    int unknown_8;
    unsigned short* indices;         // +0xc indexes into the point array
    char unknown_10[0x20 - 0x10];
};

struct PieceInfo_00458fa0 {
    char unknown_0[4];
    int vertexCount;                 // +0x4
    int faceCount;                   // +0x8
    int field_c;                     // +0xc -1 skips the first face
    char unknown_10[0x28 - 0x10];
    Face_00458fa0* faces;            // +0x28
};

#pragma pack(push, 1)
struct Piece_00458310 {
    PieceInfo_00458fa0* info;         // +0x0
    char unknown_4[0x22 - 0x4];
    Vertex_0045a610* vertices;        // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned char flags;              // +0x28
    char unknown_29[0x36 - 0x29];
};

struct Map_00458fa0 {
    char unknown_0[0x241];
    unsigned int unknown_241 : 30;
    unsigned int brightFaces : 1;    // +0x241 bit 30, the wider field
    unsigned int unknown_242 : 1;
};

struct Owner_00458fa0 {
    char unknown_0[0x92];
    Map_00458fa0* map;               // +0x92
};

struct Model_00458fa0 {
    int pieceCount;                   // +0x0
    char unknown_4[0xc - 0x4];
    Owner_00458fa0* owner;            // +0xc
    char unknown_10[0x22 - 0x10];
    Piece_00458310 pieces[1];         // +0x22
};
#pragma pack(pop)

void __stdcall FUN_004c0820(View_0045a610* view, Vertex_0045a610* points, int count, int color);

// A method that ignores `this`: its one caller (0x458dd0, MATCH) passes its own
// `this` through in ecx, and spells the parameters (image, model, palette).
class Class_00458fa0 {
public:
    void FUN_00458fa0(View_0045a610* view, Model_00458fa0* model, int color);
};

// FUNCTION: 0x458fa0
void Class_00458fa0::FUN_00458fa0(View_0045a610* view, Model_00458fa0* model, int color)
{
    Vertex_0045a610 verts[2000];
    Vertex_0045a610 tmp[25];
    int faceno = 0;
    int i = model->pieceCount - 1;
    if (i >= 0) {
        Piece_00458310* piece = &model->pieces[i];
        while (i >= 0) {
            unsigned char flags = piece->flags;
            do {} while (0);  // emits no code; without it 99.3% (header note 2)
            if (flags & 1) {
                PieceInfo_00458fa0* info = piece->info;
                Vertex_0045a610* v = piece->vertices;
                for (int j = 0; j < info->vertexCount; j++, v++) {
                    int bright = model->owner->map->brightFaces ? 125 : 50;
                    int x = (short)(v->x >> 16);
                    int y = (short)(v->y >> 16);
                    int z = (short)(-v->z >> 16);
                    verts[j].x = x;
                    verts[j].y = z - (y >> 1);
                    verts[j].z = y + bright;
                    verts[j].x += view->field_4;
                    verts[j].y += view->field_6;
                }
                Face_00458fa0* f;
                if (info->field_c != -1) {
                    f = info->faces + 1;
                    faceno = 1;
                } else {
                    f = info->faces;
                    faceno = 0;
                }
                for (; faceno < info->faceCount; faceno++, f++) {
                    unsigned short* ip = f->indices;
                    int k;
                    for (k = 0; k < f->count; k++, ip++)
                        tmp[k] = verts[*ip];
                    tmp[k] = tmp[0];
                    FUN_004c0820(view, tmp, f->count + 1, color);
                }
            }
            piece--;
            i--;
        }
    }
}
