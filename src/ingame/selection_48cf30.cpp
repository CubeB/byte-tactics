// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by space-bunny-free, finished by mimo-v2.6-pro. Names are provisional.
// The function: for every unit of the local player with flag 0x10 that is not
// g_game->units[g_game->field_2cba], if its order kind is not Standing_FireOrder
// (or the unit type allows fire) and not Standing_MoveOrder (or allows move),
// issue IssueOrCancelOrder at pos offset by the unit's delta from the player's
// average unit position, when the unit is within count*3000 of that average,
// else at pos unchanged.
// <stdio.h> and <stdlib.h> must both stay included: they set the compiler state.
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
class OrderType {
public:
    unsigned char index;
    UnitType_0048cf30* GetTableEntry();
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
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2caa - 0x2a43];
    void* field_2caa;                   // +0x2caa
    char unknown_2cae[0x2cba - 0x2cae];
    unsigned short field_2cba;          // +0x2cba
    char unknown_2cbc[0x14357 - 0x2cbc];
    Unit* field_14357;                  // +0x14357
};

extern Game* g_game;

int __stdcall OrderModeTakesTarget(unsigned char type);
Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit* unit,
                                       Unit* target, void* param_5);
void __stdcall IssueOrCancelOrder(OrderType kind, int flag, Unit* unit,
                            Unit* target, int* pos, int param_5, int param_6);

static inline Class_00438760 Order(const char* name) { return Class_00438760(name); }

// Kept out of the merged selection.cpp: it only matches at this file's symbol count.
// FUNCTION: 0x48cf30
void __stdcall IssueOrderToSelection(UnitType_0048cf30* entry, unsigned char mode,
                            OrderType kind, int* pos, int param_5, int param_6)
{
    int flag_a = (entry->flags_a >> 2) & 1;
    Unit* except = 0;
    int flag_b;
    if (mode)
        flag_b = OrderModeTakesTarget(mode);
    else
        flag_b = (kind.GetTableEntry()->flags >> 9) & 1;
    if (flag_b) {
        if (!g_game->field_2cba)
            except = 0;
        else
            except = (Unit*)((char*)g_game->field_14357 + 280 * g_game->field_2cba);
    }
    Player_0048cf30* p = &g_game->players[g_game->localPlayer];
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
    // One avg[3] aggregate (y unused): sets the frame slot layout.
    int avg[3];
    avg[0] = (sum_x / count) * 65536.0;
    avg[2] = (sum_z / count) * 65536.0;
    int range = count * 3000;
    for (u = p->first; u <= p->last; u++) {
        if (!(u->flags & 0x10) || u == except)
            continue;
        if (mode)
            kind.index = GetOrderType(mode, u, except, &g_game->field_2caa).index;
        if (!kind.index)
            continue;
        // fire and move are named locals from the Order() wrapper, not constructed
        // directly: that decides their frame slots.
        Class_00438760 fire = Order("Standing_FireOrder");
        if (kind.index != fire.index || (u->def->flags & 2)) {
            Class_00438760 move = Order("Standing_MoveOrder");
            if (kind.index != move.index || (u->def->flags & 1)) {
                if (pos && (kind.GetTableEntry()->flags & 2)) {
                    // Two-step deltas, z first then x: one-step avoids a CSE across the branch.
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
                        IssueOrCancelOrder(kind, flag_a, u, except, here, param_5, param_6);
                        continue;
                    }
                }
                IssueOrCancelOrder(kind, flag_a, u, except, pos, param_5, param_6);
            }
        }
    }
}
