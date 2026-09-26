// Decompiled by Haiku. Names are provisional.

class Class_0048ec00
{
public:
    char unknown_0[0x4];
    int field_4;

    bool FUN_0048ec00(int* param_1);
};

// FUNCTION: 0x48ec00
bool Class_0048ec00::FUN_0048ec00(int* param_1)
{
    if (*param_1 != 0) {
        field_4++;
    }
    return field_4 <= 1;
}
