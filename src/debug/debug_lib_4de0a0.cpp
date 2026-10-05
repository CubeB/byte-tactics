// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

class LoadedImage {
public:
    LoadedImage(HMODULE m);
    ~LoadedImage();
};

class Class_004de020 {
public:
    char unknown_0[0x18];
    unsigned int imageBase;            // +0x18
    void* FindFpoRecord(unsigned int address);
};

// The FPO-record lookup callback: lazily builds the module's FPO table
// (function-local static at 0x528a78) and looks an address up in it.
// FUNCTION: 0x4de0a0
void __stdcall FUN_004de0a0(int unused, unsigned int address)
{
    static LoadedImage table(GetModuleHandleA(0));
    ((Class_004de020*)&table)->FindFpoRecord(address);
}
