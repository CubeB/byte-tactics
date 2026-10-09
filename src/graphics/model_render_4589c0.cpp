// Decompiled by deepseek-v4.1-flash, space-bunny-free, LongCat 2.5 Preview Free, GPT-6, GPT-6.1-sol, deepseek-v4.1 and claude-opus-5-5. Names are provisional.

#include <string.h>
#include <stdio.h>

union Fixed { int value; struct { unsigned short fraction; short whole; }; };

struct Pos_4589c0 {
    int x;
    int y;
    int z;
    Pos_4589c0(int a, int b, int c) { x = a; y = b; z = c; }
};

struct GafFrame {
    unsigned short width;           // +0x00
    unsigned short height;          // +0x02
    short dx;                       // +0x04
    short dy;                       // +0x06
    unsigned char colour;           // +0x08
    unsigned char flag9;            // +0x09
    unsigned char count;            // +0x0a
    unsigned char kind;             // +0x0b
    char unknown_c[4];
    unsigned char* pixels;          // +0x10
    unsigned char* shade;           // +0x14
};

struct Surface_4589c0 {
    int width;                      // +0x00
    int height;                     // +0x04
    int pitch;                      // +0x08
    void* bits;                     // +0x0c
    int field_10;                   // +0x10
    int field_14;                   // +0x14
    unsigned short x;               // +0x18
    unsigned short y;               // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;         // +0x2c
    unsigned int flag1 : 1;
};

struct Model_459200;
struct UnitDef_459200;

// The unit def's type flags at +0x241 (Thaldren's UnitTypeFlags): bit 25 is noshadow and
// bit 30 is digger.
#pragma pack(push, 1)
union UnitTypeFlags_459200 {
    unsigned int word;
    struct {
        unsigned int hi : 30;
        unsigned int digger : 1;
        unsigned int lo : 1;
    } bits;
};

struct UnitDef_459200 {
    char unknown_0[0x241];
    UnitTypeFlags_459200 flags;        // +0x241
};

struct Unit_459200 {
    char unknown_0[0x6a];
    int pos_x;
    int pos_y;
    int pos_z;
    char unknown_76[0x8a-0x76];
    Unit_459200* list_head;
    Unit_459200* list_next;
    UnitDef_459200* def;
    char unknown_96[8];
    Model_459200* sprites;
    char unknown_a2[4];
    short unitDefIndex;
    char unknown_a8[0xff-0xa8];
    unsigned char kind;
    char unknown_100[4];
    float intensity;
    char unknown_108[6];
    unsigned char activateFlags;
    char unknown_10f;
    int flags;
};

struct Piece_459200 {
    int field_0;                       // +0x0
    char unknown_4[0x22 - 0x4];
    int field_22;                      // +0x22
    char unknown_26[2];
    unsigned short flags;              // +0x28
    char unknown_2a[0x36 - 0x2a];
};

struct Model_459200 {
    int count;                         // +0x0
    char unknown_4[0xc - 0x4];
    Unit_459200* owner;                // +0xc
    GafFrame* bitmap;                  // +0x10
    GafFrame* field_14;                // +0x14
    char unknown_18[0x22 - 0x18];
    Piece_459200 pieces[1];            // +0x22
    char unknown_58[0x6a - 0x58];
    int pos_x;                         // +0x6a
    int pos_y;                         // +0x6e
    int pos_z;                         // +0x72
};

union GameFlags_459200 {
    unsigned short whole;
    struct {
        unsigned short b0 : 1;
        unsigned short b1 : 1;
        unsigned short b2 : 1;
        unsigned short b3 : 1;
        unsigned short rest : 12;
    } bits;
};

struct Game {
    char unknown_0[0x2a43];
    unsigned char playerIndex;
    char unknown_2a44[0x1427f-0x2a44];
    unsigned char seaLevel;
    unsigned char debugMode;
    char unknown_14281[0x37f06-0x14281];
    GameFlags_459200 visualFlags;
};

struct Fixed_459200 {
    unsigned short frac;
    short whole;
};

struct Pos_459200 {
    Fixed_459200 x;
    Fixed_459200 y;
    Fixed_459200 z;
};

struct Ints_459200 {
    int x;
    int y;
    int z;
};

union Vec3_459200 {
    int v[3];
    Pos_459200 p;
    Ints_459200 i;
};
#pragma pack(pop)

class Surface;

