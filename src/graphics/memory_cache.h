// CMemoryCache: the model memory cache (Cavedog's own name), the arena the
// object pictures are allocated from and the handles that keep them, at
// g_game+0x1437b. The one declaration of the class for the files that call it;
// the types behind the pointers stay private to their own files, so this
// includes nothing and forward declares them. The picture methods that call
// the UnitTable side (BuildObjectPicture, DrawPieces) keep the declarations of
// that class in the file that defines them. A file that matches only at its
// old view's symbol count keeps its own view.
#ifndef MEMORY_CACHE_H
#define MEMORY_CACHE_H

struct GafFrame;
struct Chunk_00437a30;
struct Vec3;
union Vec3_459200;
struct UnitObjectState;
struct Model_4584d0;
struct Vec3_4584d0;
struct PieceInfo_4584d0;
struct Vertex_4584d0;
// Unused here: these forward declarations take the symbol ids that keep
// 0x420f30, 0x421700 and 0x45e4c0 matching (docs/c2-regalloc.md).
struct ObjectPiece;
struct Object3do;
struct Vector3s;
struct Model_00458fa0;
struct Pos_4589c0;

class CMemoryCache {
public:
    int cap;                           // +0x00, the arena's length
    // unit_motion stores the arena address as an int, explosions as an int*.
    union {
        int base;                      // +0x04, the arena
        int* basePointer;              // +0x04
    };
    Chunk_00437a30* cur;               // +0x08, the next chunk to hand out
    void* handle;                      // +0x0c
    union {
        GafFrame* image;               // +0x10, the scratch image
        GafFrame* bitmap;              // +0x10
        GafFrame* shadow;              // +0x10
        void* buffer;                  // +0x10
        void* ptr;                     // +0x10
        int field_10;                  // +0x10
    };

    CMemoryCache* ClearPointers();
    int InitCache(unsigned int size);
    void FreeCache();
    void FreeBuffer();
    int AllocHandle(void** handle, int size);
    int AllocBitmap(GafFrame** handle, int w, int h);
    int AllocTwoPlaneBitmap(GafFrame** handle, int w, int h);
    void FlushCache();
    void ReleaseHandle(int handle);
    void DrawObjectState(UnitObjectState* state, Vec3* result);
    void CopyPicture(GafFrame* source);
    void BuildShadow(UnitObjectState* obj, GafFrame* dest);
    void DrawPiece(Model_4584d0* model, void* surface, Vec3_4584d0* camera,
        PieceInfo_4584d0* info, Vertex_4584d0* vertices, unsigned int palette,
        int useColor);
    void DrawObjectPicture(int param_2, UnitObjectState* model, Vec3_459200 v, int useColor);
    void DrawObjectPieces(int param_1, UnitObjectState* list, Vec3 v, int param_6);
    void MergeIntoComposite(GafFrame* src, UnitObjectState* model);
    void AddModelBounds(int* minX, int* maxX, int* minY, int* maxY,
        UnitObjectState* model, Pos_4589c0 pos);
    void RecolorByShade(GafFrame* img, unsigned char level, int above, int below, int between);
    int ShadeByIntensity(GafFrame* image, UnitObjectState* model);
    void DrawPieceEdges(GafFrame* view, Model_00458fa0* model, int color);
    GafFrame* MakeSilhouette(GafFrame* src);
    void MeasureShadow(int* width, int* height, int* originX, int* originY, UnitObjectState* model);
    void DrawShadowShape(GafFrame* view, UnitObjectState* model);
};

#endif
