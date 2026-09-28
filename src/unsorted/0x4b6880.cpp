// Decompiled by Space Bunny Free. Names are provisional.
// Opens (or creates) HKCU\Software\Cavedog Entertainment\<subKey> and then
// either reads or writes one REG_DWORD / string / binary value in it.
// The decorated name is ?FUN_004b6880@@YGHPAD0PAEPAKKK@Z, so six stack
// arguments, ret 0x18.
//
// What this file reproduces:
// - The goto structure. All the failure exits jump to one `close:` label, and
//   the two success stores are written out in their own arms, which is what
//   makes MSVC duplicate `mov edi, 1` and put a `jmp` after each. With one
//   shared tail the store is emitted once and the function loses 4 bytes.
// - The frame holds four dwords, zeroed in the order key1, key2, key3, result,
//   which needs the three HKEYs declared with `= 0` and `result` zeroed by a
//   statement after them.
// - The result lives in edi: it is assigned 1 on both success paths and read
//   back from its stack slot on the failure path, then returned with
//   `mov eax, edi`.
// - `samDesired` is read ? KEY_READ : KEY_WRITE (`neg/sbb/and/add` in esi),
//   the flag copy is in ebp, and ebx holds the constant 0, so every comparison
//   is `cmp eax, ebx` and every zero argument is `push ebx`.
// - Every call result is stored in a LONG before it is compared, which is what
//   gives `cmp eax, ebx` rather than `test eax, eax`.
// - The read path accepts ERROR_SUCCESS and ERROR_MORE_DATA (0xea).
// - The ninth argument of all three RegCreateKeyExA calls is the address of
//   the sixth parameter, not of a local. Passing 0 instead loses the frame slot
//   and 12 bytes, so keep it.
//
// Still different (86.0% to 88.2% left on the table):
// - The prologue. The original loads the flag straight into ebp between the
//   two pushes and copies it into esi (`mov ebp, [esp+0x30]; push esi; mov esi,
//   ebp`); here MSVC hoists the load above `push ebx` into a scratch register
//   and copies it into esi early and ebp late, which costs 2 bytes. The
//   address of that same parameter is taken (passed as lpdwDisposition), and
//   that address-taken use is what stops MSVC folding the copy into a load:
//   every shape tried (flag declared first, samDesired computed from the local,
//   BOOL/unsigned/long flag, a separate `LPDWORD pRead`, passing 0) either
//   keeps the hoist or loses the frame.
#include <windows.h>

// FUNCTION: 0x4b6880
int __stdcall FUN_004b6880(char* subKey, char* valueName, LPBYTE data, LPDWORD size,
                           DWORD type, DWORD read)
{
    int result;
    HKEY key1 = 0;
    HKEY key2 = 0;
    HKEY key3 = 0;
    REGSAM samDesired = read ? KEY_READ : KEY_WRITE;
    int doRead = read;
    LONG err;
    result = 0;
    err = RegCreateKeyExA(HKEY_CURRENT_USER, "Software", 0, 0, 0, samDesired, 0,
                          &key1, &read);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key1, "Cavedog Entertainment", 0, 0, 0, samDesired, 0,
                          &key2, &read);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key2, subKey, 0, 0, 0, samDesired, 0, &key3, &read);
    if (err != 0)
        goto close;
    if (doRead) {
        err = RegQueryValueExA(key3, valueName, 0, 0, data, size);
        if (err != 0 && err != ERROR_MORE_DATA)
            goto close;
    } else {
        err = RegSetValueExA(key3, valueName, 0, type, data, *size);
        if (err != 0)
            goto close;
    }
    result = 1;
close:
    if (key3 != 0) {
        RegCloseKey(key3);
    }
    if (key2 != 0) {
        RegCloseKey(key2);
    }
    if (key1 != 0) {
        RegCloseKey(key1);
    }
    return result;
}
