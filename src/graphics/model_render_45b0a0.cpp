// Decompiled by space-bunny-free. Names are provisional.
// <string.h> is not used, but dropping it changes MSVC's register choice in
// the box sums below (the object field lands in a different register), so the
// include has to stay for byte-identical output.
#include <string.h>

struct Object3do {
    char unknown_0[4];
    int vertex_count;                  // +0x4
    char unknown_8[0x10 - 0x8];
    int box_x;                         // +0x10
    int box_y;                         // +0x14
    int box_z;                         // +0x18
    char unknown_18[0x24 - 0x18];
    void* vertices;                    // +0x24, 12 bytes each
};

#pragma pack(push, 2)
struct Piece_0045b030 {
    Object3do* object;                 // +0x0
    int rot_x;                         // +0x4
    int rot_y;                         // +0x8
    int rot_z;                         // +0xc
    short short_10;                    // +0x10
    short short_12;                    // +0x12
    short short_14;                    // +0x14
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

struct Vector3s {
    short x, y, z;
};

struct Vector3 {
    int x, y, z;
};

struct Model_0045b0a0 {
    char unknown_0[0x18];
    short pos_x;                       // +0x18
    short pos_y;                       // +0x1a
    short pos_z;                       // +0x1c
    Piece_0045b030* piece;             // +0x1e
};

void __fastcall FUN_0045b150(Model_0045b0a0* model, Piece_0045b030* piece,
                             Vector3s* pos, Vector3* box, int force);

// Offsets a piece tree by the model's position (unless `force` says the
// position was already applied) and then translates every vertex. The child's
// recursion is unconditional, the sibling's only when `force` is set.
// The three shorts of the piece map onto the short vector in reverse
// (+0x14 -> x, +0x12 -> y, +0x10 -> z), which is what the original does.
// FUNCTION: 0x45b0a0
void __fastcall FUN_0045b0a0(Model_0045b0a0* model, Piece_0045b030* piece, int force)
{
    if (piece->child)
        FUN_0045b0a0(model, piece->child, 1);

    Vector3s pos;
    pos.x = piece->short_14;
    pos.z = piece->short_10;
    pos.y = piece->short_12;
    if (!force) {
        pos.x += model->pos_x;
        pos.z += model->pos_z;
        pos.y += model->pos_y;
    }

    Object3do* object = piece->object;
    Vector3 box;
    box.x = piece->rot_x + object->box_x;
    box.y = piece->rot_y + object->box_y;
    box.z = piece->rot_z + object->box_z;

    FUN_0045b150(model, piece, &pos, &box, 0);

    if (force && piece->sibling)
        FUN_0045b0a0(model, piece->sibling, 1);
}
