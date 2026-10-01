// Decompiled by space-bunny-free, finished by GPT-6, deepseek-v4.1-flash, and GPT-6.1-sol. edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash adoption pass (#4353): 77.0% -> 81.6%, and the compiled
// size is now 1509 bytes against the original's 1506 (it was 1505 at 77.0% and
// 1515 at the first permuter step). Every number below was measured by writing
// the variant to build/scratch/0x459200/ and scoring it with
// `check.py 0x459200 <file> --sym FUN_00459200`.
//
// WHERE THE FIRST 3.2 CAME FROM (permute.py's best_ratio.cpp, which its own log
// never credited; best.cpp only reached 78.7)
//   Each of its ten `inlN` helpers was tested individually against the file.
//   Only `inl0` was load-bearing: it is the `model->owner->field_a6 != 0` term of
//   the far-sprite test, worth +2.9 on its own (77.3% inlined, 80.2% kept). It
//   stays as `unit_has_altitude`, a static inline bool taking the Unit_459200*
//   it reads. inl2, inl4, inl6 and inl7 were worth 0.0 and are inlined here;
//   inl1, inl3, inl5, inl8 and inl9 were one-use accessors, also 0.0, inlined.
//   The rest of the permuter's output was noise and all of it was deleted: the
//   `unit = unit;` and `int same0 = value; value = same0;` self-assignments,
//   every no-op cast it had introduced, the empty `if (0 != x) { } else { ... }`
//   (rewritten as `if (x == 0) ...`), `for (; unit; )` -> `while (unit)`, the
//   `int tmp1` reload, the merged `int altitude = ..., z = ...` declaration,
//   and all the swapped commutative operands. Each was verified free first.
//   The three `do { ... } while (0);` wrappers looked load-bearing (all three
//   out together cost 3.5), but that was an artefact of the pre-81.3% state:
//   once the prologue below was fixed every one of them came out free and all
//   three are gone, so no permuter construct of that shape survives here.
//
// WHERE THE NEXT 1.4 CAME FROM (all found by hand in this pass)
//   (1) The prologue `cv = v` copy, +1.0, and the size becomes EXACTLY 1506 for
//   the first time. The earlier notes here and on the shared board called this
//   permutation-space exhausted, but only ever tried the copy and the deltas
//   GROUPED (copies-then-deltas, or the deltas interleaved with all copies
//   first). Sweeping all 36 interleavings of the three copies with the three
//   deltas finds three equally good winners (81.3%): they all put the DELTAS in
//   the order y,x,z, and differ only in the copy order. This one is
//       cv.v[0] = v.v[0];  cv.v[2] = v.v[2];  cv.v[1] = v.v[1];
//       v.v[1] = model->owner->pos_y;
//       v.v[0] = model->owner->pos_x - v.v[0];
//       v.v[2] = model->owner->pos_z - v.v[2];
//   The point is that a multi-word copy's register rotation is chosen by the
//   INTERLEAVING with what follows it, not by the copy statement itself, so the
//   search has to include the interleaving. The grouped `cv = v`, the memcpy,
//   the Pos-view copy and the three-int-temporary forms all still give 79.9.
//   (2) `unit_has_altitude` reading its field through a named local
//   (`short altitude = unit->field_a6; return altitude != 0;`), +0.3. Note a
//   bare `short a6 = ...` at the CALL SITE is much worse (74.6): the helper
//   boundary is what matters, not the local. The helper's signature also
//   matters: taking `Unit_459200*` holds and so does taking `Model_459200*`
//   and re-reading `model->owner`, but returning `int` instead of `bool` drops
//   the file to 74.6, and the `!model->field_14` and two-statement shapes vary.
//   (3) A handful of free cleanups taken at the same time, each re-checked:
//   point-of-use declarations for `piece`, `op` and `value`, `!(x & m) == 0`
//   for the three mask tests, `this->bitmap` re-read instead of an inl8 local,
//   `ddy` before `ddz`, and the `if (gameFlags.whole & 4)` paren removal.
//
// STILL DIFFERING
//   (1) The prologue copy's rotation is now one register out: ours starts with
//   z in eax and x in edx, the original with x in eax, y in ecx and z in edx,
//   and the original re-reads the x delta from its argument slot
//   (`sub ebx,[esp+0x3c]`) where ours keeps the value in edx. Every grouped
//   spelling and every other interleaving was tried; headers.py over 128
//   include sets is flat. This is the same allocator tie 0x459830 and 0x458fa0
//   document.
//   (2) The second half's b30 shade block loads this->bitmap after the
//   field_92 chain where ours loads it before, and the second half's b3 shade
//   block folds `diff + ((b ? 0x4b : 0) + 0x32)` into a single
//   `lea edx,[ecx+eax+0x32]` where the original has `add ecx,0x32; add eax,ecx`.
//   The fold is unavoidable for any spelling of that expression that still tests
//   bit 30 (5 tried: named int, 0x32 first, chained +, bool local, f.bits.b30),
//   and the one spelling that avoids it, `flags.word == 0 ? 0 : 0x4b`, scores
//   81.9% but is semantically different (the whole word instead of bit 30), so
//   it is not used. Spelling it with `f.bits.b30` instead of `(word>>30)&1`
//   drops the file to 60-79%.
//   (3) The `unit_has_altitude` call site materialises the bool with
//   `xor edx,edx / cmp word ptr [ecx+0xa6],dx / setne dl / test dl,dl` where
//   the original has `cmp word ptr [ecx+0xa6],0 / jne`. This is the cost of the
//   helper that buys +2.9, and it is the one construct here that could not be
//   made to read like a plain inline test.
//   (4) The tail's `add edx,0x32; add eax,edx` fold, same as (2).
//   The `unit_has_altitude` shape, the b3 fold and the tail fold are all
//   documented in build/scratch/SHARED.md.
// BUG/ODDITY: the far-sprite test uses `field_a6 != 0 || dx >= field_1427f`,
//   i.e. the "near" sprite is drawn when the unit is off the ground OR the view
//   distance reaches it, which reads like the two conditions should be ANDed.
//   It is spelled this way in the original, so it is kept as found.
//   Everything from the list walk's tail on matches instruction for instruction.
// deepseek-v4.1-flash retry (#4085): 1 check run, base kept. Retargeting the
// delta stores from the in-place v.v[] to the separate `d` local (with every
// later v.p.* read switched to d.p.*, which is semantically identical) scores
// 65.0 percent / 1519 bytes against this base 70.7 / 1530, so the original
// dead-argument-slot delta home is not reachable by giving the delta its own
// local.
// deepseek-v4.1-flash retry (#3971): 2 check runs, base kept. Reordering the
// three in-place v.v[] deltas to z,y,x (so the first x-use is last) is
// 70.5% / 1514 bytes, worse than this base, so the entry copy scheduling is
// not flipped by the delta statement order. The previously untested union with
// a whole `unsigned short whole;` view (b2 tested as `(gameFlags.whole & 4)`,
// b3 through `bits.b3`) is 59.9% / 1483 bytes, so the b2 mask form is not
// reachable that way either.
// deepseek-v4.1-flash retry (#3858): re-baselined at 70.7% / 1530 bytes, same
// hunks as before. The entry slot run reads the Vec3 parameter in y,z,x order
// (0x40,0x44,0x3c into 0x28,0x2c,0x24) where the original is x,y,z
// (0x3c,0x40,0x44 into 0x24,0x28,0x2c); every load/store pair maps to the
// identical slot, so it is copy scheduling, not a local layout difference.
// deepseek-v4.1-flash retry (#3631): 69.9 -> 70.7. The first half's b2 block
// was an if/goto where the original is an if/else chain: the original's `jl`
// at 0x4592f1 jumps to 0x45935c, PAST the b3 block at 0x459324, so the
// bright-path test failing must skip the b3 block. Writing the b3 test as the
// `else` of `(owner+0x113 & 0x20) && !(f & 0x40000000)` reproduces the
// original's jump targets (je/jne to 0x459324, jl to 0x45935c) and the
// control flow of both halves now matches; the goto is gone. Still differing
// (all allocator, same family as 0x459830 and 0x458fa0): the cv = v copy runs
// y,z,x with edx = x (original x,y,z with edx = z and `sub ebx,[esp+0x3c]`),
// which cascades into param_2 in ebx / y in [esp+0x10] / f in edx instead of
// y in ebx / f in [esp+0x10]; and the second half's bright path folds
// (bright ? 0x4b : 0) to 0x7d and homes its bool at [esp+0x48] where the
// original recomputes the shade from memory. Tried this pass, all worse: a
// fresh bool local for the bright path's shade value (59.4), the full
// expression there plus plain int arithmetic in both 1427f-dx sites (62.6
// with a 0x1c frame, 59.0 with a 0x20 frame), reading the transform sources
// from cv instead of v (65.8, cv moves to [esp+0x18]), plain int in the tail
// value alone (61.7), memcpy / Vec3 cv = v / cv.p = v.p / component-wise
// copies (prologue identical, y,z,x invariant). The tail from 0x4596eb and
// the last basic block match instruction for instruction.
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
                if ((0x20 & *(unsigned char*)((char*)model->owner + 0x113))
                    && (f.word & 0x40000000) == 0) {
                    if (model->owner->field_a6 != 0 || dx >= g_game->field_1427f) {
                        if (!model->field_14)
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
                    bool b = (model->owner->field_92->flags.word >> 30) & 1;
                    FUN_004ba1b0(this->bitmap, (b ? 0x4b : 0) + 0x32);
                    FUN_004b8500(param_2, this->bitmap, 0x85 + v.p.x.whole, y);
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
                                if (diff > 0)
                                    FUN_004ba1b0(this->bitmap, diff + (((model->owner->field_92->flags.word >> 30) & 1 ? 0x4b : 0) + 0x32));
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
        int notRender = !(model->owner->flags & 0x20000000);
        if (notRender || model->owner->intensity == DAT_004fd4c0)
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
            int value;
            bool b = (model->owner->field_92->flags.word >> 30) & 1;
            value = ((b ? 0x4b : 0) + 0x32) + diff;
            if ((model->owner->flags & 0x200) == 0 && model->owner->kind != g_game->field_2a43) {
                FUN_004ba1b0(this->bitmap, value);
            } else {
                FUN_004b96e0(this->bitmap, value);
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
