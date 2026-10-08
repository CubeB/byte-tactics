// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by
// claude-opus-5-5 (#4634), finished by deepseek-v4.1-flash, finished by
// space-bunny-free, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished
// by Fledge Alpha Free. Names are provisional.
// Callers (0x4584b4, 0x458997, 0x4593ff, 0x459476) push the surface pointer as
// the second argument and the address of a 12-byte {x,y,z} struct as the
// third, so the argument order is (model, surface, camera, info, vertices,
// palette, useColor).

// Must include <string.h>: dropping it changes the generated code.
#include <string.h>
extern char* g_game;

#pragma pack(push, 1)
struct View_4584d0 {
    char unknown_0[0x6a];
    int originX;                             // +0x6a
    int originY;                             // +0x6e
    int originZ;                             // +0x72
};
#pragma pack(pop)

struct Model_4584d0 {
    char unknown_0[0xc];
    View_4584d0* view;                       // +0xc
};

struct Point_4584d0 { int x; int y; };
struct Vertex_4584d0 { int x; int y; int z; };
struct Vec3_4584d0 { int x; int y; int z; };

struct Pic_4584d0 {
    void* pic;                               // +0x0
    char unknown_4[4];
};

struct Flags_4584d0 {
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

struct Face_4584d0 {
    int unknown_0;                           // +0x00
    int count;                               // +0x04
    int unknown_8;                           // +0x08
    unsigned short* indices;                 // +0x0c
    Pic_4584d0 pic;                          // +0x10
    unsigned short* color;                   // +0x18
    Flags_4584d0 flags;                      // +0x1c
};

struct PieceInfo_4584d0 {
    char unknown_0[4];
    int vertexCount;                         // +0x04
    int faceCount;                           // +0x08
    int firstFace;                           // +0x0c
    char unknown_10[8];
    unsigned short* color;                   // +0x18
    char unknown_1c[0xc];
    Face_4584d0* faces;                      // +0x28
};

void* __stdcall GetGafSequenceFrame(Pic_4584d0* ref);
void* __stdcall GetGafFrame(unsigned short* table, int index);
void __stdcall FillPolygon(void* surface, Point_4584d0* points, int count, int flags);
void __stdcall DrawFrameQuad(void* surface, void* pic, Point_4584d0* points, void* src);

class CMemoryCache {
public:
    void DrawPiece(Model_4584d0* model, void* surface, Vec3_4584d0* camera,
        PieceInfo_4584d0* info, Vertex_4584d0* vertices, unsigned int palette,
        int useColor);
};

// Needed: the copy loop guard goes through this helper.
static inline int FaceCount(Face_4584d0* face) { return face->count; }

// FUNCTION: 0x4584d0
void CMemoryCache::DrawPiece(Model_4584d0* model, void* surface,
    Vec3_4584d0* camera, PieceInfo_4584d0* info, Vertex_4584d0* vertices,
    unsigned int palette, int useColor)
{
    void* pic;
    int i;
    int unit;
    // Array sizes 2000 and 25 fix the frame size; do not change them.
    Point_4584d0 projected[2000];
    Point_4584d0 poly[25];
    View_4584d0* view = model->view;
    // Field order a, y, b sets the spilled field's frame slot.
    struct Off_4584d0 { int a; int y; int b; };
    Off_4584d0 off;
    off.a = view->originX - camera->x;
    off.y = view->originY;
    off.b = view->originZ - camera->z;
    {
        // Walks the vertices parameter itself, not a local copy of it.
        for (i = 0; i < info->vertexCount; i++, vertices++) {
            projected[i].x = 0x80 + (short)((vertices->x + off.a) >> 16);
            projected[i].y = (0x20 + ((short)((off.b - vertices->z) >> 16)
                - ((short)((off.y + vertices->y) >> 16) >> 1)));
        }
    }
    Face_4584d0* face;
    if (info->firstFace != -1) {
        face = info->faces + 1;
        i = 1;
    } else {
        face = info->faces;
        i = 0;
    }
    if (i < info->faceCount) do {
        int j;
        unsigned short* p;
        j = 0, p = face->indices;
        // The while (1) shell and the count local are needed; other loop forms differ.
        if (FaceCount(face) > j) {
            while (1) {
                poly[j] = projected[*p];
                j++, p++;
                int count = face->count;
                if (j >= count)
                    break;
            }
        }
        Flags_4584d0 flags = face->flags;
        if (!flags.bits.a) {
            if (face->count != 4) goto skip0;
            if (flags.bits.b) {
                if (flags.bits.c) {
                    unit = *(int*)(((char*)g_game + 0x1b8a) + ((palette & 0xff) * 0x14b));
                    pic = GetGafFrame(face->color, *(unsigned char*)(unit + 0x96));
                } else pic = useColor ? GetGafFrame(face->color, 0) : GetGafSequenceFrame(&face->pic);
            } else pic = face->pic.pic;
            DrawFrameQuad(surface, pic, poly, 0);
skip0:;
        } else {
            FillPolygon(surface, poly, face->count, face->unknown_0);
        }
        i++, face++;
    } while (i < info->faceCount);
}