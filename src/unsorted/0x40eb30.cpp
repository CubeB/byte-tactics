// Decompiled by Sonnet. Names are provisional.

extern void operator delete(void* p);

void FUN_004d85a0(void* param_1);

class Class_0040eb30 {
public:
    void* field_0;
    void* field_4;
    char unknown_8[0x1c - 8];
    void* field_1c;
    char unknown_20[0x2c - 0x20];
    void* field_2c;

    ~Class_0040eb30();
};

// FUNCTION: 0x40eb30
Class_0040eb30::~Class_0040eb30()
{
    FUN_004d85a0(field_2c);
    operator delete(field_1c);
    operator delete(field_0);
    operator delete(field_4);
}
