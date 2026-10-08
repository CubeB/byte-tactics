// Decompiled by DeepSeek V4.1 Flash, Opus, Haiku, GPT-6-Luna, Space Bunny Free, deepseek-v4.1, Fledge Alpha Free, GPT-6 and claude-opus-5-5. Names are provisional.
// The memory cache: an arena of chunks {owner handle, size} that hands out
// bitmaps, and the object pictures built in it.
// Needed for AllocHandle's operand orders.
#include <windows.h>
#include <string.h>

void __cdecl FUN_004d85a0(int* param_1);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4,
                     int param_5, int param_6, int param_7, int param_8);
};

struct Cell_437840 {
    char unknown_0[7];
    unsigned char metal;               // +0x7
};

struct Point16_437840 {
    short x;
    short y;
};

#pragma pack(push, 1)
struct UnitType_437840 {
    char unknown_0[0x1ce];
    float extractsMetal;               // +0x1ce
    float field_1d2;                   // +0x1d2
};

struct Unit {
    char unknown_0[0x58];
    float extraction;                  // +0x58
    char unknown_5c[0x76 - 0x5c];
    Point16_437840 cell;               // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point16_437840 footprint;          // +0x7e
    char unknown_82[0x92 - 0x82];
    UnitType_437840* type;             // +0x92
    char unknown_96[0x9a - 0x96];
    CobScript* script;                 // +0x9a
};

struct Game {
    char unknown_0[0x37ed8];
    unsigned short windDirection;      // +0x37ed8
    int windSpeed;                     // +0x37eda
    char unknown_37ede[0x37ee2 - 0x37ede];
    int windEnabled;                   // +0x37ee2
};
#pragma pack(pop)

union Fixed_437840 {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    } parts;
};

struct Chunk_00437a30 {
    void** owner;                      // +0x0
    int size;                          // +0x4
};

// A 0x18-byte frame header, followed in the arena by its planes.
struct Bitmap_00437b50 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    unsigned char field_8;             // +0x8, the key colour
    char field_9;                      // +0x9
    char field_a;                      // +0xa
    char field_b;                      // +0xb
    int field_c;                       // +0xc
    unsigned char* pixels;             // +0x10
    unsigned char* plane2;             // +0x14, the second plane, mask or compressed copy
};

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
    Bitmap_00437b50* bitmap;        // +0x10
    int field_14;                 // +0x14
    char unknown_18[0x22 - 0x18];
    Piece_458810 pieces[1];       // +0x22
};
#pragma pack(pop)

struct Vec3_458810;

class UnitTable {
public:
    int BuildObjectPicture(List_458810* list, int param_2, int param_3);
};

struct Vec3_458810 { int x; int y; int z; };

struct State_0045a790 {
    char unknown_0[0x14];
    Bitmap_00437b50* sprite;           // +0x14 the picture built here
    char unknown_18[0x22 - 0x18];
};

void __stdcall CutOutFrame(Bitmap_00437b50* dst, Bitmap_00437b50* src, int x, int y);
int __stdcall CompressFrame(unsigned char* dest, Bitmap_00437b50* img);

class CMemoryCache {
public:
    int cap;                           // +0x0, the arena's length
    int base;                          // +0x4, the arena
    int cur;                           // +0x8, the next chunk to hand out
    void* handle;                      // +0xc
    Bitmap_00437b50* image;            // +0x10, the scratch image

    CMemoryCache* ClearPointers();
    int InitCache(unsigned int size);
    void FreeCache();
    void FreeBuffer();
    int AllocHandle(void** p, int need);
    int AllocBitmap(Bitmap_00437b50** handle, int w, int h);
    int AllocTwoPlaneBitmap(Bitmap_00437b50** handle, int w, int h);
    void FlushCache();
    void ReleaseHandle(int id);
    void DrawObjectState(List_458810* list, Vec3_458810* result);
    void CopyPicture(Bitmap_00437b50* source);
    void BuildShadow(State_0045a790* obj, Bitmap_00437b50* dest);
    void DrawPiece(List_458810* list, Vec3_458810* param_2, void* param_3,
        PieceInfo_458810* info, Vertex_458810* vertices, unsigned char kind, int visible);
    void DrawObjectPicture(void* param_1, List_458810* list, Vec3_458810 coords, int visible);
    void DrawObjectPieces(Vec3_458810* result, List_458810* list, Vec3_458810 v, int visible);
    void MeasureShadow(int* w, int* h, int* x, int* y, void* obj);
    void DrawShadowShape(Bitmap_00437b50* img, void* obj);
};

