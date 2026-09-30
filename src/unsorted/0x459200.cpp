// Decompiled by space-bunny-free, finished by GPT-6, deepseek-v4.1-flash, and GPT-6.1-sol. edited by deepseek-v4.1. Names are provisional.
// Partial (49.4%, 1516 vs 1506 bytes): address-taken 16-byte vector copy fixed
// the frame size; declaring dx/dy as short improved the score; not caching
// unit->sprites in a local (deref it at every use) keeps the unit in ebp/esi
// and gained 3.7 points. Remaining diffs: this lands in ebx instead of edi (the
// original keeps this in edi and screen y in ebx, ours is swapped), the vector
// copy/writeback and the dx slot land 4 bytes low (ours dx at esp+0x10 where
// the original spills f, its f slot pushes dx to esp+0x14), the original spills
// f to esp+0x10 while ours rematerializes owner->field_92->flags, and the
// second-half sprite loop still orders the d subtraction loads differently.
// Tried with no gain: gameFlags local reused for the >>3 test, one function
// scope int f, unused int spare. Tried and worse: plain 12-byte cv (41.7),
// pad-first 16-byte cv (45.2), int* op for owner pos in the sprite loop (40.2).
struct Vec3;
struct Model_459200;
struct Team_459200;

#pragma pack(push, 1)
struct Team_459200 {
    char unknown_0[0x241];
    int flags;                         // +0x241
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

struct Game_459200 {
    char unknown_0[0x2a43];
    unsigned char field_2a43;
    char unknown_2a44[0x1427f-0x2a44];
    unsigned char field_1427f;
    unsigned char field_14280;
    char unknown_14281[0x37f06-0x14281];
    unsigned short field_37f06;
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
void __stdcall FUN_004b96e0(int param_1, unsigned char value);
void __stdcall FUN_004ba1b0(int param_1, unsigned char value);

// FUNCTION: 0x459200
void Class_00459200::FUN_00459200(int param_2, Model_459200* model, Vec3_459200 v, int useColor)
{
    int bmp = model->bitmap;
    if (bmp == 0)
        return;

    struct CvBuffer_459200 { Vec3_459200 value; int unused; } cv;
    cv.value = v;
    Vec3_459200 d;
    v.v[0] = model->owner->pos_x - v.v[0];
    v.v[1] = model->owner->pos_y;
    v.v[2] = model->owner->pos_z - v.v[2];
    int altitude = FUN_00485070((Pos_459200*)&model->owner->pos_x);
    short dx = v.p.y.whole;
    short dy = v.p.z.whole;
    int z = dy - (dx >> 1) + 0x20;
    int y = dy - (altitude >> 1) + 0x20;

    if (*(int*)(bmp+0x14) == 0) {
        unsigned short gameFlags = g_game->field_37f06;
        if (gameFlags & 4) {
            int f = model->owner->field_92->flags;
            if (!(f & 0x2000000)) {
                if (*(unsigned char*)((char*)model->owner+0x113) & 0x20) {
                    if (!(f & 0x40000000)) {
                        if (model->owner->field_a6 != 0 || dx >= g_game->field_1427f) {
                            if (model->field_14 == 0)
                                ((Class_00437a30*)this)->FUN_0045a790(model,bmp);
                            FUN_004b8500(param_2, model->field_14, v.p.x.whole + 0x85, y);
                            goto tail1;
                        }
                    }
                }
                if ((gameFlags >> 3) & 1) {
                    if (!(f & 0x81000)) {
                        ((Class_0045a470*)this)->FUN_0045a470(bmp);
                        FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                    }
                }
            }
        }
    tail1:
        if (model->bitmap == 0) {
            ((Class_004581e0*)this)->FUN_004586a0(model, 0, 1);
            bmp = model->bitmap;
        }
        if (!(model->owner->field_10e & 4) && g_game->field_14280 == 0) {
            FUN_004b7f90(param_2, bmp, v.p.x.whole + 0x80, z);
        } else {
            FUN_004b8500(param_2, bmp, v.p.x.whole + 0x80, z);
        }
        for (int i = model->count - 1; i >= 0; i--) {
            Piece_459200* piece = &model->pieces[i];
            if ((piece->flags & 1) && !(piece->flags & 2)) {
                ((Class_004584d0*)this)->FUN_004584d0(model, param_2, &cv.value, piece->field_0, piece->field_22,
                             model->owner->kind, useColor);
            }
        }
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                for (int i = unit->sprites->count - 1; i >= 0; i--) {
                    Piece_459200* piece = &unit->sprites->pieces[i];
                    if (piece->flags & 1) {
                        ((Class_004584d0*)this)->FUN_004584d0(unit->sprites, param_2, &cv.value, piece->field_0, piece->field_22,
                                     unit->sprites->owner->kind, useColor);
                    }
                }
            }
            unit = unit->list_next;
        }
        return;
    }

    {
        unsigned short gameFlags = g_game->field_37f06;
        if (gameFlags & 4) {
            int f = model->owner->field_92->flags;
            if (!(f & 0x2000000)) {
                if ((f >> 30) & 1) {
                    ((Class_0045a470*)this)->FUN_0045a470(bmp);
                    FUN_004ba1b0(this->bitmap, ((model->owner->field_92->flags >> 30) & 1 ? 0x4b : 0) + 0x32);
                    FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                } else {
                    if (model->owner->flags & 0x20000000) {
                        if (model->owner->field_a6 != 0 || dx >= g_game->field_1427f) {
                            if (model->field_14 == 0)
                                ((Class_00437a30*)this)->FUN_0045a790(model,bmp);
                            FUN_004b8500(param_2, model->field_14, v.p.x.whole + 0x85, y);
                        }
                    } else {
                        if ((gameFlags >> 3) & 1) {
                            if (!(f & 0x81000)) {
                                ((Class_0045a470*)this)->FUN_0045a470(bmp);
                                if (g_game->field_1427f - dx > 0) {
                                    FUN_004ba1b0(this->bitmap,
                                                 (unsigned char)(g_game->field_1427f - dx)
                                                     + ((model->owner->field_92->flags >> 30) & 1 ? 0x4b : 0) + 0x32);
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
        if (!(model->owner->flags & 0x20000000) || model->owner->intensity == DAT_004fd4c0)
            ((Class_004581e0*)this)->FUN_00459830(this->bitmap,model,model->owner->kind,0);
        Unit_459200* unit = model->owner->list_head;
        while (unit) {
            if (!(unit->flags & 0x20000)) {
                ((Class_004581e0*)this)->FUN_004586a0(unit->sprites,1,-1);
                if (unit->sprites->bitmap) {
                    ((Class_00458d30*)this)->FUN_00458dd0(unit->sprites->bitmap,unit->sprites);
                    d.v[0] = unit->pos_x - model->owner->pos_x;
                    d.v[1] = unit->pos_y - model->owner->pos_y;
                    d.v[2] = unit->pos_z - model->owner->pos_z;
                    int ddx = d.p.x.whole;
                    int ddy = d.p.y.whole;
                    int ddz = d.p.z.whole;
                    FUN_004b90a0(unit->sprites->bitmap, this->bitmap, ddx, ddz - (ddy >> 1), ddy);
                }
            }
            unit = unit->list_next;
        }
        if (g_game->field_1427f - dx > 0) {
            unsigned char value = (unsigned char)(g_game->field_1427f - dx)
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
