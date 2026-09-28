// Decompiled by GPT-6-Luna. Names are provisional.
// Best result: 54.0%. Register allocation in the opening state checks and
// vertex loop still differs. local_8 is intentionally uninitialized, as in
// the target's stack value used for the projected y coordinate.
extern char* g_game;

struct Vertex_458810 {
    int x;
    int y;
    int z;
};

struct Owner_458810 {
    void* relation;
    char unknown_4[0x1c];
    int field_20;
    char unknown_24[0xda];
    unsigned char field_fe;
    unsigned char kind;
    char unknown_100[4];
    float intensity;
    char unknown_108[6];
    unsigned char field_10e;
    char unknown_10f;
    unsigned int flags;
    unsigned char field_114;
};

struct PieceInfo_458810 {
    char unknown_0[4];
    int vertexCount;
};

#pragma pack(push, 1)
struct Piece_458810 {
    PieceInfo_458810* info;
    char unknown_4[0x1e];
    Vertex_458810* vertices;
    char unknown_26[2];
    unsigned char flags;
    char unknown_29[0xd];
};

struct List_458810 {
    int pieceCount;
    int frame;
    int faceCount;
    Owner_458810* owner;
    void* bitmap;
    int field_14;
    void* object;
    char unknown_1c[6];
    Piece_458810 pieces[1];
};
#pragma pack(pop)

struct Vec3_458810 {
    int x;
    int y;
    int z;
};

class Class_004584d0 {
public:
    void FUN_004584d0(List_458810* list, Vec3_458810* param_2, int* out, PieceInfo_458810* info,
        Vertex_458810* vertices, int kind, int visible);
};

class Class_00459200 {
public:
    void FUN_00459200(void* param_1, Vec3_458810 coords, List_458810* list, int flag);
};

class Class_004581e0 {
public:
    int FUN_004586a0(List_458810* list, int param_2, int param_3);
    void FUN_00458810(List_458810* list, Vec3_458810* result);
};

// FUNCTION: 0x458810
void Class_004581e0::FUN_00458810(List_458810* list, Vec3_458810* result)
{
    int x = *(int*)(g_game + 0x1431f);
    int z = *(int*)(g_game + 0x14323);
    x <<= 16;
    z <<= 16;
    int rebuild = 0;
    int visible = 0;
    if ((list->owner->flags & 0x20000000) != 0) {
        visible = (unsigned char)~list->owner->field_10e;
        visible &= 1;
    } else {
        visible = *(int*)((char*)list->owner->relation + 0x20) == 0;
    }
    if (list->frame == 0)
        rebuild = 1;
    void* bitmap = list->bitmap;
    if (bitmap == 0)
        list->field_14 = 0;
    unsigned int flags = list->owner->flags;
    int special = flags & 0x20000000;
    if (special != 0) {
        if (bitmap == 0
            || (list->owner->intensity != 0.0f
                && (flags & 0x2000) != 0
                && *(int*)((char*)bitmap + 0x14) == 0))
            rebuild = 1;
    }
    if (bitmap == 0 && special != 0)
        rebuild = 1;
    if ((list->owner->field_114 & 1) != 0 && bitmap == 0)
        rebuild = 1;

    if (rebuild) {
        list->field_14 = 0;
        FUN_004586a0(list, 0, 1);
    }

    bitmap = list->bitmap;
    Vec3_458810 coords;
        coords.x = x;
    int local_8;
    coords.y = local_8;
        coords.z = z;
    if (bitmap != 0) {
        ((Class_00459200*)this)->FUN_00459200(result, coords, list, visible);
        list->frame++;
        return;
    }

    for (int i = list->pieceCount - 1; i >= 0; i--) {
        Piece_458810* piece = &list->pieces[i];
        if (piece->flags & 1) {
            int buffer[4];
            Class_004584d0* renderer = (Class_004584d0*)this;
            renderer->FUN_004584d0(list, result, buffer, piece->info, piece->vertices,
                list->owner->kind, visible);
        }
    }
    list->frame++;
}
