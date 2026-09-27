// Decompiled by Haiku. Names are provisional.

int atoi(const char* str);

struct Class_004b73e0
{
public:
    char unknown_0[0xd0];
    int field_d0;

    int FUN_004b73e0(int param_1, int param_2);
};

// FUNCTION: 0x4b73e0
int Class_004b73e0::FUN_004b73e0(int param_1, int param_2)
{
    if (param_1 < 0 || param_1 >= field_d0) {
        return param_2;
    }
    const char* str = ((char**)this)[param_1];
    return atoi(str);
}
