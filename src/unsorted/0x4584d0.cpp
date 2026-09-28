// Decompiled by GPT-6-Luna. Names are provisional.
// Best result: 26.2%. The frame is four bytes larger than the target and the
// compiler chooses different registers for the vertex and face loops.
struct Point_4584d0 { int x; int y; };
struct Vertex_4584d0 { int x; int y; int z; };
struct View_4584d0 { char unknown_0[0x6a]; int originX; int originY; int originZ; };
struct Model_4584d0 { char unknown_0[0xc]; View_4584d0* view; };
struct Face_4584d0 { char unknown_0[4]; int count; char unknown_8[4]; unsigned short* indices; char unknown_10[8]; unsigned short* color; unsigned int flags; };
struct PieceInfo_4584d0 { char unknown_0[4]; int vertexCount; int faceCount; int firstFace; char unknown_10[8]; unsigned short* color; char unknown_1c[0xc]; Face_4584d0* faces; };
struct Vec3_4584d0 { int x; int y; int z; };
class Render_4584d0 { public: void FUN_004584d0(Model_4584d0*, Vec3_4584d0*, void*, PieceInfo_4584d0*, Vertex_4584d0*, unsigned int, int); };
extern char* g_game;
void __stdcall FUN_004b7ee0(unsigned short*);
void __stdcall FUN_004b7f30(unsigned short*, int);
void __stdcall FUN_004c0310(Point_4584d0*, int, int, int);
void __stdcall FUN_004c7580(int, int, Point_4584d0*, int);

// FUNCTION: 0x4584d0
void Render_4584d0::FUN_004584d0(Model_4584d0* model, Vec3_4584d0* camera,
    void*, PieceInfo_4584d0* info, Vertex_4584d0* vertices, unsigned int palette, int useColor)
{
    Point_4584d0 projected[2000];
    Point_4584d0 polygon[26];
    View_4584d0* view = model->view;
    int originZ = view->originZ;
    int originY = view->originY;
    int originX = view->originX;
    for (int i = 0; i < info->vertexCount; i++) {
        int x = (short)((vertices[i].x + (originX - camera->x)) >> 16) + 0x80;
        int z = (short)((originZ - camera->z - vertices[i].z) >> 16);
        int y = (short)((vertices[i].y + originY) >> 16);
        projected[i].x = x;
        projected[i].y = z - (y >> 1) + 0x20;
    }
    Face_4584d0* face = info->faces;
    int faceIndex;
    if (info->firstFace != -1) { face++; faceIndex = 1; }
    else faceIndex = 0;
    for (; faceIndex < info->faceCount; faceIndex++, face++) {
        for (int i = 0; i < face->count; i++)
            polygon[i] = projected[face->indices[i]];
        if (face->flags & 1) {
            FUN_004c0310(polygon, face->count, (int)face->indices, 0);
        } else if (face->count == 4) {
            if ((face->flags >> 1) & 1) {
                if ((face->flags >> 2) & 1) {
                    unsigned int value = palette & 0xff;
                    int offset = value * 0x14b;
                    unsigned int entry = value * 0x21;
                    int mapIndex = (int)(g_game + 0x1b8a)[offset + entry];
                    FUN_004b7f30(face->color, *(unsigned char*)(mapIndex + 0x96));
                } else if (useColor) {
                    FUN_004b7f30(face->color, 0);
                } else {
                    FUN_004b7ee0((unsigned short*)((char*)face + 0x10));
                }
            } else {
                FUN_004c7580((int)face->color, (int)polygon, (Point_4584d0*)face->indices, 0);
            }
        }
    }
}
