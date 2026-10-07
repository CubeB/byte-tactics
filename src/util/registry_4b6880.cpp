// Decompiled by Space Bunny Free, finished by GPT-6.1-sol and mimo-v2.6-pro,
// finished by Space Bunny Free. Names are provisional.
//
// The read path accepts ERROR_SUCCESS and ERROR_MORE_DATA (0xea).
#include <windows.h>

// FUNCTION: 0x4b6880
int __stdcall AccessRegistryValue(char* subKey, char* valueName, LPBYTE data, LPDWORD size,
                           DWORD type, DWORD read)
{
    // Zeroed in this order: HKEYs with `= 0`, result by a later statement.
    int result;
    HKEY key1 = 0;
    HKEY key2 = 0;
    HKEY key3 = 0;
    REGSAM samDesired = read ? KEY_READ : KEY_WRITE;
    int doRead = read;
    // lpdwDisposition for the three calls. Left uninitialised on purpose: the
    // key is never read back, and initialising it costs a store (301 bytes).
    DWORD disp;
    // Call results go through a LONG before the compare.
    LONG err;
    result = 0;
    err = RegCreateKeyExA(HKEY_CURRENT_USER, "Software", 0, 0, 0, samDesired, 0,
                          &key1, &disp);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key1, "Cavedog Entertainment", 0, 0, 0, samDesired, 0,
                          &key2, &disp);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key2, subKey, 0, 0, 0, samDesired, 0, &key3, &disp);
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