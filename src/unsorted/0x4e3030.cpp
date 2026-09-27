// Decompiled by Opus. Names are provisional.

class Class_004e2d00 {
public:
    int FUN_004e2d00(char* name, int minValue, int maxValue, int defaultValue);
};

class Class_004e2d70 {
public:
    void FUN_004e2d70(void* param1, unsigned int param2);
};

// Unsigned byte version of the setting accessor in 0x4e2f90.cpp.
class Class_004e3030 {
public:
    void* key;                       // +0x0
    char reading;                    // +0x4
    void FUN_004e3030(char* name, unsigned char* value, unsigned char minValue, unsigned char maxValue, unsigned char defaultValue);
};

// FUNCTION: 0x4e3030
void Class_004e3030::FUN_004e3030(char* name, unsigned char* value, unsigned char minValue, unsigned char maxValue, unsigned char defaultValue)
{
    if (reading) {
        *value = ((Class_004e2d00*)this)->FUN_004e2d00(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->FUN_004e2d70(name, *value);
    }
}
