// Decompiled by Haiku. Names are provisional.

struct Class_004c9390 {
    void ReleaseRef();
};

// FUNCTION: 0x4c5170
void __stdcall FUN_004c5170(char* param_1)
{
    ((Class_004c9390*)(param_1 + 4))->ReleaseRef();
    ((Class_004c9390*)(param_1))->ReleaseRef();
}
