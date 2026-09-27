// Decompiled by Haiku. Names are provisional.

struct Data {
    int field_0;
    int field_4;
};

class Class_0040a5b0 {
public:
    int field_0;
    int field_4;

    Class_0040a5b0* FUN_0040a5b0(const Data* src);
};

// FUNCTION: 0x40a5b0
Class_0040a5b0* Class_0040a5b0::FUN_0040a5b0(const Data* src)
{
    field_0 = src->field_0;
    field_4 = src->field_4;
    return this;
}
