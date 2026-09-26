// Decompiled by Haiku. Names are provisional.

extern int _strcmpi(const char*, const char*);

struct Class_00428f40 {
    bool FUN_00428f40(char* param_1);
};

// FUNCTION: 0x428f40
bool Class_00428f40::FUN_00428f40(char* param_1)
{
    return _strcmpi(param_1, (const char*)this) == 0;
}
