// Decompiled by Opus. Names are provisional.
// Load counterpart of 0x4010b0: reads the two 0x18-byte blocks back from the
// unit's "u%04xacc" entry, if it exists.
#include <stdio.h>

struct UnitInfo_401110 {
    char unknown_0[0xa8];
    unsigned short id;               // +0xa8
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4c80 {
public:
    int FUN_004b4c80(void* buf, int size);
};

class Class_00401110 {
public:
    char acc0[0x18];                 // +0x00
    char acc1[0x18];                 // +0x18
    void LoadUnitAccounts(UnitInfo_401110* info, Class_004b4ba0* file);
};

// FUNCTION: 0x401110
void Class_00401110::LoadUnitAccounts(UnitInfo_401110* info, Class_004b4ba0* file)
{
    char name[32];
    sprintf(name, "u%04xacc", info->id);
    if (file->FUN_004b4ba0(name)) {
        ((Class_004b4c10*)file)->FUN_004b4c10(0);
        ((Class_004b4c80*)file)->FUN_004b4c80(acc0, 0x18);
        ((Class_004b4c80*)file)->FUN_004b4c80(acc1, 0x18);
    }
}
