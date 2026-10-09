// Decompiled by Opus, space-bunny-free, deepseek-v4.1-flash, Haiku, Space Bunny Free, Sonnet and GPT-6. Names are provisional.

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include "../util/hapi_bank.h"

extern void* DAT_005119b8;

struct Unit;
class HapiBank;

// Mission victory/defeat condition (6 virtual slots).
// Derived classes declare exactly the slots they override, no appended virtuals.
class MissionCondition {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    MissionCondition() { satisfied = celebrated = 0; }
    virtual int IsSatisfied();           // IsSatisfied
    virtual void OnUnitDied(Unit* unit);     // Slot1
    virtual void OnUnitCaptured(Unit* unit);  // Slot2
    virtual void OnUnitCreated(Unit* unit);  // Slot3
    virtual void SaveState(HapiBank* file);            // Save
    virtual void LoadState(HapiBank* file);            // Load
};

// Secondary interface of a condition that visits units (vtable 0x4fd940):
// slot 0 is called for each unit and returns whether to keep going.
class Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

// The same one-slot interface again, as its own type (vtable 0x4fd8a8),
// whose slot returns nothing.
// Must stay a separate type: VictoryMoveUnitToRadius's base uses this vtable.
class Listener_0048f250 {
public:
    virtual void VisitUnit(Unit* unit) = 0;
};

struct Vec3 {
    int x, y, z;
    Vec3() {}
    Vec3(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

// The registration parameter: the command reader lives at +0x4.
struct Param_0048e010 {
    char unknown_0[4];
    void* reader;                        // +0x4
};

#pragma pack(push, 1)
struct UnitType {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Owner_0048ea40 {
    char unknown_0[0x95];
    unsigned char side;                // +0x95
};

struct Link_0048ea40 {
    char unknown_0[0x27];
    Owner_0048ea40* owner;             // +0x27
};

struct Unit {
    int motion;                       // +0x0
    char unknown_4[0x76 - 0x4];
    short cell;                    // +0x76
    short field_78;                    // +0x78
    char unknown_7a[0x86 - 0x7a];
    Unit* carrier;                     // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType* def;                     // +0x92
    Link_0048ea40* player;             // +0x96
    char unknown_9a[0xa6 - 0x9a];
    short unitDefIndex;                    // +0xa6
    char unknown_a8[0xfb - 0xa8];
    int postTransferHoldoff;                      // +0xfb
    unsigned char playerIndex;         // +0xff
    char unknown_100[0x104 - 0x100];
    float buildLeft;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct UnitList {
    Unit* first;                       // +0x0
    Unit* last;                        // +0x4 (inclusive)

    void ForEach(Listener_0048ff40* visitor)
    {
        for (Unit* unit = first; unit <= last; unit++) {
            if (unit->unitDefIndex != 0) {
                int result = visitor->VisitUnit(unit);
                if (!result) {
                    break;
                }
            }
        }
    }
};

struct PlayerInfo {                    // flags at +0x9b and +0x9d
    char unknown_0[0x9b];
    unsigned char flags_9b;            // +0x9b, bit 0x40 is tested
    unsigned char unknown_9c;
    unsigned char flags_9d;            // +0x9d, bit 2 is tested
};

#include "../network/player.h"

struct PlayerName {
    char name[0x232];                  // +0x00
};

#include "../map/mission.h"

struct Game {
    char unknown_0[0x1b63];
    union {
        Player players[10];            // +0x1b63
        struct {
            char unknown_1b63[0x1bca - 0x1b63];
            UnitList units;            // +0x1bca
            char unknown_1bd2[0x1d15 - 0x1bd2];
            UnitList units2;           // +0x1d15
            char unknown_1d1d[0x1df2 - 0x1d1d];
            short field_1df2;          // +0x1df2
        };
    };
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;              // +0x2a42
    char unknown_2a43[0x1427f - 0x2a43];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x37ef6 - 0x14280];
    int value_37ef6;                   // +0x37ef6
    char unknown_37efa[0x37f5f - 0x37efa];
    union {
        PlayerName names[10];          // +0x37f5f
        struct {
            char unknown_37f5f[0x38a47 - 0x37f5f];
            unsigned int ticks;        // +0x38a47
            char unknown_38a4b[0x391e9 - 0x38a4b];
            Mission* mode;             // +0x391e9
        };
    };
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

void __stdcall PlaySoundByName(char* str, int flag);
short __stdcall FindUnitTypeId(char* name);
void __stdcall ClampWorldPosToTerrain(int x, int z, Vec3* out);
void __stdcall VisitObjectsInRange(Vec3* pos, int radius, Listener_0048f250* visitor);

// KillEnemyCommander (vtable 0x4fd960).
class VictoryKillEnemyCommander : public MissionCondition {
public:
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// DestroyAllUnits (vtable 0x4fd948).
class VictoryDestroyAllUnits : public MissionCondition {
public:
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// KillAllMobileUnits (vtable 0x4fd928, listener vtable 0x4fd920).
class VictoryKillAllMobileUnits : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit);
    int numUnits;                        // +0x10
    VictoryKillAllMobileUnits() { numUnits = 0; }
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// BuildUnitType (vtable 0x4fd908, listener vtable 0x4fd900).
// pack(2): size 0x32 would otherwise be allocated as 0x34.
#pragma pack(push, 2)
class VictoryBuildUnitType : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit);
    char name[0x20];                     // +0x10
    short id;                            // +0x30

    VictoryBuildUnitType(const char* text)
    {
        strcpy(name, text);
        id = 0;
    }
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};
#pragma pack(pop)

// CaptureUnitType (vtable 0x4fd8e8).
class VictoryCaptureUnitType : public MissionCondition {
public:
    char name[0x20];                     // +0xc

    VictoryCaptureUnitType(const char* text) { strcpy(name, text); }
    virtual void OnUnitCaptured(Unit* unit);
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// KillAllOfType (vtable 0x4fd8d0, listener vtable 0x4fd8c8).
// pack(2): size 0x36 would otherwise be allocated as 0x38.
#pragma pack(push, 2)
class VictoryKillAllOfType : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit);
    char name[0x20];                     // +0x10
    short id;                            // +0x30
    int count;                           // +0x32

    VictoryKillAllOfType(const char* text) { strcpy(name, text); }
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};
#pragma pack(pop)

// KillUnitType (vtable 0x4fd8b0).
class VictoryKillUnitType : public MissionCondition {
public:
    char name[0x20];                     // +0xc
    int numLeftToKill;                   // +0x2c

    VictoryKillUnitType(const char* text, int n)
    {
        strcpy(name, text);
        numLeftToKill = n;
    }
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// MoveUnitToRadius (vtable 0x4fd890, listener vtable 0x4fd888).
#pragma pack(push, 2)
class VictoryMoveUnitToRadius : public MissionCondition, public Listener_0048f250 {
public:
    virtual void VisitUnit(Unit* unit);
    char name[0x20];                     // +0x10
    Vec3 pos;                   // +0x30
    int radius;                          // +0x3c

    VictoryMoveUnitToRadius(const char* text, int x, int z, int r)
    {
        if (_strcmpi(text, "ANYTYPE") == 0)
            name[0] = 0;
        else
            strcpy(name, text);
        pos.x = x;
        pos.z = z;
        radius = r << 16;
        // Stored after pos.z and radius.
        pos.y = 0x12345678;
    }
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};
#pragma pack(pop)

// UnitTypePassesX (vtable 0x4fd870, listener vtable 0x4fd868).
#pragma pack(push, 2)
class VictoryUnitTypePassesX : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit);
    char name[0x20];                     // +0x10
    int field_30;                        // +0x30

    VictoryUnitTypePassesX(const char* text, int v)
    {
        if (_strcmpi(text, "ANYTYPE") == 0)
            name[0] = 0;
        else
            strcpy(name, text);
        field_30 = v >> 4;
    }
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};
#pragma pack(pop)

// UnitTypePassesZ (vtable 0x4fd850, listener vtable 0x4fd848).
#pragma pack(push, 2)
class VictoryUnitTypePassesZ : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit);
    char name[0x20];                     // +0x10
    int field_30;                        // +0x30

    VictoryUnitTypePassesZ(const char* text, int v)
    {
        if (_strcmpi(text, "ANYTYPE") == 0)
            name[0] = 0;
        else
            strcpy(name, text);
        field_30 = v >> 4;
    }
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};
#pragma pack(pop)

