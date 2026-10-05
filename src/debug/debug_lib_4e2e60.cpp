// Decompiled by Opus. Names are provisional.
// Reads or writes an int registry value depending on the mode flag
// (compare 0x4e2e20 and 0x4e2ee0).

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
    void* key;                       // +0x0
    char reading;                    // +0x4
    void FUN_004e2e60(char* name, int* value, int minValue, int maxValue, int defaultValue);
};

// FUNCTION: 0x4e2e60
void CavedogRegistryKey::FUN_004e2e60(char* name, int* value, int minValue, int maxValue, int defaultValue)
{
    if (reading) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}
