// WeaponDef: one weapon definition (Thaldren's WeaponDef, 0x115 bytes), the
// elements of the table at g_game+0x2cf3. The one declaration of the struct for
// every file that reads a weapon's range, damage, flags or projectile numbers;
// the types behind the pointers stay private to their own files. The files that
// match only at their old view's symbol count (weapons_49b090.cpp,
// weapons_49b720.cpp) keep their own view.
#ifndef WEAPON_DEF_H
#define WEAPON_DEF_H

#pragma pack(push, 1)

struct Table_00499cd0;

// One word of weapon flags. Every file that reads it names the bits it tests;
// the merged view keeps the raw word and each caller's bit names.
union Flags_0049d580 {
    struct {
        unsigned int f0 : 1;          // bit 0
        unsigned int f1 : 1;          // bit 1
        unsigned int f2_29 : 28;
        unsigned int f30 : 1;         // bit 30, detonatesWeapons
        unsigned int f31 : 1;
    } b;
    struct {                          // 49d270's bit view
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int b2 : 2;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int b6 : 2;
        unsigned int b8 : 1;
        unsigned int b9 : 11;
        unsigned int b20 : 1;
        unsigned int b21 : 11;
    };
    struct {                          // 49aa80's and 49abb0's bit view
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;
        unsigned int bit17 : 1;
        unsigned int bit18_31 : 14;
    };
    struct {                          // 49d120's bit view
        unsigned int unused29 : 29;
        unsigned int hitscan : 1;
        unsigned int unused30 : 2;
    };
    struct {                          // 49db70's bit view
        unsigned int unused29b : 30;
        unsigned int special : 1;
        unsigned int unused31 : 1;
    };
    unsigned int value;               // +0x111
    unsigned char flags8;             // the low byte, read as a byte in 0x40b7b0
};

// The weapon definition, 0x115 bytes (Thaldren's WeaponDef). One view for the
// whole module: the damage table and default damage of 0x499cd0, the launch
// angle and range of 0x49aa80 and 0x49abb0, the flags the firing code tests,
// the coverage radius of 0x49d120 and the loader's name index.
struct WeaponDef {
    char unknown_0[0x64];
    Table_00499cd0* table;            // +0x64, the name-keyed damage overrides
    int speed;                        // +0x68, 49b720's maxSpeed
    char unknown_6c[0x70 - 0x6c];
    int acceleration;                 // +0x70, 49b720's accel
    char unknown_74[0x7c - 0x74];
    void* splash;                     // +0x7c, 49b720's water explosion sequence
    char unknown_80[0xc8 - 0x80];
    union {
        float pitch;                  // +0xc8
        int field_c8;                 // the launch-angle solvers read the float's bits
    };
    char unknown_cc[0xd4 - 0xcc];
    unsigned short damage;            // +0xd4, the default damage
    unsigned short areaOfEffect;      // +0xd6, the splash radius
    float edgeDamage;                 // +0xd8, the falloff at the edge
    int range;                        // +0xdc
    int radius;                       // +0xe0, the coverage radius
    unsigned short reloadTime;        // +0xe4, in ticks
    unsigned short lifetime;          // +0xe6, 49b720's nWeaponTimer
    char unknown_e8[0xec - 0xe8];
    unsigned short burstRate;         // +0xec
    unsigned short spread;            // +0xee
    unsigned short duration;          // +0xf0, 49b720's f0
    unsigned short lifeRand;          // +0xf2
    unsigned short sound;             // +0xf4
    char unknown_f6[0xfa - 0xf6];
    unsigned short smokeRate;         // +0xfa
    unsigned short flightTime;        // +0xfc, 49b720's fc
    unsigned short deathSound;        // +0xfe, 49b090's sound
    char unknown_100[0x104 - 0x100];
    short f_104;                      // +0x104
    char unknown_106[0x10a - 0x106];
    unsigned char index;              // +0x10a, the weapon's number in the definitions
    char unknown_10b[0x111 - 0x10b];
    Flags_0049d580 flags;             // +0x111
};

#pragma pack(pop)

#endif
