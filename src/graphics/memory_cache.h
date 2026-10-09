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
struct Model_459200;
struct Model_4584d0;
struct Vec3_4584d0;
struct PieceInfo_4584d0;
struct Vertex_4584d0;
struct List_458810;
struct Vec3_458810;
struct State_0045a790;
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
    void DrawObjectState(List_458810* list, Vec3_458810* result);
    void CopyPicture(GafFrame* source);
    void BuildShadow(State_0045a790* obj, GafFrame* dest);
    void DrawPiece(Model_4584d0* model, void* surface, Vec3_4584d0* camera,
        PieceInfo_4584d0* info, Vertex_4584d0* vertices, unsigned int palette,
        int useColor);
    void DrawObjectPicture(int param_2, Model_459200* model, Vec3_459200 v, int useColor);
    void DrawObjectPieces(int param_1, Model_459200* list, Vec3 v, int param_6);
    void MergeIntoComposite(GafFrame* src, Model_459200* model);
    void AddModelBounds(int* minX, int* maxX, int* minY, int* maxY,
        Model_459200* model, Pos_4589c0 pos);
    void RecolorByShade(GafFrame* img, unsigned char level, int above, int below, int between);
    int ShadeByIntensity(GafFrame* image, Model_459200* model);
    void DrawPieceEdges(GafFrame* view, Model_00458fa0* model, int color);
    GafFrame* MakeSilhouette(GafFrame* src);
    void MeasureShadow(int* width, int* height, int* originX, int* originY, Model_459200* model);
    void DrawShadowShape(GafFrame* view, Model_459200* model);
};

#endif