// VictoryTimerRunsOut (vtable 0x4fd830).
class VictoryTimerRunsOut : public MissionCondition {
public:
    unsigned int field_c;                // +0xc

    VictoryTimerRunsOut(int t) { field_c = t * 30; }
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// CommanderKilled (vtable 0x4fd818).
class DefeatCommanderKilled : public MissionCondition {
public:
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// AllUnitsKilled (vtable 0x4fd800, listener vtable 0x4fd7f8).
class DefeatAllUnitsKilled : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
    virtual int VisitUnit(Unit* unit);
};

// AllUnitsKilledOfType (vtable 0x4fd7e0, listener vtable 0x4fd7d8).
#pragma pack(push, 2)
class DefeatAllUnitsKilledOfType : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit);
    char name[0x20];                     // +0x10
    short id;                            // +0x30
    int count;                           // +0x32

    DefeatAllUnitsKilledOfType(const char* text) { strcpy(name, text); }
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};
#pragma pack(pop)

// UnitTypeKilled (vtable 0x4fd7c0).
class DefeatUnitTypeKilled : public MissionCondition {
public:
    char name[0x20];                     // +0xc
    int numLeftToKill;                   // +0x2c

    DefeatUnitTypeKilled(const char* text, int n)
    {
        strcpy(name, text);
        numLeftToKill = n;
    }
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// DeathTimerRunsOut (vtable 0x4fd7a8).
class DefeatDeathTimerRunsOut : public MissionCondition {
public:
    int field_c;                         // +0xc

    DefeatDeathTimerRunsOut(int t) { field_c = t * 30; }
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// AnyUnitPassesX (vtable 0x4fd790, listener vtable 0x4fd788).
class DefeatAnyUnitPassesX : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit);
    int field_10;                        // +0x10

    DefeatAnyUnitPassesX(int v) { field_10 = v >> 4; }
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

// AnyUnitPassesZ (vtable 0x4fd770, listener vtable 0x4fd768).
class DefeatAnyUnitPassesZ : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit);
    int field_10;                        // +0x10

    DefeatAnyUnitPassesZ(int v) { field_10 = v >> 4; }
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

extern int GetCdPathMismatch();

extern unsigned int g_cdMismatchDefeatTick;

class MissionConditions {
public:
    MissionCondition* victory[16];       // +0x00
    int victoryCount;                    // +0x40
    MissionCondition* defeat[16];        // +0x44
    int defeatCount;                     // +0x84
    int active;                          // +0x88, set to 1 by the constructor (0x48df90)

    void FreeConditions();
    void RegisterConditions(Param_0048e010* p);
    void SaveConditions(HapiBank* file);
    void LoadConditions(HapiBank* file);
    int AllVictoryConditionsMet();
    int AnyDefeatConditionMet();
    // 0x490230, in victory_490230.cpp: it matches only in a file of its own,
    // where few enough symbols come before it to keep its SIB operand order.
    int CheckVictory();
    int CheckDefeat();
    void Deactivate();
    void NotifyUnitDied(Unit* unit);
    void NotifyUnitCaptured(Unit* unit);
    void NotifyUnitCreated(Unit* unit);
};

// Appends a condition to one of the two lists, at the caller's count.
static inline void AddCondition(MissionCondition** list, int* count, MissionCondition* value)
{
    list[*count] = value;
    (*count)++;
}

// FUNCTION: 0x48dfb0
void MissionConditions::FreeConditions()
{
    int i;
    for (i = 0; i < victoryCount; i++) {
        operator delete(victory[i]);
    }
    for (i = 0; i < defeatCount; i++) {
        operator delete(defeat[i]);
    }
}

// MissionConditions::RegisterConditions, the victory/defeat condition registration
// function (counterpart of 0x48ff40). All ~18 registration blocks plus the two
// "if none registered" defaults are here.
// FUNCTION: 0x48e010
void MissionConditions::RegisterConditions(Param_0048e010* p)
{
    char buf[0x100];
    char stype[0x100];

    // Every reader result goes into a named int before the != 0 test: a bare
    // call folds to test instead of cmp against the zero register.
    int r1 = ((TdfRecord*)p->reader)->GetFieldInt("KillEnemyCommander", 0);
    if (r1 != 0) {
        AddCondition(victory, &victoryCount, new VictoryKillEnemyCommander);
    }
    int r2 = ((TdfRecord*)p->reader)->GetFieldInt("DestroyAllUnits", 0);
    if (r2 != 0) {
        AddCondition(victory, &victoryCount, new VictoryDestroyAllUnits);
    }
    int r3 = ((TdfRecord*)p->reader)->GetFieldInt("KillAllMobileUnits", 0);
    if (r3 != 0) {
        AddCondition(victory, &victoryCount, new VictoryKillAllMobileUnits);
    }
    int r4 = ((TdfRecord*)p->reader)->GetFieldString(buf, "BuildUnitType", 0x100, (char*)&DAT_005119b8);
    if (r4 != 0) {
        AddCondition(victory, &victoryCount, new VictoryBuildUnitType(buf));
    }
    int r5 = ((TdfRecord*)p->reader)->GetFieldString(buf, "CaptureUnitType", 0x100, (char*)&DAT_005119b8);
    if (r5 != 0) {
        AddCondition(victory, &victoryCount, new VictoryCaptureUnitType(buf));
    }
    int r6 = ((TdfRecord*)p->reader)->GetFieldString(buf, "KillAllOfType", 0x100, (char*)&DAT_005119b8);
    if (r6 != 0) {
        AddCondition(victory, &victoryCount, new VictoryKillAllOfType(buf));
    }
    int r7 = ((TdfRecord*)p->reader)->GetFieldString(buf, "KillUnitType", 0x100, (char*)&DAT_005119b8);
    if (r7 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        AddCondition(victory, &victoryCount, new VictoryKillUnitType(stype, n));
    }
    int r8 = ((TdfRecord*)p->reader)->GetFieldString(buf, "MoveUnitToRadius", 0x100, (char*)&DAT_005119b8);
    if (r8 != 0) {
        int a, b, c;
        sscanf(buf, "%[a-zA-Z],%i,%i,%i", stype, &a, &b, &c);
        AddCondition(victory, &victoryCount, new VictoryMoveUnitToRadius(stype, a, b, c));
    }
    int r9 = ((TdfRecord*)p->reader)->GetFieldString(buf, "UnitTypePassesX", 0x100, (char*)&DAT_005119b8);
    if (r9 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        AddCondition(victory, &victoryCount, new VictoryUnitTypePassesX(stype, n));
    }
    int r10 = ((TdfRecord*)p->reader)->GetFieldString(buf, "UnitTypePassesZ", 0x100, (char*)&DAT_005119b8);
    if (r10 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        AddCondition(victory, &victoryCount, new VictoryUnitTypePassesZ(stype, n));
    }
    {
        int t = ((TdfRecord*)p->reader)->GetFieldInt("VictoryTimerRunsOut", 0);
        if (t > 0) {
            AddCondition(victory, &victoryCount, new VictoryTimerRunsOut(t));
        }
    }
    int r11 = ((TdfRecord*)p->reader)->GetFieldInt("CommanderKilled", 0);
    if (r11 != 0) {
        AddCondition(defeat, &defeatCount, new DefeatCommanderKilled);
    }
    int r12 = ((TdfRecord*)p->reader)->GetFieldInt("AllUnitsKilled", 0);
    if (r12 != 0) {
        AddCondition(defeat, &defeatCount, new DefeatAllUnitsKilled);
    }
    int r13 = ((TdfRecord*)p->reader)->GetFieldString(buf, "AllUnitsKilledOfType", 0x100, (char*)&DAT_005119b8);
    if (r13 != 0) {
        AddCondition(defeat, &defeatCount, new DefeatAllUnitsKilledOfType(buf));
    }
    int r14 = ((TdfRecord*)p->reader)->GetFieldString(buf, "UnitTypeKilled", 0x100, (char*)&DAT_005119b8);
    if (r14 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        AddCondition(defeat, &defeatCount, new DefeatUnitTypeKilled(stype, n));
    }
    {
        int t = ((TdfRecord*)p->reader)->GetFieldInt("DeathTimerRunsOut", 0);
        if (t > 0) {
            AddCondition(defeat, &defeatCount, new DefeatDeathTimerRunsOut(t));
        }
    }
    {
        int t = ((TdfRecord*)p->reader)->GetFieldInt("AnyUnitPassesX", -1);
        if (t >= 0) {
            AddCondition(defeat, &defeatCount, new DefeatAnyUnitPassesX(t));
        }
    }
    {
        int t = ((TdfRecord*)p->reader)->GetFieldInt("AnyUnitPassesZ", -1);
        if (t >= 0) {
            AddCondition(defeat, &defeatCount, new DefeatAnyUnitPassesZ(t));
        }
    }
    if (victoryCount == 0) {
        AddCondition(victory, &victoryCount, new VictoryDestroyAllUnits);
    }
    if (defeatCount == 0) {
        AddCondition(defeat, &defeatCount, new DefeatAllUnitsKilled);
    }
}

