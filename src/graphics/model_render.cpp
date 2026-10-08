// Decompiled by Haiku, Opus, Sonnet, space-bunny-free, longcat-2.5-preview-free,
// GPT-6, GPT-6.1-sol, deepseek-v4.1, deepseek-v4.1-flash, claude-opus-5-5,
// GPT-6-Luna, Space Bunny Free, Fledge Alpha Free, LongCat 2.5 Preview Free,
// GPT-6., Claude Opus 5.5 and DeepSeek V4.1 Flash. Names are provisional.
// The model_render module: the object-picture builder (the projected bounds,
// the composite buffer and its shadow bitmap, the flat and lit piece
// rasterisers), the per-piece draw calls and their bitmap helpers, and the
// runtime "Object State" clone of a model's piece tree (its piece records,
// their links and transforms). The module's files gathered in address order;
// the functions that only match at their own file's symbol count stay in
// model_render_4581e0.cpp, model_render_4584d0.cpp, model_render_4589c0.cpp,
// model_render_458fa0.cpp and model_render_45b0a0.cpp.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// <direct.h> is here only for its symbols: they put the functions below in
// the register windows they match in.
#include <direct.h>

extern "C" void __cdecl GameFreeThunk(void* p);
void* __cdecl GameAllocIgnoreTag(char* name, int size);

// g_lightY is declared before g_lightX: the order decides which product of
// DrawLitPieces' lighting sum MSVC loads first.
extern float g_lightY;
extern float g_lightX;
extern float g_lightZ;
extern const float DAT_004fd4cc;
extern const float DAT_004fd4c0;

// GLOBAL: 0x511de8
struct Game;
extern Game* g_game;

// A 16.16 fixed-point number: the whole part in the high half.
union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

struct Pos_459200 {
    Fixed x;
    Fixed y;
    Fixed z;
};

// The 12-byte vector: its integer coordinates, or the three 16.16 words of a
// position, or the three words of a direction.
union Vec3 {
    int v[3];
    struct {
        int x;
        int y;
        int z;
    };
    Pos_459200 p;
};

struct Vec3f { float x; float y; float z; };

// The three shorts of a unit's base angles, or of its short position.
struct Vector3s {
    short x;                           // +0x0
    short y;                           // +0x2
    short z;                           // +0x4
};

// A 16-byte rectangle: a projected vertex with its shade, or a point of a
// 16.16 polygon.
struct Poly_459c70 { int x; int y; int z; int shade; };

struct Point_4584d0 { int x; int y; };

// A 12-byte 16.16 position with its constructor.
struct Pos_4589c0 {
    int x;
    int y;
    int z;
    Pos_4589c0(int a, int b, int c) { x = a; y = b; z = c; }
};

// A 0x18-byte frame header followed by its pixel planes: the picture the
// object-picture builder allocates, its shadow and the scratch frames.
struct Image_4589c0 {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
    short dx;                          // +0x04
    short dy;                          // +0x06
    unsigned char colour;              // +0x08
    unsigned char flag9;               // +0x09
    unsigned char count;               // +0x0a
    unsigned char kind;                // +0x0b
    char unknown_c[4];
    unsigned char* pixels;             // +0x10
    unsigned char* shade;              // +0x14
};

// The display surface MergeIntoComposite draws through.
struct Src_4589c0 {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
    unsigned short x;                  // +0x04
    unsigned short y;                  // +0x06
    char unknown_8[8];
    int bits;                          // +0x10
};

struct Surface_4589c0 {
    int width;                         // +0x00
    int height;                        // +0x04
    int pitch;                         // +0x08
    void* bits;                        // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    unsigned short x;                  // +0x18
    unsigned short y;                  // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;            // +0x2c
    unsigned int flag1 : 1;
};

class Surface;

// One primitive of a model node: its vertex indices, its picture and its
// flags.
struct FaceFlags_459c70 {
    union {
        unsigned int raw;
        struct {
            unsigned int textured : 1;
            unsigned int usePic : 1;
            unsigned int shaded : 1;
            unsigned int rest : 29;
        } bits;
    };
};

struct Face_459c70 {
    int unknown_0;                     // +0x00
    int count;                         // +0x04
    int unknown_8;                     // +0x08
    unsigned short* indices;           // +0x0c
    void* pic;                         // +0x10
    int unknown_14;                    // +0x14
    unsigned short* color;             // +0x18
    FaceFlags_459c70 flags;            // +0x1c
};

// One node of a model's piece tree: its geometry, its box relative to its
// parent and its child and sibling links.
struct Object3do {
    char unknown_0[4];
    int vertexCount;                   // +0x04
    int faceCount;                     // +0x08
    int firstFace;                     // +0x0c
    int box_x;                         // +0x10
    int box_y;                         // +0x14
    int box_z;                         // +0x18
    const char* name;                  // +0x1c
    char unknown_20[4];
    Vec3* points;                      // +0x24
    Face_459c70* faces;                // +0x28
    Object3do* sibling;                // +0x2c
    Object3do* child;                  // +0x30
};

// The flags word of one piece record, read whole or through its bits.
#pragma pack(push, 2)
union PieceFlags_459c70 {
    unsigned short word;
    struct {
        unsigned short visible : 1;
        unsigned short colored : 1;
        unsigned short lit : 1;
        unsigned short rest : 13;
    } bits;
};

