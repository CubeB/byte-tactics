// Decompiled by Opus. Names are provisional.
// Walks the list of entries held by the global at DAT_00511fb4 and returns
// the first one that contains `name` (0 if none does).

#include "../util/tdf.h"

struct Class_00422460 {
    int unknown_0;
    TdfFile** first;                   // +0x4
    TdfFile** last;                    // +0x8
};

extern Class_00422460* DAT_00511fb4;

// FUNCTION: 0x422460
TdfFile* __stdcall FindFeatureFile(char* name)
{
    for (TdfFile** p = DAT_00511fb4->first; p < DAT_00511fb4->last; p++) {
        (*p)->ResetCurrentRecord();
        if (((TdfFile*)*p)->SelectRecord(name))
            return *p;
    }
    return 0;
}
