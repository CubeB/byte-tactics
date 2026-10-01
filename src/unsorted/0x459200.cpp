// Decompiled by space-bunny-free, finished by GPT-6, deepseek-v4.1-flash, and GPT-6.1-sol. edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash retry (#3241): five more variants scored, all worse than
// this 69.9% base, so the base is kept. (1) plain int arithmetic in both
// f-diff shade sites (dropping the (unsigned char) casts the notes above
// suggest) gives the original `mov al,[..]; sub eax,edx` shape but drops to
// 58.3 (1491 bytes) because the whole second-half register rotation changes;
// removing only the standalone `value` cast is 60.9 (1527 bytes). (2) a union
// with a raw unsigned short view for the b2 test (test al,4) plus the bitfield
// view for b3 is 59.4 (1484 bytes). (3) component-wise cv.v[i] = v.v[i] copy in
// either order is 67.0/67.4 (1514 bytes): the y,z,x load order is invariant to
// statement order, so it is the compiler's own 12-byte copy expansion here and
// the x value is then reused in `sub ebx,edx`. (4) the same deltas written
// through `int* vv = (int*)&v;` is 67.0 (1514 bytes). Conclusion: this file is
// at a local optimum; the remaining 25-byte gap is the prologue copy order plus
// the f/shade register home (the original re-reads the parameter from its arg
// slot, ours keeps it live), the same allocator tie siblings 0x459830 and
// 0x458fa0 document.
// Partial, 69.9% (1538 -> 1531 vs 1506 bytes; deepseek-v4.1 retry). Four
// fixes, each verified by a check run: (1) the shade term written with a bool
// local, `bool bright = (flags >> 30) & 1;` then `(bright ? 0x4b : 0)`, makes
// MSVC5 emit the original shr/and al,1/neg/sbb sequence AND flips register
// homing: this lands in edi and the Model* parameter in ebp (it was the other
// way round at 49.7). (2) A plain 12-byte `Vec3_459200 cv` (no 16-byte
// padding struct) restores the 0x20 frame once the registers are right; the
// 16-byte version was only better while the registers were swapped. (3) The
// g_game+0x37f06 shadow word as a 1-bit bitfield (GameFlags_459200) gives the
// original `shr al,3; test al,1` instead of `test al,8`; Team_459200.flags is
// unsigned so `f >> 30` is a logical shift, and the second half's shade value
// is an int local (int value), not an unsigned char, which removed the byte
// spill. (4) The sprite-offset block writes through an int* to the owner pos
// (`int* op = &model->owner->pos_x;` then op[0]/op[1]/op[2]), which gives the
// original `add eax,0x6a` plus [eax]/[eax+4]/[eax+8] form.
// 49.7 -> 51.6 -> 54.2 -> 54.4 -> 61.6 -> 67.7 -> 69.1 -> 69.9.
// Still differing: the cv = v copy runs in y,z,x order (original x,y,z with a
// single eax/ecx/edx pass), the first half does not spill f to [esp+0x10]
// (original: mov [esp+0x10],ecx + test dword [esp+0x10],0x81000, ours keeps f
// in edx), the piece-loop counter uses ebx where the original uses eax, the
// second half keeps its own shade bool in a byte slot
// ([esp+0x4c]). Tried and worse this run: 12-byte cv before the register fix
// (44.7), int diff local + int value (60.8), component-wise cv copy and
// `Vec3 cv = v` initialisation (69.1, no change). deepseek-v4.1-flash retry:
// making field_37f06 an unsigned short and testing `field & 4` gives the
// original `mov ax,word; test al,4` in the second half but the first half
// rematerialises `test byte [..],4`, net 68.5; inlining the bright ternary
// alone gives 1509 bytes but 68.5 due to the changed value expression. The
// first half keeps al live only when f is spilled to [esp+0x10]; ours keeps f
// in edx so the compiler rematerialises gf.
// Last pass (deepseek-v4.1-flash, diff analysis only, no new build scored):
// (a) both f-diff blocks should use plain 32-bit arithmetic: original does
// xor eax,eax / mov al,[g_game+0x1427f] / sub eax,edx (edx = spilled dx),
// no byte truncation, then value = diff + shade + 0x32 with shade+0x32 in its
// own register (and ecx,0x4b / add ecx,0x32 / add eax,ecx). Our
// (unsigned char)(field_1427f - dx) cast forces byte arithmetic
// (mov bl,[..] / sub al,bl / and eax,0xff / lea eax,[eax+edx+0x32]) the
// original never emits. (b) the g_game+0x37f06 b2 test should be a plain mask
// test (original: mov ax,word / test al,4) while b3 keeps the shift form
// (shr al,3 / test al,1); ours emits mov dl,al / shr dl,2 / test dl,1 for b2.
// Untested idea: union with a whole ushort (test whole & 4) plus the bitfield
// member for b3. (c) ours spills y (screen y) to [esp+0x10] and later clobbers
// ebx with a dx reload; the original keeps y in ebx throughout and reloads dx
// into edx (mov edx,[esp+0x14] / sub eax,edx).
struct Vec3;
struct Model_459200;
struct Team_459200;