// One 0x36-byte piece record: the model node, its move, turn and transform
// values, its point list, and its place in the piece tree.
struct Piece_459c70 {
    Object3do* object;                 // +0x00
    int rot_x;                         // +0x04
    int rot_y;                         // +0x08
    int rot_z;                         // +0x0c
    short short_10;                    // +0x10
    short short_12;                    // +0x12
    short short_14;                    // +0x14
    Vec3 offset;                       // +0x16
    Vec3* points;                      // +0x22
    short modified;                    // +0x26
    PieceFlags_459c70 flags;           // +0x28
    Piece_459c70* sibling;             // +0x2a
    Piece_459c70* child;               // +0x2e
    Piece_459c70* parent;              // +0x32
};
#pragma pack(pop)

// The team's map and flag word at +0x241; bit 30 is the bright-faces flag.
#pragma pack(push, 1)
union TeamFlags_459200 {
    unsigned int word;
    struct {
        unsigned int hi : 30;
        unsigned int b30 : 1;
        unsigned int lo : 1;
    } bits;
};

struct Team_459200 {
    char unknown_0[0x241];
    TeamFlags_459200 flags;            // +0x241
};
#pragma pack(pop)

struct Model_459200;
struct Unit_459200;

// The game's flags word at +0x37f06.
union Flags_37f06 {
    unsigned short whole;
    struct {
        unsigned short damagebars : 1;
        unsigned short antiAlias : 1;
        unsigned short shadows : 1;
        unsigned short vehicleShadows : 1;
        unsigned short featureShadows : 1;
        unsigned short shading : 1;
        unsigned short ditheredFog : 1;
        unsigned short unused7 : 1;
        unsigned short switchAlt : 1;
    } bits;
};

// The game state the module reads: the unit table, the render flags and the
// screen height.
class CMemoryCache;

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a43];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x1427f - 0x2a44];
    unsigned char seaLevel;            // +0x1427f
    unsigned char debugMode;           // +0x14280
    char unknown_14281[0x1437b - 0x14281];
    CMemoryCache* obj;                 // +0x1437b
    char unknown_1437f[0x37f06 - 0x1437f];
    Flags_37f06 field_37f06;           // +0x37f06
    char unknown_37f08[0x38a47 - 0x37f08];
    unsigned int field_38a47;          // +0x38a47
};
#pragma pack(pop)

// The memory cache behind the composite buffer: the arena the pictures are
// allocated from and the handles that keep them.
class CMemoryCache {
public:
    char unknown_0[0x10];
    Image_4589c0* bitmap;              // +0x10

    void ClearPointers(void);
    int InitCache(unsigned int size);
    void FreeBuffer();
    void FreeCache();
    int AllocHandle(void** handle, int size);
    int AllocBitmap(Image_4589c0** handle, int w, int h);
    int AllocTwoPlaneBitmap(Image_4589c0** handle, int w, int h);
    void FlushCache();
    void ReleaseHandle(int handle);
    void BuildShadow(Model_459200*, Image_4589c0*);
    void DrawObjectState(Model_459200*, void* context);
    void DrawObjectPieces(int param_1, Model_459200* list, Vec3 v, int param_6);
    // Defined in model_render_4584d0.cpp: only matches at that file's symbol count.
    void DrawPiece(Model_459200* model, void* surface, Vec3* camera,
        Object3do* info, Vec3* vertices, unsigned char palette, int useColor);
    // Defined in model_render_4589c0.cpp: only matches at that file's symbol count.
    void DrawObjectPicture(int param_2, Model_459200* model, Vec3 v, int useColor);
    void MergeIntoComposite(Image_4589c0* src, Model_459200* model);
    void MeasureShadow(int* width, int* height, int* originX, int* originY, Model_459200* model);
    void DrawShadowShape(Image_4589c0* view, Model_459200* model);
};

// A unit or feature instance: the object the piece tree and the pictures
// belong to.
#pragma pack(push, 1)
struct Unit_459200 {
    char unknown_0[0x64];
    Vector3s pos;                      // +0x64
    int pos_x;                         // +0x6a
    int pos_y;                         // +0x6e
    int pos_z;                         // +0x72
    char unknown_76[0x86 - 0x76];
    int field_86;                      // +0x86
    Unit_459200* list_head;            // +0x8a
    Unit_459200* list_next;            // +0x8e
    Team_459200* field_92;             // +0x92
    char unknown_96[8];
    Model_459200* sprites;             // +0x9e
    char unknown_a2[4];
    short field_a6;                    // +0xa6
    unsigned short palette;            // +0xa8
    char unknown_aa[0xff - 0xaa];
    unsigned char kind;                // +0xff
    char unknown_100[4];
    float intensity;                   // +0x104
    char unknown_108[6];
    unsigned char field_10e;           // +0x10e
    char unknown_10f;
    unsigned int flags;                // +0x110
    unsigned char field_114;           // +0x114
};
#pragma pack(pop)

// The object state of one unit: a count, the draw flags, the owner and the
// pictures, the base angles, the root piece and the piece records.
#pragma pack(push, 2)
struct Model_459200 {
    int count;                         // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    Unit_459200* owner;                // +0x0c
    Image_4589c0* bitmap;              // +0x10
    Image_4589c0* shadow;              // +0x14
    Vector3s pos;                      // +0x18
    Piece_459c70* root;                // +0x1e
    Piece_459c70 pieces[1];            // +0x22
};
#pragma pack(pop)

class UnitTable {
public:
    char unknown_0[0x10];
    union {
        int field_10;                  // +0x10
        void* buffer;                  // +0x10
        void* ptr;                     // +0x10
        Image_4589c0* shadow;          // +0x10
    };

    UnitTable* Construct(void);
    int Initialize(unsigned int size);
    void Destroy();
    void MeasureModel(int* width, int* height, int* originX, int* originY, Model_459200* model, Vec3* offset);
    int BuildObjectPicture(Model_459200* list, int param_2, int param_3);
    void DrawPieces(Image_4589c0* bitmap, Model_459200* list, int kind, int useColor);
    // Defined in model_render_4581e0.cpp: only matches at that file's symbol count.
    void DrawLitPieces(Image_4589c0* bitmap, Model_459200* list, int kind, int useColor);
};

