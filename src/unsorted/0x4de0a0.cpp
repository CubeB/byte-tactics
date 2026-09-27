// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

class Class_004ddf00 {
public:
    Class_004ddf00(HMODULE m);
    ~Class_004ddf00();
};

class Class_004de020 {
public:
    char unknown_0[0x18];
    unsigned int imageBase;            // +0x18
    void* FUN_004de020(unsigned int address);
};

// The FPO-record lookup callback: lazily builds the module's FPO table
// (function-local static at 0x528a78) and looks an address up in it.
// FUNCTION: 0x4de0a0
void __stdcall FUN_004de0a0(int unused, unsigned int address)
{
    static Class_004ddf00 table(GetModuleHandleA(0));
    ((Class_004de020*)&table)->FUN_004de020(address);
}
