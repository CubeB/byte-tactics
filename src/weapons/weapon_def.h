// WeaponDef: one weapon definition (Thaldren's WeaponDef, 0x115 bytes), the
// elements of the table at g_game+0x2cf3. The one declaration of the struct for
// every file that reads a weapon's range, damage, flags or projectile numbers;
// the types behind the pointers stay private to their own files.
#ifndef WEAPON_DEF_H
#define WEAPON_DEF_H

#pragma pack(push, 1)

struct Table_00499cd0;

// One word of weapon flags. `b` has a field for every bit, named by the TDF key
// the loader (0x42e440) sets it from; `value` is the raw word for the mask
// tests and the shifts that read bits 30 and 31.
union Flags_0049d580 {
    struct {
        unsigned int lineOfSight : 1;                 // bit 0
        unsigned int ballistic : 1;                   // bit 1
        unsigned int shellWeapon : 1;                 // bit 2
        unsigned int beamWeapon : 1;                  // bit 3
        unsigned int vLaunch : 1;                     // bit 4
        unsigned int meteor : 1;                      // bit 5
        unsigned int noRadar : 1;                     // bit 6
        unsigned int paralyzer : 1;                   // bit 7
        unsigned int dropped : 1;                     // bit 8
        unsigned int startSmoke : 1;                  // bit 9
        unsigned int endSmoke : 1;                    // bit 10
        unsigned int soundTrigger : 1;                // bit 11
        unsigned int guidance : 1;                    // bit 12
        unsigned int tracks : 1;                      // bit 13
        unsigned int unitsOnly : 1;                   // bit 14
        unsigned int groundBounce : 1;                // bit 15
        unsigned int waterWeapon : 1;                 // bit 16
        unsigned int toAirWeapon : 1;                 // bit 17
        unsigned int smokeTrail : 1;                  // bit 18
        unsigned int turret : 1;                      // bit 19
        unsigned int selfProp : 1;                    // bit 20
        unsigned int propeller : 1;                   // bit 21
        unsigned int noExplode : 1;                   // bit 22
        unsigned int burnBlow : 1;                    // bit 23
        unsigned int twoPhase : 1;                    // bit 24
        unsigned int cruise : 1;                      // bit 25
        unsigned int commandFire : 1;                 // bit 26
        unsigned int noAutoRange : 1;                 // bit 27
        unsigned int stockpile : 1;                   // bit 28
        unsigned int targetable : 1;                  // bit 29
        unsigned int interceptor : 1;                 // bit 30
        unsigned int bit31 : 1;                       // bit 31
    } b;
    unsigned int value;               // +0x111
};

// The weapon definition, 0x115 bytes (Thaldren's WeaponDef). One view for the
// whole module: the damage table and default damage of 0x499cd0, the launch
// angle and range of 0x49aa80 and 0x49abb0, the flags the firing code tests,
// the coverage radius of 0x49d120 and the loader's name index.
struct WeaponDef {
    char name[0x20];                  // +0x0, the TDF section name
    char description[0x40];           // +0x20
    void* fire;                       // +0x60, the fire routine (WeaponFireFn in weapons.cpp)
    Table_00499cd0* table;           // +0x64, the name-keyed damage overrides
    int speed;                        // +0x68, 49b720's maxSpeed
    int startSpeed;                   // +0x6c, the speed of the launch
    int acceleration;                 // +0x70, 49b720's accel
    void* model;                      // +0x74, the 3D model of the shot
    void* explosionSeq;               // +0x78, the ground explosion sequence
    void* splash;                     // +0x7c, 49b720's water explosion sequence
    char modelName[0x40];             // +0x80
    float energyPerShot;              // +0xc0
    float metalPerShot;               // +0xc4
    union {
        float pitch;                  // +0xc8
        int field_c8;                 // the launch-angle solvers read the float's bits
    };
    int shakeMagnitude;               // +0xcc, the screen shake of the blast
    int shakeDuration;                // +0xd0
    unsigned short damage;            // +0xd4, the default damage
    unsigned short areaOfEffect;      // +0xd6, the splash radius
    float edgeDamage;                 // +0xd8, the falloff at the edge
    int range;                        // +0xdc
    int radius;                       // +0xe0, the coverage radius
    unsigned short reloadTime;        // +0xe4, in ticks
    unsigned short lifetime;          // +0xe6, 49b720's nWeaponTimer
    unsigned short turnRate;          // +0xe8
    unsigned short burst;             // +0xea
    unsigned short burstRate;         // +0xec
    unsigned short spread;            // +0xee
    unsigned short duration;          // +0xf0, 49b720's f0
    unsigned short lifeRand;          // +0xf2
    unsigned short sound;             // +0xf4
    unsigned short soundHit;          // +0xf6, played where the shot lands
    unsigned short soundWater;        // +0xf8, played where it lands in water
    unsigned short smokeRate;         // +0xfa
    unsigned short flightTime;        // +0xfc, 49b720's fc
    unsigned short holdTime;          // +0xfe, the TDF holdtime in ticks (nHoldTime), the camera hold
    char unknown_100[0x104 - 0x100];
    short accuracy;                   // +0x104
    unsigned short tolerance;         // +0x106, how far off the aim may be and still fire
    unsigned short pitchTolerance;    // +0x108
    unsigned char index;              // +0x10a, the weapon's number in the definitions
    unsigned char fireStarter;        // +0x10b
    unsigned char shotKind;           // +0x10c, how the shot is drawn (0 to 7)
    unsigned char colour;             // +0x10d, the palette index of the line
    unsigned char colour2;            // +0x10e, the second line's palette index
    char unknown_10f[0x111 - 0x10f];
    Flags_0049d580 flags;             // +0x111
};

#pragma pack(pop)

#endif
