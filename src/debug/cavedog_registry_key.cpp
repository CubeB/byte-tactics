// Decompiled by space-bunny-free, Haiku and Opus. Names are provisional.

#include <windows.h>

extern char* DAT_00529e80;

class Class_004e2d00 {
public:
    int ReadInt(char* name, int minValue, int maxValue, int defaultValue);
};

class Class_004e2d70 {
public:
    void WriteDword(const char* name, unsigned long value);
};

class CavedogRegistryKey {
public:
    HKEY key;                          // +0x00
    unsigned char readOnly;            // +0x04, set: read the values; clear: write them

    CavedogRegistryKey(char readOnly, char* app, char* section);
    ~CavedogRegistryKey();
    DWORD ReadDword(LPCSTR name, DWORD minValue, DWORD maxValue, DWORD defaultValue);
    void FUN_004e2e60(char* name, int* value, int minValue, int maxValue, int defaultValue);
    void FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue);
    void FUN_004e2ee0(char* name, short* value, short minValue, short maxValue, short defaultValue);
    void FUN_004e2f30(char* name, unsigned short* value, unsigned short minValue, unsigned short maxValue, unsigned short defaultValue);
    void FUN_004e2f90(char* name, char* value, char minValue, char maxValue, char defaultValue);
    void FUN_004e3030(char* name, unsigned char* value, unsigned char minValue, unsigned char maxValue, unsigned char defaultValue);
};

// Registry key helper: the constructor opens (or creates) a key under
// HKCU\Software\Cavedog Entertainment, the destructor is empty.
// `readOnly` selects the family: nonzero only opens, zero creates as well.
// A null `section` falls back to the DAT_00529e80 default section name.
// Note: 0x4e2cb0 is this class's (empty, out-of-line) destructor; it is
// called with ecx = the local key object at the end of its scope.
// FUNCTION: 0x4e2be0
CavedogRegistryKey::CavedogRegistryKey(char readOnly, char* app, char* section)
{
    HKEY k;
    if (section == 0) {
        section = DAT_00529e80;
    }
    key = 0;
    if (readOnly) {
        if (RegOpenKeyA(HKEY_CURRENT_USER, "Software\\Cavedog Entertainment", &k) == ERROR_SUCCESS
            && RegOpenKeyA(k, section, &k) == ERROR_SUCCESS
            && RegOpenKeyA(k, app, &k) == ERROR_SUCCESS) {
            this->readOnly = readOnly;
            key = k;
            return;
        }
    } else {
        if (RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Cavedog Entertainment", &k) == ERROR_SUCCESS
            && RegCreateKeyA(k, section, &k) == ERROR_SUCCESS
            && RegCreateKeyA(k, app, &k) == ERROR_SUCCESS) {
            key = k;
        }
    }
    this->readOnly = readOnly;
}

// FUNCTION: 0x4e2cb0
CavedogRegistryKey::~CavedogRegistryKey()
{
}

// FUNCTION: 0x4e2d90
DWORD CavedogRegistryKey::ReadDword(LPCSTR name, DWORD minValue, DWORD maxValue, DWORD defaultValue)
{
    DWORD value;
    DWORD size = 4;
    DWORD type = REG_DWORD;
    if (RegQueryValueExA(key, name, 0, &type, (LPBYTE)&value, &size) == ERROR_SUCCESS
        && size == 4 && type == REG_DWORD) {
        if (value < minValue) {
            value = minValue;
        }
        if (value > maxValue) {
            return maxValue;
        }
        return value;
    }
    return defaultValue;
}

// Reads or writes an int registry value depending on the mode flag
// (compare 0x4e2e20 and 0x4e2ee0).
// FUNCTION: 0x4e2e60
void CavedogRegistryKey::FUN_004e2e60(char* name, int* value, int minValue, int maxValue, int defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// The int version of 0x4e2ee0: reads a registry value into *value (clamped
// by ReadInt) when reading (readOnly), otherwise writes *value.
// FUNCTION: 0x4e2ea0
void CavedogRegistryKey::FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e2ee0
void CavedogRegistryKey::FUN_004e2ee0(char* name, short* value, short minValue, short maxValue, short defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e2f30
void CavedogRegistryKey::FUN_004e2f30(char* name, unsigned short* value, unsigned short minValue, unsigned short maxValue, unsigned short defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e2f90
void CavedogRegistryKey::FUN_004e2f90(char* name, char* value, char minValue, char maxValue, char defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}

// FUNCTION: 0x4e3030
void CavedogRegistryKey::FUN_004e3030(char* name, unsigned char* value, unsigned char minValue, unsigned char maxValue, unsigned char defaultValue)
{
    if (readOnly) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}
