// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Draws a piece of a 3D model: projects obj->count vertices through
// FUN_004b6cc0 (the same rotate/project idiom as the matched 0x467a50) into
// screen points, then draws each primitive in obj->prims (from index 1 when
// obj->field_c is not -1, otherwise from 0). A primitive whose bit 0 is set
// is a flat filled polygon (FillPolygon); otherwise a 4-vertex textured quad
// (DrawFrameQuad), whose texture is either the direct pointer at +0x10 or, when
// bit 1 is set, the entry GetGafSequenceFrame looks up from the reference at +0x10.
//
// MATCH. Two things the compiler only does when the per-iteration pointer
// updates sit in the loop's increment clause:
//
// 1. Both loops must put every increment in the `for` header
//    (`i++, v++, scratch++, points++` and `k++, idx++, dst++`). With the
//    pointer bumps written as statements at the end of the body, MSVC 5 keeps
//    the vertex pointer in ebp and spills the counter, which is one register
//    the wrong way round.
//
// 2. The primitive flag is a dword bitfield (`flag0`, `texIndexed`), not an
//    int tested with `>>`/`&`: a bitfield read gives the original's
//    `mov eax, [e+0x1c]; test al, 1` then `shr eax, 1; test al, 1`, while
//    `(flags >> 1) & 1` folds to `test cl, 2`.

struct Vec3_0046bae0 {
    int x;
    int y;
    int z;
};

struct Point_0046bae0 {
    int x;
    int y;
};

class Prim_0046bae0 {                  // a model primitive, 0x20 bytes
public:
    int field_0;                       // +0x00 color
    int count;                         // +0x04 number of vertices
    int field_8;                       // +0x08
    unsigned short* indices;           // +0x0c
    int field_10;                      // +0x10 texture pointer or index ref
    char unknown_14[8];
    unsigned int flag0 : 1;            // +0x1c bit 0
    unsigned int texIndexed : 1;       // +0x1c bit 1
    unsigned int rest : 30;            // +0x1c
};

class Object_0046bae0 {                // the model or piece being drawn
public:
    int field_0;                       // +0x00
    int count;                         // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    char unknown_10[0x24 - 0x10];
    Vec3_0046bae0* verts;              // +0x24
    Prim_0046bae0* prims;              // +0x28
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14383];
    Vec3_0046bae0* scratch;            // +0x14383
    Point_0046bae0* points;            // +0x14387
    Point_0046bae0* vertices;          // +0x1438b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004b6cc0(Vec3_0046bae0* in, Vec3_0046bae0* out, short* angles);
int __stdcall GetGafSequenceFrame(short* ref);
void __stdcall FillPolygon(void* surface, Point_0046bae0* points, int count, int color);
void __stdcall DrawFrameQuad(void* surface, void* texture, Point_0046bae0* points, void* src);

// FUNCTION: 0x46bae0
void __stdcall FUN_0046bae0(void* surface, Vec3_0046bae0* offset,
                            Object_0046bae0* obj, short* angles)
{
    Vec3_0046bae0* v = obj->verts;
    Vec3_0046bae0* scratch = g_game->scratch;
    Point_0046bae0* points = g_game->points;
    int i = 0;
    for (; i < obj->count; i++, v++, scratch++, points++) {
        FUN_004b6cc0(v, scratch, angles);
        int y = (short)((scratch->y + offset->y) >> 16);
        int z = (short)((offset->z - scratch->z) >> 16);
        int x = (short)((scratch->x + offset->x) >> 16);
        points->x = x + 0x80;
        points->y = z - (y >> 1) + 0x20;
    }

    int j;
    Prim_0046bae0* e = obj->prims;
    if (obj->field_c != -1) {
        e++;
        j = 1;
    } else {
        j = 0;
    }
    for (; j < obj->field_8; j++, e++) {
        unsigned short* idx = e->indices;
        Point_0046bae0* dst = g_game->vertices;
        for (int k = 0; k < e->count; k++, idx++, dst++) {
            *dst = g_game->points[*idx];
        }
        if (!e->flag0) {
            if (e->count == 4) {
                void* tex;
                if (e->texIndexed)
                    tex = (void*)GetGafSequenceFrame((short*)((char*)e + 0x10));
                else
                    tex = (void*)e->field_10;
                DrawFrameQuad(surface, tex, g_game->vertices, 0);
            }
        } else {
            FillPolygon(surface, g_game->vertices, e->count, e->field_0);
        }
    }
}
