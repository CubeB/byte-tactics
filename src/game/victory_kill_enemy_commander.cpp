// Decompiled by Opus. Names are provisional.

#include <string.h>

#pragma pack(push, 1)
struct Info_0048ea40 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Owner_0048ea40 {
    char unknown_0[0x95];
    unsigned char playerIndex;         // +0x95
};

struct Link_0048ea40 {
    char unknown_0[0x27];
    Owner_0048ea40* owner;             // +0x27
};

struct Unit {
    char unknown_0[0x92];
    Info_0048ea40* info;               // +0x92
    Link_0048ea40* link;               // +0x96
    char unknown_9a[0xff - 0x9a];
    unsigned char kind;                // +0xff
};

struct Player_0048ea40 {
    char name[0x232];                  // +0x00
};

struct Game {
    char unknown_0[0x37f5f];
    Player_0048ea40 players[10];       // +0x37f5f
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall PlaySoundByName(char* str, int flag);

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
    int GetIntegerItem(char* name, int def);
};

extern char DAT_00508f3c[]; // "VictoryCondition_KillEnemyCommander"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryKillEnemyCommander {
public:
    int satisfied;                          // +0x04
    int celebrated;                         // +0x08
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 1 of the "kill enemy commander" victory condition (vtable 0x4fd960,
// compare 0x48f6b0 and 0x48ec20): when a unit of kind 1 is named after its
// owner, marks the condition met and announces it once.
// FUNCTION: 0x48ea40
void VictoryKillEnemyCommander::OnUnitDied(Unit* unit)
{
    if (unit->kind == 1) {
        if (_strcmpi(unit->info->name, g_game->players[unit->link->owner->playerIndex].name) == 0) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
}

// Same shape as 0x48f160: writes the "kill enemy commander" victory
// condition's state to a section.
// FUNCTION: 0x48eac0
void VictoryKillEnemyCommander::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508f3c);
    obj->SetIntegerItem(DAT_00508f30, satisfied);
    obj->SetIntegerItem(DAT_00508f24, celebrated);
}

// Reads the "kill enemy commander" victory condition's state from a section;
// the writing counterpart is 0x48eac0, compare 0x48f880.
// FUNCTION: 0x48eb00
void VictoryKillEnemyCommander::LoadState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508f3c);
    satisfied = obj->GetIntegerItem(DAT_00508f30, 0);
    celebrated = obj->GetIntegerItem(DAT_00508f24, 0);
}
