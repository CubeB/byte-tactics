// Decompiled by Opus. Names are provisional.

class Class_004e2d00 {
public:
    int ReadInt(char* name, int minValue, int maxValue, int defaultValue);
};

class Class_004e2d70 {
public:
    void WriteDword(void* param1, unsigned int param2);
};

class Class_004e2f30 {
public:
    void* key;                       // +0x0
    char reading;                    // +0x4
    void FUN_004e2f30(char* name, unsigned short* value, unsigned short minValue, unsigned short maxValue, unsigned short defaultValue);
};

// FUNCTION: 0x4e2f30
void Class_004e2f30::FUN_004e2f30(char* name, unsigned short* value, unsigned short minValue, unsigned short maxValue, unsigned short defaultValue)
{
    if (reading) {
        *value = ((Class_004e2d00*)this)->ReadInt(name, minValue, maxValue, defaultValue);
    } else {
        ((Class_004e2d70*)this)->WriteDword(name, *value);
    }
}