void CMemoryCache::DrawObjectPieces(Vec3_458810* result, List_458810* list, Vec3_458810 v, int visible)
{
    if (list->bitmap != 0) {
        DrawObjectPicture(result, list, v, visible);
        return;
    }
    for (int i = list->pieceCount - 1; i >= 0; i--) {
        if (list->pieces[i].flags & 1) {
            DrawPiece(list, result, &v, list->pieces[i].info,
                list->pieces[i].vertices, list->owner->kind, visible);
        }
    }
}

extern Game* g_game;

Cell_437840* __stdcall GetMapCell(int x, int y);

// Wind/metal picker for a metal extractor: when the unit type extracts metal
// (+0x1ce > 0), sums the metal byte of every map cell under the unit's
// footprint and stores the resulting rate at +0x58, then tells the script.
//
// The sum is accumulated in 16.16 fixed point: the unity's footprint is a
// Point16 at +0x7e, the cell is at +0x76, the type pointer at +0x92 and the
// script at +0x9a. The accumulator keeps the count in the high half of a
// dword so the final `(float)` conversion and the 2^-16 scale cancel out.
// FUNCTION: 0x437840
void __stdcall UpdateMetalExtraction(Unit* unit)
{
    if (unit->type->extractsMetal > 0.0f) {
        Fixed_437840 total;
        total.value = 0;
        Point16_437840 fp = unit->footprint;
        for (int y = unit->cell.y; y < unit->cell.y + fp.y; y++) {
            for (int x = unit->cell.x; x < unit->cell.x + fp.x; x++) {
                Cell_437840* c = GetMapCell(x, y);
                if (c) {
                    total.parts.whole += c->metal + 1;
                }
            }
        }
        unit->extraction = unit->type->extractsMetal * 1.52587890625e-05 * (float)total.value;
        if (unit->script)
            unit->script->StartScriptWithArgs("SetSpeed", 0, 0, 1, total.parts.whole, 0, 0, 0);
    }
}

// For a unit whose type has a positive value at +0x1d2 (presumably the wind
// generator rating), passes the current wind direction and speed to its
// script's SetDirection and SetSpeed functions when wind is enabled.
// FUNCTION: 0x437910
void __stdcall UpdateWindGenerator(Unit* unit)
{
    if (unit->type->field_1d2 > 0.0f && g_game->windEnabled) {
        unit->script->StartScriptWithArgs("SetDirection", 0, 0, 1, g_game->windDirection, 0, 0, 0);
        unit->script->StartScriptWithArgs("SetSpeed", 0, 0, 1, g_game->windSpeed << 4, 0, 0, 0);
    }
}

// FUNCTION: 0x4379a0
CMemoryCache* CMemoryCache::ClearPointers()
{
    int zero = 0;
    base = zero;
    handle = (void*)zero;
    return this;
}

// Memory cache initialisation: frees any previous block (as 0x437a00 does),
// allocates a "CMemoryCache CCH" block of the given size and makes it one
// free chunk covering the whole block.
// FUNCTION: 0x4379b0
int CMemoryCache::InitCache(unsigned int newSize)
{
    if (base != 0) {
        FUN_004d85a0((int*)base);
        base = 0;
    }
    Chunk_00437a30* chunk = (Chunk_00437a30*)FUN_004d83b0("CMemoryCache CCH", newSize);
    base = (int)chunk;
    cap = newSize;
    cur = (int)chunk;
    chunk->owner = 0;
    ((Chunk_00437a30*)cur)->size = cap;
    return 1;
}

