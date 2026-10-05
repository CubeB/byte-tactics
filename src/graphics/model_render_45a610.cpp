// Decompiled by space-bunny-free. Names are provisional.
// Builds the screen-space vertex list of every drawable piece of a model into
// a 2000-entry local array, then hands each piece's line segments to
// FillFlatPolygon (a bounding-box/line pass) through a 25-entry scratch buffer.
// Pieces are walked last to first: the `dec ecx; js` guard, the `inc ecx`
// after it, and the `dec ecx; jne` latch together mean the walk starts at
// pieces[count-1], runs exactly count times and stops at pieces[0].
//
// The piece induction variable only lands on the element base (lea ..+0x22,
// with the fields at +0x28, +0x22 and +0) when the piece pointer is a loop
// variable updated next to the index (`piece--` beside `i--`) inside a
// guarded block. A plain indexed `for` over model->pieces[i] anchors on
// piece->flags (+0x4a) instead, and a pointer initialised in the `for` header
// computes it before the `js` guard, which the original does not.
//
// The segment index has to be a local of the function itself, initialised
// there, and not one of the flags block: MSVC orders the locals of the block
// that follows the vertex copy loop by scope depth, so with the index declared
// inside `if (flags & 2)` the two reloads out of that loop come out
// `mov ebx,[info]` then `mov edi,[segno]`, where the original reloads the
// index into edi first. A bare `int segno;` at the top keeps the old order;
// only the initialised declaration moves it ahead of `info`.
//
// Every byte now matches. What is left is the name the linker puts on the
// stack probe at +6: the original calls the function at 0x4e4b20, which
// data/symbols.csv calls _alloca_probe, while this object references
// __chkstk. They are the same code: toolchain/msvc5-sp3/LIB/CHKSTK.OBJ
// defines both names at the same offset in .text, and the exe's 0x4e4b20 is
// that object byte for byte. MSVC 5 only emits _alloca_probe for a function
// that contains an alloca, and an alloca also forces `push ebp; mov ebp,esp`
// into the prologue, which the original does not have, so no source of this
// function can reference that name. __chkstk is the only name reachable here,
// with or without the sp3 patch, and the same holds for the other large
// frame functions in the exe (20 call sites of 0x4e4b20, each 5 bytes into
// its function, i.e. in the prologue).
//
// Rechecked in #321: `char buf[8000];` compiles to `call __chkstk` and
// `_alloca(n)` to `call __alloca_probe`, and every N from 0 to 400 unused
// `extern int` declarations leaves this file byte-identical. With the line
// `_chkstk,0x4e4b20` added to data/aliases.csv (or 0x4e4b20 renamed to
// `_chkstk` in data/symbols.csv), check.py prints MATCH for this file as it
// is. That data change is the orchestrator's to make.
#include <string.h>

struct Vertex_0045a610 {
    int x;                           // screen x
    int y;                           // screen y
    int z;                           // height above the ground
};

struct View_0045a610 {
    short field_0;                   // width, read by FillFlatPolygon
    short field_2;                   // height, read by FillFlatPolygon
    short field_4;                   // x origin, added to every vertex x
    short field_6;                   // y origin, added to every vertex y
};

struct Segment_0045a610 {
    int unknown_0;
    int count;                        // +0x4 number of vertices in the segment
    int unknown_8;
    unsigned short* indices;          // +0xc indices into the vertex array
    char unknown_10[0x20 - 0x10];
};

struct PieceInfo_0045a610 {
    char unknown_0[4];
    int vertexCount;                  // +0x4
    int segmentCount;                 // +0x8
    int field_c;                      // +0xc -1 draws the first segment set
    char unknown_10[0x28 - 0x10];
    Segment_0045a610* segments;       // +0x28
};

#pragma pack(push, 1)
struct Piece_0045a610 {
    PieceInfo_0045a610* info;         // +0x0
    char unknown_4[0x22 - 0x4];
    Vertex_0045a610* vertices;        // +0x22 (16.16 fixed point)
    char unknown_26[0x28 - 0x26];
    unsigned short flags;             // +0x28 bit 0 and bit 1
    char unknown_2a[0x36 - 0x2a];
};

struct Model_0045a610 {
    int pieceCount;                   // +0x0
    char unknown_4[0x22 - 0x4];
    Piece_0045a610 pieces[1];         // +0x22
};
#pragma pack(pop)

void __stdcall FillFlatPolygon(View_0045a610* view, Vertex_0045a610* verts, int field_10, int count);

// A method whose `this` is never used: its caller 0x45a790 loads ecx before
// the call. It compiles the same as a __stdcall free function.
class Class_0045a610 {
public:
    void DrawShadowShape(View_0045a610* view, Model_0045a610* model);
};

// FUNCTION: 0x45a610
void Class_0045a610::DrawShadowShape(View_0045a610* view, Model_0045a610* model)
{
    Vertex_0045a610 verts[2000];
    Vertex_0045a610 tmp[25];
    int segno = 0;                    // the initialiser only sets the scope, see above
    int i = model->pieceCount - 1;
    if (i >= 0) {
        Piece_0045a610* piece = &model->pieces[i];
        while (i >= 0) {
            if (piece->flags & 1) {
                if (piece->flags & 2) {
                    PieceInfo_0045a610* info = piece->info;
                    Vertex_0045a610* v = piece->vertices;
                    for (int j = 0; j < info->vertexCount; j++) {
                        int y = (short)(v->y >> 16);
                        verts[j].x = (short)(v->x >> 16) + (y >> 2);
                        verts[j].y = (short)(-v->z >> 16) - (y >> 2);
                        verts[j].z = y + 25;
                        verts[j].x += view->field_4;
                        verts[j].y += view->field_6;
                        v++;
                    }
                    Segment_0045a610* seg = info->segments;
                    if (info->field_c != -1) {
                        seg++;
                        segno = 1;
                    } else {
                        segno = 0;
                    }
                    // A segment with more than 25 vertices overruns tmp[].
                    for (; segno < info->segmentCount; segno++, seg++) {
                        unsigned short* ip = seg->indices;
                        for (int k = 0; k < seg->count; k++, ip++) {
                            tmp[k] = verts[*ip];
                        }
                        FillFlatPolygon(view, tmp, seg->count, 0);
                    }
                }
            }
            piece--;
            i--;
        }
    }
}
