// Decompiled by Opus. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

// The "death timer runs out" defeat condition; writes its state to a section.
class Class_0048fd70 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    void FUN_0048fd70(Class_004b4560* obj);
};

// FUNCTION: 0x48fd70
void Class_0048fd70::FUN_0048fd70(Class_004b4560* obj)
{
    obj->FUN_004b4560("DefeatCondition_DeathTimerRunsOut");
    ((Class_004b4630*)obj)->FUN_004b4630("Satisfied", satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630("Celebrated", celebrated);
}
