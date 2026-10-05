// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by space-bunny-free, finished by mimo-v2.6-pro. Names are provisional.
// MATCH, 742 of 742 bytes. (Was 84.9% with a frame-slot permutation that no
// amount of declaration reordering could move.) Four source-shape facts
// produce the original exactly:
//   1. Class_00438760 fire/move are NAMED locals initialised from an inline
//      wrapper that returns the class by value
//      (`static inline Class_00438760 Order(const char*) { return
//      Class_00438760(name); }`). Such a byte's address is taken as a hidden
//      return pointer (an ARGUMENT), so fire takes the dead entry-parameter
//      slot at 0x48 exactly where the original puts it, and move pools at
//      0x12 beside the FUN_0043f0e0 return temporary at 0x13. A named ctor
//      object (`Class_00438760 fire("...")`) is a THIS pointer instead and
//      always gets its own dword slot; a condition temporary pools but is
//      then read through the constructor's returned this (`mov dl, [eax]`),
//      which is not what the original does.
//   2. The averages are one int avg[3] aggregate (x/y/z: map coordinates are
//      x/z and y is height, never written). avg[1] is the dead dword at 0x30
//      between avg[0] (0x2c) and avg[2] (0x34). Frame slots are handed out by
//      size class (1-byte pool, 4-byte scalars, the 8-byte (int64) cast temp,
//      then the 12-byte arrays), so the 12-byte avg sorts after range/p and
//      before here[3]; within a size class the order is reads desc, then
//      last-use asc. The dead dword at 0x24 plus the dz sign-extension spill
//      at 0x28 are the low/high halves of the 8-byte (int64) cast temp.
//   3. The deltas are written in two steps (`int dx = u->x; dx -= avg[0];`):
//      the one-step `int dx = u->x - avg[0];` lets MSVC CSE `u->x - avg[0]`
//      across the branch, which reassociates pos[0] + u->x - avg[0] into
//      pos[0] - avg[0] + u->x in the here block (and swaps that block's
//      register pairs).
//   4. The delta statements are written z first, x second (`int dz = u->z;
//      dz -= avg[2]; int dx = u->x; dx -= avg[0];`), which is what chains
//      avg[0]'s dying register into dx and avg[2]'s into dz in the distance
//      block (edi/ebx instead of crossed ebx/edi). The loads still come out
//      in the original's x, avg[0], z, avg[2] order either way.
// <stdio.h> + <stdlib.h> is load-bearing compiler state (tools/headers.py
// sweeps all 768 sets at 98.3% without fact 4 and MATCH with it).
//
// The function: for every unit of the local player with flag 0x10 that is not
// g_game->units[g_game->field_2cba], if its order kind is not Standing_FireOrder
// (or the unit type allows fire) and not Standing_MoveOrder (or allows move),
// issue FUN_0043afc0 at pos offset by the unit's delta from the player's
// average unit position, when the unit is within count*3000 of that average,
// else at pos unchanged.
//
// Suspected original bug: none spotted; field_2cba indexes g_game->units with
// no bounds check but every reference agrees and 0 means "no exception unit".
#include <stdio.h>
#include <stdlib.h>

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

struct Unit {
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
    Unit* first;                        // +0x67
    Unit* last;                         // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game {
    char unknown_0[0x1b63];
    Player_0048cf30 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char field_2a42;           // +0x2a42
    char unknown_2a43[0x2caa - 0x2a43];
    void* field_2caa;                   // +0x2caa
    char unknown_2cae[0x2cba - 0x2cae];
    unsigned short field_2cba;          // +0x2cba
    char unknown_2cbc[0x14357 - 0x2cbc];
    Unit* field_14357;                  // +0x14357
};

extern Game* g_game;

int __stdcall FUN_0043e470(unsigned char type);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char mode, Unit* unit,
                                       Unit* target, void* param_5);
void __stdcall FUN_0043afc0(Class_00438830 kind, int flag, Unit* unit,
                            Unit* target, int* pos, int param_5, int param_6);

static inline Class_00438760 Order(const char* name) { return Class_00438760(name); }

// FUNCTION: 0x48cf30
void __stdcall IssueOrderToSelection(UnitType_0048cf30* entry, unsigned char mode,
                            Class_00438830 kind, int* pos, int param_5, int param_6)
{
    int flag_a = (entry->flags_a >> 2) & 1;
    Unit* except = 0;
    int flag_b;
    if (mode)
        flag_b = FUN_0043e470(mode);
    else
        flag_b = (kind.FUN_00438830()->flags >> 9) & 1;
    if (flag_b) {
        if (!g_game->field_2cba)
            except = 0;
        else
            except = (Unit*)((char*)g_game->field_14357 + 280 * g_game->field_2cba);
    }
    Player_0048cf30* p = &g_game->players[g_game->field_2a42];
    int count = 0;
    int sum_x = 0;
    int sum_z = 0;
    Unit* u;
    for (u = p->first; u <= p->last; u++) {
        if ((u->flags & 0x10) && u != except) {
            count++;
            sum_x += (short)(u->x >> 16);
            sum_z += (short)(u->z >> 16);
        }
    }
    if (!count)
        return;
    int avg[3];
    avg[0] = (sum_x / count) * 65536.0;
    avg[2] = (sum_z / count) * 65536.0;
    int range = count * 3000;
    for (u = p->first; u <= p->last; u++) {
        if (!(u->flags & 0x10) || u == except)
            continue;
        if (mode)
            kind.index = FUN_0043f0e0(mode, u, except, &g_game->field_2caa).index;
        if (!kind.index)
            continue;
        Class_00438760 fire = Order("Standing_FireOrder");
        if (kind.index != fire.index || (u->def->flags & 2)) {
            Class_00438760 move = Order("Standing_MoveOrder");
            if (kind.index != move.index || (u->def->flags & 1)) {
                if (pos && (kind.FUN_00438830()->flags & 2)) {
                    int dz = u->z;
                    dz -= avg[2];
                    int dx = u->x;
                    dx -= avg[0];
                    if ((int)(((__int64)dx * dx) >> 32)
                        + (int)(((__int64)dz * dz) >> 32) <= range) {
                        int here[3];
                        here[0] = pos[0] + u->x - avg[0];
                        here[1] = pos[1];
                        here[2] = pos[2] + u->z - avg[2];
                        FUN_0043afc0(kind, flag_a, u, except, here, param_5, param_6);
                        continue;
                    }
                }
                FUN_0043afc0(kind, flag_a, u, except, pos, param_5, param_6);
            }
        }
    }
}
