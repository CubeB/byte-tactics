// Decompiled by Opus. Names are provisional.

class Class_004b7410 {
public:
    float FUN_004b7410(int index, float def);
};

extern int DAT_00511dd0;
extern int DAT_00511dd4;

// FUNCTION: 0x416db0
void __stdcall FUN_00416db0(Class_004b7410* args)
{
    DAT_00511dd0 = (int)(args->FUN_004b7410(1, 0.0f) * 256.0f);
    DAT_00511dd4 = (int)(args->FUN_004b7410(2, 0.75f) * 256.0f);
}
