// Decompiled by space-bunny-free. Names are provisional.
// Parses the x1/y1/x2/y2 fields of the current TDF node into four ints.
// On a missing node it reports a fatal error and restores the cursor.
#include <stdio.h>

class TdfFile {
public:
    int GetCurrentRecord();
    void SetCurrentRecord(int val);
    int SelectRecord(char* name);
};

class TdfRecord {
public:
    int GetFieldInt(const char* name, int def);
};

struct Obj_00431950 {
    char unknown_0[4];
    TdfRecord* table;                   // +0x4
};

void __stdcall FatalError(char* message);

// FUNCTION: 0x431950
void __stdcall ReadSideRect(Obj_00431950* obj, int* out, char* name, char* side)
{
    int saved = ((TdfFile*)obj)->GetCurrentRecord();
    if (!((TdfFile*)obj)->SelectRecord(name)) {
        char buf[256];
        sprintf(buf, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", name, side);
        FatalError(buf);
        ((TdfFile*)obj)->SetCurrentRecord(saved);
        return;
    }
    out[0] = obj->table->GetFieldInt("x1", 0);
    out[1] = obj->table->GetFieldInt("y1", 0);
    out[2] = obj->table->GetFieldInt("x2", 0);
    out[3] = obj->table->GetFieldInt("y2", 0);
    ((TdfFile*)obj)->SetCurrentRecord(saved);
}
