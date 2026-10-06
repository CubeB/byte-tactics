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

struct Image_4589c0 {
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

struct Src_4589c0 {
    unsigned short width;           // +0x00
    unsigned short height;          // +0x02
    unsigned short x;               // +0x04
    unsigned short y;               // +0x06
    char unknown_8[8];
    int bits;                       // +0x10
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
struct Team_459200;

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

struct Unit_459200 {
    char unknown_0[0x6a];
    int pos_x;
    int pos_y;
    int pos_z;
    char unknown_76[0x8a-0x76];
    Unit_459200* list_head;
    Unit_459200* list_next;
    Team_459200* field_92;
    char unknown_96[8];
    Model_459200* sprites;
    char unknown_a2[4];
    short field_a6;
    char unknown_a8[0xff-0xa8];
    unsigned char kind;
    char unknown_100[4];
    float intensity;
    char unknown_108[6];
    unsigned char field_10e;
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
    Image_4589c0* bitmap;              // +0x10
    Image_4589c0* field_14;            // +0x14
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
    unsigned char field_2a43;
    char unknown_2a44[0x1427f-0x2a44];
    unsigned char field_1427f;
    unsigned char field_14280;
    char unknown_14281[0x37f06-0x14281];
    GameFlags_459200 field_37f06;
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

class Class_00458310 {
public:
    void AddModelBounds(int* minX, int* maxX, int* minY, int* maxY, Model_459200* model,
                      Pos_4589c0 pos);
};

class Class_00458d30 {
public:
    int ShadeByIntensity(Image_4589c0* image, Model_459200* model);
};

class Surface;

void __stdcall SurfaceFromFrame(Surface_4589c0* dst, Src_4589c0* src);
void __stdcall DrawFrame(Surface* dst, Image_4589c0* bmp, int x, int y);

template <class T> inline void Swap(T& a, T& b)
{
    T t = a;
    a = b;
    b = t;
}

extern Game* g_game;
extern const float DAT_004fd4c0;

struct CMemoryCache { void BuildShadow(Model_459200*, Image_4589c0*); };
struct Class_0045a470 { void MakeSilhouette(Image_4589c0*); };
struct Class_004581e0 { void BuildObjectPicture(Model_459200*,int,int); void DrawPieces(Image_4589c0*,Model_459200*,int,int); };
struct Class_004584d0 { void DrawPiece(Model_459200*,int,Vec3_459200*,int,int,unsigned char,int); };

int __stdcall GetGroundHeight(Pos_459200* p);
void __stdcall DrawFrameBlended(int param_1, Image_4589c0* param_2, int x, int y);
void __stdcall DrawFrameDepth(Image_4589c0* bmp, Image_4589c0* param_2, int x, int y, int z);
void __stdcall TintFrameBelow(Image_4589c0* param_1, int value);
void __stdcall CutFrameBelow(Image_4589c0* param_1, int value);

static inline int team_bias(Model_459200* model)
{
    if (model->owner->field_92->flags.bits.b30)
        return 125;
    return 50;
}

static inline int shade_bias(Model_459200* model)
{
    bool c = ((model->owner->field_92->flags.word >> 30) & 1) != 0;
    return c ? 125 : 50;
}

class Class_00459200 {
public:
    char unknown_0[0x10];
    Image_4589c0* bitmap;           // +0x10