// The victory/defeat condition base's IsSatisfied (slot 0 of the condition
// vtables whose class does not override it).
// FUNCTION: 0x48ea00
int MissionCondition::IsSatisfied()
{
    return satisfied;
}

// The victory/defeat condition base's slot 1, called with a unit: does
// nothing unless a condition overrides it.
// FUNCTION: 0x48ea10
void MissionCondition::OnUnitDied(Unit*)
{
}

// The victory/defeat condition base's slot 2, called with a unit: does
// nothing unless a condition overrides it.
// FUNCTION: 0x48ea20
void MissionCondition::OnUnitCaptured(Unit*)
{
}

// The victory/defeat condition base's slot 3, which takes one pointer (taken
// to be a unit, like slots 1 and 2): no condition overrides it.
// FUNCTION: 0x48ea30
void MissionCondition::OnUnitCreated(Unit*)
{
}

extern char DAT_00508f3c[]; // "VictoryCondition_KillEnemyCommander"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

// Slot 1 of the "kill enemy commander" victory condition (vtable 0x4fd960,
// compare 0x48f6b0 and 0x48ec20): when a unit of player index 1 is named after its
// owner, marks the condition met and announces it once.
// FUNCTION: 0x48ea40
void VictoryKillEnemyCommander::OnUnitDied(Unit* unit)
{
    if (unit->playerIndex == 1) {
        if (_strcmpi(unit->def->name, g_game->names[unit->player->owner->side].name) == 0) {
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

extern char DAT_00508f60[]; // "VictoryCondition_DestroyAllUnits"

// Slot 0 (IsSatisfied) of the "destroy all units" victory condition (vtable
// 0x4fd948, state saved by 0x48eb80): satisfied while the game field at
// +0x1df2 is zero, announcing "Victory Condition" once.
// FUNCTION: 0x48eb40
int VictoryDestroyAllUnits::IsSatisfied()
{
    if (g_game->field_1df2 == 0) {
        if (celebrated == 0) {
            PlaySoundByName("Victory Condition", 0);
            celebrated = 1;
        }
        return 1;
    }
    return 0;
}

// Same shape as 0x48eac0: writes the "destroy all units" victory condition's
// state to a section.
// FUNCTION: 0x48eb80
void VictoryDestroyAllUnits::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508f60);
    obj->SetIntegerItem(DAT_00508f30, satisfied);
    obj->SetIntegerItem(DAT_00508f24, celebrated);
}

// Same shape as 0x48faf0: reads the "destroy all units" victory condition's
// state from a section.
// FUNCTION: 0x48ebc0
void VictoryDestroyAllUnits::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_DestroyAllUnits");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

extern char DAT_00508f90[]; // "VictoryCondition_KillAllMobileUnits"
extern char DAT_00508f84[]; // "NumUnits"

// Slot 0 of the unit visitor at +0xc of the "kill all mobile units" victory
// condition (visitor vtable 0x4fd920, driven by 0x48ec20): counts the units
// whose first field is set and returns whether at most one has been seen.
// VisitUnit overrides the visitor's slot, so its `this` is the visitor
// subobject (+0xc) and numUnits sits at +0x4 from it.
// FUNCTION: 0x48ec00
int VictoryKillAllMobileUnits::VisitUnit(Unit* unit)
{
    if (unit->motion != 0) {
        numUnits++;
    }
    return numUnits <= 1;
}

// Slot 1 of the "kill all mobile units" victory condition (vtable 0x4fd928,
// visitor vtable 0x4fd920 holding 0x48ec00; state saved by 0x48ecb0): counts
// the live units through the visitor and announces the victory condition
// when at most one is left.
// FUNCTION: 0x48ec20
void VictoryKillAllMobileUnits::OnUnitDied(Unit* unit)
{
    if (unit->playerIndex == 1 && unit->motion != 0) {
        numUnits = 0;
        g_game->units2.ForEach(this);
        if (numUnits <= 1) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
}

// FUNCTION: 0x48ecb0
void VictoryKillAllMobileUnits::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508f90);
    obj->SetIntegerItem(DAT_00508f84, numUnits);
    obj->SetIntegerItem(DAT_00508f30, satisfied);
    obj->SetIntegerItem(DAT_00508f24, celebrated);
}

// The load counterpart of 0x48ecb0.
// FUNCTION: 0x48ed00
void VictoryKillAllMobileUnits::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_KillAllMobileUnits");
    numUnits = obj->GetIntegerItem("NumUnits", 0);
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

extern char DAT_00508fb4[]; // "VictoryCondition_BuildUnitType"

