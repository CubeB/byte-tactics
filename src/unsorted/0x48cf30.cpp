// Decompiled by space-bunny-free. Names are provisional.
// Partial (62%, 687 of 742 bytes). The shape is right: both loops, the two
// averages, the tail-merged FUN_0043afc0 call and every field offset agree.
// Still different, and all of it is one cause, the stack slot assignment:
//   * the frame lays its locals out in another order (except at esp+0x14 and
//     the `p` local at esp+0x34 agree, flag_a, range, p, avg_x, avg_z and the
//     two byte locals do not), so every [esp+N] load differs;
//   * consequently the fild temp is not here[1]'s slot, the byte locals are
//     not at the bottom of the frame, and `entry` gets a register (edi)
//     where the original reloads it from its argument slot for the
//     Class_00438760 constructor calls;
//   * the "Standing_MoveOrder" block is laid out before the first def test
//     instead of after it, so the second u->def load is CSE'd instead of
//     reloaded (the ctor call in between should have broken it);
//   * the second 64x64 multiply (_allmul) takes its high word from the
//     never-written slot at esp-0x1c. MSVC 5 emits `cdq` and pushes edx
//     there, so the original must have had that high word in memory, which
//     no spelling of `(__int64)dz * dz` reproduces.
// Tried and kept: the call written in both arms (it tail-merges into the
// original's single call plus jmp), the ! polarity of the two def tests
// (the body runs when kind == X || (def->flags & bit), so the `je` to the
// loop latch is right), `(char*)g_game->field_14357 + 280 * field_2cba`
// (MSVC then folds it to 8*(35*team) in one lea) and the Game offsets
// 0x1b63 / 0x2a42 / 0x2caa / 0x2cba / 0x14357.
#include <new.h>

#pragma pack(push, 1)

// The unit type table entry, 0x19 bytes each.
struct UnitType_0048cf30 {
    unsigned char index;                // +0x00
    char unknown_1[0x8 - 0x1];
    unsigned int flags_a;               // +0x08
    char unknown_c[0x11 - 0xc];
    unsigned int flags;                 // +0x11
};

// A unit type index, passed and returned by value.
class Class_00438830 {
public:
    unsigned char index;
    UnitType_0048cf30* FUN_00438830();
};

// A unit type index built from a name.
class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct UnitDef_0048cf30 {
    char unknown_0[0x245];
    unsigned char flags;                // +0x245
};

struct Unit_0048cf30 {
    char unknown_0[0x6a];
    int x;                              // +0x6a
    char unknown_6e[0x72 - 0x6e];
    int z;                              // +0x72
    char unknown_76[0x92 - 0x76];
    UnitDef_0048cf30* def;              // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned char flags;                // +0x110
    char unknown_111[0x118 - 0x111];
};

struct Player_0048cf30 {
    char unknown_0[0x67];
    Unit_0048cf30* first;               // +0x67
    Unit_0048cf30* last;                // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048cf30 {
    char unknown_0[0x1b63];
    Player_0048cf30 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char field_2a42;           // +0x2a42
    char unknown_2a43[0x2caa - 0x2a43];
    void* field_2caa;                   // +0x2caa
    char unknown_2cae[0x2cba - 0x2cae];
    unsigned short field_2cba;          // +0x2cba
    char unknown_2cbc[0x14357 - 0x2cbc];
    Unit_0048cf30* field_14357;         // +0x14357
};

extern Game_0048cf30* g_game;

int __stdcall FUN_0043e470(unsigned char type);
unsigned char* __stdcall FUN_0043f0e0(unsigned char* buf, unsigned char mode,
                                       Unit_0048cf30* unit, Unit_0048cf30* target,
                                       void* param_5);
void __stdcall FUN_0043afc0(Class_00438830 kind, int flag, Unit_0048cf30* unit,
                            Unit_0048cf30* target, int* pos, int param_5, int param_6);

// FUNCTION: 0x48cf30
void __stdcall FUN_0048cf30(UnitType_0048cf30* entry, unsigned char mode,
                            Class_00438830 kind, int* pos, int param_5, int param_6)
{
    int flag_a = (entry->flags_a >> 2) & 1;
    Unit_0048cf30* except = 0;
    int flag_b;
    if (mode)
        flag_b = FUN_0043e470(mode);
    else
        flag_b = (kind.FUN_00438830()->flags >> 9) & 1;
    if (flag_b) {
        if (g_game->field_2cba)
            except = (Unit_0048cf30*)((char*)g_game->field_14357 + 280 * g_game->field_2cba);
        else
            except = 0;
    }
    Player_0048cf30* p = &g_game->players[g_game->field_2a42];
    int count = 0;
    int sum_x = 0;
    int sum_z = 0;
    Unit_0048cf30* u;
    for (u = p->first; u <= p->last; u++) {
        if ((u->flags & 0x10) && u != except) {
            count++;
            sum_x += (short)(u->x >> 16);
            sum_z += (short)(u->z >> 16);
        }
    }
    if (!count)
        return;
    int avg_x = (sum_x / count) * 65536.0;
    int avg_z = (sum_z / count) * 65536.0;
    int range = count * 3000;
    for (u = p->first; u <= p->last; u++) {
        if (!(u->flags & 0x10) || u == except)
            continue;
        unsigned char buf;
        if (mode)
            kind.index = *FUN_0043f0e0(&buf, mode, u, except, g_game->field_2caa);
        if (!kind.index)
            continue;
        new ((Class_00438760*)entry) Class_00438760("Standing_FireOrder");
        if (kind.index != entry->index || (u->def->flags & 2)) {
            Class_00438760 move("Standing_MoveOrder");
            if (kind.index != move.index || (u->def->flags & 1)) {
                int* use_pos = pos;
                int use_flag = range;
                if (pos && (kind.FUN_00438830()->flags & 2)) {
                    int dx = u->x - avg_x;
                    int dz = u->z - avg_z;
                    if ((int)(((__int64)dx * dx) >> 32)
                        + (int)(((__int64)dz * dz) >> 32) <= range) {
                        int here[3];
                        here[0] = pos[0] + u->x - avg_x;
                        here[1] = pos[1];
                        here[2] = pos[2] + u->z - avg_z;
                        use_pos = here;
                        use_flag = flag_a;
                    }
                }
                FUN_0043afc0(kind, use_flag, u, except, use_pos, param_5, param_6);
            }
        }
    }
}
