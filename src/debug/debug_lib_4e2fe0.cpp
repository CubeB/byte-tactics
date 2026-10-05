// Decompiled by Opus. Names are provisional.
// Reads or writes a bool registry value, like the char/short versions at
// 0x4e2f90 and 0x4e2ee0.

class Class_004e2d00 {
public:
    int FUN_004e2d00(char* name, int minValue, int maxValue, int defaultValue);
};

class Class_004e2d70 {
public:
    void FUN_004e2d70(void* param1, unsigned int param2);
};

class Class_004e2fe0 {
public:
    void* key;                       // +0x0
    char reading;                    // +0x4
    void FUN_004e2fe0(char* name, bool* value, bool defaultValue);
};

// FUNCTION: 0x4e2fe0
void Class_004e2fe0::FUN_004e2fe0(char* name, bool* value, bool defaultValue)
{
    if (reading) {
        *value = ((Class_004e2d00*)this)->FUN_004e2d00(name, 0, 1, defaultValue) ? true : false;
    } else {
        ((Class_004e2d70*)this)->FUN_004e2d70(name, *value);
    }
}
