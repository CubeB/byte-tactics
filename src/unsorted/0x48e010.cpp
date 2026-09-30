// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Class_0048ff40::FUN_0048e010, the victory/defeat condition registration
// function (counterpart of 0x48ff40). All ~18 registration blocks plus the two
// "if none registered" defaults are here, written from the disassembly and the
// class declarations of the sibling condition files (0x48eeb0, 0x48efb0,
// 0x48f0f0, 0x48f250, 0x48f3e0, 0x48f530, 0x48f610, 0x48f6b0, 0x48f7e0,
// 0x48f8c0, 0x48f9d0, 0x48fb60, 0x48fc70, 0x48fd50).
// MATCH.
//
// Three things were needed on top of the 98.2% version, and each is a
// declaration-shape fix rather than a code-shape fix, so none of them moves a
// single byte of the function:
// 1. Every reader call's result goes into a named int before the `!= 0` test.
//    The original writes `cmp eax, ebx` at all 16 sites, where ebx is the
//    live zero that is also the `push ebx` argument, and a bare
//    `reader->FUN_...(...) != 0` folds to `test eax, eax`. Assigning the
//    result to a local first is what stops the fold (docs/agent-guide.md,
//    "cmp reg, reg against a zero register instead of test reg, reg").
// 2. The two one-virtual listener bases are separate types. The nine
//    conditions that listen take Listener_0048ff40 (vtable 0x4fd940), while
//    Class_0048f250's base subobject is initialised from 0x4fd8a8, a second
//    vtable with the same single _purecall slot. Declaring both bases as the
//    same class made the reference at +0x3b7 resolve to 0x4fd940.
// 3. Every derived class's virtual is an OVERRIDE of a base slot, never a new
//    appended slot: the base is `Listener_0048ff40::FUN_0048f790` (a pure
//    virtual, so slot 0 holds _purecall in the original), and
//    Class_0048eb40, Class_0048ec20 and Class_0048f6b0 override the
//    condition base's existing slots 0 and 1 rather than appending a 7th.
//    Appending ran the vtable into the next class's, which check.py catches.
//
// Two other things that mattered for the bytes: the condition classes whose
// size is not a multiple of 4 (BuildUnitType 0x32, KillAllOfType 0x36) need
// `#pragma pack(2)` or `operator new` asks for 0x34/0x38; and
// Class_0048f250's constructor must store pos.z and radius before
// pos.y = 0x12345678.

#include <string.h>
#include <stdio.h>

extern void* DAT_005119b8;

// Mission victory/defeat condition (6 virtual slots).
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
    virtual void FUN_0048f790(void* event) = 0;
};

// The same one-slot interface again, as its own type (vtable 0x4fd8a8).
class Listener_0048f250 {
public:
    virtual void FUN_0048f790(void* event) = 0;
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

struct Vec3_0048f250 {
    int x;                               // +0x0
    int y;                               // +0x4
    int z;                               // +0x8
};

// KillEnemyCommander (vtable 0x4fd960).
class Class_0048ea00 : public Condition_0048ff40 {
public:
    virtual int FUN_0048ea00();
};

// DestroyAllUnits (vtable 0x4fd948).
class Class_0048eb40 : public Condition_0048ff40 {
public:
    virtual int FUN_0048ea00();
};

// KillAllMobileUnits (vtable 0x4fd928, listener vtable 0x4fd920).
class Class_0048ec20 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event);
    int count;                           // +0x10
    Class_0048ec20() { count = 0; }
    virtual void FUN_0048ea10();
};

// BuildUnitType (vtable 0x4fd908, listener vtable 0x4fd900).
#pragma pack(push, 2)
class Class_0048edb0 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event);
    char name[0x20];                     // +0x10
    short field_30;                      // +0x30

    Class_0048edb0(const char* text)
    {
        strcpy(name, text);
        field_30 = 0;
    }
};
#pragma pack(pop)

// CaptureUnitType (vtable 0x4fd8e8).
class Class_0048eeb0 : public Condition_0048ff40 {
public:
    char name[0x20];                     // +0xc

    Class_0048eeb0(const char* text) { strcpy(name, text); }
};

// KillAllOfType (vtable 0x4fd8d0, listener vtable 0x4fd8c8).
#pragma pack(push, 2)
class Class_0048efb0 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event);
    char name[0x26];                     // +0x10

    Class_0048efb0(const char* text) { strcpy(name, text); }
};
#pragma pack(pop)

