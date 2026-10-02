// Decompiled by space-bunny-free, finished by GPT-6, deepseek-v4.1-flash, and GPT-6.1-sol. edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
//
// Partial, 87.6% at exactly 1506 bytes (issue #4863 took it from 82.2%).
//
// WHAT CHANGED IN #4863
//  1. The shade value is the same `c ? 125 : 50` bias that 0x459c70 and
//     0x459830 use (bit 30 of the team flags), written as a `shade_bias`
//     helper. At the two "diff" sites it is added in place, `diff +=
//     shade_bias(model);`, and diff itself is passed on. That is what gives
//     the original's `and X, 0x4b; add X, 0x32; add eax, X` with the result
//     in diff's register; every `value = bias + diff` or `(b ? 0x4b : 0) +
//     0x32 + diff` spelling either folds into `lea r, [a + b + 0x32]` or
//     moves the sum into the bias register. The tail now matches to the end
//     of the function. (The b30 arm's helper call is byte-identical to the
//     old inline bool.)
//  2. The FUN_00459830 guard is the plain test, `if ((owner->flags &
//     0x20000000) == 0 || owner->intensity == 0.0f)`. With 1 in place the old
//     `int notRender = !(...)` local is no longer needed, and dropping it
//     gives the original's `test dword ptr [ecx + 0x110], 0x20000000` and
//     lands the function on 1506 bytes (+5 points).
//  3. Free cleanups, each byte-identical: the first half's owner test reads
//     `model->owner->flags & 0x20000000` instead of a cast to byte 0x113,
//     `v.p.x.whole + 0x85` in the b30 arm, `model->field_14 == 0` in both
//     halves.
//
// STILL DIFFERING (all register choice; about 30 instructions):
//  * Prologue: the original copies v into cv in x, y, z order (eax, ecx,
//    edx) and re-reads v.x from its argument slot for the x delta. Every
//    copy/delta order and the struct copy `cv = v` were swept again on this
//    body: the field orders tie at 87.6 or lose, and `cv = v` puts f in a
//    register, shrinks the frame by 4 and costs 5 points.
//  * Both halves start with the same `mov eax, [g_game]; mov ax,
//    [eax + 0x37f06]`, so MSVC hoists it above the `jne`; the original's
//    first half uses ecx for g_game, so its two heads differ and stay put.
//    Five spellings of the flags read (assignment, whole-word copy, a game
//    local, function-scope local) are byte-identical here.
//  * The second half's b30 and b3 arms use the temp registers one step
//    rotated from the original (ecx, edx, eax where it has eax, ecx, edx),
//    and the b30 arm loads this->bitmap after the field_92 chain. Spelling
//    the arguments through locals or the helper's parameter changes nothing.
//  * `unit_has_altitude` still materialises a bool (setne) where the original
//    has `cmp word ptr [ecx + 0xa6], 0; jne`. Writing the test inline gives
//    that instruction but re-rotates the second half: 73.9%. The helper's
//    `short` local and `bool` return still matter (82.0% without the local).
//  * tools/permute.py, 15 minutes from this body: 88.0% at best, all of it
//    from noise (merged declarations, `(unsigned int)` self-casts, empty
//    do-while wrappers); no single hunk of its diff scores above 87.6.
// BUG/ODDITY (kept as found): the far-sprite test is `field_a6 != 0 || dx >=
//   field_1427f`, so the sprite is drawn when the unit is off the ground OR
//   in view range, which reads as if it should be AND. Both halves have it.
//   Also `v.v[1] = pos_y` is a plain copy where x and z are deltas.
#include <string.h>
#include <stdio.h>
struct Vec3;
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
    int bitmap;                       // +0x10
    int field_14;                      // +0x14
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


