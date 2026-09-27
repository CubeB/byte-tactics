// Decompiled by Opus. Names are provisional.
// The int version of 0x4e2ee0: reads a registry value into *value (clamped
// by FUN_004e2d00) when reading, otherwise writes *value.

class Class_004e2d00 {
public:
    int FUN_004e2d00(char* name, int minValue, int maxValue, int defaultValue);
};

class Class_004e2d70 {
public:
    void FUN_004e2d70(void* param1, unsigned int param2);
};

class Class_004e2ea0 {
public:
    void* key;                       // +0x0
    char reading;                    // +0x4
    void FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue);
};

// FUNCTION: 0x4e2ea0
void Class_004e2ea0::FUN_004e2ea0(char* name, int* value, int minValue, int maxValue, int defaultValue)
{
    if (reading) {
        *value = ((Class_004e2d00*)this)->FUN_004e2d00(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->FUN_004e2d70(name, *value);
    }
}
