// Decompiled by space-bunny-free. Names are provisional.
// GAVE UP at 19.3 percent, 6 check.py runs. What still differs:
//   * the frame is one dword larger (`sub esp,0x24` against `sub esp,0x20`), so
//     every esp-relative slot in the body sits 4 higher and the whole tail is
//     out of alignment;
//   * `this` lands in ebx, the original keeps it in edi, and the original
//     keeps the bitmap pointer in esi across the whole body where mine spills
//     it, so the prologue and epilogue differ;
//   * the original's 7th argument to FUN_004584d0 is read from [E+0x18], one
//     dword PAST the end of this function's own 0x18-byte argument area, while
//     the epilogue is `ret 0x18`, that is it only pops its own four
//     parameters.  So the original forwards an argument it never received (see
//     the report).  No C++ spelling produces a read one slot past the
//     parameters, so that argument is a literal 0 here.
// Two things that moved the number a lot and are worth keeping:
//   * the by-value Vec3 parameter lives IN THE CALLER'S ARGUMENT AREA, MSVC 5
//     does not copy it, and the source takes a separate copy for
//     FUN_004584d0.  `Vec3 cv = v;` plus `&cv` is what produces the original's
//     `mov [esp+0x40],ebx` write-backs into the incoming slots; without that
//     copy MSVC folds everything into one local and the body shifts by 8.
//   * every fixed-point read is `v.p.<axis>.whole`, a `short` member of a
//     Fixed union, which is what gives `movsx edx, word ptr [esp + 0x42]`.

struct Vec3;
struct Model_459200;
struct Team_459200;

#pragma pack(push, 1)
struct Team_459200 {
    char unknown_0[0x241];
    int flags;                         // +0x241
};

struct Unit_459200 {
    char unknown_0[0x8a];
    Unit_459200* list_head;            // +0x8a
    Unit_459200* list_next;            // +0x8e
    Team_459200* field_92;             // +0x92
    char unknown_96[0x9e - 0x96];
    Model_459200* sprites;             // +0x9e
    char unknown_a2[2];
    short field_a6;                    // +0xa6
    int pos_x;                         // +0xa8
    int pos_y;                         // +0xac
    int pos_z;                         // +0xb0
    char unknown_b4[0xff - 0xb4];
    unsigned char kind;                // +0xff
    char unknown_100[4];
    float intensity;                   // +0x104
    char unknown_108[0x10e - 0x108];
    unsigned char field_10e;           // +0x10e
    char unknown_10f;
    int flags;                         // +0x110
    char unknown_114[0x113 - 0x110];
    unsigned char field_113;           // +0x113
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

struct Game_459200 {
    char unknown_0[0x2a43];
    unsigned char field_2a43;          // +0x2a43
    char unknown_2a44[0x10cc2];
    unsigned short field_37f06;        // +0x37f06
    char unknown_37f08[0x10a77];
    unsigned char field_1427f;         // +0x1427f
    unsigned char field_14280;         // +0x14280
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

class Class_00459200 {
public:
    char unknown_0[0x10];
    int bitmap;                       // +0x10
    void FUN_00459200(Model_459200* model, int param_2, int param_3, Vec3_459200 v);
    void FUN_004589c0(int bmp, Model_459200* model);
    void FUN_0045a790(Model_459200* model, int bmp);
    int FUN_0045a470(int bmp);
    void FUN_004586a0(Model_459200* model, int param_2, int param_3);
    void FUN_00459830(int param_1, Model_459200* model, unsigned char kind);
    void FUN_00458dd0(int bmp, Model_459200* sprites);
    void FUN_004584d0(Model_459200* list, int y, Vec3_459200* v, int field_0, int field_22,
                      unsigned char kind, int param_7);
};

int __stdcall FUN_00485070(Pos_459200* p);
void __stdcall FUN_004b7f90(int param_1, int param_2, int x, int y);
void __stdcall FUN_004b8500(int param_1, int param_2, int x, int y);
void __stdcall FUN_004b90a0(int bmp, int param_2, int x, int y, int z);
void __stdcall FUN_004b96e0(int param_1, int value);
void __stdcall FUN_004ba1b0(int param_1, int value);

// FUNCTION: 0x459200
void Class_00459200::FUN_00459200(Model_459200* model, int param_2, int param_3, Vec3_459200 v)
{
    int bmp = model->bitmap;
    if (bmp == 0)
        return;

    Vec3_459200 cv = v;
    Vec3_459200 d;
    v.v[0] = model->pos_x - v.v[0];
    v.v[1] = model->pos_y;
    v.v[2] = model->pos_z - v.v[2];
    int dx = v.p.x.whole;
    int dy = v.p.y.whole;
    int z = dy - (dx >> 1) + 0x20;
    int y = dy - (FUN_00485070((Pos_459200*)&model->pos_x) >> 1) + 0x20;

    if (model->field_14 == 0) {
        if (g_game->field_37f06 & 4) {
            int f = model->owner->field_92->flags;
            if (!(f & 0x2000000)) {
                if (model->owner->field_113 & 0x20) {
                    if (!(f & 0x40000000)) {
                        if (model->owner->field_a6 != 0 || dx >= g_game->field_1427f) {
                            if (v.v[2] == 0)
                                FUN_0045a790(model, bmp);
                            FUN_004b8500(param_2, v.v[2], v.p.x.whole + 0x85, y);
                            goto tail1;
                        }
                    }
                }
                if ((g_game->field_37f06 >> 3) & 1) {
                    if (!(f & 0x81000)) {
                        FUN_0045a470(bmp);
                        FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                    }
                }
            }
        }
    tail1:
        if (v.v[1] == 0)
            FUN_004586a0(model, 0, 1);
        if (!(model->owner->field_10e & 4) && g_game->field_14280 == 0) {
            FUN_004b7f90(param_2, bmp, v.p.x.whole + 0x80, z);
        } else {
            FUN_004b8500(param_2, bmp, v.p.x.whole + 0x80, z);
        }
        for (int i = model->count - 1; i >= 0; i--) {
            Piece_459200* piece = &model->pieces[i];
            if ((piece->flags & 1) && !(piece->flags & 2)) {
                FUN_004584d0(model, y, &cv, piece->field_0, piece->field_22,
                             model->owner->kind, 0);
            }
        }
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                Model_459200* sprites = unit->sprites;
                for (int i = sprites->count - 1; i >= 0; i--) {
                    Piece_459200* piece = &sprites->pieces[i];
                    if (piece->flags & 1) {
                        d.v[0] = unit->pos_x - model->owner->pos_x;
                        d.v[1] = unit->pos_y - model->owner->pos_y;
                        d.v[2] = unit->pos_z - model->owner->pos_z;
                        FUN_004584d0(sprites, y, &d, piece->field_0, piece->field_22,
                                     sprites->owner->kind, 0);
                    }
                }
            }
            unit = unit->list_next;
        }
        return;
    }

