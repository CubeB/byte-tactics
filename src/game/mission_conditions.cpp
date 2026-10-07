// Decompiled by deepseek-v4.1-flash, space-bunny-free, GPT-6, Opus and Haiku. Names are provisional.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

class TdfRecord {
public:
    int GetFieldString(char* buf, const char* name, int size, void* def);
    int GetFieldInt(const char* name, int def);
};

// The registration parameter: the command reader lives at +0x4.
struct Param_0048e010 {
    char unknown_0[4];
    void* reader;                        // +0x4
};

struct Vec3_0048f250 {
    int x;                               // +0x0
    int y;                               // +0x4
    int z;                               // +0x8
};

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
    int count;                           // +0x10
    VictoryKillAllMobileUnits() { count = 0; }
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
    short field_30;                      // +0x30

    VictoryBuildUnitType(const char* text)
    {
        strcpy(name, text);
        field_30 = 0;
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
    char name[0x26];                     // +0x10

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
    int count;                           // +0x2c

    VictoryKillUnitType(const char* text, int n)
    {
        strcpy(name, text);
        count = n;
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
    Vec3_0048f250 pos;                   // +0x30
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
    int field_c;                         // +0xc

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

class Mission {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Player {                          // 0x14b bytes
    char unknown_0[0x108];
    unsigned char allied[10];            // +0x108, one entry per other team
    char unknown_112[0x144 - 0x112];
    short count;                         // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                  // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;                // +0x2a42
    char unknown_2a43[0x38a47 - 0x2a43];
    unsigned int ticks;                  // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* mode;                       // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

extern int GetCdPathMismatch();

extern unsigned int DAT_0051e6c4;

class MissionConditions {
public:
    MissionCondition* victory[16];       // +0x00
    int victoryCount;                    // +0x40
    MissionCondition* defeat[16];        // +0x44
    int defeatCount;                     // +0x84
    int active;                          // +0x88, set to 1 by Class_0048df90

    void RegisterConditions(Param_0048e010* p);
    void SaveConditions(HapiBank* file);
    void LoadConditions(HapiBank* file);
    int AllVictoryConditionsMet();
    int AnyDefeatConditionMet();
    // 0x490230, in victory_490230.cpp: it matches only in a file of its own,
    // where few enough symbols come before it to keep its SIB operand order.
    int CheckVictory();
    int CheckDefeat();
    void FUN_004904b0();
    void NotifyUnitDied(Unit* unit);
    void NotifyUnitCaptured(Unit* unit);
    void NotifyUnitCreated(Unit* unit);
};

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
        victory[victoryCount] = new VictoryKillEnemyCommander;
        victoryCount++;
    }
    int r2 = ((TdfRecord*)p->reader)->GetFieldInt("DestroyAllUnits", 0);
    if (r2 != 0) {
        victory[victoryCount] = new VictoryDestroyAllUnits;
        victoryCount++;
    }
    int r3 = ((TdfRecord*)p->reader)->GetFieldInt("KillAllMobileUnits", 0);
    if (r3 != 0) {
        victory[victoryCount] = new VictoryKillAllMobileUnits;
        victoryCount++;
    }
    int r4 = ((TdfRecord*)p->reader)->GetFieldString(buf, "BuildUnitType", 0x100, &DAT_005119b8);
    if (r4 != 0) {
        victory[victoryCount] = new VictoryBuildUnitType(buf);
        victoryCount++;
    }
    int r5 = ((TdfRecord*)p->reader)->GetFieldString(buf, "CaptureUnitType", 0x100, &DAT_005119b8);
    if (r5 != 0) {
        victory[victoryCount] = new VictoryCaptureUnitType(buf);
        victoryCount++;
    }
    int r6 = ((TdfRecord*)p->reader)->GetFieldString(buf, "KillAllOfType", 0x100, &DAT_005119b8);
    if (r6 != 0) {
        victory[victoryCount] = new VictoryKillAllOfType(buf);
        victoryCount++;
    }
    int r7 = ((TdfRecord*)p->reader)->GetFieldString(buf, "KillUnitType", 0x100, &DAT_005119b8);
    if (r7 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        victory[victoryCount] = new VictoryKillUnitType(stype, n);
        victoryCount++;
    }
    int r8 = ((TdfRecord*)p->reader)->GetFieldString(buf, "MoveUnitToRadius", 0x100, &DAT_005119b8);
    if (r8 != 0) {
        int a, b, c;
        sscanf(buf, "%[a-zA-Z],%i,%i,%i", stype, &a, &b, &c);
        victory[victoryCount] = new VictoryMoveUnitToRadius(stype, a, b, c);
        victoryCount++;
    }
    int r9 = ((TdfRecord*)p->reader)->GetFieldString(buf, "UnitTypePassesX", 0x100, &DAT_005119b8);
    if (r9 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        victory[victoryCount] = new VictoryUnitTypePassesX(stype, n);
        victoryCount++;
    }
    int r10 = ((TdfRecord*)p->reader)->GetFieldString(buf, "UnitTypePassesZ", 0x100, &DAT_005119b8);
    if (r10 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        victory[victoryCount] = new VictoryUnitTypePassesZ(stype, n);
        victoryCount++;
    }
    {
        int t = ((TdfRecord*)p->reader)->GetFieldInt("VictoryTimerRunsOut", 0);
        if (t > 0) {
            victory[victoryCount] = new VictoryTimerRunsOut(t);
            victoryCount++;
        }
    }
    int r11 = ((TdfRecord*)p->reader)->GetFieldInt("CommanderKilled", 0);
    if (r11 != 0) {
        defeat[defeatCount] = new DefeatCommanderKilled;
        defeatCount++;
    }
    int r12 = ((TdfRecord*)p->reader)->GetFieldInt("AllUnitsKilled", 0);
    if (r12 != 0) {
        defeat[defeatCount] = new DefeatAllUnitsKilled;
        defeatCount++;
    }
    int r13 = ((TdfRecord*)p->reader)->GetFieldString(buf, "AllUnitsKilledOfType", 0x100, &DAT_005119b8);
    if (r13 != 0) {
        defeat[defeatCount] = new DefeatAllUnitsKilledOfType(buf);
        defeatCount++;
    }
    int r14 = ((TdfRecord*)p->reader)->GetFieldString(buf, "UnitTypeKilled", 0x100, &DAT_005119b8);
    if (r14 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        defeat[defeatCount] = new DefeatUnitTypeKilled(stype, n);
        defeatCount++;
    }
    {
        int t = ((TdfRecord*)p->reader)->GetFieldInt("DeathTimerRunsOut", 0);
        if (t > 0) {
            defeat[defeatCount] = new DefeatDeathTimerRunsOut(t);
            defeatCount++;
        }
    }
    {
        int t = ((TdfRecord*)p->reader)->GetFieldInt("AnyUnitPassesX", -1);
        if (t >= 0) {
            defeat[defeatCount] = new DefeatAnyUnitPassesX(t);
            defeatCount++;
        }
    }
    {
        int t = ((TdfRecord*)p->reader)->GetFieldInt("AnyUnitPassesZ", -1);
        if (t >= 0) {
            defeat[defeatCount] = new DefeatAnyUnitPassesZ(t);
            defeatCount++;
        }
    }
    if (victoryCount == 0) {
        victory[victoryCount] = new VictoryDestroyAllUnits;
        victoryCount++;
    }
    if (defeatCount == 0) {
        defeat[defeatCount] = new DefeatAllUnitsKilled;
        defeatCount++;
    }
}

// FUNCTION: 0x48fdf0
void MissionConditions::SaveConditions(HapiBank* file)
{
    if (g_game->mode->FUN_00435100() == 1) {
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
    if (g_game->mode->FUN_00435100() == 1) {
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

// FUNCTION: 0x490360
int MissionConditions::CheckDefeat()
{
    if (active) {
        if (GetCdPathMismatch()) {
            if (DAT_0051e6c4 == 0) {
                DAT_0051e6c4 = (int)((__int64)rand() * 0x2328 / 0x8000) + 0x2328;
            }
            if (DAT_0051e6c4 <= g_game->ticks) {
                DAT_0051e6c4 = 0;
                return 1;
            }
        }
        // The two mode cases share one body, but writing them out separately
        // is what makes MSVC lower the switch to the dec/je chain.
        switch (g_game->mode->FUN_00435100()) {
        case 1:
            return AnyDefeatConditionMet();
        case 2:
            return g_game->players[g_game->player].count == 0;
        case 3:
            return g_game->players[g_game->player].count == 0;
        }
    }
    return 0;
}

// FUNCTION: 0x4904b0
void MissionConditions::FUN_004904b0()
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