// FUNCTION: 0x437a00
void CMemoryCache::FreeCache()
{
    if (base != 0) {
        FUN_004d85a0((int*)base);
        base = 0;
    }
}

// FUNCTION: 0x437a20
void CMemoryCache::FreeBuffer()
{
    FUN_004d85a0((int*)base);
}

// Memory cache allocation: grows or reuses the handle in *p, splitting the
// block found at `cur` (or wrapping to the base) into an allocated chunk plus
// a leftover free chunk. Chunks are {owner handle, size} and the handle points
// just past the 8-byte header.
// FUNCTION: 0x437a30
int CMemoryCache::AllocHandle(void** p, int need)
{
    int total = 0;
    if (need <= 0) {
        *p = 0;
        return 0;
    }
    int size = need + 8;
    int existing = (int)*p;
    if (existing != 0)
        existing = *(int*)(existing - 4);
    if (existing >= size)
        return 1;
    if (size > cap) {
        *p = 0;
        return 0;
    }
    int start;
    int end = base + cap;
    if (cur + size > end) {
        int q = cur;
        start = base;
        while (q < end) {
            void** owner = *(void***)q;
            if (owner != 0 && (int)*owner == q + 8)
                *owner = 0;
            q += *(int*)(q + 4);
            end = base + cap;
        }
    } else {
        start = cur;
    }
    int q = start;
    while (total < size) {
        total += *(int*)(q + 4);
        void** owner = *(void***)q;
        if (owner != 0 && (int)*owner == q + 8)
            *owner = 0;
        q += *(int*)(q + 4);
    }
    if (total - size > 8u) {
        Chunk_00437a30* nc = (Chunk_00437a30*)(start + size);
        nc->owner = 0;
        nc->size = total - size;
        cur = (int)nc;
    } else {
        size = total;
        int e = start + total;
        if (e >= base + cap)
            e = base;
        cur = e;
    }
    ((Chunk_00437a30*)start)->owner = p;
    ((Chunk_00437a30*)start)->size = size;
    *p = (void*)(start + 8);
    return 1;
}

// Allocates a one-plane w*h bitmap from the arena (0x437a30): a 0x18-byte
// header, then the pixels, filled with 1. One-plane version of 0x437be0.
// The original calls this from BuildShadow (0x45a790) rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x437b50
int CMemoryCache::AllocBitmap(Bitmap_00437b50** handle, int w, int h)
{
    int n = w * h;
    if (!AllocHandle((void**)handle, n + 0x18))
        return 0;
    Bitmap_00437b50* b = *handle;
    if (!b)
        return 0;
    b->plane2 = 0;
    b->field_4 = 0;
    b->field_6 = 0;
    b->field_a = 0;
    b->field_b = 0;
    b->field_c = 0;
    b->field_9 = 0;
    b->width = w;
    b->height = h;
    b->pixels = (unsigned char*)(b + 1);
    b->field_8 = 1;
    memset(b->pixels, 1, n);
    return 1;
}
#pragma auto_inline(on)

// Allocates a two-plane w*h bitmap from the arena (0x437a30): a 0x18-byte
// header, then a plane filled with 1 and a plane cleared to 0.
// FUNCTION: 0x437be0
int CMemoryCache::AllocTwoPlaneBitmap(Bitmap_00437b50** handle, int w, int h)
{
    int n = w * h;
    if (!AllocHandle((void**)handle, n * 2 + 0x18))
        return 0;
    Bitmap_00437b50* b = *handle;
    if (!b)
        return 0;
    // Plane pointer taken before the header stores: it keeps the zero out of eax.
    unsigned char* p = (unsigned char*)(b + 1);
    b->field_4 = 0;
    b->field_6 = 0;
    b->field_a = 0;
    b->field_b = 0;
    b->field_c = 0;
    b->field_9 = 0;
    b->width = w;
    b->height = h;
    b->pixels = p;
    b->plane2 = p + n;
    memset(b->plane2, 0, n);
    b->field_8 = 1;
    memset(b->pixels, 1, n);
    return 1;
}

