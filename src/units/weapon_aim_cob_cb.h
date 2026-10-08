// WeaponAimCobCb: the completion callback of a weapon slot's Cob "Aim"
// command (8 bytes, Thaldren's WeaponAimCobCb), embedded at +0x4 of each of a
// unit's three weapon slots and reached through the callback table at
// 0x4fd6f0. The one declaration of the class, for unit_script_calls.cpp,
// which defines the method, and every file that reads a slot.
#ifndef WEAPON_AIM_COB_CB_H
#define WEAPON_AIM_COB_CB_H

class WeaponAimCobCb {
public:
    void** ppVtbl;                     // +0x0
    int nAimCobDone;                   // +0x4

    void OnAimCobReturn(int enable);
};

#endif
