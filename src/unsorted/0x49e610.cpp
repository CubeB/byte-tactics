// Decompiled by Haiku. Names are provisional.
#include <stdlib.h>

extern char DAT_0051f310[];

class Class_004c2ea0 {
public:
    void FUN_004c2ea0();
};

extern void FUN_0049e630();

// FUNCTION: 0x49e610
void __cdecl FUN_0049e610()
{
    ((Class_004c2ea0*)DAT_0051f310)->FUN_004c2ea0();
    atexit(FUN_0049e630);
}
