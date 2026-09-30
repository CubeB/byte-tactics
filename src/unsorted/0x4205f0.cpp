// Decompiled by Haiku. Names are provisional.
#include <stdlib.h>

struct Class_004379a0
{
    void FUN_004379a0();
};

extern Class_004379a0 DAT_00511f80;
extern void __cdecl FUN_00420610();

// FUNCTION: 0x4205f0
void FUN_004205f0(void)
{
    DAT_00511f80.FUN_004379a0();
    atexit(FUN_00420610);
}
