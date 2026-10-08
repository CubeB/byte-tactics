// UnitResources: a unit's (and an AI player's) resource account (Thaldren's
// UnitResourceSlot, 0x34 bytes), embedded at +0xbc of the unit, its player
// pointer at +0x30 pointing back at the unit's owner (unit+0xec). The one
// declaration of the class, for economy.cpp, which defines the methods, and
// every file that reads it; UnitInfo, the save record the save and load
// methods take, stays private to economy.cpp.
#ifndef UNIT_RESOURCES_H
#define UNIT_RESOURCES_H

struct Player;
struct UnitInfo;
class HapiBank;

class UnitResources {
public:
    float energyMake;                  // +0x0
    float energyUse;                   // +0x4
    float energyUsePaid;               // +0x8
    float energyStall;                 // +0xc
    float energyMakePrev;              // +0x10
    float energyUsePrev;               // +0x14
    float metalMake;                   // +0x18
    float metalUse;                    // +0x1c
    float metalUsePaid;                // +0x20
    float metalStall;                  // +0x24
    float metalMakePrev;               // +0x28
    float metalUsePrev;                // +0x2c
    Player* player;                    // +0x30

    void SaveUnitAccounts(UnitInfo* info, HapiBank* file);
    void LoadUnitAccounts(UnitInfo* info, HapiBank* file);
    void Reset(unsigned char playerIndex);
    int RequestEnergy(UnitResources* r, float amount);
    int RequestEnergyAndMetal(float dx, float dy);
    int SpendEnergy(float amount);
    int SpendMetal(float amount);
    int SpendEnergyAndMetal(float energy, float metal);
};

#endif
