// Decompiled by space-bunny-free. Names are provisional.

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Class_0045b150;   // owning object, only ever passed through

struct Object_0045b150 {
    char unknown_0[4];
    int count;                      // +0x4
};

#pragma pack(push, 2)
struct Piece_0045b150 {
    Object_0045b150* object;        // +0x0
    char unknown_4[0x16 - 0x4];
    Vec3 offset;                    // +0x16
    Vec3* points;                   // +0x22
    short modified;                 // +0x26
    char unknown_28[2];
    Piece_0045b150* sibling;        // +0x2a
    Piece_0045b150* child;          // +0x2e
};
#pragma pack(pop)

void __stdcall FUN_004b6cc0(Vec3* in, Vec3* out, short* angles);

// FUNCTION: 0x45b150
void __fastcall TransformPieces(Class_0045b150* owner, Piece_0045b150* piece,
                             short* angles, Vec3* delta, int deep)
{
    for (;;) {
        if (piece->modified == 0) {
            Vec3* offset = &piece->offset;
            FUN_004b6cc0(offset, offset, angles);
            int count = piece->object->count;
            for (int i = count - 1; i >= 0; i--)
                FUN_004b6cc0(&piece->points[i], &piece->points[i], angles);
            offset->x += delta->x;
            offset->y += delta->y;
            offset->z += delta->z;
            count = piece->object->count;
            for (int j = count - 1; j >= 0; j--) {
                Vec3* p = &piece->points[j];
                p->x += delta->x;
                p->y += delta->y;
                p->z += delta->z;
            }
        }
        if (piece->child) {
            TransformPieces(owner, piece->child, angles, delta, 1);
        }
        if (deep == 0)
            break;
        piece = piece->sibling;
        if (piece == 0)
            break;
        deep = 1;
    }
}
