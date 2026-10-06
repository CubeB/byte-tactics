// Decompiled by Opus. Names are provisional.

#include <string.h>

#pragma pack(push, 1)
struct Info_0048f6b0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Owner_0048f6b0 {
    char unknown_0[0x95];
    unsigned char playerIndex;         // +0x95
};

struct Link_0048f6b0 {
    char unknown_0[0x27];
    Owner_0048f6b0* owner;             // +0x27
};

struct Unit {
    char unknown_0[0x92];
    Info_0048f6b0* info;               // +0x92
    Link_0048f6b0* link;               // +0x96
    char unknown_9a[0xff - 0x9a];
    unsigned char kind;                // +0xff
};

struct Player_0048f6b0 {
    char name[0x232];                  // +0x00
};

struct Game {
    char unknown_0[0x37f5f];
    Player_0048f6b0 players[10];       // +0x37f5f
};
#pragma pack(pop)

extern Game* g_game;

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
    int GetIntegerItem(char* name, int def);
};

class DefeatCommanderKilled {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 1 of the "commander killed" defeat condition (vtable 0x4fd818, state
// saved by 0x48f710).
// FUNCTION: 0x48f6b0
void DefeatCommanderKilled::OnUnitDied(Unit* unit)
{
    if (unit->kind == 0) {
        if (_strcmpi(unit->info->name, g_game->players[unit->link->owner->playerIndex].name) == 0) {
            satisfied = 1;
        }
    }
}

// Writes the "commander killed" defeat condition's state to a section; the
// reading counterpart is 0x48f750, compare 0x48ef00.
// FUNCTION: 0x48f710
void DefeatCommanderKilled::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_CommanderKilled");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48ef40: reads the "commander killed" defeat condition's
// state from a section.
// FUNCTION: 0x48f750
void DefeatCommanderKilled::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_CommanderKilled");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
