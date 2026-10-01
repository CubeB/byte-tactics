// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Slot 0 of Class_00407a90 (vtable 0x4fc998), derived from Class_00407350
// (the family is listed in 0x407350.cpp, whose declarations this copies).
// Sets field_c to 30..929 ticks from now. A group of fewer than 5 units
// either moves (mode 9) to the unit nearest its average position, or, when
// FUN_0040ba80 gives a rally point for field_10, is sent to 2..3 random points
// around it (mode 2 first, then mode 9). A bigger group is sent to a random
// point on the map edge.
//
// Partial (91.1%): everything outside the scatter loop matches byte for byte,
// including the whole "5 units or more" branch. One hunk differs, the loop
// preheader plus the first half of the loop body, and it is one block-ordering
// decision:
// - The original splits the loop preheader: `sar esi,3; sar edi,3` (w and h)
//   come before the entry test `cmp ecx,ebx; jle`, and the two halves (ebp =
//   w/2, [esp+0x14] = h/2) come after it as a block of their own. Here the
//   halves are computed before the test, and the test is `test ecx,ecx`,
//   emitted between the halves' last shift and the spill of h/2.
// - In the body the original loads pos.x into edx and adds the shifted random
//   delta into it (`add edx, eax`), loading pos.y only after dest.x is
//   finished; here pos.x goes to ecx, pos.y to edx, both are loaded up front
//   and both sums land in eax. Same knock-on: which register the block takes
//   as its first free temp.
// - space-bunny-free re-confirmed the walls the earlier passes hit. Both are
//   register-allocation walls, not byte counts: the early `mov [esp+0x14],
//   esi` spill of `this` (which is what frees esi for w), `this` living in
//   esi, and ebp holding w/2. Writing the halves as loop-body expressions
//   (w / 2 inline, so LICM would sink them below the test) drops the file to
//   27.2%: the early spill of `this` disappears and ebx becomes ebp. An
//   explicit `if (n > 0) { int hw, hh; for (...) }` is 58.7% and puts `this`
//   in ebp, as the notes said. Declaring w and h in ONE statement is
//   byte-identical to two statements, so that split is not a statement
//   boundary effect either.
// - deepseek-v4.1: both remaining ideas collapse the whole allocation, so the
//   91.1% form is a hard local optimum. Declaring hw and hh inside the loop
//   body (LICM then hoists them into the preheader after the entry test) is
//   52.9%: `this` leaves esi for ebx and the loop counter takes ebp. Dropping
//   the `dx` local and inlining the FUN_004b6c30(w) call into the dest.x
//   expression is 52.0% (595 bytes) with the same shake-up. Writing the loop
//   as `for (int i = 0; n > i; i++)` is 90.2%: it only swaps the bottom test
//   to `cmp eax,ebx; jg` and moves the `if (i == 0)` test, while our top guard
//   stays `test ecx,ecx`. The `cmp ecx,ebx` form is part of the block split,
//   not of the loop condition's spelling. An early `if (n <= 0) return;` plus
//   a do-while is 59.0% (`this` moves to ebp), and moving w and h into the
//   loop with the halves is 19.5% (frame drops to 0x20: the divisions are not
//   hoisted).
// - Untried: making the halves depend on something LICM cannot hoist out of
//   the body (a value reloaded from the group), which is the only way seen so
//   far to get a block between the entry test and the body without opening a
//   nested scope.
// - tools/headers.py: no header set does better than 91.1%.
// - deepseek-v4.1-flash retry: re-derived the two branches the note already
//   lists and confirmed them. `if (n > i) { int hw, hh; do {...} while (i < n); }`
//   with i declared at else-scope is 52.9% (same as declaring hw/hh in the
//   body): the guard `cmp ecx,ebx` does appear, but declaring the counter at
//   branch scope moves `this` out of esi (to ebx) at the very top of the
//   function, so the whole function shifts. `int i = 0; if (n > i) { int hw,
//   hh; for (; i < n; i++) }` is 52.7% with the same shake-up. The 91.1% form
//   above remains the best: the only real difference is the loop preheader
//   ordering (halves computed before the entry test, and `test ecx, ecx`
//   instead of `cmp ecx, ebx`) plus the register the body loads pos.x into.
// - deepseek-v4.1-flash retry 2 (brute-forced about 30 scratch variants, all
//   scored with check.py --sym): the `cmp ecx, ebx` guard only appears once
//   the counter `i` is a named variable declared at branch scope, but that
//   alone moves `this` from esi to ebx at the first instruction (the whole
//   function shifts, 52.9%). Ordering the halves as an assignment inside the
//   `if` (the do-while form that reproduces the original block order) always
//   triggers that same `this` -> ebx move: `int i=0; int hw,hh; if (n>i) {
//   hw=w/2; hh=h/2; do {...} while (i<n); }` is 52.9% and `this` leaves esi.
//   Moving the for-init scope earlier (`int i,hw,hh; for (i=0; ...)`) is
//   52.9%, and `for (int i=0; n>i; i++)` / `while (n>i)` are 90.2%: they only
//   swap the bottom test to `jg`. Keeping the counter in the for-init and the
//   halves in the preheader is the only form that keeps `this` in esi, and it
//   is exactly the 91.1% file below. The two remaining diffs (halves before
//   the entry test, and pos.x loaded into ecx instead of edx) both follow from
//   that register choice, so this is a local optimum for this compiler.
// - The packet for this address misprints one operand: 0x407c8b is
//   `mov dword ptr [esp + 0x20], eax`, not [esp + 0x24]. The >= 5 branch of
//   the saved source is byte-exact, so trust check.py, not the packet.
//
// `field_c = g_game->ticks + FUN_004b6c30(900) + 30` in one expression folds
// to `lea eax, [eax+edx+0x1e]`; the delay has to be computed first.
// The final MakeFixed ternaries give the `lea eax, [tmp]; mov ecx, [eax]`
// selection. The unit FUN_004071f0 returns is used without a null check.
#include <vector>

