// Decompiled by Haiku. Names are provisional.

struct Data1 {
    int field_0;
};

struct Data2 {
    char field_0;
};

class Class_004e2a10 {
public:
    int field_0;
    char field_4;

    Class_004e2a10* FUN_004e2a10(const Data1* param_1, const Data2* param_2);
};

// FUNCTION: 0x4e2a10
Class_004e2a10* Class_004e2a10::FUN_004e2a10(const Data1* param_1, const Data2* param_2)
{
    field_0 = param_1->field_0;
    field_4 = param_2->field_0;
    return this;
}