// Slot 0 of the unit visitor at +0xc of the "build unit type" victory
// condition (visitor vtable 0x4fd900, driven by 0x48edb0). VisitUnit overrides
// the visitor's slot, so MSVC passes `this` as the visitor subobject (+0xc)
// and the other fields appear at negative offsets.
// FUNCTION: 0x48ed50
int VictoryBuildUnitType::VisitUnit(Unit* unit)
{
    if (unit->unitDefIndex == id && unit->buildLeft == 0.0f) {
        satisfied = 1;
        if (celebrated == 0) {
            PlaySoundByName("Victory Condition", 0);
            celebrated = 1;
        }
    }
    return satisfied == 0;
}

// FUNCTION: 0x48edb0
int VictoryBuildUnitType::IsSatisfied()
{
    if (satisfied != 0) {
        return 1;
    }
    if (id == 0) {
        id = FindUnitTypeId(name);
    }
    g_game->units.ForEach(this);
    return satisfied;
}

// Same shape as 0x48eb80: writes the "build unit type" victory condition's
// state to a section.
// FUNCTION: 0x48ee30
void VictoryBuildUnitType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_BuildUnitType");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Reads the "build unit type" victory condition's state from a section; the
// writing counterpart is 0x48ee30, same shape as 0x48eb00.
// FUNCTION: 0x48ee70
void VictoryBuildUnitType::LoadState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508fb4);
    satisfied = obj->GetIntegerItem(DAT_00508f30, 0);
    celebrated = obj->GetIntegerItem(DAT_00508f24, 0);
}

// Slot 2 of the condition (state saved by 0x48ef00).
// FUNCTION: 0x48eeb0
void VictoryCaptureUnitType::OnUnitCaptured(Unit* unit)
{
    if (unit->playerIndex == 1 && _strcmpi(name, unit->def->name) == 0) {
        satisfied = 1;
        if (celebrated == 0) {
            PlaySoundByName("Victory Condition", 0);
            celebrated = 1;
        }
    }
}

// Writes the "capture unit type" victory condition's state to a section;
// the reading counterpart is 0x48ef40, compare 0x48f840.
// FUNCTION: 0x48ef00
void VictoryCaptureUnitType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_CaptureUnitType");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// FUNCTION: 0x48ef40
void VictoryCaptureUnitType::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_CaptureUnitType");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 0 of the unit visitor at +0xc of the "kill all of type" victory
// condition (visitor vtable 0x4fd8c8, driven by 0x48efb0): counts the units
// of the condition's type and returns whether at most one has been seen.
// FUNCTION: 0x48ef80
int VictoryKillAllOfType::VisitUnit(Unit* unit)
{
    if (unit->unitDefIndex == id) {
        count++;
    }
    return count <= 1;
}

// Slot 1 of the "kill all of type" victory condition (vtable 0x4fd8d0,
// visitor vtable 0x4fd8c8 holding 0x48ef80; state saved by 0x48f070).
// FUNCTION: 0x48efb0
void VictoryKillAllOfType::OnUnitDied(Unit* unit)
{
    if (satisfied == 0 && unit->playerIndex == 1 && _strcmpi(name, unit->def->name) == 0) {
        id = FindUnitTypeId(name);
        count = 0;
        g_game->units2.ForEach(this);
        if (count <= 1) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
}

// Writes the "kill all of type" victory condition's state to a section;
// the reading counterpart is 0x48f0b0, compare 0x48ef00.
// FUNCTION: 0x48f070
void VictoryKillAllOfType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_KillAllOfType");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48ef40.
// FUNCTION: 0x48f0b0
void VictoryKillAllOfType::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_KillAllOfType");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

extern char DAT_00509028[]; // "VictoryCondition_KillUnitType"
extern char DAT_00509018[]; // "NumLeftToKill"

// Slot 1 of the "kill unit type" victory condition (vtable 0x4fd8b0, state
// saved by 0x48f160): called with a unit; counts the named unit type down and
// announces the victory condition when the count runs out.
// FUNCTION: 0x48f0f0
void VictoryKillUnitType::OnUnitDied(Unit* unit)
{
    if (numLeftToKill > 0 && unit->playerIndex == 1 && _strcmpi(name, unit->def->name) == 0) {
        if (--numLeftToKill <= 0) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
}

// FUNCTION: 0x48f160
void VictoryKillUnitType::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00509028);
    obj->SetIntegerItem(DAT_00509018, numLeftToKill);
    obj->SetIntegerItem(DAT_00508f30, satisfied);
    obj->SetIntegerItem(DAT_00508f24, celebrated);
}

// Load counterpart of 0x48f160 (the "kill unit type" victory condition).
// FUNCTION: 0x48f1b0
void VictoryKillUnitType::LoadState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00509028);
    numLeftToKill = obj->GetIntegerItem(DAT_00509018, 0);
    satisfied = obj->GetIntegerItem(DAT_00508f30, 0);
    celebrated = obj->GetIntegerItem(DAT_00508f24, 0);
}

// The "move unit to radius" victory condition (vtable 0x4fd890, visitor
// vtable 0x4fd888): slot 0 fills in the target height on first use
// (0x12345678 marks it unset), visits the units within the radius and
// returns whether the condition is met.
// FUNCTION: 0x48f200
int VictoryMoveUnitToRadius::IsSatisfied()
{
    if (pos.y == 0x12345678) {
        ClampWorldPosToTerrain(pos.x, pos.z, &pos);
    }
    VisitObjectsInRange(&pos, radius, this);
    return satisfied;
}