// KillUnitType (vtable 0x4fd8b0).
class Class_0048f0f0 : public Condition_0048ff40 {
public:
    char name[0x20];                     // +0xc
    int count;                           // +0x2c

    Class_0048f0f0(const char* text, int n)
    {
        strcpy(name, text);
        count = n;
    }
};

// MoveUnitToRadius (vtable 0x4fd890, listener vtable 0x4fd888).
#pragma pack(push, 2)
class Class_0048f250 : public Condition_0048ff40, public Listener_0048f250 {
public:
    virtual void FUN_0048f790(void* event);
    char name[0x20];                     // +0x10
    Vec3_0048f250 pos;                   // +0x30
    int radius;                          // +0x3c

    Class_0048f250(const char* text, int x, int z, int r)
    {
        if (_strcmpi(text, "ANYTYPE") == 0)
            name[0] = 0;
        else
            strcpy(name, text);
        pos.x = x;
        pos.z = z;
        radius = r << 16;
        pos.y = 0x12345678;
    }
};
#pragma pack(pop)

// UnitTypePassesX (vtable 0x4fd870, listener vtable 0x4fd868).
#pragma pack(push, 2)
class Class_0048f3e0 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event);
    char name[0x20];                     // +0x10
    int field_30;                        // +0x30

    Class_0048f3e0(const char* text, int v)
    {
        if (_strcmpi(text, "ANYTYPE") == 0)
            name[0] = 0;
        else
            strcpy(name, text);
        field_30 = v >> 4;
    }
};
#pragma pack(pop)

// UnitTypePassesZ (vtable 0x4fd850, listener vtable 0x4fd848).
#pragma pack(push, 2)
class Class_0048f530 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event);
    char name[0x20];                     // +0x10
    int field_30;                        // +0x30

    Class_0048f530(const char* text, int v)
    {
        if (_strcmpi(text, "ANYTYPE") == 0)
            name[0] = 0;
        else
            strcpy(name, text);
        field_30 = v >> 4;
    }
};
#pragma pack(pop)

// VictoryTimerRunsOut (vtable 0x4fd830).
class Class_0048f610 : public Condition_0048ff40 {
public:
    int field_c;                         // +0xc

    Class_0048f610(int t) { field_c = t * 30; }
};

// CommanderKilled (vtable 0x4fd818).
class Class_0048f6b0 : public Condition_0048ff40 {
public:
    virtual void FUN_0048ea10();
};

// AllUnitsKilled (vtable 0x4fd800, listener vtable 0x4fd7f8).
class Class_0048f840 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual int FUN_0048f7e0();
    virtual void FUN_0048f840(void* file);
    virtual void FUN_0048f880(void* file);
    virtual void FUN_0048f790(void* event);
};

// AllUnitsKilledOfType (vtable 0x4fd7e0, listener vtable 0x4fd7d8).
#pragma pack(push, 2)
class Class_0048f9d0 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event);
    char name[0x20];                     // +0x10
    short id;                            // +0x30
    int count;                           // +0x32

    Class_0048f9d0(const char* text) { strcpy(name, text); }
};
#pragma pack(pop)

// UnitTypeKilled (vtable 0x4fd7c0).
class Class_0048f8c0 : public Condition_0048ff40 {
public:
    char name[0x20];                     // +0xc
    int numLeftToKill;                   // +0x2c

    Class_0048f8c0(const char* text, int n)
    {
        strcpy(name, text);
        numLeftToKill = n;
    }
};

// DeathTimerRunsOut (vtable 0x4fd7a8).
class Class_0048fd50 : public Condition_0048ff40 {
public:
    int field_c;                         // +0xc

    Class_0048fd50(int t) { field_c = t * 30; }
};

// AnyUnitPassesX (vtable 0x4fd790, listener vtable 0x4fd788).
class Class_0048fb60 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event);
    int field_10;                        // +0x10

    Class_0048fb60(int v) { field_10 = v >> 4; }
};

