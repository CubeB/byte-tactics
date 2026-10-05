// Decompiled by Opus. Names are provisional.
// The int version of 0x4e2ee0: reads a registry value into *value (clamped
// by ReadInt) when reading, otherwise writes *value.

class Class_004e2d00 {
public:
    int ReadInt(char* name, int minValue, int maxValue, int defaultValue);
};

class Class_004e2d70 {
public:
    void WriteDword(void* param1, unsigned int param2);
};

class CavedogRegistryKey {
public:
    void* key;                       // +0x0
    char reading;                    // +0x4
    void FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue);
};

// FUNCTION: 0x4e2ea0
void CavedogRegistryKey::FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue)
{
    if (reading) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}
