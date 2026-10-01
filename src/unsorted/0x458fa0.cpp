// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// 2026-10-01 deepseek-v4.1-flash retry (issue 2980): still 84.1%, the same
// single ebx/ebp swap documented above (ours: view=ebx, info=ebp; original:
// view=ebp, info=ebx). New levers tried this pass, all 84.1% or worse:
// the sibling renderer 0x459200's register-homing trick (a `bool bright =
// (field) & 1;` local feeding `bright ? 125 : 50`), an unsigned-dword read of
// the +0x241 flags word, operand swaps in every `+=`, an explicit destination
// pointer, reference-to-info, pointer-to-int info, `register` hints on view,
// info and v, const on the view parameter, parameter order swap (82.8), moving
// faceno/piece declarations between function and block scope (82.1), and
// adding <memory.h> / <string.h>. None moves the tie.
// GPT-6.1-sol retry (#1616): unchanged at 84.1%; the remaining mismatch is the documented ebx/ebp allocation swap.
// Best result: 84.1% (455 of 455 bytes). The body now compiles to the
// original except for one global register swap: MSVC puts the `view` pointer
// in ebx and the face `info` pointer in ebp, while the original keeps view in
// ebp (loaded between `push ebp` and `push ebx` in the preheader) and info in
// ebx. Everything else matches byte for byte: the frame size (0x5efc with
// `mov eax, 0x5efc; call __chkstk`), the local slots (faceno at E+0x10, piece
// at E+0x14, info at E+0x18, count at E+0x1c, tmp[25] at E+0x20, verts[2000]
// at E+0x14c), the backwards piece walk, both loop shapes, the batched vertex
// loads `[ecx]`, `[ecx+4]`, `[ecx+8]`, the destination pre-increment
// (`add eax, 0xc` with negative offsets), the branchless `? 125 : 50` shade
// kept in edi, the two `+=` reloads, the face start, the copy into tmp,
// `tmp[k] = tmp[0]`, the FUN_004c0820 call and the unaligned bit-30 flag at
// +0x241.
//
// How to get the body right: the vertex loop needs `v++` moved into the for
// header (`for (...; j++, v++)`) and one of the large headers at the top of
// the file. Without them MSVC sinks the z load and keeps the destination
// offsets positive, and the whole loop is laid out differently. With
// <windows.h> the loop is byte-identical apart from the ebx/ebp swap.
// `tools/headers.py` tried every combination of the standard headers and
// none flips that swap, so the cause is compiler state from the original
// source file's other contents, not this function's source. A renderer file
// in a DirectDraw game would have included <windows.h>/<ddraw.h> anyway.
//
// deepseek-v4.1-flash re-checked that conclusion and confirmed it. The swap
// survives every source lever tried in build/scratch/0x458fa0: caching
// vertexCount/faceCount/field_c in locals, hoisting or sinking the bright
// shade, swapping the two pointer declarations, while/for/do forms, reading
// view's offsets into locals or through accessors, address-taken pointers to
// view and info, extra live locals and dead uses to move the allocator, and
// reordering the vertex-array declarations. The N-declarations test (0 to
// 1000 unused `extern int`, step 1) only ever toggles between 84.1% and
// 66.9% (never MATCH), and headers.py --cpp (768 sets, including <string>,
// <vector>, <map>, <list>, <iostream>) finds no match either. Declaring the
// real preceding neighbour 0x458dd0 in the same file reproduces the 66.9%
// vertex-loop layout, not the swap, so the missing state is still elsewhere
// in the original file. /Gz, /Gr and /Ob1 change nothing; /G6 changes the
// layout and is worse. This is the one remaining diff, so treat it as
// compiler state, not a source shape.
//
// A second deepseek-v4.1-flash pass added: third parameter as `unsigned char`
// (worse, 81.8%), unsigned loop counters (80.7%), `piece->info` inline with no
// local (56.9%), a local alias `vp = view` used everywhere, a pointer-to-
// pointer intermediate for info, swapping the info/v declaration order, and
// moving the info declaration outside the while. Every one keeps the exact
// same 84.1% and the identical ebx/ebp swap, so the file stands at 84.1%.
//
// deepseek-v4.1 (2026) tried 12 more shapes; all stay at exactly 84.1% with
// the same ebx/ebp swap: inline accessors for info->faces, info->vertexCount,
// info->faceCount, info->field_c and for view->field_4/field_6 (the 0x4bcb50
// "one more use" trick), a View* alias declared first / after info / inside the
// loop / at function scope, info assigned at function scope, v before info,
// unsigned j, and an inline wrapper around the FUN_004c0820 call passing view.
// None of them moves the tie, so the swap is not reachable from this file's text.
// 2026-09-30 deepseek-v4.1: four more threads closed off. Renaming every local
// and both parameters, renaming the class, the method, and the two parameter
// struct types (all four change the mangled name and the symbol table) each
// still compile to exactly 84.1% with the identical ebx/ebp swap, so the tie is
// not name or hash-order sensitive. Swapping the info/v declaration order,
// hoisting both declarations out of the while loop, shrinking faceno's live
// range into the flags block, and using `int j;` declared at function scope are
// also all 84.1%. The one thing that does move the layout is the ABI: compiling
// the same body as a free `void __stdcall FUN_00458fa0(...)` instead of a
// __thiscall member shrinks it to 452 bytes and drops to 66.9% (the vertex loop
// loses the batched loads), so the implicit `this` variable is part of the
// allocator state, and this function must stay a member.
// 2026-09-30 second deepseek-v4.1 pass: 40 more runs, every one still exactly
// 84.1% with the same swap: register/const on the pointers, a reference
// parameter for view, void*/unsigned/long third param and callee prototype
// changes, a fresh view alias inside the loop, an explicit destination pointer
// (58.9%), all countdown and `!=` vertex-loop counter forms (62.3%, so the
// countdown shape is wrong), `d++` at the body end, `*tmp` and declaration
// order swaps in the copy loop, class virtuals and an empty base,
// `#pragma pack(push,1)` around every struct and at file scope, reordering the
// struct definitions, typedef signatures, and dummy type/function/data symbols
// of eight kinds. /Oa, /Oi- and /Og change nothing; /Os drops to 8.7%. The
// verdict stands: the ebx/ebp tie is not reachable from this file's text.
// Earlier note from space-bunny-free: several scratch variants scored 80-90%
// but are wrong; they lay the vertex arrays out 4 bytes high at [esp+0x150],
// where the original uses [esp+0x14c]. Always check the `lea eax, ...` base
// of verts before trusting a higher score.
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
    unsigned int brightFaces : 1;   // +0x241 bit 30, the wider field
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

