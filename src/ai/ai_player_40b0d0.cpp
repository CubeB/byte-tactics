// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <vector>
#pragma pack(push, 1)
struct Vec3_40b0d0 {
    int x;
    int y;
    int z;
};

struct Unit {
    char unknown_0[0x6a];
    Vec3_40b0d0 pos;
    char unknown_76[0x110 - 0x76];
    unsigned int flags;
};

struct Player_40b0d0 {
    char unknown_0[5];
    std::vector<Unit*> units;
};
#pragma pack(pop)

extern Player_40b0d0* g_playerAI[];

// FUNCTION: 0x40b0d0
bool __stdcall FUN_0040b0d0(int player, Vec3_40b0d0* p, int range)
{
    int r2 = range * range;
    Player_40b0d0* t = g_playerAI[player];
    std::vector<Unit*>& units = t->units;
    std::vector<Unit*>::iterator it = units.begin();
    if (it != units.end()) {
        do {
            Unit* u = *it;
            int dz = p->z - u->pos.z;
            int dx = p->x - u->pos.x;
            if ((int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32) <= r2
                && (u->flags & 0x10000000)
                && !(u->flags & 0x4000))
                return true;
        } while (++it != units.end());
    }
    return false;
}
