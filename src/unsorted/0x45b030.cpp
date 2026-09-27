// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Object3do {
    char unknown_0[4];
    int vertex_count;                  // +0x4
    char unknown_8[0x24 - 0x8];
    void* vertices;                    // +0x24, 12 bytes each
};

#pragma pack(push, 2)
struct Piece_0045b030 {
    Object3do* object;                 // +0x0
    char unknown_4[0x16 - 0x4];
    int offset_x;                      // +0x16
    int offset_y;                      // +0x1a
    int offset_z;                      // +0x1e
    void* vertices;                    // +0x22
    short modified;                    // +0x26
    char unknown_28[2];
    Piece_0045b030* sibling;           // +0x2a
    Piece_0045b030* child;             // +0x2e
};
#pragma pack(pop)

// Restores the vertices of every modified piece in the tree (or every piece,
// when `force` is set) from the object, and clears its offset. Returns
// nonzero if the last piece visited was restored.
// FUNCTION: 0x45b030
int __fastcall FUN_0045b030(Piece_0045b030* piece, int force)
{
    int result;
    do {
        result = force;
        if (piece->modified == 0 || force) {
            memcpy(piece->vertices, piece->object->vertices, piece->object->vertex_count * 12);
            piece->offset_x = 0;
            piece->offset_y = 0;
            piece->offset_z = 0;
            piece->modified = 0;
            result = 1;
        }
        if (piece->child) {
            result = FUN_0045b030(piece->child, result);
        }
        piece = piece->sibling;
    } while (piece);
    return result;
}
