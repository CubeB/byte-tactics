// Decompiled by Opus. Names are provisional.
#include <stdio.h>

struct UnitInfo_4010b0 {
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

class Class_004b4cf0 {
public:
    int FUN_004b4cf0(void* data, int size);
};

class Class_004010b0 {
public:
    char acc0[0x18];                 // +0x00
    char acc1[0x18];                 // +0x18
    void SaveUnitAccounts(UnitInfo_4010b0* info, Class_004b4ba0* file);
};

// FUNCTION: 0x4010b0
void Class_004010b0::SaveUnitAccounts(UnitInfo_4010b0* info, Class_004b4ba0* file)
{
    char name[32];
    sprintf(name, "u%04xacc", info->id);
    file->FUN_004b4ba0(name);
    ((Class_004b4c10*)file)->FUN_004b4c10(0);
    ((Class_004b4cf0*)file)->FUN_004b4cf0(acc0, 0x18);
    ((Class_004b4cf0*)file)->FUN_004b4cf0(acc1, 0x18);
}
