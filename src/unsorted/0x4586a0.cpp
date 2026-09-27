// Decompiled by space-bunny-free. Names are provisional.
// Builds the composite bitmap for a model's piece list: asks 0x4581e0 for the
// bounds, allocates a one-plane (0x437b50) or two-plane (0x437be0) bitmap for
// them, stores the origin in the bitmap header and hands the drawing to
// 0x459c70 / 0x459830.
// Notes: all four bounds results are plain ints (the bitmap header fields are
// shorts, so the stores narrow them); `kind` is an int parameter so the byte at
// owner+0xff is pushed zero-extended.

extern void* g_game;

struct Bitmap_004586a0 {
    short width;                       // +0x0
    short height;                      // +0x2
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    char unknown_8[0x10 - 0x8];
    unsigned char* plane1;             // +0x10
    char unknown_14[0x18 - 0x14];
};

struct Vertex_004586a0 {
    int x;
    int y;
    int z;
};

struct PieceInfo_004586a0 {
    char unknown_0[4];
    int vertexCount;
};

#pragma pack(push, 1)
struct Piece_004586a0 {
    PieceInfo_004586a0* info;          // +0x0
    char unknown_4[0x22 - 0x4];
    Vertex_004586a0* vertices;         // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned char flags;               // +0x28
    char unknown_29[0x36 - 0x29];
};

struct Owner_004586a0 {
    char unknown_0[0xff];
    unsigned char kind;                // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    int field_110;                     // +0x110
    unsigned char field_114;           // +0x114
    char unknown_115[4];
};

struct List_004586a0 {
    int count;                         // +0x0
    char unknown_4[0xc - 0x4];
    Owner_004586a0* owner;             // +0xc
    Bitmap_004586a0* bitmap;           // +0x10
    char unknown_14[0x22 - 0x14];
    Piece_004586a0 pieces[1];          // +0x22
};
#pragma pack(pop)

class Class_00437a30 {
public:
    int FUN_00437b50(Bitmap_004586a0** handle, int w, int h);
    int FUN_00437be0(Bitmap_004586a0** handle, int w, int h);
};

class Class_004581e0 {
public:
    void FUN_004581e0(int* width, int* height, int* originX, int* originY, List_004586a0* list, void* offset);
    int FUN_00459c70(Bitmap_004586a0* bitmap, List_004586a0* list, int kind, int param);
    int FUN_00459830(Bitmap_004586a0* bitmap, List_004586a0* list, int kind, int param);
    int FUN_004586a0(List_004586a0* list, int param_2, int param_3);
};

// FUNCTION: 0x4586a0
int Class_004581e0::FUN_004586a0(List_004586a0* list, int param_2, int param_3)
{
    int w;
    int h;
    int oy;
    int ox;
    Owner_004586a0* owner = list->owner;
    FUN_004581e0(&w, &h, &oy, &ox, list, 0);
    if (param_2 == 0 && (owner->field_114 & 1) == 0 && owner->field_104 == 0.0f) {
        ((Class_00437a30*)this)->FUN_00437b50(&list->bitmap, w, h);
    } else {
        ((Class_00437a30*)this)->FUN_00437be0(&list->bitmap, w, h);
    }
    Bitmap_004586a0* bitmap = list->bitmap;
    if (bitmap != 0) {
        bitmap->field_4 = (short)oy;
        bitmap->field_6 = (short)ox;
        if ((owner->field_110 & 0x20000000) != 0
            && (*(unsigned char*)((char*)g_game + 0x37f06) & 0x20) != 0) {
            FUN_00459c70(bitmap, list, owner->kind, param_3);
        } else {
            FUN_00459830(bitmap, list, owner->kind, param_3);
        }
        return 1;
    }
    return 0;
}
