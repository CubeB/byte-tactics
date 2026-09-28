// Decompiled by GPT-6-Luna, finished by Space Bunny Free. Names are provisional.
// Callers (0x4584b4, 0x458997, 0x4593ff, 0x459476) push the surface pointer as
// the second argument and the address of a 12-byte {x,y,z} struct as the third,
// so the argument order is (model, surface, camera, info, vertices, palette, useColor).
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

void* __stdcall FUN_004b7ee0(Pic_4584d0* ref);
void* __stdcall FUN_004b7f30(unsigned short* table, int index);
void __stdcall FUN_004c0310(void* surface, Point_4584d0* points, int count, int flags);
void __stdcall FUN_004c7580(void* surface, void* pic, Point_4584d0* points, void* src);

class Class_004584d0 {
public:
    void FUN_004584d0(Model_4584d0* model, void* surface, Vec3_4584d0* camera,
        PieceInfo_4584d0* info, Vertex_4584d0* vertices, unsigned int palette,
        int useColor);
};

// FUNCTION: 0x4584d0
void Class_004584d0::FUN_004584d0(Model_4584d0* model, void* surface,
    Vec3_4584d0* camera, PieceInfo_4584d0* info, Vertex_4584d0* vertices,
    unsigned int palette, int useColor)
{
    Point_4584d0 projected[2000];
    Point_4584d0 poly[25];
    int i;
    View_4584d0* view = model->view;
    struct Off_4584d0 { int a; int b; int y; int c; };
    Off_4584d0 off;
    off.a = view->originX - camera->x;
    off.y = view->originY;
    off.b = view->originZ - camera->z;
    for (i = 0; i < info->vertexCount; i++) {
        projected[i].x = (short)((vertices[i].x + off.a) >> 16) + 0x80;
        projected[i].y = (short)((off.b - vertices[i].z) >> 16)
            - ((short)((vertices[i].y + off.y) >> 16) >> 1) + 0x20;
    }
    int j;
    Face_4584d0* face = info->faces;
    if (info->firstFace != -1) {
        face++;
        i = 1;
    } else {
        i = 0;
    }
    for (; i < info->faceCount; i++, face++) {
        unsigned short* idx = face->indices;
        for (j = 0; j < face->count; j++)
            poly[j] = projected[*idx++];
        Flags_4584d0 flags = face->flags;
        if (!flags.bits.a) {
            if (face->count == 4) {
                void* pic;
                if (flags.bits.b) {
                    if (flags.bits.c) {
                        int player = palette & 0xff;
                        int unit = *(int*)((char*)g_game + 0x1b8a + player * 0x14b);
                        pic = FUN_004b7f30(face->color, *(unsigned char*)(unit + 0x96));
                    } else if (useColor) {
                        pic = FUN_004b7f30(face->color, 0);
                    } else {
                        pic = FUN_004b7ee0(&face->pic);
                    }
                } else {
                    pic = face->pic.pic;
                }
                FUN_004c7580(surface, pic, poly, 0);
            }
        } else {
            FUN_004c0310(surface, poly, face->count, face->unknown_0);
        }
    }
}
