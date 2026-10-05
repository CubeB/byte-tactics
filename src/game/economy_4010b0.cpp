// Decompiled by Opus. Names are provisional.
#include <stdio.h>

struct UnitInfo_4010b0 {
    char unknown_0[0xa8];
    unsigned short id;               // +0xa8
};

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int WriteBox(void* data, int size);
};

class Class_004010b0 {
public:
    char acc0[0x18];                 // +0x00
    char acc1[0x18];                 // +0x18
    void SaveUnitAccounts(UnitInfo_4010b0* info, HapiBank* file);
};

// FUNCTION: 0x4010b0
void Class_004010b0::SaveUnitAccounts(UnitInfo_4010b0* info, HapiBank* file)
{
    char name[32];
    sprintf(name, "u%04xacc", info->id);
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->WriteBox(acc0, 0x18);
    ((HapiBank*)file)->WriteBox(acc1, 0x18);
}
