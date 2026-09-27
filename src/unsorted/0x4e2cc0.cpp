// Decompiled by Sonnet. Names are provisional.

class Class_004e2d00 {
public:
    void* FUN_004e2d00(const char* param1, int param2, int param3, unsigned int param4);
};

class Class_004e2cc0 {
public:
    bool FUN_004e2cc0(const char* param1, unsigned int param2);
};

// FUNCTION: 0x4e2cc0
bool Class_004e2cc0::FUN_004e2cc0(const char* param1, unsigned int param2)
{
    void* r = ((Class_004e2d00*)this)->FUN_004e2d00(param1, 0, 1, param2 & 0xff);
    return r != 0 ? true : false;
}
