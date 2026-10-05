// Decompiled by Opus. Names are provisional.
// Changes the protection of the whole pages inside [addr, addr + size),
// only when IsGonzo (the "gonzo" switch) is on.
#include <windows.h>

bool IsGonzo();

// FUNCTION: 0x4d86b0
void __cdecl ProtectPages(int addr, int size, int protect)
{
    DWORD old;
    if (IsGonzo()) {
        unsigned int start = (addr + 0xfff) & 0xfffff000;
        unsigned int end = (addr + size) & 0xfffff000;
        if (start < end)
            VirtualProtect((void*)start, end - start, protect, &old);
    }
}
