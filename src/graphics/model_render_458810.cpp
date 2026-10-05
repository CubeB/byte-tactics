// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by Space Bunny Free, finished by Fledge Alpha Free, finished by GPT-6, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, #5153). Earlier passes reached 97.2% with the frame
// test and the bitmap test emitted in the wrong order; writing them in the
// original's order recoloured the function. What was missing:
//  - The whole tail (fast path through DrawObjectPicture, or the piece loop) is the
//    neighbouring Class_00458430::DrawObjectPieces, which has no callers in the exe
//    and is inlined here. Its by-value Vec3 parameter is also what copies the
//    never-written `coords.y` through its own frame slot, so the old `src`
//    trick is gone. Inlined, it raises x and z from priority 27 to 35
//    (c2prio), well clear of the coords copies at 26 to 32.
//  - With that margin the frame test can come first, as in the original, if the
//    bitmap-null case inside the 0x20000000 block is its own `rebuild = 1`
//    statement (`if (!bitmap) ... else if (...)`): MSVC merges the two stores
//    in the output, but the extra block lowers the bitmap temp from 16 to 14,
//    below the flags temp (15), so flags take edx and the bitmap ebx.
//  - The class is CMemoryCache, the name data/symbols.csv and the caller
//    0x45ac20 use.
// The doubled `test eax, eax` still needs the last conjunct spelled through the
// `bitmap` local and the one before it through `list->bitmap`.

extern char* g_game;

struct Vertex_458810 { int x; int y; int z; };

struct Owner_458810 {
    void* relation;               // +0x00
    char unknown_4[0x1c];
    int field_20;                 // +0x20
    char unknown_24[0xff - 0x24];
    unsigned char kind;           // +0xff
    char unknown_100[4];
    float intensity;              // +0x104
    char unknown_108[6];
    unsigned char field_10e;      // +0x10e
    char unknown_10f;
    unsigned int flags;           // +0x110
    unsigned char field_114;      // +0x114
    char unknown_115[3];
};

struct Bitmap_458810 {
    char unknown_0[0x14];
    int field_14;                 // +0x14
};

struct PieceInfo_458810 { char unknown_0[4]; int vertexCount; };

#pragma pack(push, 1)
struct Piece_458810 {
    PieceInfo_458810* info;       // +0x00
    char unknown_4[0x1e];
    Vertex_458810* vertices;      // +0x22
    char unknown_26[2];
    unsigned char flags;          // +0x28
    char unknown_29[0xd];
};

struct List_458810 {
    int pieceCount;               // +0x00
    int frame;                    // +0x04
    char unknown_8[4];
    Owner_458810* owner;          // +0x0c
    Bitmap_458810* bitmap;        // +0x10
    int field_14;                 // +0x14
    char unknown_18[0x22 - 0x18];
    Piece_458810 pieces[1];       // +0x22
};
#pragma pack(pop)

struct Vec3_458810;

class Class_004584d0 {
public:
    void DrawPiece(List_458810* list, Vec3_458810* param_2, void* param_3,
        PieceInfo_458810* info, Vertex_458810* vertices, unsigned char kind, int visible);
};

class Class_00459200 {
public:
    void DrawObjectPicture(void* param_1, List_458810* list, Vec3_458810 coords, int visible);
};

class Class_004581e0 {
public:
    int BuildObjectPicture(List_458810* list, int param_2, int param_3);
};

class CMemoryCache {
public:
    void DrawObjectState(List_458810* list, Vec3_458810* result);
};

struct Vec3_458810 { int x; int y; int z; };

class Class_00458430 {
public:
    void DrawObjectPieces(Vec3_458810* result, List_458810* list, Vec3_458810 v, int visible);
};

void Class_00458430::DrawObjectPieces(Vec3_458810* result, List_458810* list, Vec3_458810 v, int visible)
{
    if (list->bitmap != 0) {
        ((Class_00459200*)this)->DrawObjectPicture(result, list, v, visible);
        return;
    }
    for (int i = list->pieceCount - 1; i >= 0; i--) {
        if (list->pieces[i].flags & 1) {
            ((Class_004584d0*)this)->DrawPiece(list, result, &v, list->pieces[i].info,
                list->pieces[i].vertices, list->owner->kind, visible);
        }
    }
}

// FUNCTION: 0x458810
void CMemoryCache::DrawObjectState(List_458810* list, Vec3_458810* result)
{
    int rebuild = 0;
    int x = *(int*)(g_game + 0x1431f) << 16;
    int z = *(int*)(g_game + 0x14323) << 16;
    int visible;
    Owner_458810* owner = list->owner;
    if (owner->flags & 0x20000000) {
        int t = (unsigned char)~owner->field_10e;
        if (t & 1)
            visible = 1;
        else
            visible = 0;
    } else {
        visible = *(int*)((char*)owner->relation + 0x20) == 0;
    }
    if (list->frame == 0)
        rebuild = 1;
    if (list->bitmap == 0)
        list->field_14 = 0;
    Bitmap_458810* bitmap = list->bitmap;
    if ((owner->flags & 0x20000000) != 0) {
        if (list->bitmap == 0)
            rebuild = 1;
        else if (owner->intensity != 0.0f
                && (owner->flags & 0x2000) != 0
                && list->bitmap->field_14 == 0
                && bitmap->field_14 == 0)
            rebuild = 1;
    }
    if (list->bitmap == 0 && (owner->flags & 0x20000000) != 0)
        rebuild = 1;
    if ((owner->field_114 & 1) != 0 && list->bitmap == 0)
        rebuild = 1;
    if (rebuild) {
        list->field_14 = 0;
        ((Class_004581e0*)this)->BuildObjectPicture(list, 0, 1);
    }
    Vec3_458810 coords;
    coords.x = x;
    coords.z = z;
    ((Class_00458430*)this)->DrawObjectPieces(result, list, coords, visible);
    list->frame++;
}
