// Decompiled by Claude Opus 5.5. Names are provisional.

class Class_00439e80 {
public:
    void FUN_00439e80(int ticks);
};

class UnitResources {
public:
    char unknown_0[0x28];
    int FUN_004011c0(float energy, float metal);
};

#pragma pack(push, 1)
struct WeaponType {
    char unknown_0[0xc0];
    float energyCost;                  // +0xc0 (TDF energypershot)
    float metalCost;                   // +0xc4 (TDF metalpershot)
    char unknown_c8[0xe4 - 0xc8];
    unsigned short buildTime;          // +0xe4
};

struct Weapon {
    WeaponType* type;                  // +0x0
    char unknown_4[0xe - 0x4];
    unsigned char stockpile;           // +0xe
    char unknown_f[0x1c - 0xf];
};

struct Unit {
    char unknown_0[0x10];
    Weapon weapons[3];                 // +0x10
    char unknown_64[0xbc - 0x64];
    UnitResources resources;           // +0xbc
};

struct Order {
    char unknown_0[5];
    unsigned char state;               // +0x5
    char unknown_6[0x36 - 6];
    int weapon;                        // +0x36
    int count;                         // +0x3a
    int progress;                      // +0x3e
};
#pragma pack(pop)

void __stdcall FUN_0041c150(Unit* unit);

// Order handler "Nanolathing" of a building that stockpiles weapons (the
// "BuildingBuild" entry of the order table at 0x4fc490): builds `count`
// rounds for weapon `weapon`, 5 ticks of build time per step, paying the
// energy and metal share of each step (energy first, as FUN_004011c0 takes
// them), up to 200 stockpiled rounds.
// FUNCTION: 0x402b70
int __stdcall BuildWeaponOrder(Unit* unit, Order* order, int unused)
{
    WeaponType* t = unit->weapons[order->weapon].type;
    switch (order->state) {
    case 0:
        if (order->count <= 0)
            return 5;
        if (unit->weapons[order->weapon].stockpile >= 200) {
            ((Class_00439e80*)order)->FUN_00439e80(300);
            return 2;
        }
        order->progress = 0;
        return 1;
    case 1: {
        int prev = order->progress;
        int next = prev + 5 < t->buildTime ? prev + 5 : t->buildTime;
        int total = t->buildTime;
        // Float locals declared next, total, prev (reverse of first use): fixes the
        // fild order; total stays after the ternary.
        float fnext = next;
        float ftotal = total;
        float fprev = prev;
        int metalCharge = (int)(fnext * t->metalCost / ftotal) - (int)(fprev * t->metalCost / ftotal);
        int energyCharge = (int)(fnext * t->energyCost / ftotal) - (int)(fprev * t->energyCost / ftotal);
        if (unit->resources.FUN_004011c0(energyCharge, metalCharge)) {
            order->progress = next;
            if (next >= t->buildTime)
                return 1;
            ((Class_00439e80*)order)->FUN_00439e80(5);
            return 2;
        }
        ((Class_00439e80*)order)->FUN_00439e80(10);
        return 2;
    }
    case 2:
        unit->weapons[order->weapon].stockpile++;
        order->count--;
        FUN_0041c150(unit);
        return 0;
    default:
        return 7;
    }
}
