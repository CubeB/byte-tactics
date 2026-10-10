// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free, verified by GPT-6., retried by Claude Opus 5.5, matched by claude-opus-5-5. Names are provisional.
// Keep <windows.h> and fewer than 58 symbols after it: the symbol count sets
// the register choice.
#include <windows.h>

struct Vertex_0045a610 {
    int x;
    int y;
    int z;
};

struct GafFrame {
    short width;                     // +0x0
    short height;                    // +0x2
    short xOffset;                   // +0x4, added to every vertex x
    short yOffset;                   // +0x6, added to every vertex y
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
    int firstFace;               // +0xc -1 skips the first face
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

void __stdcall DrawPolygonEdges(GafFrame* view, Vertex_0045a610* points, int count, int color);

// A method that ignores `this`: its one caller (0x458dd0, MATCH) passes its own
// `this` through in ecx, and spells the parameters (image, model, palette).
// It stays in its own file: merged, one of the copy loop's two pointer loads
// is scheduled before the other, one instruction off.
class CMemoryCache {
public:
    void DrawPieceEdges(GafFrame* view, Model_00458fa0* model, int color);
};

// FUNCTION: 0x458fa0
void CMemoryCache::DrawPieceEdges(GafFrame* view, Model_00458fa0* model, int color)
{
    Vertex_0045a610 verts[2000];
    Vertex_0045a610 tmp[25];
    int faceno = 0;
    int i = model->pieceCount - 1;
    if (i >= 0) {
        Piece_00458310* piece = &model->pieces[i];
        while (i >= 0) {
            // Byte flags local plus the empty do-while set the reload order after the copy loop.
            unsigned char flags = piece->flags;
            do {} while (0);  // emits no code; the match needs it (header note 2)
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
                    verts[j].x += view->xOffset;
                    verts[j].y += view->yOffset;
                }
                // f is assigned in both arms, not set before the if.
                Face_00458fa0* f;
                if (info->firstFace != -1) {
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
                    DrawPolygonEdges(view, tmp, f->count + 1, color);
                }
            }
            piece--;
            i--;
        }
    }
}