#pragma pack(push, 1)
struct Game_00407ae0 {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    char unknown_1422b[0x38a47 - 0x1422b];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game_00407ae0* g_game;

struct FixedParts_00407ae0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_00407ae0 {
    int value;
    FixedParts_00407ae0 parts;
};

static inline Fixed_00407ae0 MakeFixed(int i)
{
    Fixed_00407ae0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

struct Vec3_00407410 {
    int x;
    int y;
    int z;

    Vec3_00407410() {}
    Vec3_00407410(int a, int b, int c) : x(a), y(b), z(c) {}
};

union Coord_00407ae0 {
    int value;
    struct {
        unsigned short frac;
        short whole;
    } s;
};

struct Pos_00407ae0 {
    Coord_00407ae0 x;
    Coord_00407ae0 y;
    Coord_00407ae0 z;
};

#pragma pack(push, 1)
struct Unit_00407ae0 {
    char unknown_0[0x6a];
    Vec3_00407410 pos;                 // +0x6a
};
#pragma pack(pop)

struct Group_00407ae0 {
    void* player;                      // +0x0
    int id;                            // +0x4
    char unknown_8[0x10 - 0x8];
    std::vector<Unit_00407ae0*> units; // +0x10
};

struct Class_00408cb0 {                // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

class Class_004071f0 {
public:
    Unit_00407ae0* FUN_004071f0(Vec3_00407410 pos);
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class Class_00407350 {
public:
    Class_00408cb0* owner;             // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    Class_00407350(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0
    virtual ~Class_00407350() {}                    // slot 1

    int FUN_00407410(Vec3_00407410* out);
};

// Vtable 0x4fc998, constructor 0x407a90, ??_G 0x407ac0.
class Class_00407a90 : public Class_00407350 {
public:
    Class_00407a90(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x407ae0
};

int __stdcall FUN_004b6c30(int range);
void __stdcall FUN_0040ba80(int index, Pos_00407ae0* out);
void __stdcall FUN_00480460(void* player, int id, int mode, int remove, int* target,
                            Vec3_00407410* pos, int flags, int extra);

// FUNCTION: 0x407ae0
void Class_00407a90::FUN_00407380()
{
    Vec3_00407410 dest;
    int delay = FUN_004b6c30(900) + 30;
    field_c = g_game->ticks + delay;
    if ((int)((Group_00407ae0*)field_8)->units.size() < 5) {
        Pos_00407ae0 pos;
        FUN_0040ba80(field_10, &pos);
        if ((pos.x.s.whole | pos.z.s.whole) == 0) {
            FUN_00407410(&dest);
            Unit_00407ae0* target = ((Class_004071f0*)owner)->FUN_004071f0(dest);
            FUN_00480460(((Group_00407ae0*)field_8)->player, ((Group_00407ae0*)field_8)->id,
                         9, 1, 0, &target->pos, 0, 0);
        } else {
            int n = FUN_004b6c30(2) + 2;
            int w = g_game->baseX / 8, h = g_game->baseY / 8;
            int hw = w / 2;
            int hh = h / 2;
            for (int i = 0; i < n; i++) {
                int dx = FUN_004b6c30(w) - hw;
                dest.x = pos.x.value + (dx << 16);
                dest.y = pos.y.value;
                dest.z = pos.z.value + ((FUN_004b6c30(h) - hh) << 16);
                if (i == 0)
                    FUN_00480460(((Group_00407ae0*)field_8)->player, ((Group_00407ae0*)field_8)->id,
                                 2, 0, 0, &dest, 0, 0);
                else
                    FUN_00480460(((Group_00407ae0*)field_8)->player, ((Group_00407ae0*)field_8)->id,
                                 9, 1, 0, &dest, 0, 0);
            }
        }
    } else {
        dest.y = 0;
        if (FUN_004b6c30(2)) {
            dest.x = FUN_004b6c30(g_game->baseX) << 16;
            dest.z = (FUN_004b6c30(2) ? MakeFixed(0) : MakeFixed(g_game->baseY - 1)).value;
        } else {
            dest.x = (FUN_004b6c30(2) ? MakeFixed(0) : MakeFixed(g_game->baseX - 1)).value;
            dest.z = FUN_004b6c30(g_game->baseY) << 16;
        }
        FUN_00480460(((Group_00407ae0*)field_8)->player, ((Group_00407ae0*)field_8)->id,
                     9, 0, 0, &dest, 0, 0);
    }
}