struct Game_459200 {
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

union Vec3_459200 {
    int v[3];
    Pos_459200 p;
};
#pragma pack(pop)

extern Game_459200* g_game;
extern const float DAT_004fd4c0;

struct Class_00437a30 { void FUN_0045a790(Model_459200*, int); };
struct Class_0045a470 { void FUN_0045a470(int); };
struct Class_004581e0 { void FUN_004586a0(Model_459200*,int,int); void FUN_00459830(int,Model_459200*,int,int); };
struct Class_00458d30 { void FUN_00458dd0(int,Model_459200*); };
struct Class_004584d0 { void FUN_004584d0(Model_459200*,int,Vec3_459200*,int,int,unsigned char,int); };

class Class_00459200 {
public:
    char unknown_0[0x10];
    int bitmap;                       // +0x10
    void FUN_00459200(int param_2, Model_459200* model, Vec3_459200 v, int useColor);
    void FUN_004589c0(int bmp, Model_459200* model);
};

int __stdcall FUN_00485070(Pos_459200* p);
void __stdcall FUN_004b7f90(int param_1, int param_2, int x, int y);
void __stdcall FUN_004b8500(int param_1, int param_2, int x, int y);
void __stdcall FUN_004b90a0(int bmp, int param_2, int x, int y, int z);
void __stdcall FUN_004b96e0(int param_1, int value);
void __stdcall FUN_004ba1b0(int param_1, int value);

// The unit's own vertical offset, when it has one, pushes the sprite far enough
// forward that it is drawn even beyond the view distance.
static inline bool unit_has_altitude(Unit_459200* unit)
{
    short altitude = unit->field_a6;
    return altitude != 0;
}

static inline int shade_bias(Model_459200* model)
{
    bool c = ((model->owner->field_92->flags.word >> 30) & 1) != 0;
    return c ? 125 : 50;
}

// FUNCTION: 0x459200
void Class_00459200::FUN_00459200(int param_2, Model_459200* model, Vec3_459200 v, int useColor)
{
    int bmp = model->bitmap;
    TeamFlags_459200 f;
    if (bmp == 0)
        return;

    Vec3_459200 cv;
    Vec3_459200 d;
    cv.v[0] = v.v[0];
    cv.v[2] = v.v[2];
    cv.v[1] = v.v[1];
    v.v[1] = model->owner->pos_y;
    v.v[0] = model->owner->pos_x - v.v[0];
    v.v[2] = model->owner->pos_z - v.v[2];
    int altitude = FUN_00485070((Pos_459200*)&model->owner->pos_x);
    int z = v.p.z.whole - (v.p.y.whole >> 1) + 0x20;
    int y = v.p.z.whole - (altitude >> 1) + 0x20;
    short dx = v.p.y.whole;

    if (*(int*)(0x14+bmp) == 0) {
        GameFlags_459200 gameFlags = g_game->field_37f06;
        if (gameFlags.whole & 4) {
            f = model->owner->field_92->flags;
            if ((f.word & 0x2000000) == 0) {
                if ((model->owner->flags & 0x20000000)
                    && (f.word & 0x40000000) == 0) {
                    if (model->owner->field_a6 != 0 || dx >= g_game->field_1427f) {
                        if (model->field_14 == 0)
                            ((Class_00437a30*)this)->FUN_0045a790(model,bmp);
                        FUN_004b8500(param_2, model->field_14, v.p.x.whole + 0x85, y);
                    }
                } else {
                    if (gameFlags.bits.b3) {
                        if ((f.word & 0x81000) == 0) {
                            ((Class_0045a470*)this)->FUN_0045a470(bmp);
                            FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                        }
                    }
                }
            }
        }
        if (model->bitmap == 0) {
            ((Class_004581e0*)this)->FUN_004586a0(model, 0, 1);
            bmp = model->bitmap;
        }
        if (!(model->owner->field_10e & 4) && g_game->field_14280 == 0)
            FUN_004b7f90(param_2, bmp, v.p.x.whole + 0x80, z);
        else
            FUN_004b8500(param_2, bmp, v.p.x.whole + 0x80, z);
        for (int i = model->count - 1; i >= 0; i--) {
            if ((1 & model->pieces[i].flags) && !(model->pieces[i].flags & 2)) {
                    ((Class_004584d0*)this)->FUN_004584d0(model, param_2, &cv, model->pieces[i].field_0,
                                 model->pieces[i].field_22, model->owner->kind, useColor);
                }
        }
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                for (int i = unit->sprites->count - 1; i >= 0; i--) {
                    Piece_459200* piece = &unit->sprites->pieces[i];
                    if (piece->flags & 1) {
                        ((Class_004584d0*)this)->FUN_004584d0(unit->sprites, param_2, &cv, piece->field_0, piece->field_22,
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
                    ((Class_0045a470*)this)->FUN_0045a470(bmp);
                    FUN_004ba1b0(this->bitmap, shade_bias(model));
                    FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                } else {
                    if (model->owner->flags & 0x20000000) {
                        if (unit_has_altitude(model->owner) || dx >= g_game->field_1427f) {
                            if (model->field_14 == 0)
                                ((Class_00437a30*)this)->FUN_0045a790(model,bmp);
                            FUN_004b8500(param_2, model->field_14, v.p.x.whole + 0x85, y);
                        }
                    } else {
                        if (gameFlags.bits.b3) {
                            if ((f.word & 0x81000) == 0) {
                                ((Class_0045a470*)this)->FUN_0045a470(bmp);
                                int diff = g_game->field_1427f - dx;
                                if (diff > 0) {
                                    diff += shade_bias(model);
                                    FUN_004ba1b0(this->bitmap, diff);
                                }
                                FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                            }
                        }
                    }
                }
            }
        }
        if (model->bitmap == 0) {
            ((Class_004581e0*)this)->FUN_004586a0(model, 0, 1);
            bmp = model->bitmap;
        }
        FUN_004589c0(bmp, model);
        if ((model->owner->flags & 0x20000000) == 0 || model->owner->intensity == DAT_004fd4c0)
            ((Class_004581e0*)this)->FUN_00459830(this->bitmap,model,model->owner->kind,0);
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                ((Class_004581e0*)this)->FUN_004586a0(unit->sprites,1,-1);
                if (unit->sprites->bitmap) {
                    ((Class_00458d30*)this)->FUN_00458dd0(unit->sprites->bitmap,unit->sprites);
                    int* op = &model->owner->pos_x;
                    d.v[0] = unit->pos_x - op[0];
                    d.v[1] = unit->pos_y - op[1];
                    d.v[2] = unit->pos_z - op[2];
                    int ddy = d.p.y.whole;
                    int ddz = d.p.z.whole;
                    FUN_004b90a0(unit->sprites->bitmap, this->bitmap, d.p.x.whole, ddz - (ddy >> 1), ddy);
                }
            }
            unit = unit->list_next;
        }
        int diff = g_game->field_1427f - dx;
        if (diff > 0) {
            diff += shade_bias(model);
            if ((model->owner->flags & 0x200) == 0 && model->owner->kind != g_game->field_2a43) {
                FUN_004ba1b0(this->bitmap, diff);
            } else {
                FUN_004b96e0(this->bitmap, diff);
            }
        }
        if (model->owner->field_92->flags.bits.b30)
            FUN_004ba1b0(this->bitmap, 0x7d);
        if (!(model->owner->field_10e & 4) && g_game->field_14280 == 0)
            FUN_004b7f90(param_2, this->bitmap, v.p.x.whole + 0x80, z);
        else
            FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x80, z);
    }
}
