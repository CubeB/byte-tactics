// Decompiled by space-bunny-free, finished by Space Bunny Free. Names are provisional.
// Partial (63.4%). Every control-flow edge, call target, argument order and
// field offset now agrees with the original, and the loops, the two averages
// and the __int64 distance test all have the original's shape. What is left
// is one cause: MSVC 5's frame layout puts our locals in a different order,
// so every [esp+N] that names a local differs.
//   original frame: 0x12/0x13 the two 1-byte temps, 0x14 except, 0x18 flag_a,
//   0x1c range, 0x20 p, 0x28 the sign word of dz, 0x2c avg_x, 0x34 avg_z,
//   0x38 here[3].
//   ours: 0x10 except, 0x18 range, 0x1c/0x20 use_pos/use_flag, 0x24 the
//   move-order temp, 0x28 avg_x, 0x2c avg_z, 0x30 flag_a, 0x34 p, 0x38 here.
// Both frames are 0x34 bytes and both put `here` at 0x38, so the frame size
// and the block structure agree; only the order differs. Reordering the
// declarations changes nothing at all (MSVC 5 assigns the offsets in the
// back end, four very different declaration orders all compile to the same
// 733 bytes), so the order has to be steered by the shape of the code.
// The other, smaller difference follows from it: because use_pos/use_flag are
// live across the FUN_00438830 call, the compiler spills them and the tail
// merge of the two FUN_0043afc0 calls is lost. Writing the call out in both
// arms instead (with `continue` in the inner one) restores the tail merge but
// makes MSVC materialise both 64x64 products in memory, which costs more than
// the spill (54.3%).
// Known-good detail: the fifth argument of FUN_0043f0e0 is the ADDRESS of
// g_game->field_2caa (the original emits `add ecx, 0x2caa`), so the call
// passes `&g_game->field_2caa`; passing the field's value instead costs a
// point. The field_2cba test is `if (!x) except = 0; else ...` so that the
// zero store is the fall-through of the `jne` (+1.4 points over the other
// polarity).
// One lead left open: the original constructs "Standing_FireOrder" straight
// into the dead argument-0 slot (lea ecx, [esp+0x48] with the string pushed,
// then mov al, [esp+0x48]), and reads `entry` nowhere else in the loop, so the
// first parameter is probably dead after the prologue. Neither spelling
// reproduces it: a placement new into `entry` keeps `entry` in edi, and a
// local Class_00438760 temporary gets its own frame slot instead (63.3% either
// way). The 1-byte buffer in the original stays at esp+0x13 while ours is
// pushed into the same dead argument slot, which may be the same root cause.
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
        if (!g_game->field_2cba)
            except = 0;
        else
            except = (Unit_0048cf30*)((char*)g_game->field_14357 + 280 * g_game->field_2cba);
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
            kind.index = *FUN_0043f0e0(&buf, mode, u, except, &g_game->field_2caa);
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