    {
        if (g_game->field_37f06 & 4) {
            int f = model->owner->field_92->flags;
            if (!(f & 0x2000000)) {
                if ((f >> 30) & 1) {
                    FUN_0045a470(bmp);
                    FUN_004ba1b0(this->bitmap, ((f >> 30) & 1 ? 0x4b : 0) + 0x32);
                    FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                } else {
                    if (model->owner->flags & 0x20000000) {
                        if (model->owner->field_a6 != 0 || dx >= g_game->field_1427f) {
                            if (v.v[2] == 0)
                                FUN_0045a790(model, bmp);
                            FUN_004b8500(param_2, v.v[2], v.p.x.whole + 0x85, y);
                        }
                    } else {
                        if ((g_game->field_37f06 >> 3) & 1) {
                            if (!(f & 0x81000)) {
                                FUN_0045a470(bmp);
                                if (g_game->field_1427f - dx > 0) {
                                    FUN_004ba1b0(this->bitmap,
                                                 (g_game->field_1427f - dx)
                                                     + ((f >> 30) & 1 ? 0x4b : 0) + 0x32);
                                }
                                FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                            }
                        }
                    }
                }
            }
        }
        if (v.v[1] == 0)
            FUN_004586a0(model, 0, 1);
        FUN_004589c0(v.v[1], model);
        if (!(model->owner->flags & 0x20000000) || model->owner->intensity == 0.0f)
            FUN_00459830(this->bitmap, model, model->owner->kind);
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                Model_459200* sprites = unit->sprites;
                FUN_004586a0(sprites, 1, -1);
                if (sprites->bitmap) {
                    FUN_00458dd0(sprites->bitmap, sprites);
                    d.v[0] = unit->pos_x - model->owner->pos_x;
                    d.v[1] = unit->pos_y - model->owner->pos_y;
                    d.v[2] = unit->pos_z - model->owner->pos_z;
                    int ddx = d.p.x.whole;
                    int ddy = d.p.y.whole;
                    int ddz = d.p.z.whole;
                    FUN_004b90a0(sprites->bitmap, this->bitmap, ddx, ddz - (ddy >> 1), ddy);
                }
            }
            unit = unit->list_next;
        }
        if (g_game->field_1427f - dx > 0) {
            int value = (g_game->field_1427f - dx)
                + ((model->owner->field_92->flags >> 30) & 1 ? 0x4b : 0) + 0x32;
            if (!(model->owner->flags & 0x200)
                && model->owner->kind != g_game->field_2a43) {
                FUN_004ba1b0(this->bitmap, value);
            } else {
                FUN_004b96e0(this->bitmap, value);
            }
        }
        if ((model->owner->field_92->flags >> 30) & 1)
            FUN_004ba1b0(this->bitmap, 0x7d);
        if (!(model->owner->field_10e & 4) && g_game->field_14280 == 0) {
            FUN_004b7f90(param_2, this->bitmap, v.p.x.whole + 0x80, z);
        } else {
            FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x80, z);
        }
    }
}
