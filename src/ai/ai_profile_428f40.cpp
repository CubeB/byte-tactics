// Decompiled by Haiku. Names are provisional.

extern int __cdecl _strcmpi(const char*, const char*);

struct AIProfileParser {
    bool TokenEquals(char* param_1);
};

// FUNCTION: 0x428f40
bool AIProfileParser::TokenEquals(char* param_1)
{
    return _strcmpi(param_1, (const char*)this) == 0;
}