// Hands the whole arena (its length minus the 8-byte record header) to the
// allocator 0x437a30 as one block, storing the handle at +0xc.
// FUNCTION: 0x437c80
void CMemoryCache::FlushCache()
{
    AllocHandle(&handle, cap - 8);
}

// Walks a buffer of variable-sized records {owner, size, ...} and clears the owner
// of every record whose owner is `id`.
// FUNCTION: 0x437c90
void CMemoryCache::ReleaseHandle(int id)
{
    Chunk_00437a30* r = (Chunk_00437a30*)base;
    for (int offset = 0; offset < cap; ) {
        offset += r->size;
        if ((int)r->owner == id) {
            r->owner = 0;
        }
        r = (Chunk_00437a30*)((char*)r + r->size);
    }
}

// FUNCTION: 0x437b30
int __stdcall GetHandleSize(int param_1)
{
    if (param_1 == 0) {
        return 0;
    }
    return *(int*)(param_1 - 4);
}

// FUNCTION: 0x458810
void CMemoryCache::DrawObjectState(List_458810* list, Vec3_458810* result)
{
    int rebuild = 0;
    int x = *(int*)((char*)g_game + 0x1431f) << 16;
    int z = *(int*)((char*)g_game + 0x14323) << 16;
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
    Bitmap_00437b50* bitmap = list->bitmap;
    if ((owner->flags & 0x20000000) != 0) {
        // The bitmap-null case is its own `rebuild = 1` statement.
        if (list->bitmap == 0)
            rebuild = 1;
        // The last conjunct goes through `bitmap`, the one before it through `list->bitmap`.
        else if (owner->intensity != 0.0f
                && (owner->flags & 0x2000) != 0
                && list->bitmap->plane2 == 0
                && bitmap->plane2 == 0)
            rebuild = 1;
    }
    if (list->bitmap == 0 && (owner->flags & 0x20000000) != 0)
        rebuild = 1;
    if ((owner->field_114 & 1) != 0 && list->bitmap == 0)
        rebuild = 1;
    if (rebuild) {
        list->field_14 = 0;
        ((UnitTable*)this)->BuildObjectPicture(list, 0, 1);
    }
    Vec3_458810 coords;
    coords.x = x;
    coords.z = z;
    DrawObjectPieces(result, list, coords, visible);
    list->frame++;
}

// Copies an image (size, three header fields, pixels and optional mask) into
// the object's own image, whose buffers are already allocated.
// FUNCTION: 0x459170
void CMemoryCache::CopyPicture(Bitmap_00437b50* source)
{
    image->width = source->width;
    image->height = source->height;
    image->field_4 = source->field_4;
    image->field_6 = source->field_6;
    image->field_8 = source->field_8;
    memcpy(image->pixels, source->pixels, source->width * source->height);
    if (source->plane2)
        memcpy(image->plane2, source->plane2, source->width * source->height);
}

// Builds the picture of a piece: MeasureShadow measures the piece bounding box
// of the state, the scratch image at this->image is cleared to its key
// colour and the pieces are drawn into it (DrawShadowShape), the background image
// is blitted over it, and the run length compressed result becomes the state's
// sprite (CompressFrame, then a one plane bitmap from the arena).
// FUNCTION: 0x45a790
void CMemoryCache::BuildShadow(State_0045a790* obj, Bitmap_00437b50* dest)
{
    int w, h, x, y;
    MeasureShadow(&w, &h, &x, &y, obj);
    image->width = (unsigned short)w;
    image->height = (unsigned short)h;
    image->field_4 = (unsigned short)x;
    image->field_6 = (unsigned short)y;
    memset(image->pixels, image->field_8, h * w);
    memset(image->plane2, 0, h * w);
    DrawShadowShape(image, obj);
    CutOutFrame(dest, image, 5, 0);
    int size = CompressFrame(image->plane2, image);
    AllocBitmap(&obj->sprite, size, 1);
    Bitmap_00437b50* bmp = obj->sprite;
    memcpy(bmp->pixels, image->plane2, size);
    bmp->width = image->width;
    bmp->height = image->height;
    bmp->field_4 = image->field_4;
    bmp->field_6 = image->field_6;
    bmp->field_9 = 1;
}