// AnyUnitPassesZ (vtable 0x4fd770, listener vtable 0x4fd768).
class Class_0048fc70 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event);
    int field_10;                        // +0x10

    Class_0048fc70(int v) { field_10 = v >> 4; }
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
    char stype[0x100];

    int r1 = ((Class_004c46c0*)p->reader)->FUN_004c46c0("KillEnemyCommander", 0);
    if (r1 != 0) {
        victory[victoryCount] = new Class_0048ea00;
        victoryCount++;
    }
    int r2 = ((Class_004c46c0*)p->reader)->FUN_004c46c0("DestroyAllUnits", 0);
    if (r2 != 0) {
        victory[victoryCount] = new Class_0048eb40;
        victoryCount++;
    }
    int r3 = ((Class_004c46c0*)p->reader)->FUN_004c46c0("KillAllMobileUnits", 0);
    if (r3 != 0) {
        victory[victoryCount] = new Class_0048ec20;
        victoryCount++;
    }
    int r4 = ((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "BuildUnitType", 0x100, &DAT_005119b8);
    if (r4 != 0) {
        victory[victoryCount] = new Class_0048edb0(buf);
        victoryCount++;
    }
    int r5 = ((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "CaptureUnitType", 0x100, &DAT_005119b8);
    if (r5 != 0) {
        victory[victoryCount] = new Class_0048eeb0(buf);
        victoryCount++;
    }
    int r6 = ((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "KillAllOfType", 0x100, &DAT_005119b8);
    if (r6 != 0) {
        victory[victoryCount] = new Class_0048efb0(buf);
        victoryCount++;
    }
    int r7 = ((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "KillUnitType", 0x100, &DAT_005119b8);
    if (r7 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        victory[victoryCount] = new Class_0048f0f0(stype, n);
        victoryCount++;
    }
    int r8 = ((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "MoveUnitToRadius", 0x100, &DAT_005119b8);
    if (r8 != 0) {
        int a, b, c;
        sscanf(buf, "%[a-zA-Z],%i,%i,%i", stype, &a, &b, &c);
        victory[victoryCount] = new Class_0048f250(stype, a, b, c);
        victoryCount++;
    }
    int r9 = ((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "UnitTypePassesX", 0x100, &DAT_005119b8);
    if (r9 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        victory[victoryCount] = new Class_0048f3e0(stype, n);
        victoryCount++;
    }
    int r10 = ((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "UnitTypePassesZ", 0x100, &DAT_005119b8);
    if (r10 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        victory[victoryCount] = new Class_0048f530(stype, n);
        victoryCount++;
    }
    {
        int t = ((Class_004c46c0*)p->reader)->FUN_004c46c0("VictoryTimerRunsOut", 0);
        if (t > 0) {
            victory[victoryCount] = new Class_0048f610(t);
            victoryCount++;
        }
    }
    int r11 = ((Class_004c46c0*)p->reader)->FUN_004c46c0("CommanderKilled", 0);
    if (r11 != 0) {
        defeat[defeatCount] = new Class_0048f6b0;
        defeatCount++;
    }
    int r12 = ((Class_004c46c0*)p->reader)->FUN_004c46c0("AllUnitsKilled", 0);
    if (r12 != 0) {
        defeat[defeatCount] = new Class_0048f840;
        defeatCount++;
    }
    int r13 = ((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "AllUnitsKilledOfType", 0x100, &DAT_005119b8);
    if (r13 != 0) {
        defeat[defeatCount] = new Class_0048f9d0(buf);
        defeatCount++;
    }
    int r14 = ((Class_004c48c0*)p->reader)->FUN_004c48c0(buf, "UnitTypeKilled", 0x100, &DAT_005119b8);
    if (r14 != 0) {
        int n;
        sscanf(buf, "%[a-zA-Z],%i", stype, &n);
        defeat[defeatCount] = new Class_0048f8c0(stype, n);
        defeatCount++;
    }
    {
        int t = ((Class_004c46c0*)p->reader)->FUN_004c46c0("DeathTimerRunsOut", 0);
        if (t > 0) {
            defeat[defeatCount] = new Class_0048fd50(t);
            defeatCount++;
        }
    }
    {
        int t = ((Class_004c46c0*)p->reader)->FUN_004c46c0("AnyUnitPassesX", -1);
        if (t >= 0) {
            defeat[defeatCount] = new Class_0048fb60(t);
            defeatCount++;
        }
    }
    {
        int t = ((Class_004c46c0*)p->reader)->FUN_004c46c0("AnyUnitPassesZ", -1);
        if (t >= 0) {
            defeat[defeatCount] = new Class_0048fc70(t);
            defeatCount++;
        }
    }
    if (victoryCount == 0) {
        victory[victoryCount] = new Class_0048eb40;
        victoryCount++;
    }
    if (defeatCount == 0) {
        defeat[defeatCount] = new Class_0048f840;
        defeatCount++;
    }
}