// Slot 0 of the unit visitor at +0xc of the "move unit to radius" victory
// condition (visitor vtable 0x4fd888, driven by 0x48f200).
// FUNCTION: 0x48f250
void VictoryMoveUnitToRadius::VisitUnit(Unit* unit)
{
    if (unit->playerIndex == 0) {
        if (name[0] != 0 && _strcmpi(name, unit->def->name) != 0) {
            return;
        }
        if ((unit->flags & 0x20) && unit->buildLeft == 0.0f && unit->postTransferHoldoff == 0
            && (unit->carrier == 0 || (unit->carrier->flags & 0x40000000))) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
}

// Writes the "move unit to radius" victory condition's state to a section
// (same shape as 0x48f070).
// FUNCTION: 0x48f2f0
void VictoryMoveUnitToRadius::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_MoveUnitToRadius");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48f0b0.
// FUNCTION: 0x48f330
void VictoryMoveUnitToRadius::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_MoveUnitToRadius");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 0 of the unit visitor at +0xc of the "unit type passes X" victory
// condition (visitor vtable 0x4fd868, driven by 0x48f3e0).
// FUNCTION: 0x48f370
int VictoryUnitTypePassesX::VisitUnit(Unit* unit)
{
    if (name[0] == 0 || _strcmpi(name, unit->def->name) == 0) {
        if (abs(unit->cell - field_30) <= 2) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
    return satisfied == 0;
}

// Slot 0 of the "unit type passes X" victory condition (vtable 0x4fd870,
// visitor vtable 0x4fd868 holding 0x48f370; state saved by 0x48f440): until
// the condition is met, visits every live unit and returns whether it is met.
// FUNCTION: 0x48f3e0
int VictoryUnitTypePassesX::IsSatisfied()
{
    if (satisfied == 0) {
        g_game->units.ForEach(this);
    }
    return satisfied;
}

// Writes the "unit type passes X" victory condition's state to a section;
// the reading counterpart is 0x48f480 (compare 0x48ee30).
// FUNCTION: 0x48f440
void VictoryUnitTypePassesX::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_UnitTypePassesX");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48f330: reads the "unit type passes X" victory condition's
// state from a section.
// FUNCTION: 0x48f480
void VictoryUnitTypePassesX::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_UnitTypePassesX");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 0 of the unit visitor at +0xc of the "unit type passes Z" victory
// condition (visitor vtable 0x4fd848, driven by 0x48f530).
// FUNCTION: 0x48f4c0
int VictoryUnitTypePassesZ::VisitUnit(Unit* unit)
{
    if (name[0] == 0 || _strcmpi(name, unit->def->name) == 0) {
        if (abs(unit->field_78 - field_30) <= 2) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
    return satisfied == 0;
}

// Slot 0 of the "unit type passes Z" victory condition (vtable 0x4fd850,
// visitor vtable 0x4fd848 holding 0x48f4c0; state saved by 0x48f590): until
// the condition is met, visits every live unit and returns whether it is met.
// FUNCTION: 0x48f530
int VictoryUnitTypePassesZ::IsSatisfied()
{
    if (satisfied == 0) {
        g_game->units.ForEach(this);
    }
    return satisfied;
}

// Writes the "unit type passes Z" victory condition's state to a section;
// the reading counterpart is 0x48f5d0 (compare 0x48f440).
// FUNCTION: 0x48f590
void VictoryUnitTypePassesZ::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_UnitTypePassesZ");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48f480: reads the "unit type passes Z" victory condition's
// state from a section.
// FUNCTION: 0x48f5d0
void VictoryUnitTypePassesZ::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_UnitTypePassesZ");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 0 (IsSatisfied) of the "victory timer runs out" victory condition
// (vtable 0x4fd830): met once the game's tick count reaches the limit.
// FUNCTION: 0x48f610
int VictoryTimerRunsOut::IsSatisfied()
{
    unsigned int game_val = g_game->ticks;
    return game_val >= field_c;
}

// Same shape as 0x48fd70: writes the "victory timer runs out" victory
// condition's state to a section (its reader is 0x48f670).
// FUNCTION: 0x48f630
void VictoryTimerRunsOut::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_VictoryTimerRunsOut");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48f480: reads the "victory timer runs out" victory
// condition's state from a section.
// FUNCTION: 0x48f670
void VictoryTimerRunsOut::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_VictoryTimerRunsOut");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 1 of the "commander killed" defeat condition (vtable 0x4fd818, state
// saved by 0x48f710).
// FUNCTION: 0x48f6b0
void DefeatCommanderKilled::OnUnitDied(Unit* unit)
{
    if (unit->playerIndex == 0) {
        if (_strcmpi(unit->def->name, g_game->names[unit->player->owner->side].name) == 0) {
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

extern char DAT_005090fc[]; // "DefeatCondition_AllUnitsKilled"

// Slot 0 of the unit visitor at +0xc of the "all units killed" defeat
// condition (vtable 0x4fd7f8, stored by the constructors inlined at 0x48e6e5
// and friends). Clears the condition when it meets a live, finished unit.
// VisitUnit's `this` is the visitor subobject (+0xc), so satisfied sits at -8.
// FUNCTION: 0x48f790
int DefeatAllUnitsKilled::VisitUnit(Unit* unit)
{
    if ((unit->flags & 0x20) && unit->buildLeft == 0.0f && unit->postTransferHoldoff == 0
        && (unit->carrier == 0 || (unit->carrier->flags & 0x40000000))) {
        satisfied = 0;
    }
    return satisfied;
}

// Slot 0 of the defeat condition with vtable 0x4fd800 (state saved by
// 0x48f840): assumes the condition is met and offers every live unit to the
// visitor at +0xc, which can clear it.
// FUNCTION: 0x48f7e0
int DefeatAllUnitsKilled::IsSatisfied()
{
    satisfied = 1;
    g_game->units.ForEach(this);
    return satisfied;
}

// FUNCTION: 0x48f840
void DefeatAllUnitsKilled::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_005090fc);
    obj->SetIntegerItem(DAT_00508f30, satisfied);
    obj->SetIntegerItem(DAT_00508f24, celebrated);
}

// Reads the "all units killed" defeat condition's state from a section;
// the writing counterpart is 0x48f840, compare 0x48ef40.
// FUNCTION: 0x48f880
void DefeatAllUnitsKilled::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AllUnitsKilled");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 1 of the "unit type killed" defeat condition (vtable 0x4fd7c0, state
// saved by 0x48f900): when a unit of the named type dies, counts it down and
// marks the condition satisfied once none are left.
// FUNCTION: 0x48f8c0
void DefeatUnitTypeKilled::OnUnitDied(Unit* unit)
{
    if (_strcmpi(name, unit->def->name) == 0) {
        if (--numLeftToKill <= 0) {
            satisfied = 1;
        }
    }
}

// FUNCTION: 0x48f900
void DefeatUnitTypeKilled::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_UnitTypeKilled");
    obj->SetIntegerItem("NumLeftToKill", numLeftToKill);
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Reads the "unit type killed" defeat condition's state from a section;
// the writing counterpart is 0x48f900, compare 0x48f880.
// FUNCTION: 0x48f950
void DefeatUnitTypeKilled::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_UnitTypeKilled");
    numLeftToKill = obj->GetIntegerItem("NumLeftToKill", 0);
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 0 of the unit visitor at +0xc of the "all units killed of type"
// defeat condition (visitor vtable 0x4fd7d8, driven by 0x48f9d0): counts
// the units of the condition's type and returns whether at most one has
// been seen.
// FUNCTION: 0x48f9a0
int DefeatAllUnitsKilledOfType::VisitUnit(Unit* unit)
{
    if (unit->unitDefIndex == id) {
        count++;
    }
    return count <= 1;
}

// Slot 1 of the "all units killed of type" defeat condition (vtable
// 0x4fd7e0, visitor vtable 0x4fd7d8 holding 0x48f9a0; state saved by
// 0x48fab0). Like 0x48efb0, but it counts the live units of the two lists in
// g_game (the ones at +0x1bca and +0x1d15) and only then decides.
// FUNCTION: 0x48f9d0
void DefeatAllUnitsKilledOfType::OnUnitDied(Unit* unit)
{
    if (_strcmpi(name, unit->def->name) == 0) {
        id = FindUnitTypeId(name);
        count = 0;
        g_game->units.ForEach(this);
        g_game->units2.ForEach(this);
        if (count <= 1) {
            satisfied = 1;
        }
    }
}

// Same shape as 0x48eb80: writes the "all units killed of type" defeat
// condition's state to a section.
// FUNCTION: 0x48fab0
void DefeatAllUnitsKilledOfType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AllUnitsKilledOfType");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48ef40.
// FUNCTION: 0x48faf0
void DefeatAllUnitsKilledOfType::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AllUnitsKilledOfType");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 0 of the unit visitor at +0xc of the "any unit passes X" defeat
// condition (visitor vtable 0x4fd788): marks the condition met when a unit
// is within 2 of the target X, and returns whether to keep visiting.
// VisitUnit's `this` is the visitor subobject (+0xc) and satisfied sits at -8.
// FUNCTION: 0x48fb30
int DefeatAnyUnitPassesX::VisitUnit(Unit* unit)
{
    int diff = abs((int)unit->cell - field_10);

    if (diff <= 2) {
        satisfied = 1;
    }
    return satisfied == 0;
}

// Slot 0 of the "any unit passes X" defeat condition (vtable 0x4fd790,
// visitor vtable 0x4fd788 holding 0x48fb30; state saved by 0x48fbc0): until
// the condition is met, visits every live unit and returns whether it is.
// FUNCTION: 0x48fb60
int DefeatAnyUnitPassesX::IsSatisfied()
{
    if (satisfied == 0) {
        g_game->units2.ForEach(this);
    }
    return satisfied;
}

// Same shape as 0x48ee30: writes the "any unit passes X" defeat condition's
// state to a section.
// FUNCTION: 0x48fbc0
void DefeatAnyUnitPassesX::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AnyUnitPassesX");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Reads the "any unit passes X" defeat condition's state from a section
// (same shape as 0x48f0b0).
// FUNCTION: 0x48fc00
void DefeatAnyUnitPassesX::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AnyUnitPassesX");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 0 of the unit visitor at +0xc of the "any unit passes Z" defeat
// condition (visitor vtable 0x4fd768): marks the condition met when a unit
// is within 2 of the target Z, and returns whether to keep visiting.
// VisitUnit's `this` is the visitor subobject (+0xc) and satisfied sits at -8.
// FUNCTION: 0x48fc40
int DefeatAnyUnitPassesZ::VisitUnit(Unit* unit)
{
    int diff = abs((int)unit->field_78 - field_10);

    if (diff <= 2) {
        satisfied = 1;
    }
    return satisfied == 0;
}

// Slot 0 of the "any unit passes Z" defeat condition (vtable 0x4fd770,
// visitor vtable 0x4fd768 holding 0x48fc40; state saved by 0x48fcd0): until
// the condition is met, visits every live unit and returns whether it is.
// FUNCTION: 0x48fc70
int DefeatAnyUnitPassesZ::IsSatisfied()
{
    if (satisfied == 0) {
        g_game->units2.ForEach(this);
    }
    return satisfied;
}

// Same shape as 0x48fbc0: writes the "any unit passes Z" defeat condition's
// state to a section.
// FUNCTION: 0x48fcd0
void DefeatAnyUnitPassesZ::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AnyUnitPassesZ");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Reads the "any unit passes Z" defeat condition's state from a section
// (same shape as 0x48f480).
// FUNCTION: 0x48fd10
void DefeatAnyUnitPassesZ::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AnyUnitPassesZ");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// Slot 0 (IsSatisfied) of the "death timer runs out" defeat condition
// (vtable 0x4fd7a8): met once the game's tick count reaches the limit.
// FUNCTION: 0x48fd50
int DefeatDeathTimerRunsOut::IsSatisfied()
{
    unsigned int game_val = g_game->ticks;
    return game_val >= (unsigned int)field_c;
}

// FUNCTION: 0x48fd70
void DefeatDeathTimerRunsOut::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_DeathTimerRunsOut");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48f670: reads the "death timer runs out" defeat
// condition's state from a section (its writer is 0x48fd70).
// FUNCTION: 0x48fdb0
void DefeatDeathTimerRunsOut::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_DeathTimerRunsOut");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}

// FUNCTION: 0x48fdf0
void MissionConditions::SaveConditions(HapiBank* file)
{
    if (g_game->mode->GetGameType() == 1) {
        int i;
        for (i = 0; i < victoryCount; i++) {
            victory[i]->SaveState(file);
        }
        for (i = 0; i < defeatCount; i++) {
            defeat[i]->SaveState(file);
        }
    }
}

// FUNCTION: 0x48fe60
void MissionConditions::LoadConditions(HapiBank* file)
{
    if (g_game->mode->GetGameType() == 1) {
        int i;
        for (i = 0; i < victoryCount; i++) {
            victory[i]->LoadState(file);
        }
        for (i = 0; i < defeatCount; i++) {
            defeat[i]->LoadState(file);
        }
    }
}

// Victory counterpart of 0x48ff40: with no victory condition set, adds the
// default "destroy all units" condition (vtable 0x4fd948), then reports
// whether every victory condition is satisfied.
// FUNCTION: 0x48fed0
int MissionConditions::AllVictoryConditionsMet()
{
    if (victoryCount == 0) {
        victory[victoryCount] = new VictoryDestroyAllUnits;
        victoryCount++;
    }
    for (int i = 0; i < victoryCount; i++) {
        if (!victory[i]->IsSatisfied())
            return 0;
    }
    return 1;
}

// FUNCTION: 0x48ff40
int MissionConditions::AnyDefeatConditionMet()
{
    if (defeatCount == 0) {
        defeat[defeatCount] = new DefeatAllUnitsKilled;
        defeatCount++;
    }
    for (int i = 0; i < defeatCount; i++) {
        if (defeat[i]->IsSatisfied())
            return 1;
    }
    return 0;
}

// Returns 1 when every other player is either allied with the local player
// or has nothing left (the short at player +0x144 is zero).
// FUNCTION: 0x48ffd0
int AreEnemiesEliminated()
{
    Player* p = &g_game->players[g_game->player];
    for (unsigned char i = 0; i < 10; i++) {
        if (i != g_game->player && !p->allied[i] && g_game->players[i].unitCount != 0)
            return 0;
    }
    return 1;
}

// Returns 1 when the local player has nothing left (the short at player
// +0x144 is zero; compare 0x48ffd0).
// FUNCTION: 0x490050
int IsLocalPlayerEliminated()
{
    return g_game->players[g_game->player].unitCount == 0 ? 1 : 0;
}

// FUNCTION: 0x490080
int CheckAlliedVictory()
{
    // declared before the loop counters on purpose: that is what puts `other`
    // in the index slot of the inner load
    Player* mine;
    Player* other;
    unsigned char i;
    int j;
    unsigned char state;

    if (g_game->value_37ef6 == 2) {
        return 0;
    }
    mine = &g_game->players[g_game->player];
    for (i = 0; i < 10; i++) {
        other = &g_game->players[i];
        if (i == g_game->player) {
            continue;
        }
        if (other->active == 0) {
            continue;
        }
        // state is a local because the original tests it twice, the second
        // time still in al, without reloading it.
        state = other->type;
        if (state == 1 || state == 2 || state == 3) {
            if (other->index == 0xa) {
                continue;
            }
            if (other->info->flags_9b & 0x40) {
                continue;
            }
            if (other->unitsCreated == 0) {
                return 0;
            }
            if (state == 1 || state == 2 || state == 3) {
                if (other->unitCount == 0) {
                    continue;
                }
                if (!(other->info->flags_9d & 2)) {
                    return 0;
                }
                if (!(mine->info->flags_9d & 2)) {
                    return 0;
                }
                if (mine->allied[i] == 0) {
                    return 0;
                }
                if (mine->alliedBy[i] == 0) {
                    return 0;
                }
                for (j = 0; j < 10; j++) {
                    Player* o = &g_game->players[j];
                    if (o->active != 0
                        && (o->type == 1 || o->type == 2 || o->type == 3)
                        && o->index != 0xa
                        && (o->unitCount != 0 || o->unitsCreated == 0)) {
                        if (other->allied[j] == 0) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

// Whether the word at +0x144 of the local player's record is zero.
// FUNCTION: 0x490200
int FUN_00490200()
{
    return g_game->players[g_game->player].unitCount == 0 ? 1 : 0;
}

// FUNCTION: 0x490360
int MissionConditions::CheckDefeat()
{
    if (active) {
        if (GetCdPathMismatch()) {
            if (g_cdMismatchDefeatTick == 0) {
                g_cdMismatchDefeatTick = (int)((__int64)rand() * 0x2328 / 0x8000) + 0x2328;
            }
            if (g_cdMismatchDefeatTick <= g_game->ticks) {
                g_cdMismatchDefeatTick = 0;
                return 1;
            }
        }
        // The two mode cases share one body, but writing them out separately
        // is what makes MSVC lower the switch to the dec/je chain.
        switch (g_game->mode->GetGameType()) {
        case 1:
            return AnyDefeatConditionMet();
        case 2:
            return g_game->players[g_game->player].unitCount == 0;
        case 3:
            return g_game->players[g_game->player].unitCount == 0;
        }
    }
    return 0;
}

// FUNCTION: 0x4904b0
void MissionConditions::Deactivate()
{
    active = 0;
}

// FUNCTION: 0x4904c0
void MissionConditions::NotifyUnitDied(Unit* unit)
{
    int i;
    for (i = 0; i < victoryCount; i++)
        victory[i]->OnUnitDied(unit);
    for (i = 0; i < defeatCount; i++)
        defeat[i]->OnUnitDied(unit);
}

// FUNCTION: 0x490520
void MissionConditions::NotifyUnitCaptured(Unit* unit)
{
    int i;
    for (i = 0; i < victoryCount; i++) {
        victory[i]->OnUnitCaptured(unit);
    }
    for (i = 0; i < defeatCount; i++) {
        defeat[i]->OnUnitCaptured(unit);
    }
}

// The conditions object is g_game+0x391ed.
// FUNCTION: 0x490580
void MissionConditions::NotifyUnitCreated(Unit* unit)
{
    int i;
    for (i = 0; i < victoryCount; i++) {
        victory[i]->OnUnitCreated(unit);
    }
    for (i = 0; i < defeatCount; i++) {
        defeat[i]->OnUnitCreated(unit);
    }
}

// Inline and by value: all three differences are computed before the first store.
static inline Vec3 operator-(const Vec3& p, const Vec3& q)
{
    Vec3 r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    return r;
}

#include "../units/unit_def.h"

// The flags at +0x241 read as the original's bitfield; the header keeps the
// plain word.
#pragma pack(push, 1)
struct UnitDefFlags_0048dfb0 {
    char unknown_0[0x241];
    unsigned int unknown_241_0 : 22;
    unsigned int seaUnit : 1;          // bit 22
    unsigned int unknown_241_1 : 9;
};
#pragma pack(pop)

class BitWriter {
public:
    void WriteBits(int value, int bits);
};

class BitReader {
public:
    int ReadBits(int bits);
};

struct Struct_004907e0;

class UnitMotion {
public:
    void SetFlightMode(Struct_004907e0* owner, int state);
};

struct Target_00490880 {
    char unknown_0[0x2e];
    union {
        unsigned char field_2e;        // +0x2e
        struct {
            unsigned char mode : 2;    // +0x2e bits 0-1
        };
    };
};

#pragma pack(push, 2)
struct Struct_004907e0 {               // the owner, the object at +0x8
    union {
        Target_00490880* target;       // +0x0
        UnitMotion* obj;               // +0x0
    };
    char pad4[0x66 - 0x4];
    short field_66;                    // +0x66
    short field_68;                    // +0x68
    Vec3 pos;                 // +0x6a
    char pad76[0x82 - 0x76];
    unsigned char* field_82;           // +0x82
    char pad86[0x92 - 0x86];
    UnitDef* def;                      // +0x92
};
#pragma pack(pop)

class AiSearchGoal;                    // slot 6's result

// The object at +0x4. Only the offsets of the virtuals it is asked for matter.
class Class_0044ced0 {
public:
    virtual ~Class_0044ced0();
    virtual void vf1();
    virtual int GetType();                          // +0x08
    virtual void vf3();
    virtual int ExportGoalPose(Struct_004907e0* param);  // slot 4
    virtual void vf5();
    virtual void vf6();
    virtual void vf7();
    virtual void SerializeNetUnitState(Vec3* param);  // slot 8
    virtual int FUN_0044efd0(short* param);         // slot 9
    virtual void Write(BitWriter* stream);          // +0x28
    virtual int FUN_0044ef50_11();                  // slot 11
    void AddFlags(int param);
};

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
class PathGoal {
public:
    Class_0044ced0* field_4;            // +0x4
    Struct_004907e0* owner;             // +0x8

    PathGoal(Struct_004907e0* p);
    virtual ~PathGoal() {}                          // slot 0
    virtual void SetPathOrder(void* param);         // slot 1
    virtual void TickTowardGoal();                  // slot 2
    virtual void FUN_0044ef40(Vec3*, int, int);  // slot 3
    virtual void ExportGoalPose(Vec3*, Vec3*, short*);  // slot 4
    virtual int FUN_0044ef80();                     // slot 5
    virtual AiSearchGoal* TryClaimRepath();         // slot 6
    virtual int HasNetUnitState();                  // slot 7
    virtual void SerializeNetUnitState(BitWriter*);  // slot 8
    virtual void FUN_0044efd0(BitReader*);          // slot 9
    virtual void DrawOnSurface(void*);              // slot 10
};

// PackedGoal: a moving object that follows its owner, derived from
// PathGoal (see order_targets_44ef20.cpp for the family) and the base of
// LiteGoal and PackedPosGoal.
// Vtable 0x4fd980, constructor 0x4905e0, ??_G 0x490630.
class PackedGoal : public PathGoal {
public:
    Vec3 pos;                  // +0xc
    Vec3 vel;                  // +0x18
    short field_24;                     // +0x24
    char field_26;                      // +0x26
    union {
        struct {
            unsigned char dirty : 1;    // +0x27 bit 0
            unsigned char mode : 2;     // +0x27 bits 1-2
        };
        struct {
            unsigned char state : 3;    // +0x27, both
        };
        unsigned char field_27;
    };

    PackedGoal(Struct_004907e0* p);
    virtual void TickTowardGoal();                  // slot 2, 0x490690
    virtual void ExportGoalPose(Vec3*, Vec3*, short*);  // slot 4, 0x490650
};

unsigned short __stdcall GetHeadingBetween(Vec3* from, Vec3* to);

// The constructor (0x4905e0) and, emitted with the vtable it stores, the
// compiler-generated scalar deleting destructor: the destructor is trivial,
// so only the inlined base destructor's store of 0x4fd428 is left. 0x4907e0
// inlines the same constructor.
// FUNCTION: 0x4905e0
// FUNCTION: 0x490630 ??_GPackedGoal@@UAEPAXI@Z
PackedGoal::PackedGoal(Struct_004907e0* p)
    : PathGoal(p)
{
    pos = p->pos;
    vel = Vec3(0, 0, 0);
    field_24 = p->field_66;
}

// PackedGoal's override of slot 4 (vtable 0x4fd980, inherited by
// LiteGoal and PackedPosGoal; the class family is listed in
// order_targets_44ef20.cpp): copies out the position, the velocity and field_24
// (the heading 0x490690 turns towards the owner).
// FUNCTION: 0x490650
void PackedGoal::ExportGoalPose(Vec3* outPos, Vec3* outVel,
                                  short* outHeading)
{
    *outPos = pos;
    *outVel = vel;
    *outHeading = field_24;
}

// PackedGoal's override of slot 2 (vtable 0x4fd980, inherited by
// PackedPosGoal and called directly by LiteGoal's own override,
// 0x490880; the class family is listed in order_targets_44ef20.cpp): one update
// step of a moving object. The object at +0x4 moves this object's position (its
// slot 8) and the difference goes into vel. When the object has drifted further
// than 0xa00000 from its owner, its height is snapped to the ground under it,
// the sea level for a unit whose def has the flag at +0x241 bit 22 set, the
// owner's field_82 byte 1 otherwise, plus the def's altitude, and past
// 0x1400000 (or 0x100000 with slot 9 refusing) it turns to face the owner. Then
// the object at +0x4 gets the last word: slot 4 saying it is done, plus slot
// 11, means slot 1 with 0.
// FUNCTION: 0x490690
void PackedGoal::TickTowardGoal()
{
    if (!field_4)
        return;
    Vec3 old = pos;
    field_4->SerializeNetUnitState(&pos);
    vel = pos - old;
    int dist = (int)_hypot(owner->pos.x - pos.x, owner->pos.z - pos.z);
    if (dist > 0xa00000) {
        if (((UnitDefFlags_0048dfb0*)owner->def)->seaUnit)
            pos.y = (g_game->seaLevel + owner->def->altitude) << 16;
        else
            // altitude via owner->def-> and first in the add: keeps the two
            // branches from being tail-merged.
            pos.y = (owner->def->altitude + owner->field_82[1]) << 16;
    }
    if (dist > 0x1400000 || (!field_4->FUN_0044efd0(&field_24) && dist > 0x100000))
        field_24 = (short)GetHeadingBetween(&owner->pos, &pos);
    if (field_4->ExportGoalPose(owner)) {
        field_4->AddFlags(0x20);
        if (!field_4->FUN_0044ef50_11())
            SetPathOrder(0);
    }
}

// LiteGoal (vtable 0x4fd9b0), derived from PackedGoal and
// PathGoal (see order_targets_44ef20.cpp for the family): the moving
// object that also sends its state, with a dirty bit and the owner's mode.
// Vtable 0x4fd9b0, constructor 0x4907e0, ??_G 0x490840.
class LiteGoal : public PackedGoal {
public:
    LiteGoal(Struct_004907e0* p);
    virtual void SetPathOrder(void* param);         // slot 1, 0x490860
    virtual void TickTowardGoal();                  // slot 2, 0x490880
    virtual int HasNetUnitState();                  // slot 7, 0x4908b0
    virtual void SerializeNetUnitState(BitWriter*);  // slot 8, 0x4908c0
};

// The constructor: the base constructor (0x44ef20) is out of line, the middle
// class's (out-of-line copy at 0x4905e0) is inlined. The middle class assigns
// its members in the body. Its vtable reference makes the
// compiler emit the scalar deleting destructor here too; both derived
// destructors are trivial, so only the inlined base destructor's store of
// 0x4fd428 is left in it.
// FUNCTION: 0x4907e0
// FUNCTION: 0x490840 ??_GLiteGoal@@UAEPAXI@Z
LiteGoal::LiteGoal(Struct_004907e0* p)
    : PackedGoal(p)
{
    dirty = 1;
    mode = 0;
}

// Slot 1: the base's (0x44ef90) and then the dirty bit.
// FUNCTION: 0x490860
void LiteGoal::SetPathOrder(void* param)
{
    PathGoal::SetPathOrder(param);
    dirty = 1;
}

// Slot 2: sets the dirty bit when the mode differs from the owner's, then runs
// the middle class's own slot 2 (0x490690).
// FUNCTION: 0x490880
void LiteGoal::TickTowardGoal()
{
    if ((owner->target->field_2e & 3) != mode)
        dirty = 1;
    PackedGoal::TickTowardGoal();
}

// Slot 7: the dirty bit.
// FUNCTION: 0x4908b0
int LiteGoal::HasNetUnitState()
{
    return field_27 & 1;
}

// Slot 8: writes the object at +0x4 (a 2-bit kind, then its own data) and the
// owner's mode to the stream, then takes that mode as its own and clears the
// dirty bit. The matching reader looks like 0x490a10 (PackedPosGoal's slot 9).
// FUNCTION: 0x4908c0
void LiteGoal::SerializeNetUnitState(BitWriter* stream)
{
    if (field_4 == 0) {
        stream->WriteBits(0, 2);
    } else if (field_4->GetType() == 2) {
        stream->WriteBits(1, 2);
        field_4->Write(stream);
    } else if (field_4->GetType() == 3) {
        stream->WriteBits(2, 2);
        field_4->Write(stream);
    }
    stream->WriteBits(owner->target->mode, 2);
    state = owner->target->mode << 1;     // clears the dirty bit too
}

#pragma pack(push, 2)
class Class_0044e080 : public Class_0044ced0 {
public:
    char unknown_4[0x36 - 0x4];
    Class_0044e080(Struct_004907e0* owner, BitReader* reader);
};

struct Owner_0044e9c0;

class AirManeuverOrder : public Class_0044ced0 {
public:
    char unknown_4[0x2c - 0x4];
    AirManeuverOrder(Owner_0044e9c0* owner, BitReader* reader);
};
#pragma pack(pop)

// PackedPosGoal (vtable 0x4fd9e0), derived from PackedGoal and
// PathGoal (see order_targets_44ef20.cpp for the family): the moving
// object that reads its state from the stream.
// Vtable 0x4fd9e0, constructor 0x490940, destructor 0x4909e0, ??_G 0x4909a0.
// Slots 2 and 4 are inherited from PackedGoal.
class PackedPosGoal : public PackedGoal {
public:
    PackedPosGoal(Struct_004907e0* p);
    virtual ~PackedPosGoal();                       // slot 0
    virtual void FUN_0044efd0(BitReader*);          // slot 9, 0x490a10
};

// The constructor: the base constructor (0x44ef20) is out of line, the middle
// class's constructor is inlined, as in LiteGoal's.
// FUNCTION: 0x490940
PackedPosGoal::PackedPosGoal(Struct_004907e0* p)
    : PackedGoal(p)
{
}

// The destructor frees the object at +0x4 through its virtual destructor. Its
// scalar deleting destructor inlines the same body; in both, the middle
// class's vtable store is dead, and the empty inline base destructor leaves
// only the base vtable store of 0x4fd428.
// FUNCTION: 0x4909a0 ??_GPackedPosGoal@@UAEPAXI@Z
// FUNCTION: 0x4909e0
PackedPosGoal::~PackedPosGoal()
{
    delete field_4;
    field_4 = 0;
}

// Slot 9: rebuilds the object at +0x4 from the bit stream (a 2-bit kind: 1 and
// 2 pick its class, anything else leaves none), then passes a 2-bit state read
// after it to the owner.
// FUNCTION: 0x490a10
void PackedPosGoal::FUN_0044efd0(BitReader* reader)
{
    if (field_4) {
        delete field_4;
        field_4 = 0;
    }
    int kind = reader->ReadBits(2);
    if (kind == 1)
        field_4 = new Class_0044e080(owner, reader);
    else if (kind == 2)
        field_4 = new AirManeuverOrder((Owner_0044e9c0*)owner, reader);
    int state = reader->ReadBits(2);
    owner->obj->SetFlightMode(owner, state);
}

// FUNCTION: 0x490aa0
void QueryGlobalMemoryStatus(void)
{
    MEMORYSTATUS mem;
    mem.dwLength = 0x20;
    GlobalMemoryStatus(&mem);
}
