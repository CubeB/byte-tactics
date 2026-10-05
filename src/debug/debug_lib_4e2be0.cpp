// Decompiled by space-bunny-free. Names are provisional.
// Registry key helper: the constructor opens (or creates) a key under
// HKCU\Software\Cavedog Entertainment, the destructor is empty.
// `readOnly` selects the family: nonzero only opens, zero creates as well.
// A null `section` falls back to the DAT_00529e80 default section name.
// Note: 0x4e2cb0 is this class's (empty, out-of-line) destructor; it is
// called with ecx = the local key object at the end of its scope.
#include <windows.h>

extern char* DAT_00529e80;

class Class_004e2be0 {
public:
    int key;                           // +0x00
    unsigned char readOnly;            // +0x04
    Class_004e2be0(char readOnly, char* app, char* section);
    ~Class_004e2be0();
};

// FUNCTION: 0x4e2be0
Class_004e2be0::Class_004e2be0(char readOnly, char* app, char* section)
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
            key = (int)k;
            return;
        }
    } else {
        if (RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Cavedog Entertainment", &k) == ERROR_SUCCESS
            && RegCreateKeyA(k, section, &k) == ERROR_SUCCESS
            && RegCreateKeyA(k, app, &k) == ERROR_SUCCESS) {
            key = (int)k;
        }
    }
    this->readOnly = readOnly;
}
