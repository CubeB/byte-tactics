// UnitWeaponSlot: one of a unit's three weapon slots (Thaldren's
// UnitWeaponSlot, 0x1c bytes), the array at +0x4 of the unit. The one
// declaration of the class, for weapons.cpp, which initialises the slots and
// fires from them, and every file that reads them; the types behind the
// pointers stay private to their own files.
#ifndef UNIT_WEAPON_SLOT_H
#define UNIT_WEAPON_SLOT_H

#include "../units/weapon_aim_cob_cb.h"

struct WeaponDef;

struct UnitWeaponSlot {
    short aimTargetXOrUnitId;          // +0x0
    short aimTargetZOrUnitSentinel;    // +0x2
    WeaponAimCobCb aimCob;             // +0x4
    WeaponDef* weapon;                 // +0xc
    int muzzleAimFromDeltaZ;           // +0x10
    unsigned short reloadTimer;        // +0x14
    short fireHeading;                 // +0x16
    short firePitch;                   // +0x18
    unsigned char stockpile;           // +0x1a
    unsigned char flags;               // +0x1b
};

#endif