void __stdcall FUN_004c0820(View_0045a610* view, Vertex_0045a610* points, int count, int param_4);

// A method that ignores `this`: its one caller (0x458dd0) passes its own
// `this` through in ecx.
class Class_00458fa0 {
public:
    void FUN_00458fa0(View_0045a610* view, Model_00458fa0* model, int param_3);
};

// FUNCTION: 0x458fa0
void Class_00458fa0::FUN_00458fa0(View_0045a610* view, Model_00458fa0* model, int param_3)
{
    Vertex_0045a610 verts[2000];
    Vertex_0045a610 tmp[25];
    int faceno = 0;
    int i = model->pieceCount - 1;
    if (i >= 0) {
        Piece_00458310* piece = &model->pieces[i];
        while (i >= 0) {
            if (piece->flags & 1) {
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
                Face_00458fa0* f = info->faces;
                if (info->field_c != -1) {
                    f++;
                    faceno = 1;
                } else {
                    faceno = 0;
                }
                for (; faceno < info->faceCount; faceno++, f++) {
                    unsigned short* ip = f->indices;
                    int k = 0;
                    for (; k < f->count; k++, ip++) {
                        tmp[k] = verts[*ip];
                    }
                    tmp[k] = tmp[0];
                    FUN_004c0820(view, tmp, f->count + 1, param_3);
                }
            }
            piece--;
            i--;
        }
    }
}