// Unused here: the symbol ids these declarations take keep MeasureModel's
// allocation, standing in for the three view classes merged above
// (docs/c2-regalloc.md).
void WriteScreenshot(char*, char*, int, int, int, int);
int ScanDirectory(char*, char*, char*, int, int, int);
int AccessRegistryValue(char*, char*, unsigned char*, unsigned long*, unsigned long, unsigned long);

class Class_00458310 {
public:
    void AddModelBounds(int* minX, int* maxX, int* minY, int* maxY, Model_459200* model,
                        Pos_4589c0 pos);
};

class Class_00458d30 {
public:
    // level stays unsigned char: only the low byte of the quotient is needed.
    void RecolorByShade(Image_4589c0* img, unsigned char level, int above, int below, int between);
    int ShadeByIntensity(Image_4589c0* image, Model_459200* model);
};

class Class_00458fa0 {
public:
    // Defined in model_render_458fa0.cpp: only matches at that file's symbol count.
    void DrawPieceEdges(Image_4589c0* view, Model_459200* model, int color);
};

class Class_0045a470 {
public:
    char unknown_0[0x10];
    Image_4589c0* image;               // +0x10
    Image_4589c0* MakeSilhouette(Image_4589c0* src);
};

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int RIReport(int, int, int, int, int, int, int, int, int, int);
void WalkFrameChain(int*, int*, int, int, int*, int, int*, int*, int, int*);
int DrawWrappedText(char*, char*, int, int, int, int, int);
void ScaleUnitWeights(int, unsigned int*, float, int);

void* __stdcall AllocDepthFrame(const char* name, int width, int height);
Vec3f __stdcall VectorFromToInt(Vec3 a, Vec3 b);
Vec3f __stdcall CrossProduct(Vec3f a, Vec3f b);
Vec3f __stdcall NormalizeVector(Vec3f v);
void* __stdcall GetGafSequenceFrame(void** pic);
void* __stdcall GetGafFrame(unsigned short* table, int index);
void __stdcall DownsampleFrame(Image_4589c0* dst, Image_4589c0* src);
void __stdcall FillFlatPolygon(Image_4589c0* surface, void* poly, int count, int flag);
void __stdcall DrawTexturedPolygon(Image_4589c0* surface, void* pic, void* poly, int flag);
void __stdcall FillShadedPolygon(Image_4589c0* surface, void* poly, int count, int flag);
void __stdcall DrawLitTexturedPolygon(Image_4589c0* surface, void* pic, void* poly, int flag);
void __stdcall FillPolygon(void* surface, Point_4584d0* points, int count, int flags);
void __stdcall DrawFrameQuad(void* surface, void* pic, Point_4584d0* points, void* src);
void __stdcall DrawPolygonEdges(Image_4589c0* view, Vec3* points, int count, int color);
void __stdcall SurfaceFromFrame(Surface_4589c0* dst, Src_4589c0* src);
void __stdcall DrawFrame(Surface* dst, Image_4589c0* bmp, int x, int y);
int __stdcall GetGroundHeight(Pos_459200* p);
void __stdcall DrawFrameBlended(int param_1, Image_4589c0* param_2, int x, int y);
void __stdcall DrawFrameDepth(Image_4589c0* bmp, Image_4589c0* param_2, int x, int y, int z);
void __stdcall TintFrameBelow(Image_4589c0* param_1, int value);
void __stdcall CutFrameBelow(Image_4589c0* param_1, int value);
void __stdcall ZeroFramePixels(Image_4589c0* image);
void __stdcall RotateByAngles(Vec3* in, Vec3* out, short* angles);
int __stdcall CountObjects(Object3do* obj);
Piece_459c70* __stdcall AddStateEntries(Model_459200* state, Object3do* obj, Piece_459c70* parent);
Piece_459c70* __stdcall LinkStateEntries(Model_459200* state, Object3do* obj, Piece_459c70* parent);
// Defined in model_render_45b0a0.cpp: only matches at that file's symbol count.
void __fastcall PoseModel(Model_459200* state, Piece_459c70* entry, int flag);
void __fastcall TransformPieces(Model_459200* model, Piece_459c70* piece,
                                Vector3s* pos, Vec3* box, int force);

// FUNCTION: 0x458160
UnitTable* UnitTable::Construct(void)
{
    ((CMemoryCache*)this)->ClearPointers();
    field_10 = 0;
    return this;
}

// FUNCTION: 0x458180
int UnitTable::Initialize(unsigned int size)
{
    if (!((CMemoryCache*)this)->InitCache(size))
        return 0;
    buffer = AllocDepthFrame("CompositeBuffer", 600, 600);
    return buffer != 0;
}

// FUNCTION: 0x4581c0
void UnitTable::Destroy()
{
    if (ptr) {
        GameFreeThunk(ptr);
    }
    ((CMemoryCache*)this)->FreeBuffer();
}

// The 50 or 125 bias the original materialises separately in each arm of the
// doubled-bitmap test, once per projected vertex.
static __inline int shade_bias(Unit_459200* owner)
{
    bool c = ((owner->field_92->flags.word >> 30) & 1) != 0;
    return c ? 125 : 50;
}

