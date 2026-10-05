// Decompiled by space-bunny-free. Names are provisional.
// Parses the x1/y1/x2/y2 fields of the current TDF node into four ints.
// On a missing node it reports a fatal error and restores the cursor.
#include <stdio.h>

class Class_004c3e20 {
public:
    int FUN_004c3e20();
};

class Class_004c3e30 {
public:
    void FUN_004c3e30(int val);
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

struct Obj_00431950 {
    char unknown_0[4];
    Class_004c46c0* table;              // +0x4
};

void __stdcall FatalError(char* message);

// FUNCTION: 0x431950
void __stdcall FUN_00431950(Obj_00431950* obj, int* out, char* name, char* side)
{
    int saved = ((Class_004c3e20*)obj)->FUN_004c3e20();
    if (!((Class_004c3410*)obj)->FUN_004c3410(name)) {
        char buf[256];
        sprintf(buf, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", name, side);
        FatalError(buf);
        ((Class_004c3e30*)obj)->FUN_004c3e30(saved);
        return;
    }
    out[0] = obj->table->FUN_004c46c0("x1", 0);
    out[1] = obj->table->FUN_004c46c0("y1", 0);
    out[2] = obj->table->FUN_004c46c0("x2", 0);
    out[3] = obj->table->FUN_004c46c0("y2", 0);
    ((Class_004c3e30*)obj)->FUN_004c3e30(saved);
}
