// Decompiled by Haiku. Names are provisional.

struct Param
{
public:
    char unknown_0[0xa6];
    unsigned short field_a6;
};

struct Class_0048f9a0
{
public:
    char unknown_0[0x2a];

    bool FUN_0048f9a0(Param* p);
};

// FUNCTION: 0x48f9a0
bool Class_0048f9a0::FUN_0048f9a0(Param* p)
{
    unsigned short* field_24_ptr = (unsigned short*)&unknown_0[0x24];
    int* field_26_ptr = (int*)&unknown_0[0x26];

    if (p->field_a6 == *field_24_ptr) {
        (*field_26_ptr)++;
    }
    return *field_26_ptr <= 1;
}
