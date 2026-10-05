// Decompiled by space-bunny-free. Names are provisional.
// Bounding box over every flagged piece of a model, shrunk by two pixels and
// merged into the four integers the caller passes in. A method that ignores
// `this`: its callers (0x4589c0) pass their own `this` through in ecx, and the
// offsets of the pieces match those of 0x4581e0, the same model walked in
// reverse. The four bounds start at zero rather than at INT_MAX/INT_MIN, so a
// model entirely on one side of the origin is clipped to the origin.

struct Vertex_00458310 {
    int x;                           // +0x0 (16.16 fixed point)
    int y;                           // +0x4
    int z;                           // +0x8
};

struct PieceInfo_00458310 {
    char unknown_0[4];
    int vertexCount;                 // +0x4
};

#pragma pack(push, 1)
struct Piece_00458310 {
    PieceInfo_00458310* info;        // +0x0
    char unknown_4[0x22 - 0x4];
    Vertex_00458310* vertices;       // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned char flags;             // +0x28
    char unknown_29[0x36 - 0x29];
};

struct Model_00458310 {
    int count;                       // +0x0
    char unknown_4[0x22 - 0x4];
    Piece_00458310 pieces[1];        // +0x22
};
#pragma pack(pop)

class Class_00458310 {
public:
    void FUN_00458310(int* minX, int* maxX, int* minY, int* maxY, Model_00458310* model,
                      int posX, int posY, int posZ);
};

// FUNCTION: 0x458310
void Class_00458310::FUN_00458310(int* minX, int* maxX, int* minY, int* maxY,
                                  Model_00458310* model, int posX, int posY, int posZ)
{
    int hiY = 0;
    int loY = 0;
    int hiX = 0;
    int loX = 0;
    for (int i = model->count - 1; i >= 0; i--) {
        Piece_00458310* piece = &model->pieces[i];
        if (piece->flags & 1) {
            Vertex_00458310* v = piece->vertices;
            for (int n = 0; n < piece->info->vertexCount; n++) {
                int x = (short)((v->x + posX) >> 16);
                int y = (short)((v->y + posY) >> 16);
                int z = (short)((posZ - v->z) >> 16);
                int sy = z - (y >> 1);
                if (x < loX) loX = x;
                if (x > hiX) hiX = x;
                if (sy < loY) loY = sy;
                if (sy > hiY) hiY = sy;
                v++;
            }
        }
    }
    loX -= 2;
    hiX += 2;
    loY -= 2;
    hiY += 2;
    if (loX < *minX) *minX = loX;
    if (loY < *minY) *minY = loY;
    if (hiX > *maxX) *maxX = hiX;
    if (hiY > *maxY) *maxY = hiY;
}