void __stdcall SurfaceFromFrame(Surface_4589c0* dst, GafFrame* src);
void __stdcall DrawFrame(Surface* dst, GafFrame* bmp, int x, int y);

template <class T> inline void Swap(T& a, T& b)
{
    T t = a;
    a = b;
    b = t;
}

extern Game* g_game;
extern const float DAT_004fd4c0;

struct UnitTable { void BuildObjectPicture(Model_459200*,int,int); void DrawPieces(GafFrame*,Model_459200*,int,int); };

// The cache under the name the picture code uses: the same object, so it
// derives from UnitTable to reach BuildObjectPicture and DrawPieces without a
// cast (the empty base costs nothing).
struct CMemoryCache : UnitTable {
    char unknown_0[0x10];
    GafFrame* bitmap;               // +0x10

    void BuildShadow(Model_459200*, GafFrame*);
    void DrawPiece(Model_459200*, int, Vec3_459200*, int, int, unsigned char, int);
    void DrawObjectPicture(int param_2, Model_459200* model, Vec3_459200 v, int useColor);
    void MergeIntoComposite(GafFrame* src, Model_459200* model);
    int ShadeByIntensity(GafFrame* image, Model_459200* model);
    GafFrame* MakeSilhouette(GafFrame* src);
};

int __stdcall GetGroundHeight(Pos_459200* p);
void __stdcall DrawFrameBlended(int param_1, GafFrame* param_2, int x, int y);
void __stdcall DrawFrameDepth(GafFrame* bmp, GafFrame* param_2, int x, int y, int z);
void __stdcall TintFrameBelow(GafFrame* param_1, int value);
void __stdcall CutFrameBelow(GafFrame* param_1, int value);

// digger_bias (if/return) is used only in the digger arm; other sites keep shade_bias.
static inline int digger_bias(Model_459200* model)
{
    if (model->owner->def->flags.bits.digger)
        return 125;
    return 50;
}

static inline int shade_bias(Model_459200* model)
{
    bool c = ((model->owner->def->flags.word >> 30) & 1) != 0;
    return c ? 125 : 50;
}

