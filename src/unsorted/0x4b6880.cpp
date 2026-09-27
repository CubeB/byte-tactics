// Decompiled by Space Bunny Free. Names are provisional.
// Opens (or creates) HKCU\Software\Cavedog Entertainment\<subKey> and either
// reads or writes one REG_DWORD / string / binary value in it, returning 1 on
// success.
//
// Partial: 76.9%. The frame (sub esp, 0x10, four pushes), the four zeroed
// locals, the samDesired ternary and the whole call sequence match. What still
// differs is the register shuffle in the prologue and the form of the zero
// test on each API result:
//   - the original loads the flag straight from its parameter slot into ebp
//     between two pushes and then computes samDesired in esi with a single
//     `mov esi, ebp`. This source hoists the load above `push ebx` and needs
//     `mov ebp, eax` plus `mov esi, eax` to get there, two extra instructions;
//   - the original tests each RegCreateKeyExA result with `cmp eax, ebx`
//     against the zero register it already has, this source emits
//     `test eax, eax`.
//
// The one thing that mattered most, and is worth keeping: the flag has to be
// copied into a local (`DWORD flag = read;`) and that local used both for
// samDesired and for the read/write branch. Without the copy MSVC rematerialises
// the parameter load from the stack at each use, never allocates a callee-saved
// register for it, ends up with only three callee-saved registers in use, and
// keeps the result local in a register: the frame drops to sub esp, 0xc with
// three pushes and only three locals are zeroed. That single change took this
// function from 37.8% to 76.9%.
//
// Tried at 76.9%, all identical or worse: taking samDesired from the parameter
// and the branch from the local (and the reverse); the ternary as `!= 0`; the
// ternary written as an if that assigns 0x20006 then 0x20019; the call tests as
// `!call(...)` rather than `call(...) == 0`; the read and write arms sharing one
// `err` local; declaring the result before the three HKEYs; and a separate
// `LPDWORD disp` local for the out-parameter. `cmp eax, ebx` versus
// `test eax, eax` did not move for any of them, which is consistent with the
// remaining difference being register allocation rather than source shape.
#include <windows.h>

// FUNCTION: 0x4b6880
int __stdcall FUN_004b6880(char* subKey, char* valueName, LPBYTE data, LPDWORD size,
                           DWORD type, DWORD read)
{
    HKEY key1 = 0;
    HKEY key2 = 0;
    HKEY key3 = 0;
    int result = 0;
    // Copied on purpose: this local is what makes MSVC keep the flag in a
    // callee-saved register and spill the result, see the note above.
    DWORD flag = read;
    REGSAM samDesired = flag ? 0x20019 : 0x20006;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software", 0, 0, 0, samDesired, 0,
                        &key1, &read) == 0
        && RegCreateKeyExA(key1, "Cavedog Entertainment", 0, 0, 0, samDesired, 0,
                           &key2, &read) == 0
        && RegCreateKeyExA(key2, subKey, 0, 0, 0, samDesired, 0, &key3, &read) == 0) {
        LONG err;
        if (flag) {
            err = RegQueryValueExA(key3, valueName, 0, 0, data, size);
            if (err == 0 || err == ERROR_FILE_NOT_FOUND)
                result = 1;
        } else {
            err = RegSetValueExA(key3, valueName, 0, type, data, *size);
            if (err == 0)
                result = 1;
        }
    }
    if (key3 != 0) RegCloseKey(key3);
    if (key2 != 0) RegCloseKey(key2);
    if (key1 != 0) RegCloseKey(key1);
    return result;
}
