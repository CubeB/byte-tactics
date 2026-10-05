// Decompiled by Opus. Names are provisional.
// Walks the list of entries held by the global at DAT_00511fb4 and returns
// the first one that contains `name` (0 if none does).

class Class_004c3e10 {
public:
    char unknown_0[4];
    int field_0x4;

    void FUN_004c3e10();
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

struct Class_00422460 {
    int unknown_0;
    Class_004c3e10** first;            // +0x4
    Class_004c3e10** last;             // +0x8
};

extern Class_00422460* DAT_00511fb4;

// FUNCTION: 0x422460
Class_004c3e10* __stdcall FindFeatureFile(char* name)
{
    for (Class_004c3e10** p = DAT_00511fb4->first; p < DAT_00511fb4->last; p++) {
        (*p)->FUN_004c3e10();
        if (((Class_004c3410*)*p)->FUN_004c3410(name))
            return *p;
    }
    return 0;
}
