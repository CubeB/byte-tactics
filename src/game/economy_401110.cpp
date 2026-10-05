// Decompiled by Opus. Names are provisional.
// Load counterpart of 0x4010b0: reads the two 0x18-byte blocks back from the
// unit's "u%04xacc" entry, if it exists.
#include <stdio.h>

struct UnitInfo_401110 {
    char unknown_0[0xa8];
    unsigned short id;               // +0xa8
};

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int ReadBox(void* buf, int size);
};

class Class_00401110 {
public:
    char acc0[0x18];                 // +0x00
    char acc1[0x18];                 // +0x18
    void LoadUnitAccounts(UnitInfo_401110* info, HapiBank* file);
};

// FUNCTION: 0x401110
void Class_00401110::LoadUnitAccounts(UnitInfo_401110* info, HapiBank* file)
{
    char name[32];
    sprintf(name, "u%04xacc", info->id);
    if (file->OpenNamedBox(name)) {
        ((HapiBank*)file)->SeekBox(0);
        ((HapiBank*)file)->ReadBox(acc0, 0x18);
        ((HapiBank*)file)->ReadBox(acc1, 0x18);
    }
}
