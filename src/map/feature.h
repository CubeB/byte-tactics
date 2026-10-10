// Feature: one placed map feature (a rock, a tree, a wreck), 0x100 bytes, the
// elements of the pool at g_game+0x1426f. The one declaration of the struct for
// the files that read or write a feature record. map/features.cpp, which owns
// the module and parses the feature TDFs, keeps its own view, since its loader
// spellings and its by-value animation types do not fit this one; the files
// that read the animation handle by value (ingame/info_panel.cpp and
// ingame/info_panel_46a610.cpp) do too, as do the network snapshots, which are
// a WeaponDef (src/weapons/weapon_def.h).
#ifndef FEATURE_H
#define FEATURE_H

#pragma pack(push, 1)

struct Feature {
    char name[0x80];                   // +0x00
    char description[0x14];            // +0x80
    short footprintX;                  // +0x94, the footprint in cells
    short footprintZ;                  // +0x96
    char unknown_98[0x4c];             // +0x98, the object and the animation
                                       // sequences
    char* burnweapon;                  // +0xe4
    unsigned short burnTime;           // +0xe8
    unsigned short damage;             // +0xea
    float metal;                       // +0xec
    float value;                       // +0xf0
    unsigned short dead;               // +0xf4
    unsigned short burnt;              // +0xf6
    unsigned short reclamate;          // +0xf8
    unsigned char height;              // +0xfa
    unsigned char spreadChance;        // +0xfb
    unsigned char seedChance;          // +0xfc
    unsigned char seedSpread;          // +0xfd
    unsigned char flags;               // +0xfe
    char unknown_ff;                   // +0xff
};

#pragma pack(pop)

#endif
