// Decompiled by Haiku. Names are provisional.

int __cdecl atoi(const char* str);

struct CommandArgs
{
public:
    char unknown_0[0xd0];
    int field_d0;

    int GetIntArg(int param_1, int param_2);
};

// FUNCTION: 0x4b73e0
int CommandArgs::GetIntArg(int param_1, int param_2)
{
    if (param_1 < 0 || param_1 >= field_d0) {
        return param_2;
    }
    const char* str = ((char**)this)[param_1];
    return atoi(str);
}