    void DrawObjectPicture(int param_2, Model_459200* model, Vec3_459200 v, int useColor);
    void MergeIntoComposite(Image_4589c0* src, Model_459200* model);
};

// Draws a model relative to the camera position `v` (the 16.16 vector the
// callers pass by value), then its attached units.
//
// It comes before MergeIntoComposite (0x4589c0) on purpose: compiled after it,
// the b3 arm's `diff += shade_bias(model)` sums into ecx (`add ecx, eax`)
// instead of diff's register, whatever the symbol count.
//
// MATCH (#5309, from 90.1%). The last two changes:
//  * The camera copy reads the argument list, not `v` itself:
//    `cv = *(Vec3_459200*)(&param_2 + 2);`. The original loads v.x twice
//    (for the copy, then `sub ebx, [esp+0x3c]` for the x delta) but keeps
//    the copy's z in edx for the z delta (`sub eax, edx`). With any copy
//    that MSVC sees as a read of `v` (`cv = v`, member copies in every order,
//    memcpy, a pointer to v, inline copy helpers by pointer, reference or
//    value: several hundred spellings over five passes) the copy's x load becomes
//    a common subexpression of the x delta's read, and tools/c2prio.py shows
//    both x and z as 2-reference candidates at the same priority; register
//    pressure at the x delta (owner, the lea'd argument, bmp, this, model)
//    then makes C2 split z, the one live through it, and keep x in edx.
//    Read through another argument's address, the copy's loads belong to
//    that argument, so the x delta reads v.x afresh and the z delta, which
//    reads the copy (`cv.v[2]`), gets the copy's own load forwarded in edx.
//    Anchored on `model` (`&model + 1`) the registers are right but the
//    owner load stays below the copy's stores (99.8%); a struct view of the
//    whole argument list or a memcpy from `&param_2 + 2` moves the loop
//    counters out of param_2's slot (98.7%). Writing the deltas into an
//    uninitialised local that shares v's dead slot gives the same prologue
//    code, but then f and d take that slot and the frame shrinks by 4, so
//    the deltas do live in `v` itself.
//  * The z delta reads the copy and the x delta reads `v`; reading both from
//    `v` loads x early into ebx and z from memory (87.1%).
//  * One empty `do {} while (0);` after `diff += shade_bias(model)` in the b3
//    arm: it emits no code but ends the code generator's block there, so the
//    sum goes into diff's register (`add eax, ecx`) as in the original. It is
//    most likely a debug macro that compiled to nothing (as in 0x459830 and
//    0x459c70); without it this is 99.6%.
// Earlier levers that still matter (#4924): the b30 arm's bias is the
// if/return helper `team_bias` and the other two sites keep the ternary
// `shade_bias`; the second half's altitude test is the inline
// `model->owner->field_a6 != 0`; the attached-unit deltas read the owner's
// position through `int* op` (the original's `add eax, 0x6a` base).
// BUG/ODDITY (kept as found): the far-sprite test is `field_a6 != 0 || dx >=
//   field_1427f`, so the sprite is drawn when the unit is off the ground OR
//   in view range, which reads as if it should be AND. Both halves have it.
//   Also `v.v[1] = pos_y` is a plain copy where x and z are deltas.
// FUNCTION: 0x459200
void Class_00459200::DrawObjectPicture(int param_2, Model_459200* model, Vec3_459200 v, int useColor)
{
    Image_4589c0* bmp = model->bitmap;
    TeamFlags_459200 f;
    if (bmp == 0)
        return;

    Vec3_459200 cv;
    Vec3_459200 d;
    // The camera copy, read through the argument list rather than `v`: see
    // the notes at the top.
    cv = *(Vec3_459200*)(&param_2 + 2);
    v.v[0] = model->owner->pos_x - v.v[0];
    v.v[1] = model->owner->pos_y;
    v.v[2] = model->owner->pos_z - cv.v[2];
    int altitude = GetGroundHeight((Pos_459200*)&model->owner->pos_x);
    int z = v.p.z.whole - (v.p.y.whole >> 1) + 0x20;
    int y = v.p.z.whole - (altitude >> 1) + 0x20;
    short dx = v.p.y.whole;

    if (bmp->shade == 0) {
        GameFlags_459200 gameFlags = g_game->field_37f06;
        if (gameFlags.whole & 4) {
            f = model->owner->field_92->flags;
            if ((f.word & 0x2000000) == 0) {
                if ((model->owner->flags & 0x20000000)
                    && (f.word & 0x40000000) == 0) {
                    if (model->owner->field_a6 != 0 || dx >= g_game->field_1427f) {
                        if (model->field_14 == 0)
                            ((CMemoryCache*)this)->BuildShadow(model,bmp);
                        DrawFrameBlended(param_2, model->field_14, v.p.x.whole + 0x85, y);
                    }
                } else {
                    if (gameFlags.bits.b3) {
                        if ((f.word & 0x81000) == 0) {
                            ((Class_0045a470*)this)->MakeSilhouette(bmp);
                            DrawFrameBlended(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                        }
                    }
                }
            }
        }
        if (model->bitmap == 0) {
            ((Class_004581e0*)this)->BuildObjectPicture(model, 0, 1);
            bmp = model->bitmap;
        }
        if (!(model->owner->field_10e & 4) && g_game->field_14280 == 0)
            DrawFrame((Surface*)param_2, bmp, v.p.x.whole + 0x80, z);
        else
            DrawFrameBlended(param_2, bmp, v.p.x.whole + 0x80, z);
        for (int i = model->count - 1; i >= 0; i--) {
            if ((1 & model->pieces[i].flags) && !(model->pieces[i].flags & 2)) {
                    ((Class_004584d0*)this)->DrawPiece(model, param_2, &cv, model->pieces[i].field_0,
                                 model->pieces[i].field_22, model->owner->kind, useColor);
                }
        }
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                for (int i = unit->sprites->count - 1; i >= 0; i--) {
                    Piece_459200* piece = &unit->sprites->pieces[i];
                    if (piece->flags & 1) {
                        ((Class_004584d0*)this)->DrawPiece(unit->sprites, param_2, &cv, piece->field_0, piece->field_22,
                                     unit->sprites->owner->kind, useColor);
                    }
                }
            }
            unit = unit->list_next;
        }
        return;
    }

