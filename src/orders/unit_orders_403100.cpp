// Decompiled by Opus. Names are provisional.

struct Weapon_00403100 {
    char unknown_0[0xf];
    unsigned char flags;                 // +0xf
    char unknown_10[0xc];
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x10];
    Weapon_00403100 weapons[3];          // +0x10
    char unknown_64[0x110 - 0x64];
    unsigned int unknown_bits : 20;      // +0x110
    unsigned int mode : 2;               // +0x110, bits 20-21
    unsigned int unknown_bits2 : 10;
};

struct Order {
    char unknown_0[0x36];
    int mode;                            // +0x36
};
#pragma pack(pop)

void __stdcall ClearWeaponTarget(Unit* unit, int weapon);

// A char loop counter gives the separate countdown register (ebx = 3).
// FUNCTION: 0x403100
int __stdcall StandingFireOrder(Unit* unit, Order* order, int unused)
{
    unit->mode = order->mode;
    if (order->mode == 0 || order->mode == 1) {
        for (char i = 0; i < 3; i++) {
            if (unit->weapons[i].flags & 0x10) {
                ClearWeaponTarget(unit, i);
            }
        }
    }
    return 5;
}
