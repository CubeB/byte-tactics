// Decompiled by Opus. Names are provisional.
// Clears target entry `index` (the same reset as FUN_0048a160) unless it is
// already clear, then tells the unit's script "StartBuilding" and
// "TargetCleared".
class Class_004b07c0 {
public:
    int FUN_004b07c0(char* name);
};

class Class_004b0a70 {
public:
    int FUN_004b0a70(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

struct Point_0048a0f0 {
    short a;                           // +0x0
    short b;                           // +0x2
};

struct Entry_0048a0f0 {
    Point_0048a0f0 point;              // +0x0
    char unknown_4[0x1c - 4];
};

#pragma pack(push, 1)
struct Unit_0048a0f0 {
    int unknown_0;
    Entry_0048a0f0 entries[5];         // +0x4
    char unknown_90[0x9a - 0x90];
    Class_004b07c0* script;            // +0x9a
};
#pragma pack(pop)

// FUNCTION: 0x48a0f0
void __stdcall FUN_0048a0f0(Unit_0048a0f0* unit, int index)
{
    Point_0048a0f0* p = &unit->entries[index].point;
    if (p->a != 0 || p->b != (short)0x8000) {
        p->a = 0;
        p->b = (short)0x8000;
        unit->script->FUN_004b07c0("StartBuilding");
        ((Class_004b0a70*)unit->script)->FUN_004b0a70("TargetCleared", 0, 0, 1, index, 0, 0, 0);
    }
}