    {
        GameFlags_459200 gameFlags = g_game->field_37f06;
        if (gameFlags.whole & 4) {
            f = model->owner->field_92->flags;
            if ((f.word & 0x2000000) == 0) {
                if (f.bits.b30) {
                    ((Class_0045a470*)this)->MakeSilhouette(bmp);
                    CutFrameBelow(this->bitmap, team_bias(model));
                    DrawFrameBlended(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                } else {
                    if (model->owner->flags & 0x20000000) {
                        if (model->owner->field_a6 != 0 || dx >= g_game->field_1427f) {
                            if (model->field_14 == 0)
                                ((CMemoryCache*)this)->BuildShadow(model,bmp);
                            DrawFrameBlended(param_2, model->field_14, v.p.x.whole + 0x85, y);
                        }
                    } else {
                        if (gameFlags.bits.b3) {
                            if ((f.word & 0x81000) == 0) {
                                ((Class_0045a470*)this)->MakeSilhouette(bmp);
                                int diff = g_game->field_1427f - dx;
                                if (diff > 0) {
                                    diff += shade_bias(model);
                                    do {} while (0);    // no code: see the notes at the top
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
            ((Class_004581e0*)this)->BuildObjectPicture(model, 0, 1);
            bmp = model->bitmap;
        }
        MergeIntoComposite(bmp, model);
        if ((model->owner->flags & 0x20000000) == 0 || model->owner->intensity == DAT_004fd4c0)
            ((Class_004581e0*)this)->DrawPieces(this->bitmap,model,model->owner->kind,0);
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                ((Class_004581e0*)this)->BuildObjectPicture(unit->sprites,1,-1);
                if (unit->sprites->bitmap) {
                    ((Class_00458d30*)this)->ShadeByIntensity(unit->sprites->bitmap,unit->sprites);
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
        int diff = g_game->field_1427f - dx;
        if (diff > 0) {
            diff += shade_bias(model);
            if ((model->owner->flags & 0x200) == 0 && model->owner->kind != g_game->field_2a43) {
                CutFrameBelow(this->bitmap, diff);
            } else {
                TintFrameBelow(this->bitmap, diff);
            }
        }
        if (model->owner->field_92->flags.bits.b30)
            CutFrameBelow(this->bitmap, 0x7d);
        if (!(model->owner->field_10e & 4) && g_game->field_14280 == 0)
            DrawFrame((Surface*)param_2, this->bitmap, v.p.x.whole + 0x80, z);
        else
            DrawFrameBlended(param_2, this->bitmap, v.p.x.whole + 0x80, z);
    }
}

// MATCH (claude-opus-5-5, #5067, from 93.6%). Grows this->bitmap to cover the model
// and its child pieces, then copies or re-blits `bmp` (pixels, then the shade plane
// through a pixels/shade swap) into it. The last three levers:
//   * One named `origin(0, 0, 0)` is passed to both AddModelBounds calls. Built in
//     place for the first call it gives the three separate zero registers; in the
//     loop its fields are rematerialised from one `xor eax, eax`. A fresh
//     `Pos_4589c0()` (memset) in the loop stores the middle field through a
//     `mov ecx, eax` copy, and three-store constructors give three xors.
//   * The pixels/shade swaps and the dx/dy save and restore go through one inline
//     `Swap(T&, T&)`. Through the references MSVC 5 keeps each load and store in
//     source order (load dx, store dx, load dy, store dy); written out by hand it
//     hoists the second load above the first store.
// Earlier levers that still matter: the y projection goes through a `Fixed`
// temporary (one expression folds to `(z - ya) << 16` and loses the 16-bit sar);
// the outer bounds are declared `minX, maxX, minY, maxY` and the child ones
// `cminX, cminY, cmaxX, cmaxY` (frame order); `surface.bits = ...shade` is read
// before the first swap, as the original loads it before the swap stores.
// FUNCTION: 0x4589c0
void Class_00459200::MergeIntoComposite(Image_4589c0* bmp, Model_459200* model)
{
    Pos_4589c0 origin(0, 0, 0);
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
        Swap(bmp->dx, sdx);
        Swap(bmp->dy, sdy);
        Surface_4589c0 surface;
        SurfaceFromFrame(&surface, (Src_4589c0*)this->bitmap);
        memset(this->bitmap->pixels, this->bitmap->colour,
               this->bitmap->height * this->bitmap->width);
        memset(this->bitmap->shade, 0, this->bitmap->height * this->bitmap->width);
        DrawFrame((Surface*)&surface, bmp,
                     this->bitmap->dx - sdx, this->bitmap->dy - sdy);
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
