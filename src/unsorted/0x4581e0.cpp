// Decompiled by Opus. Names are provisional.

struct Vertex_4581e0 {
    int x;                           // +0x0 (16.16 fixed point)
    int y;                           // +0x4
    int z;                           // +0x8
};

struct PieceInfo_4581e0 {
    char unknown_0[4];
    int vertexCount;                 // +0x4
};

#pragma pack(push, 1)
struct Piece_4581e0 {
    PieceInfo_4581e0* info;          // +0x0
    char unknown_4[0x22 - 0x4];
    Vertex_4581e0* vertices;         // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned char flags;             // +0x28
    char unknown_29[0x36 - 0x29];
};

struct Model_4581e0 {
    int pieceCount;                  // +0x0
    char unknown_4[0x22 - 0x4];
    Piece_4581e0 pieces[1];          // +0x22
};
#pragma pack(pop)

// FUNCTION: 0x4581e0
void __stdcall FUN_004581e0(int* width, int* height, int* originX, int* originY, Model_4581e0* model, Vertex_4581e0* offset)
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    minX = maxX = minY = maxY = 0;
    for (int i = model->pieceCount - 1; i >= 0; i--) {
        Piece_4581e0* piece = &model->pieces[i];
        if (piece->flags & 1) {
            Vertex_4581e0* v = piece->vertices;
            for (int n = 0; n < piece->info->vertexCount; n++) {
                int x;
                int y;
                int z;
                if (offset != 0) {
                    x = (short)((v->x + offset->x) >> 16);
                    y = (short)((offset->y + v->y) >> 16);
                    z = (short)((offset->z - v->z) >> 16);
                } else {
                    x = (short)(v->x >> 16);
                    y = (short)(v->y >> 16);
                    z = (short)(-v->z >> 16);
                }
                int sx = x;
                int sy = z - (y >> 1);
                if (sx < minX) minX = sx;
                if (sx > maxX) maxX = sx;
                if (sy < minY) minY = sy;
                if (sy > maxY) maxY = sy;
                v++;
            }
        }
    }
    minX -= 2;
    minY -= 2;
    *width = maxX - minX + 2;
    *height = maxY - minY + 2;
    *originX = -minX;
    *originY = -minY;
}
