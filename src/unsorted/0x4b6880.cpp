// Decompiled by space-bunny-free. Names are provisional.
// Opens (or creates) HKCU\Software\Cavedog Entertainment\<subKey> and then
// either reads or writes one REG_DWORD / string / binary value in it.
//
// What the original does, and what this file reproduces:
// - The frame holds four dwords, in the order the compiler prints their
//   offsets: result (-4), key1, the "Software" handle (-8), key2, the
//   "Cavedog Entertainment" handle (-0xc), and key3, the subKey handle
//   (-0x10). All four are zeroed in the prologue in the order key1, key2,
//   key3, result, which only happens when `result` has no initialiser in its
//   declaration and is zeroed by a statement after the three HKEY
//   declarations.
// - samDesired is read ? KEY_READ : KEY_WRITE, and ebx holds the constant 0
//   across every call (four RegCreateKeyExA arguments and every comparison),
//   while the flag itself lives in ebp.
// - Every call result is stored in a LSTATUS and only then compared, which is
//   what makes the original compare with `cmp eax, ebx`; comparing the call
//   result directly gives `test eax, eax` instead.
// - The read path accepts ERROR_SUCCESS and ERROR_MORE_DATA (0xea), not
//   ERROR_FILE_NOT_FOUND.
//
// Still different (see the pull request table):
// - The prologue hoists the flag load into eax (`mov eax,[arg6]` +
//   `mov ebp,eax` + `mov esi,eax`) where the original loads it straight into
//   ebp and copies it to esi. MSVC 5 folds the local flag copy back into the
//   parameter, and the hoisted load wins over the shorter encoding.
// - The original keeps the return value in edi (two `mov edi,1` stores, a
//   `mov edi,<result slot>` on the failure path, `mov eax,edi` at the end);
//   here the variable stays in its stack slot, which gives the same block
//   layout but not the same bytes.
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
    if (err == 0) {
        err = RegCreateKeyExA(key1, "Cavedog Entertainment", 0, 0, 0, samDesired, 0,
                              &key2, &read);
        if (err == 0) {
            err = RegCreateKeyExA(key2, subKey, 0, 0, 0, samDesired, 0, &key3, &read);
            if (err == 0) {
                if (doRead) {
                    err = RegQueryValueExA(key3, valueName, 0, 0, data, size);
                    if (err == 0 || err == ERROR_MORE_DATA) {
                        result = 1;
                    }
                } else {
                    err = RegSetValueExA(key3, valueName, 0, type, data, *size);
                    if (err == 0) {
                        result = 1;
                    }
                }
            }
        }
    }
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