// Draws a model relative to the camera position `v` (the 16.16 vector the
// callers pass by value), then its attached units. It stays in its own file:
// the merged model_render.cpp cannot place it at the symbol count its
// registers need.
//
// BUG/ODDITY (kept as found): the far-sprite test is `unitDefIndex != 0 || dx >=
//   seaLevel`, so the shadow is drawn for every unit whose def index is not
//   0, whatever dx is, which reads as if it should be AND. Both halves have it.
//   Also `v.v[1] = pos_y` is a plain copy where x and z are deltas.
//
// Must stay before MergeIntoComposite (0x4589c0): compiled after it, the sum in
// the b3 arm goes into the wrong register.
// FUNCTION: 0x459200
void CMemoryCache::DrawObjectPicture(int param_2, Model_459200* model, Vec3_459200 v, int useColor)
{
    GafFrame* bmp = model->bitmap;
    UnitTypeFlags_459200 f;
    if (bmp == 0)
        return;

    Vec3_459200 cv;
    Vec3_459200 d;
    // The copy must read through the argument list, not `v`: x delta reads v,
    // z delta reads the copy, and the deltas are written into v itself.
    cv = *(Vec3_459200*)(&param_2 + 2);
    v.v[0] = model->owner->pos_x - v.v[0];
    v.v[1] = model->owner->pos_y;
    v.v[2] = model->owner->pos_z - cv.v[2];
    int altitude = GetGroundHeight((Pos_459200*)&model->owner->pos_x);
    int z = v.p.z.whole - (v.p.y.whole >> 1) + 0x20;
    int y = v.p.z.whole - (altitude >> 1) + 0x20;
    short dx = v.p.y.whole;

    if (bmp->shade == 0) {
        GameFlags_459200 gameFlags = g_game->visualFlags;
        if (gameFlags.whole & 4) {
            f = model->owner->def->flags;
            if ((f.word & 0x2000000) == 0) {
                if ((model->owner->flags & 0x20000000)
                    && (f.word & 0x40000000) == 0) {
                    if (model->owner->unitDefIndex != 0 || dx >= g_game->seaLevel) {
                        if (model->field_14 == 0)
                            BuildShadow(model,bmp);
                        DrawFrameBlended(param_2, model->field_14, v.p.x.whole + 0x85, y);
                    }
                } else {
                    if (gameFlags.bits.b3) {
                        if ((f.word & 0x81000) == 0) {
                            MakeSilhouette(bmp);
                            DrawFrameBlended(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                        }
                    }
                }
            }
        }
        if (model->bitmap == 0) {
            this->BuildObjectPicture(model, 0, 1);
            bmp = model->bitmap;
        }
        if (!(model->owner->activateFlags & 4) && g_game->debugMode == 0)
            DrawFrame((Surface*)param_2, bmp, v.p.x.whole + 0x80, z);
        else
            DrawFrameBlended(param_2, bmp, v.p.x.whole + 0x80, z);
        for (int i = model->count - 1; i >= 0; i--) {
            if ((1 & model->pieces[i].flags) && !(model->pieces[i].flags & 2)) {
                    DrawPiece(model, param_2, &cv, model->pieces[i].field_0,
                                 model->pieces[i].field_22, model->owner->kind, useColor);
                }
        }
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                for (int i = unit->sprites->count - 1; i >= 0; i--) {
                    Piece_459200* piece = &unit->sprites->pieces[i];
                    if (piece->flags & 1) {
                        DrawPiece(unit->sprites, param_2, &cv, piece->field_0, piece->field_22,
                                     unit->sprites->owner->kind, useColor);
                    }
                }
            }
            unit = unit->list_next;
        }
        return;
    }

    {
        GameFlags_459200 gameFlags = g_game->visualFlags;
        if (gameFlags.whole & 4) {
            f = model->owner->def->flags;
            if ((f.word & 0x2000000) == 0) {
                if (f.bits.digger) {
                    MakeSilhouette(bmp);
                    CutFrameBelow(this->bitmap, digger_bias(model));
                    DrawFrameBlended(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                } else {
                    if (model->owner->flags & 0x20000000) {
                        if (model->owner->unitDefIndex != 0 || dx >= g_game->seaLevel) {
                            if (model->field_14 == 0)
                                BuildShadow(model,bmp);
                            DrawFrameBlended(param_2, model->field_14, v.p.x.whole + 0x85, y);
                        }
                    } else {
                        if (gameFlags.bits.b3) {
                            if ((f.word & 0x81000) == 0) {
                                MakeSilhouette(bmp);
                                int diff = g_game->seaLevel - dx;
                                if (diff > 0) {
                                    diff += shade_bias(model);
                                    // The empty do-while ends the block: keeps the sum in diff's register.
                                    do {} while (0);    // emits no code; needed for the match
                                    CutFrameBelow(this->bitmap, diff);
                                }
                                DrawFrameBlended(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                            }
                        }
                    }
                }
            }
        }
        if (model->bitmap == 0) {
            this->BuildObjectPicture(model, 0, 1);
            bmp = model->bitmap;
        }
        MergeIntoComposite(bmp, model);
        if ((model->owner->flags & 0x20000000) == 0 || model->owner->intensity == DAT_004fd4c0)
            this->DrawPieces(this->bitmap,model,model->owner->kind,0);
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                this->BuildObjectPicture(unit->sprites,1,-1);
                if (unit->sprites->bitmap) {
                    ShadeByIntensity(unit->sprites->bitmap,unit->sprites);
                    // Owner position read through int* op.
                    int* op = &model->owner->pos_x;
                    d.v[0] = unit->pos_x - op[0];
                    d.v[1] = unit->pos_y - op[1];
                    d.v[2] = unit->pos_z - op[2];
                    int ddy = d.p.y.whole;
                    int ddz = d.p.z.whole;
                    DrawFrameDepth(unit->sprites->bitmap, this->bitmap, d.p.x.whole, ddz - (ddy >> 1), ddy);
                }
            }
            unit = unit->list_next;
        }
        int diff = g_game->seaLevel - dx;
        if (diff > 0) {
            diff += shade_bias(model);
            if ((model->owner->flags & 0x200) == 0 && model->owner->kind != g_game->playerIndex) {
                CutFrameBelow(this->bitmap, diff);
            } else {
                TintFrameBelow(this->bitmap, diff);
            }
        }
        if (model->owner->def->flags.bits.digger)
            CutFrameBelow(this->bitmap, 0x7d);
        if (!(model->owner->activateFlags & 4) && g_game->debugMode == 0)
            DrawFrame((Surface*)param_2, this->bitmap, v.p.x.whole + 0x80, z);
        else
            DrawFrameBlended(param_2, this->bitmap, v.p.x.whole + 0x80, z);
    }
}