// FUNCTION: 0x4581e0
void UnitTable::MeasureModel(int* width, int* height, int* originX, int* originY, Model_459200* model, Vec3* offset)
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    minX = maxX = minY = maxY = 0;
    for (int i = model->count - 1; i >= 0; i--) {
        Piece_459c70* piece = &model->pieces[i];
        if (piece->flags.bits.visible) {
            Vec3* v = piece->points;
            for (int n = 0; n < piece->object->vertexCount; n++) {
                int x;
                int y;
                int z;
                if (offset != 0) {
                    x = (short)((v->x + offset->x) >> 16);
                    y = (short)((offset->y + v->y) >> 16);
                    z = (short)((offset->z - v->z) >> 16);
                } else {
                    x = (short)(v->x >> 16);
                    y = (short)(v->y >> 16);
                    z = (short)(-v->z >> 16);
                }
                int sx = x;
                int sy = z - (y >> 1);
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

// Builds the composite bitmap for a model's piece list: asks 0x4581e0 for the
// bounds, allocates a one-plane (0x437b50) or two-plane (0x437be0) bitmap for
// them, stores the origin in the bitmap header and hands the drawing to
// 0x459c70 / 0x459830.
// FUNCTION: 0x4586a0
int UnitTable::BuildObjectPicture(Model_459200* list, int param_2, int param_3)
{
    // Plain ints: the stores into the short header fields narrow them.
    int w;
    int h;
    int oy;
    int ox;
    Unit_459200* owner = list->owner;
    MeasureModel(&w, &h, &oy, &ox, list, 0);
    if (param_2 == 0 && (owner->field_114 & 1) == 0 && owner->intensity == 0.0f) {
        ((CMemoryCache*)this)->AllocBitmap(&list->bitmap, w, h);
    } else {
        ((CMemoryCache*)this)->AllocTwoPlaneBitmap(&list->bitmap, w, h);
    }
    Image_4589c0* bitmap = list->bitmap;
    if (bitmap != 0) {
        bitmap->dx = (short)oy;
        bitmap->dy = (short)ox;
        // kind is an int parameter so the byte at owner+0xff is pushed zero-extended.
        if ((owner->flags & 0x20000000) != 0
            && (*(unsigned char*)&g_game->field_37f06 & 0x20) != 0) {
            DrawLitPieces(bitmap, list, owner->kind, param_3);
        } else {
            DrawPieces(bitmap, list, owner->kind, param_3);
        }
        return 1;
    }
    return 0;
}

// Draws a unit model's pieces into a bitmap. With anti-aliasing on and the
// unit flagged, it draws into the doubled shadow bitmap instead and then
// downsamples that back into the caller's bitmap.
// FUNCTION: 0x459830
void UnitTable::DrawPieces(Image_4589c0* bitmap, Model_459200* list,
    int kind, int useColor)
{
    Vec3 vertex[2000];
    Vec3 poly[25];

    int mode;
    Image_4589c0* src;
    if (g_game->field_37f06.bits.antiAlias) {
        if ((list->owner->flags & 0x20000000) != 0 && useColor != 0) {
            Image_4589c0* shadow = this->shadow;
            mode = 1;
            shadow->width = (unsigned short)(bitmap->width << 1);
            shadow->height = (unsigned short)(bitmap->height << 1);
            shadow->flag9 = 0;
            shadow->colour = 1;
            shadow->dx = (short)(bitmap->dx << 1);
            shadow->dy = (short)(bitmap->dy << 1);
            memset(shadow->shade, 0, shadow->width * shadow->height);
            memset(shadow->pixels, 1, shadow->width * shadow->height);
            // Last in the shadow branch.
            src = bitmap;
            bitmap = shadow;
        } else {
            mode = 0;
        }
    } else {
        mode = 0;
    }

    for (int p = list->count - 1; p >= 0; p--) {
        // Flags tested as positive nested ifs on the bitfield, not via a piece pointer.
        if (list->pieces[p].flags.bits.visible) {
            if (useColor == -1 || useColor == list->pieces[p].flags.bits.colored
                    || list->owner->intensity != 0.0f) {
                Object3do* info = list->pieces[p].object;
                int n = list->pieces[p].object->vertexCount;
                Vec3* verts = list->pieces[p].points;
                for (int k = 0; k < n; k++) {
                    int x;
                    int y;
                    int z;
                    // Each arm reads verts->x itself, not a shared x load before the test.
                    if (mode) {
                        x = (short)(verts->x >> 16) << 1;
                        y = (short)(verts->y >> 16) << 1;
                        z = (short)(-verts->z >> 16) << 1;
                    } else {
                        x = (short)(verts->x >> 16);
                        y = (short)(verts->y >> 16);
                        z = (short)(-verts->z >> 16);
                    }
                    vertex[k].x = x;
                    vertex[k].y = z - (y >> 1);
                    if (mode) vertex[k].z = y/2 + shade_bias(list->owner);
                    else vertex[k].z = y + shade_bias(list->owner);
                    vertex[k].x += (short)bitmap->dx;
                    vertex[k].y += (short)bitmap->dy;
                    verts++;
                }

                Face_459c70* face;
                int fi;
                if (info->firstFace != -1) {
                    face = info->faces + 1;
                    fi = 1;
                } else {
                    face = info->faces;
                    fi = 0;
                }
                for (; fi < info->faceCount; fi++, face++) {
                    unsigned short* idx = face->indices;
                    for (int j = 0; j < face->count; j++, idx++) {
                        poly[j] = vertex[*idx];
                    }
                    FaceFlags_459c70 fflags = face->flags;
                    if (!fflags.bits.textured) {
                        if (face->count == 4) {
                            void* pic;
                            if (fflags.bits.usePic) {
                                if (fflags.bits.shaded) {
                                    int unit = *(int*)((char*)g_game + 0x1b8a + kind * 0x14b);
                                    pic = GetGafFrame(face->color,
                                        *(unsigned char*)(unit + 0x96));
                                } else if (useColor) {
                                    pic = GetGafFrame(face->color, 0);
                                } else {
                                    pic = GetGafSequenceFrame(&face->pic);
                                }
                            } else {
                                pic = face->pic;
                            }
                            DrawTexturedPolygon(bitmap, pic, poly, 0);
                        }
                    } else {
                        FillFlatPolygon(bitmap, poly, face->count, face->unknown_0);
                    }
                }
            }
        }
    }

    if (g_game->field_37f06.bits.antiAlias) {
        if (mode != 0) {
            DownsampleFrame(bitmap, src);
            unsigned char* s = (unsigned char*)src->shade;
            if (s != 0) {
                unsigned char* d = (unsigned char*)bitmap->shade;
                for (int y = 0; y < src->height; y++) {
                    int x = src->width;
                    while (x--) {
                        unsigned int c = *d;
                        *s++ = c;
                        d += 2;
                    }
                    d += bitmap->width;
                }
            }
        }
    }
}

// FUNCTION: 0x458310
void Class_00458310::AddModelBounds(int* minX, int* maxX, int* minY, int* maxY,
                                    Model_459200* model, Pos_4589c0 pos)
{
    int hiY = 0;
    int loY = 0;
    int hiX = 0;
    int loX = 0;
    for (int i = model->count - 1; i >= 0; i--) {
        Piece_459c70* piece = &model->pieces[i];
        if (piece->flags.bits.visible) {
            Vec3* v = piece->points;
            for (int n = 0; n < piece->object->vertexCount; n++) {
                int x = (short)((v->x + pos.x) >> 16);
                int y = (short)((v->y + pos.y) >> 16);
                int z = (short)((pos.z - v->z) >> 16);
                int sy = z - (y >> 1);
                if (x < loX) loX = x;
                if (x > hiX) hiX = x;
                if (sy < loY) loY = sy;
                if (sy > hiY) hiY = sy;
                v++;
            }
        }
    }
    loX -= 2;
    hiX += 2;
    loY -= 2;
    hiY += 2;
    if (loX < *minX) *minX = loX;
    if (loY < *minY) *minY = loY;
    if (hiX > *maxX) *maxX = hiX;
    if (hiY > *maxY) *maxY = hiY;
}

// FUNCTION: 0x458430
void CMemoryCache::DrawObjectPieces(int param_1, Model_459200* list, Vec3 v, int param_6)
{
    if (list->bitmap != 0) {
        DrawObjectPicture(param_1, list, v, param_6);
        return;
    }
    for (int i = list->count - 1; i >= 0; i--) {
        if (list->pieces[i].flags.bits.visible) {
            DrawPiece(list, (void*)param_1, &v, list->pieces[i].object,
                                                  list->pieces[i].points, list->owner->kind, param_6);
        }
    }
}

// FUNCTION: 0x4587b0
void __stdcall HalveFrame(Image_4589c0* src, Image_4589c0* dst)
{
    unsigned char* d = dst->shade;
    if (d == 0)
        return;
    unsigned char* s = src->shade;
    for (int y = 0; y < dst->height; y++) {
        unsigned int n = dst->width;
        while (n--) {
            unsigned int c = *s;
            *d++ = c;
            s += 2;
        }
        s += src->width;
    }
}

template <class T> inline void Swap(T& a, T& b)
{
    T t = a;
    a = b;
    b = t;
}

// Grows this->bitmap to cover the model and its child pieces, then copies or
// re-blits `bmp` (pixels, then the shade plane through a pixels/shade swap) into
// it.
// FUNCTION: 0x4589c0
void CMemoryCache::MergeIntoComposite(Image_4589c0* bmp, Model_459200* model)
{
    // One named origin is passed to both AddModelBounds calls.
    Pos_4589c0 origin(0, 0, 0);
    // Outer bounds declared in this order, child bounds as cminX, cminY, cmaxX, cmaxY.
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;

    ((Class_00458310*)this)->AddModelBounds(&minX, &maxX, &minY, &maxY, model, origin);
    Unit_459200* child = model->owner->list_head;
    while (child != 0) {
        if ((child->flags & 0x20000) == 0) {
            int cminX = 0;
            int cminY = 0;
            int cmaxX = 0;
            int cmaxY = 0;
            ((Class_00458310*)this)->AddModelBounds(&cminX, &cmaxX, &cminY, &cmaxY,
                                                  child->sprites, origin);
            // The y projection goes through a Fixed temporary.
            struct Vec { Fixed x, y, z; };
            int* op = &model->owner->pos_x;
            Vec d;
            d.x.value = child->pos_x - op[0];
            d.y.value = child->pos_y - op[1];
            d.z.value = child->pos_z - op[2];
            Vec s = d;
            short ya = d.y.whole >> 1;
            Fixed yv;
            yv.value = (ya << 16) - ya + d.z.whole;
            yv.value <<= 16;
            s.y = yv;
            int xoff = s.x.whole;
            int yo = s.y.whole;
            cminX += xoff;
            cminY += yo;
            cmaxX += xoff;
            cmaxY += yo;
            if (cminX < minX) minX = cminX;
            if (cmaxX > maxX) maxX = cmaxX;
            if (cminY < minY) minY = cminY;
            if (cmaxY > maxY) maxY = cmaxY;
        }
        child = child->list_next;
    }
    int x0 = -bmp->dx;
    int x1 = bmp->width - bmp->dx;
    int y0 = -bmp->dy;
    int y1 = bmp->height - bmp->dy;
    if (minX < x0) x0 = minX;
    if (maxX > x1) x1 = maxX;
    if (minY < y0) y0 = minY;
    if (maxY > y1) y1 = maxY;
    int newW = x1 - x0;
    int newH = y1 - y0;
    x0 = -x0;
    y0 = -y0;
    this->bitmap->width = (unsigned short)newW;
    this->bitmap->height = (unsigned short)newH;
    this->bitmap->dx = (short)x0;
    this->bitmap->dy = (short)y0;
    this->bitmap->colour = bmp->colour;
    if (newW == bmp->width && newH == bmp->height) {
        memcpy(this->bitmap->pixels, bmp->pixels, bmp->width * bmp->height);
        memcpy(this->bitmap->shade, bmp->shade, bmp->width * bmp->height);
    } else {
        short sdx = 0;
        short sdy = 0;
        // All swaps and save/restores go through the inline Swap(T&, T&).
        Swap(bmp->dx, sdx);
        Swap(bmp->dy, sdy);
        Surface_4589c0 surface;
        SurfaceFromFrame(&surface, (Src_4589c0*)this->bitmap);
        memset(this->bitmap->pixels, this->bitmap->colour,
               this->bitmap->height * this->bitmap->width);
        memset(this->bitmap->shade, 0, this->bitmap->height * this->bitmap->width);
        DrawFrame((Surface*)&surface, bmp,
                     this->bitmap->dx - sdx, this->bitmap->dy - sdy);
        // Read before the first swap.
        surface.bits = this->bitmap->shade;
        Swap(bmp->pixels, bmp->shade);
        DrawFrame((Surface*)&surface, bmp,
                     this->bitmap->dx - sdx, this->bitmap->dy - sdy);
        Swap(bmp->pixels, bmp->shade);
        Swap(bmp->dx, sdx);
        Swap(bmp->dy, sdy);
    }
    ((Class_00458d30*)this)->ShadeByIntensity(this->bitmap, model);
}

struct Count_00458d20 {
    int count;                  // +0x00
};

// FUNCTION: 0x458d20
void __stdcall DecrementCountArgument(int param_1, Count_00458d20* list, int param_3)
{
    for (int i = list->count - 1; i >= 0; i--) {
    }
}

// The three branches without - 1 go through this helper: the call boundary keeps the tail merge.
static inline unsigned char Scale(int v, int num, int div) { return (unsigned char)((v * num) / div); }

// Recolours every opaque pixel by its shade: below level - 4, at or above
// level, or in between. -1 leaves the pixel, -2 makes it transparent.
// FUNCTION: 0x458d30
void Class_00458d30::RecolorByShade(Image_4589c0* img, unsigned char level, int above, int below, int between)
{
    unsigned char low;
    if (level < 4)
        low = 0;
    else
        low = level - 4;
    for (int i = 0; i < img->width * img->height; i++) {
        unsigned char* p = &img->pixels[i];
        if (img->pixels[i] != img->colour) {
            int c;
            if (img->shade[i] < low)
                c = below;
            else if (img->shade[i] >= level)
                c = above;
            else
                c = between;
            switch (c) {
            case -2:
                *p = img->colour;
                break;
            case -1:
                break;
            default:
                *p = c;
            }
        }
    }
}

// FUNCTION: 0x458dd0
int Class_00458d30::ShadeByIntensity(Image_4589c0* image, Model_459200* model)
{
    if (image->shade == 0)
        return 0;

    Unit_459200* owner = model->owner;
    if (owner->intensity == 0.0f)
        return 0;

    int b = 0;
    b = owner->palette;
    int a = b;
    b ^= 9;
    a ^= 5;
    unsigned int map = g_game->field_38a47;
    a += map * 0x21 / 30;
    b += map * 0x39 / 30;
    int palette1;
    int palette2;
    if (a & 0x10)
        palette1 = 0xaf - (a & 0xf);
    else
        palette1 = (a & 0xf) + 0xa0;
    if (b & 0x10)
        palette2 = 0xaf - (b & 0xf);
    else
        palette2 = (b & 0xf) + 0xa0;

    int alpha = (int)(owner->intensity * 255.0f);
    if (alpha > 0xeb) {
        RecolorByShade(image, Scale(alpha - 0xeb, 255, 20), -2, -2, palette1);
    } else if (alpha > 200) {
        RecolorByShade(image, Scale(alpha - 200, 255, 35), -2, -2, palette1);
    } else if (alpha > 0x73) {
        RecolorByShade(image, (unsigned char)(((0x73 - alpha) * 255) / 85 - 1), -2, palette1, palette2);
    } else if (alpha > 0x1e) {
        RecolorByShade(image, (unsigned char)(((0x1e - alpha) * 255) / 85 - 1), palette1, -1, palette2);
    } else {
        RecolorByShade(image, Scale(alpha, 255, 30), -1, -1, palette1);
    }

    ((Class_00458fa0*)this)->DrawPieceEdges(image, model, palette2);
    return 1;
}

// FUNCTION: 0x4597f0
void __stdcall SetLightVector(int param_1, int param_2, int param_3)
{
    g_lightX = (float)param_1 * 0.01f;
    g_lightY = (float)param_2 * 0.01f;
    g_lightZ = (float)param_3 * 0.01f;
}

// Copies an 8-bit image (header, pixels and the optional second plane) into
// the image at +0x10, clears its non-key pixels (ZeroFramePixels) and returns it.
// FUNCTION: 0x45a470
Image_4589c0* Class_0045a470::MakeSilhouette(Image_4589c0* src)
{
    image->width = src->width;
    image->height = src->height;
    image->dx = src->dx;
    image->dy = src->dy;
    image->colour = src->colour;
    memcpy(image->pixels, src->pixels, src->width * src->height);
    if (src->shade)
        memcpy(image->shade, src->shade, src->width * src->height);
    ZeroFramePixels(image);
    return image;
}

// FUNCTION: 0x45a510
void CMemoryCache::MeasureShadow(int* width, int* height, int* originX, int* originY, Model_459200* model)
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    minX = maxX = minY = maxY = 0;
    *width = *height = *originX = *originY = 0;
    for (int i = model->count - 1; i >= 0; i--) {
        Piece_459c70* piece = &model->pieces[i];
        if (piece->flags.bits.visible) {
            Vec3* v = piece->points;
            for (int n = 0; n < piece->object->vertexCount; n++) {
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

// FUNCTION: 0x45a610
void CMemoryCache::DrawShadowShape(Image_4589c0* view, Model_459200* model)
{
    Vec3 verts[2000];
    Vec3 tmp[25];
    // Function-level and initialised: keeps the reload order after the vertex loop.
    int segno = 0;                    // the initialiser only sets the scope, see above
    int i = model->count - 1;
    if (i >= 0) {
        // Loop variable stepped beside the index: anchors on the element base.
        Piece_459c70* piece = &model->pieces[i];
        while (i >= 0) {
            if (piece->flags.word & 1) {
                if (piece->flags.word & 2) {
                    Object3do* info = piece->object;
                    Vec3* v = piece->points;
                    for (int j = 0; j < info->vertexCount; j++) {
                        int y = (short)(v->y >> 16);
                        verts[j].x = (short)(v->x >> 16) + (y >> 2);
                        verts[j].y = (short)(-v->z >> 16) - (y >> 2);
                        verts[j].z = y + 25;
                        verts[j].x += view->dx;
                        verts[j].y += view->dy;
                        v++;
                    }
                    Face_459c70* seg = info->faces;
                    if (info->firstFace != -1) {
                        seg++;
                        segno = 1;
                    } else {
                        segno = 0;
                    }
                    // A segment with more than 25 vertices overruns tmp[].
                    for (; segno < info->faceCount; segno++, seg++) {
                        unsigned short* ip = seg->indices;
                        for (int k = 0; k < seg->count; k++, ip++) {
                            tmp[k] = verts[*ip];
                        }
                        FillFlatPolygon(view, tmp, seg->count, 0);
                    }
                }
            }
            piece--;
            i--;
        }
    }
}

// FUNCTION: 0x45a8d0
Model_459200* __stdcall CreateObjectState(Object3do* obj)
{
    int count = 1;
    if (obj->child != 0) {
        count = CountObjects(obj->child);
        count++;
    }
    if (obj->sibling != 0) {
        count += CountObjects(obj->sibling);
    }
    int size = count * 0x36 + 0x22;
    Model_459200* state = (Model_459200*)GameAllocIgnoreTag("Object State", size);
    memset(state, 0, size);
    state->root = AddStateEntries(state, obj, 0);
    state->field_8 = 1;
    return state;
}

struct BuildList_0045a950 {
    char unknown_0[8];
    int count;                          // +0x8
    char unknown_c[0x20 - 0xc];
    const char** names;                 // +0x20
};

// FUNCTION: 0x45a950
Model_459200* __stdcall CreatePlayerObjectState(Object3do* obj, BuildList_0045a950* list, int player)
{
    int count = 1;
    if (obj->child != 0) {
        count = CountObjects(obj->child);
        count++;
    }
    if (obj->sibling != 0) {
        count += CountObjects(obj->sibling);
    }
    int size = count * 0x36 + 0x22;
    Model_459200* state = (Model_459200*)GameAllocIgnoreTag("Object State", size);
    memset(state, 0, size);
    state->root = AddStateEntries(state, obj, 0);
    state->field_8 = 1;
    state->owner = (Unit_459200*)player;
    for (int i = 0; i < list->count; i++) {
        for (int j = i; j < state->count; j++) {
            if (_strcmpi(list->names[i], state->pieces[j].object->name) == 0) {
                if (i != j) {
                    Piece_459c70 temp = state->pieces[i];
                    state->pieces[i] = state->pieces[j];
                    state->pieces[j] = temp;
                }
                break;
            }
        }
    }
    state->root = LinkStateEntries(state, obj, 0);
    return state;
}

// FUNCTION: 0x45aaa0
void __stdcall FreeObjectState(Model_459200* state)
{
    for (int i = 0; i < state->count; i++) {
        GameFreeThunk(state->pieces[i].points);
    }
    if (g_game->obj != 0) {
        g_game->obj->ReleaseHandle((int)&state->bitmap);
        g_game->obj->ReleaseHandle((int)&state->shadow);
    }
    GameFreeThunk(state);
}

// Restores the vertices of every modified piece in the tree (or of every
// piece, when `force` is set) from the object and clears its offset. Defined
// here so that /Ob2 inlines its first level into UpdateObjectState and
// DrawUnit, as the original does; the out-of-line body is the loop MSVC
// makes of it.
// FUNCTION: 0x45b030
int __fastcall RestorePieceVertices(Piece_459c70* piece, int force)
{
    int result = force;
    if (piece->modified == 0 || force) {
        memcpy(piece->points, piece->object->points, piece->object->vertexCount * 12);
        piece->offset.x = 0;
        piece->offset.y = 0;
        piece->offset.z = 0;
        piece->modified = 0;
        result = 1;
    }
    // Recursive, not a loop: the first level gets inlined.
    if (piece->child)
        result = RestorePieceVertices(piece->child, result);
    if (piece->sibling)
        return RestorePieceVertices(piece->sibling, force);
    return result;
}

// Nonzero when the state's position is 8 or more away from `pos` on any axis.
// Takes a pointer to the position: other signatures change the register use.
static inline int FarFrom(Model_459200* state, const Vector3s* pos)
{
    return abs((short)(state->pos.z - pos->z)) >= 8
        || abs((short)(state->pos.y - pos->y)) >= 8
        || abs((short)(state->pos.x - pos->x)) >= 8;
}

// FUNCTION: 0x45ab10
void __stdcall UpdateObjectState(Unit_459200* unit)
{
    Model_459200* state = unit->sprites;
    if (FarFrom(state, &unit->pos)) {
        // One 6-byte struct copy.
        state->pos = unit->pos;
        state->field_8 = 1;
        state->root->modified = 0;
        if (state->root->flags.bits.colored) {
            state->field_4 = 0;
        }
    }
    if (unit->sprites->field_8 != 0) {
        RestorePieceVertices(unit->sprites->root, 0);
        PoseModel(unit->sprites, unit->sprites->root, 0);
        unit->sprites->field_8 = 0;
    }
}

// FUNCTION: 0x45ac20
void __stdcall DrawUnit(void* context, Unit_459200* unit)
{
    if (unit->field_86 == 0) {
        Model_459200* state = unit->sprites;
        if (FarFrom(state, &unit->pos)) {
            // One 6-byte struct copy.
            state->pos = unit->pos;
            state->field_8 = 1;
            state->root->modified = 0;
            if (state->root->flags.bits.colored) {
                state->field_4 = 0;
            }
        }
        if (unit->sprites->field_8 != 0) {
            RestorePieceVertices(unit->sprites->root, 0);
            PoseModel(unit->sprites, unit->sprites->root, 0);
            unit->sprites->field_8 = 0;
        }
        for (Unit_459200* u = unit->list_head; u; u = u->list_next) {
            if (!(u->flags & 0x20000)) {
                Model_459200* child = u->sprites;
                if (FarFrom(child, &u->pos)) {
                    child->pos = u->pos;
                    child->field_8 = 1;
                    child->root->modified = 0;
                    if (child->root->flags.bits.colored) {
                        child->field_4 = 0;
                    }
                }
                if (u->sprites->field_8 != 0) {
                    RestorePieceVertices(u->sprites->root, 0);
                    PoseModel(u->sprites, u->sprites->root, 0);
                    u->sprites->field_8 = 0;
                }
            }
        }
        g_game->obj->DrawObjectState(unit->sprites, context);
    }
}

// The original calls this one out of line from the state builders; in one
// file /Ob2 would inline it, so the pragmas hold it back.
#pragma auto_inline(off)

// FUNCTION: 0x45ae80
int __stdcall CountObjects(Object3do* edi)
{
    int esi = 1;
    int eax;

    eax = (int)edi->child;
    if (eax != 0) {
        esi = CountObjects((Object3do*)eax);
        esi++;
    }

    eax = (int)edi->sibling;
    if (eax != 0) {
        eax = CountObjects((Object3do*)eax);
        esi = esi + eax;
    }

    return esi;
}

#pragma auto_inline(on)

// FUNCTION: 0x45aec0
Piece_459c70* __stdcall AddStateEntries(Model_459200* state, Object3do* obj,
                                       Piece_459c70* parent)
{
    Piece_459c70* e = &state->pieces[state->count];
    e->flags.word |= 2;
    e->object = obj;
    e->modified = 0;
    e->points = (Vec3*)GameAllocIgnoreTag("Point List", obj->vertexCount * 12);
    memcpy(e->points, obj->points, obj->vertexCount * 12);
    if (obj->vertexCount >= 3) {
        e->flags.word |= 1;
    } else {
        e->flags.word &= 0xfffe;
    }
    e->flags.word |= 4;
    state->count++;
    if (obj->child != 0) {
        e->child = AddStateEntries(state, obj->child, e);
    } else {
        e->child = 0;
    }
    if (obj->sibling != 0) {
        e->sibling = AddStateEntries(state, obj->sibling, parent);
    } else {
        e->sibling = 0;
    }
    // Stored after the sibling if/else, with no early return: keeps the null
    // tests as `test reg,reg` and the argument load order.
    e->parent = parent;
    return e;
}

// FUNCTION: 0x45af90
Piece_459c70* __stdcall LinkStateEntries(Model_459200* state, Object3do* obj, Piece_459c70* parent)
{
    int i = state->count - 1;
    if (i >= 0) {
        while (1) {
            if (state->pieces[i].object == obj)
                break;
            i--;
            if (i < 0)
                return 0;
        }
        Piece_459c70* e = &state->pieces[i];
        if (e->object->sibling != 0) {
            e->sibling = LinkStateEntries(state, e->object->sibling, parent);
        } else {
            e->sibling = 0;
        }
        if (e->object->child != 0) {
            e->child = LinkStateEntries(state, e->object->child, e);
        } else {
            e->child = 0;
        }
        e->parent = parent;
        return e;
    }
    return 0;
}

// FUNCTION: 0x45b150
void __fastcall TransformPieces(Model_459200* owner, Piece_459c70* piece,
                             Vector3s* angles, Vec3* delta, int deep)
{
    for (;;) {
        if (piece->modified == 0) {
            Vec3* offset = &piece->offset;
            RotateByAngles(offset, offset, (short*)angles);
            int count = piece->object->vertexCount;
            for (int i = count - 1; i >= 0; i--)
                RotateByAngles(&piece->points[i], &piece->points[i], (short*)angles);
            offset->x += delta->x;
            offset->y += delta->y;
            offset->z += delta->z;
            count = piece->object->vertexCount;
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
