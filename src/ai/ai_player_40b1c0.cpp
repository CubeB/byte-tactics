// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Sums the player's signed byte table (at +0x91) over the units within
// `range` of `pos`; the distance is the 64-bit high product (>> 32).

#include <vector>
#pragma pack(push, 1)
struct Vec_0040b1c0 {
    int x;
    int y;
    int z;
};

struct Unit {
    char unknown_0[0x6a];
    int x;                             // +0x6a
    char unknown_6e[0x72 - 0x6e];
    int z;                             // +0x72
    char unknown_76[0xa6 - 0x76];
    unsigned short id;                 // +0xa6
};

class Class_0040b1c0 {
public:
    char unknown_0[5];
    std::vector<Unit*> units; // +0x5
    char unknown_15[0x91 - 0x15];
    signed char* table;                // +0x91
};
#pragma pack(pop)

extern Class_0040b1c0* g_playerAI[];

// FUNCTION: 0x40b1c0
int __stdcall FUN_0040b1c0(int player, Vec_0040b1c0* pos, int range)
{
    int total = 0;
    Class_0040b1c0* p = g_playerAI[player];
    int r2 = range * range;
    std::vector<Unit*>& units = p->units;
    for (std::vector<Unit*>::iterator it = units.begin(); it != units.end(); it++) {
        Unit* u = *it;
        int dz = pos->z - u->z;
        int dx = pos->x - u->x;
        int d = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
        if (d <= r2)
            total += p->table[u->id];
    }
    return total;
}
