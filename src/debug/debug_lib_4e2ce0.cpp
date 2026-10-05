// Decompiled by Sonnet. Names are provisional.

class Class_004e2d70 {
public:
    void WriteDword(void* param1, unsigned int param2);
};

class Class_004e2ce0 {
public:
    void FUN_004e2ce0(void* param1, unsigned int param2);
};

// FUNCTION: 0x4e2ce0
void Class_004e2ce0::FUN_004e2ce0(void* param1, unsigned int param2)
{
    ((Class_004e2d70*)this)->WriteDword(param1, param2 & 0xff);
}
