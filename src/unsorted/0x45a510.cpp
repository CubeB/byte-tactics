// Decompiled by space-bunny-free. Names are provisional.
// UNVERIFIED: my session had no shell, so tools/check.py could not be run even
// once. The body follows the matched near-copy 0x4581e0 (same function with an
// extra offset argument) and the 0x36-byte entry walk of 0x45aec0 / 0x45af90.
// The two points Ghidra shows that the sibling does not have are kept as they
// are: the four out parameters are stored with 0 before the loop, and the
// shared (y >> 2) term is added to x as well as subtracted from z.
// Bounding box of every flagged 0x36-byte piece entry of a model, walked last
// to first: writes width, height, origin x and origin y, each grown by 2 on
// every side (so an empty model leaves all four at 0). The two headers are the
// ones 0x4581e0 needed to keep the operand order of its offset sums; the only
// sums here are x + (y >> 2) and z - (y >> 2).
#include <stdio.h>
#include <stdlib.h>

struct Vertex_45a510 {
    int x;                           // +0x0 (16.16 fixed point)
    int y;                           // +0x4
    int z;                           // +0x8
};

struct PieceInfo_45a510 {
    char unknown_0[4];
    int vertexCount;                 // +0x4
};

#pragma pack(push, 1)
struct Piece_45a510 {
    PieceInfo_45a510* info;          // +0x0
    char unknown_4[0x22 - 0x4];
    Vertex_45a510* vertices;         // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned char flags;             // +0x28
    char unknown_29[0x36 - 0x29];
};

struct Model_45a510 {
    int pieceCount;                  // +0x0
    char unknown_4[0x22 - 0x4];
    Piece_45a510 pieces[1];          // +0x22
};
#pragma pack(pop)

// A method whose `this` is never used: its caller 0x45a790 loads ecx before
// the call. It compiles the same as a __stdcall free function.
class Class_0045a510 {
public:
    void FUN_0045a510(int* width, int* height, int* originX, int* originY, Model_45a510* model);
};

// FUNCTION: 0x45a510
void Class_0045a510::FUN_0045a510(int* width, int* height, int* originX, int* originY, Model_45a510* model)
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    minX = maxX = minY = maxY = 0;
    *width = *height = *originX = *originY = 0;
    for (int i = model->pieceCount - 1; i >= 0; i--) {
        Piece_45a510* piece = &model->pieces[i];
        if (piece->flags & 1) {
            Vertex_45a510* v = piece->vertices;
            for (int n = 0; n < piece->info->vertexCount; n++) {
                int x;
                int y;
                int z;
                x = (short)(v->x >> 16);
                y = (short)(v->y >> 16);
                z = (short)(-v->z >> 16);
                int sx = x + (y >> 2);
                int sy = z - (y >> 2);
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
