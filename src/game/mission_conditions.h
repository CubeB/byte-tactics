// MissionConditions: the mission's victory and defeat condition lists, 0x8c
// bytes at g_game+0x391ed: the two arrays of condition pointers with their
// counts and the active flag the constructor sets. The one declaration of the
// class, for every file that reads the lists or calls its methods; the
// condition, reader, bank and unit types behind the pointers stay private to
// their own files.
#ifndef MISSION_CONDITIONS_H
#define MISSION_CONDITIONS_H

class HapiBank;
class MissionCondition;
struct Param_0048e010;
struct Unit;

class MissionConditions {
public:
    MissionCondition* victory[16];       // +0x00
    int victoryCount;                    // +0x40
    MissionCondition* defeat[16];        // +0x44
    int defeatCount;                     // +0x84
    int active;                          // +0x88

    MissionConditions();
    // Written here so that /Ob2 inlines it into the deleting caller.
    ~MissionConditions() { FreeConditions(); }
    void FreeConditions();
    void RegisterConditions(Param_0048e010* p);
    void SaveConditions(HapiBank* file);
    void LoadConditions(HapiBank* file);
    int AllVictoryConditionsMet();
    int AnyDefeatConditionMet();
    // 0x490230, defined in victory_490230.cpp: it matches only in a file of its
    // own, where few enough symbols come before it to keep its SIB operand order.
    int CheckVictory();
    int CheckDefeat();
    void Deactivate();
    void NotifyUnitDied(Unit* unit);
    void NotifyUnitCaptured(Unit* unit);
    void NotifyUnitCreated(Unit* unit);
};

#endif
