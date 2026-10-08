// UnitDef: one unit type (Thaldren's UnitDef, 0x249 bytes), the elements of
// the table at g_game+0x1439b, holding a unit's FBI data: names, costs, the
// movement and sensor numbers, the weapons and the category masks. The one
// declaration of the class for the files that call it; units/unit_types.cpp,
// which defines the methods and loads the FBI files, keeps its own view,
// since its loader spellings (workertime, maxdamage, cruisealt, waterline,
// bmcode), its by-value Vec3 extents and its bit-by-bit operator= over the
// flag words do not fit this one. The types behind the pointers stay private
// to their own files.
#ifndef UNIT_DEF_H
#define UNIT_DEF_H

#pragma pack(push, 1)

class UnitDef {
public:
    char name[0x20];                   // +0x000
    char unitname[0x20];               // +0x020
    char description[0x40];            // +0x040
    char objectname[0x20];             // +0x080
    char side[0x1e];                   // +0x0a0
    char f0be[0x40];                   // +0x0be, the FBI's ai_weight
    char f0fe[0x40];                   // +0x0fe, the FBI's ai_limit
    int f13e;                          // +0x13e
    int f142;                          // +0x142
    int f146;                          // +0x146
    union {
        int f14a;                      // +0x14a
        struct {
            short footprintx;          // +0x14a
            short footprintz;          // +0x14c
        };
    };
    union {
        int f14e;                      // +0x14e
        char* yardmap;                 // +0x14e
    };
    int count;                         // +0x152
    // The unit type ids it can build; vtol_orders_414380.cpp tests it as
    // canBuild, but every indexing read is unsigned short.
    unsigned short* ids;               // +0x156
    int f15a;                          // +0x15a
    // The model bounding box (nModelMinX..nModelSizeZ in Thaldren's UnitDef);
    // each file that reads it as a Vec3 casts at the use.
    int modelMinX;                     // +0x15e
    int modelMinY;                     // +0x162
    int modelMinZ;                     // +0x166
    int modelMaxX;                     // +0x16a
    union {
        int modelMaxY;                 // +0x16e
        struct {
            short unknown_16e;
            short field_170;           // +0x170
        };
    };
    int modelMaxZ;                     // +0x172
    int modelSizeX;                    // +0x176
    int modelSizeY;                    // +0x17a
    int modelSizeZ;                    // +0x17e
    int radius;                        // +0x182
    float energyCost;                  // +0x186
    float metalCost;                   // +0x18a
    void* data;                        // +0x18e
    int maxvelocity;                   // +0x192
    int maxslopevelocity;              // +0x196
    int brakerate;                     // +0x19a
    int acceleration;                  // +0x19e
    int bankscale;                     // +0x1a2
    int pitchscale;                    // +0x1a6
    int damagemodifier;                // +0x1aa
    int moverate1;                     // +0x1ae
    int moverate2;                     // +0x1b2
    void* movementclass;               // +0x1b6
    short turnrate;                    // +0x1ba
    short corpse;                      // +0x1bc
    short maxwaterdepth;               // +0x1be
    short minwaterdepth;               // +0x1c0
    float energymake;                  // +0x1c2
    float energyuse;                   // +0x1c6
    float metalmake;                   // +0x1ca
    float extractsMetal;               // +0x1ce
    float windgenerator;               // +0x1d2
    float tidalgenerator;              // +0x1d6
    float cloakcost;                   // +0x1da
    float cloakcostmoving;             // +0x1de
    float energystorage;               // +0x1e2
    float metalstorage;                // +0x1e6
    int buildtime;                     // +0x1ea
    // The three weapon type pointers (WeaponDef, 0x115 bytes each); the loader
    // stores what FindWeaponByName returns, so each file that reads the fields
    // casts.
    char* weapons[3];                  // +0x1ee
    unsigned int maxHealth;            // +0x1fa
    unsigned short buildRate;          // +0x1fe
    short healtime;                    // +0x200
    short range;                       // +0x202
    short radardistance;               // +0x204
    short sonardistance;               // +0x206
    short mincloakdistance;            // +0x208
    short radardistancejam;            // +0x20a
    short sonardistancejam;            // +0x20c
    short soundcategory;               // +0x20e
    short buildangle;                  // +0x210
    unsigned short buildRange;         // +0x212
    short maneuverleashlength;         // +0x214
    unsigned short attackrunlength;    // +0x216
    short kamikazedistance;            // +0x218
    short sortbias;                    // +0x21a
    short altitude;                    // +0x21c
    unsigned short id;                 // +0x21e
    char* explodeas;                   // +0x220
    char* selfdestructas;              // +0x224
    unsigned char maxslope;            // +0x228
    unsigned char maxwaterslope;       // +0x229
    unsigned char capacity;            // +0x22a
    char transportcapacity;            // +0x22b
    unsigned char draft;               // +0x22c
    char makesMetal;                   // +0x22d
    unsigned char buildMenuPageCount;  // +0x22e
    unsigned char mobile;              // +0x22f
    char defaultmissiontype;           // +0x230
    unsigned int* weaponCategories[3]; // +0x231
    unsigned int* categories;          // +0x23d
    unsigned int flags1;               // +0x241
    unsigned int flags2;               // +0x245

    UnitDef& operator=(const UnitDef& src);
    void AddToCategories(char* category);
};

#pragma pack(pop)

#endif
