// Decompiled by Haiku. Names are provisional.

struct Class_00461d80
{
public:
    char unknown_0[0x18];
    int field_18;

    int FUN_00461d80();
};

// FUNCTION: 0x461d80
int Class_00461d80::FUN_00461d80()
{
    unsigned int val = field_18;
    return (val / 30) * 1000;
}
