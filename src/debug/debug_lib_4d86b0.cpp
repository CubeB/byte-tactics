// Decompiled by Opus. Names are provisional.
// Changes the protection of the whole pages inside [addr, addr + size),
// only when FUN_004d8140 (the "gonzo" switch) is on.
#include <windows.h>

bool FUN_004d8140();

// FUNCTION: 0x4d86b0
void __cdecl FUN_004d86b0(int addr, int size, int protect)
{
    DWORD old;
    if (FUN_004d8140()) {
        unsigned int start = (addr + 0xfff) & 0xfffff000;
        unsigned int end = (addr + size) & 0xfffff000;
        if (start < end)
            VirtualProtect((void*)start, end - start, protect, &old);
    }
}
