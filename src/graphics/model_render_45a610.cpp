// Decompiled by space-bunny-free. Names are provisional.
// Builds the screen-space vertex list of every drawable piece of a model into
// a 2000-entry local array, then hands each piece's line segments to
// FillFlatPolygon (a bounding-box/line pass) through a 25-entry scratch buffer.
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
    // Function-level and initialised: keeps the reload order after the vertex loop.
    int segno = 0;                    // the initialiser only sets the scope, see above
    int i = model->pieceCount - 1;
    if (i >= 0) {
        // Loop variable stepped beside the index: anchors on the element base.
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