#pragma pack(push, 1)
struct Team_459200 {
    char unknown_0[0x241];
    unsigned int flags;                // +0x241
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

struct GameFlags_459200 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short rest : 12;
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

// FUNCTION: 0x459200
void Class_00459200::FUN_00459200(int param_2, Model_459200* model, Vec3_459200 v, int useColor)
{
    int bmp = model->bitmap;
    unsigned int f;
    if (bmp == 0)
        return;

    Vec3_459200 cv;
    cv = v;
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
        GameFlags_459200 gameFlags = g_game->field_37f06;
        if (gameFlags.b2) {
            f = model->owner->field_92->flags;
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
                if (gameFlags.b3) {
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
            if ((model->pieces[i].flags & 1) && !(model->pieces[i].flags & 2)) {
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
        if (gameFlags.b2) {
            f = model->owner->field_92->flags;
            if (!(f & 0x2000000)) {
                bool bright = (f >> 30) & 1;
                if (bright) {
                    ((Class_0045a470*)this)->FUN_0045a470(bmp);
                    FUN_004ba1b0(this->bitmap, (bright ? 0x4b : 0) + 0x32);
                    FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x85, y);
                } else {
                    if (model->owner->flags & 0x20000000) {
                        if (model->owner->field_a6 != 0 || dx >= g_game->field_1427f) {
                            if (model->field_14 == 0)
                                ((Class_00437a30*)this)->FUN_0045a790(model,bmp);
                            FUN_004b8500(param_2, model->field_14, v.p.x.whole + 0x85, y);
                        }
                    } else {
                        if (gameFlags.b3) {
                            if (!(f & 0x81000)) {
                                ((Class_0045a470*)this)->FUN_0045a470(bmp);
                                if (g_game->field_1427f - dx > 0) {
                                    bool bright = (model->owner->field_92->flags >> 30) & 1;
                                    FUN_004ba1b0(this->bitmap,
                                                 (unsigned char)(g_game->field_1427f - dx)
                                                     + (bright ? 0x4b : 0) + 0x32);
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
                    int* op = &model->owner->pos_x;
                    d.v[0] = unit->pos_x - op[0];
                    d.v[1] = unit->pos_y - op[1];
                    d.v[2] = unit->pos_z - op[2];
                    int ddx = d.p.x.whole;
                    int ddy = d.p.y.whole;
                    int ddz = d.p.z.whole;
                    FUN_004b90a0(unit->sprites->bitmap, this->bitmap, ddx, ddz - (ddy >> 1), ddy);
                }
            }
            unit = unit->list_next;
        }
        if (g_game->field_1427f - dx > 0) {
            bool bright = (model->owner->field_92->flags >> 30) & 1;
            int value = (unsigned char)(g_game->field_1427f - dx)
                + (bright ? 0x4b : 0) + 0x32;
            if (!(model->owner->flags & 0x200)
                && model->owner->kind != g_game->field_2a43) {
                FUN_004ba1b0(this->bitmap, value);
            } else {
                FUN_004b96e0(this->bitmap, value);
            }
        }
        bool shade = (model->owner->field_92->flags >> 30) & 1;
        if (shade)
            FUN_004ba1b0(this->bitmap, 0x7d);
        if (!(model->owner->field_10e & 4) && g_game->field_14280 == 0) {
            FUN_004b7f90(param_2, this->bitmap, v.p.x.whole + 0x80, z);
        } else {
            FUN_004b8500(param_2, this->bitmap, v.p.x.whole + 0x80, z);
        }
    }
}
