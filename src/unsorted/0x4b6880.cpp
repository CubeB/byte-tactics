// Decompiled by Space Bunny Free. Names are provisional.
// Opens (or creates) HKCU\Software\Cavedog Entertainment\<subKey> and either
// reads or writes one REG_DWORD / string / binary value in it.
#include <windows.h>

// FUNCTION: 0x4b6880
int __stdcall FUN_004b6880(char* subKey, char* valueName, LPBYTE data, LPDWORD size,
                           DWORD type, DWORD read)
{
    HKEY key1 = 0;
    HKEY key2 = 0;
    HKEY key3 = 0;
    int result = 0;
    REGSAM samDesired = read ? 0x20019 : 0x20006;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software", 0, 0, 0, samDesired, 0,
                        &key1, &read) == 0
        && RegCreateKeyExA(key1, "Cavedog Entertainment", 0, 0, 0, samDesired, 0,
                           &key2, &read) == 0
        && RegCreateKeyExA(key2, subKey, 0, 0, 0, samDesired, 0, &key3, &read) == 0) {
        if (read) {
            LONG err = RegQueryValueExA(key3, valueName, 0, 0, data, size);
            if (err == 0 || err == ERROR_FILE_NOT_FOUND) {
                result = 1;
            }
        } else {
            if (RegSetValueExA(key3, valueName, 0, type, data, *size) == 0) {
                result = 1;
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
