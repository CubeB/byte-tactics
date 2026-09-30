// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol. Names are provisional.
// Slot 0 of Class_00407a90 (vtable 0x4fc998), derived from Class_00407350
// (the family is listed in 0x407350.cpp, whose declarations this copies).
// Sets field_c to 30..929 ticks from now. A group of fewer than 5 units
// either moves (mode 9) to the unit nearest its average position, or, when
// FUN_0040ba80 gives a rally point for field_10, is sent to 2..3 random points
// around it (mode 2 first, then mode 9). A bigger group is sent to a random
// point on the map edge.
//
// Partial (91.1%): everything outside the scatter loop matches. Differences
// left in the loop preheader and body:
// GPT-6.1-sol rechecked the saved source and tried an outer guard with for/do-while
// loops and direct x/z expressions; none improved on 91.1% (6 checks this pass).
// - The original rotates the loop, so w / 2 and h / 2 (ebp and a stack slot)
//   are computed after the guard `cmp ecx, ebx; jle`, while this version
//   computes them before it (`test ecx, ecx; jle`). Moving them into the loop
//   body makes the allocator give `this` ebp and spill both halves (58-59%);
//   an explicit `if (n > 0) do/while` is the same.
// - In the body the original adds the shifted random delta into the register
//   holding pos.x/pos.z (`mov edx, [pos.x]; ...; add edx, eax`) and loads pos.y
//   only after dest.x is finished; every variant here adds into eax and loads
//   pos.y early.
// - Tried without effect: every header set (tools/headers.py), halves inside
//   the loop, an explicit guard with do/while.
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
            int w = g_game->baseX / 8;
            int h = g_game->baseY / 8;
            int hw = w / 2, hh = h / 2;
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
