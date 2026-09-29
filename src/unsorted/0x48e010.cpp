// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 14.5% (365 of 2542 bytes). This is
// Class_0048ff40::FUN_0048e010, the victory-condition registration function
// (counterpart of 0x48ff40, which registers the single defeat condition).
// Only the first four registration blocks are written. The original is 2542
// bytes and continues with ~15 more blocks: MoveUnitToRadius,
// UnitTypePassesX, UnitTypePassesZ, VictoryTimerRunsOut, CommanderKilled,
// AllUnitsKilled, AllUnitsKilledOfType, UnitTypeKilled, DeathTimerRunsOut,
// AnyUnitPassesX, AnyUnitPassesZ, ... (string keys in the disassembly order).
// What still differs:
//  - frame is 0x104, original 0x21c: the missing blocks own the two 0x100
//    string buffers and the sscanf int locals that make up the rest.
//  - register roles: original keeps this in ebp and zero in ebx
//    (`mov ebp,ecx; xor ebx,ebx`); mine keeps this in ebx and zero in ebp, so
//    the call results compare as `test eax,eax` instead of `cmp eax,ebx`.
//    Adding the remaining blocks should move this and fix the roles.
//  - original spills this to [esp+0x24] at entry; mine does not (needs locals).
// The per-block structure is right and the linker-filled symbol list matches
// the original for slots 0x01a..0x126 exactly.

#include <string.h>

extern void* DAT_005119b8;

// Mission victory/defeat condition (6 virtual slots). Copied from 0x48ff40.cpp.
class Condition_0048ff40 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    Condition_0048ff40() { satisfied = celebrated = 0; }
    virtual int FUN_0048ea00();          // IsSatisfied
    virtual void FUN_0048ea10();         // Slot1
    virtual void FUN_0048ea20();         // Slot2
    virtual void FUN_0048ea30();         // Slot3
    virtual void FUN_0048f840(void* file);   // Save
    virtual void FUN_0048f880(void* file);   // Load
};

// Secondary interface of a condition that watches events (vtable 0x4fd940).
class Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* buf, const char* name, int size, void* def);
};

// The registration parameter: the command reader lives at +0x4.
struct Param_0048e010 {
    char unknown_0[4];
    void* reader;                        // +0x4
};

// KillEnemyCommander (vtable 0x4fd960).
class Class_0048ea00 : public Condition_0048ff40 {
public:
    virtual int FUN_0048ea00();
};

// DestroyAllUnits (vtable 0x4fd948).
class Class_0048eb40 : public Condition_0048ff40 {
public:
    virtual int FUN_0048eb40();
};

// KillAllMobileUnits (vtable 0x4fd928, listener vtable 0x4fd920).
class Class_0048ec20 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    int count;                           // +0x10
    virtual void FUN_0048ec20();
};

// BuildUnitType (vtable 0x4fd908, listener vtable 0x4fd900).
class Class_0048edb0 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    char name[0x20];                     // +0x10
    short field_30;                      // +0x30

    Class_0048edb0(const char* text)
    {
        strcpy(name, text);
        field_30 = 0;
    }
};

class Class_0048ff40 {
public:
    Condition_0048ff40* victory[16];     // +0x00
    int victoryCount;                    // +0x40
    Condition_0048ff40* defeat[16];      // +0x44
    int defeatCount;                     // +0x84

    void FUN_0048e010(Param_0048e010* p);
};

// FUNCTION: 0x48e010
void Class_0048ff40::FUN_0048e010(Param_0048e010* p)
{
    char buf[0x100];

    if (((Class_004c46c0*)p->reader)->FUN_004c46c0("KillEnemyCommander", 0)) {
        victory[victoryCount] = new Class_0048ea00;
        victoryCount++;
    }
    if (((Class_004c46c0*)p->reader)->FUN_004c46c0("DestroyAllUnits", 0)) {
        victory[victoryCount] = new Class_0048eb40;
        victoryCount++;
    }
    if (((Class_004c46c0*)p->reader)->FUN_004c46c0("KillAllMobileUnits", 0)) {
        victory[victoryCount] = new Class_0048ec20;
        victoryCount++;
    }
    if (((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "BuildUnitType", 0x100, &DAT_005119b8)) {
        victory[victoryCount] = new Class_0048edb0(buf);
        victoryCount++;
    }
}
