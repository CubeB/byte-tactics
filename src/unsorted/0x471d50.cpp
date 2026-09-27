// Decompiled by Opus. Names are provisional.
// Returns an object to the pool DAT_0051e610 (the pool's free method);
// called by the scalar deleting destructors 0x474d10 and 0x475110.

class Class_00470ed0 {
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);
};

extern Class_00470ed0 DAT_0051e610;

// FUNCTION: 0x471d50
void __stdcall FUN_00471d50(void* obj)
{
    DAT_0051e610.FUN_00470ed0(obj);
}
