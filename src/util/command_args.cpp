// Decompiled by Haiku, space-bunny-free and Opus. Names are provisional.

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int __cdecl atoi(const char* str);

class CommandArgs {
public:
    char* args[0x14];                  // +0x00 token pointers
    char buffer[0x7e];                 // +0x50 the tokens' text
    char unknown_ce[2];
    int count;                         // +0xd0 token count

    void Tokenize(char* text, char* end);
    CommandArgs* InitArgs();
    int GetArg(int index, int default_val);
    int GetIntArg(int index, int default_val);
    float GetFloatArg(int index, float default_val);
    void ShiftArgs(int n);
};

// FUNCTION: 0x4b73b0
CommandArgs* CommandArgs::InitArgs()
{
    count = 0;
    return this;
}

// FUNCTION: 0x4b73c0
int CommandArgs::GetArg(int index, int default_val)
{
    if (index < 0 || index >= count) {
        return default_val;
    }
    return (int)args[index];
}

// FUNCTION: 0x4b73e0
int CommandArgs::GetIntArg(int index, int default_val)
{
    if (index < 0 || index >= count) {
        return default_val;
    }
    const char* str = args[index];
    return atoi(str);
}

// FUNCTION: 0x4b7410
float CommandArgs::GetFloatArg(int index, float default_val)
{
    if (index < 0 || index >= count) {
        return default_val;
    }
    char* str = args[index];
    return (float)atof(str);
}

// FUNCTION: 0x4b7440
void CommandArgs::Tokenize(char* text, char* end)
{
    char* p = buffer;

    if (end == 0)
        end = text + strlen(text);

    count = 0;

    while (1) {
        while (text != end && isspace(*text))
            text++;

        if (text == end)
            return;

        if (*text == '#')
            return;

        if (count < 0x14) {
            args[count] = p;
            count++;
        }

        while (text != end) {
            if (isspace(*text))
                break;
            if (*text == '#')
                break;
            if (p >= &buffer[0x7e])
                break;
            *p = *text;
            p++;
            text++;
        }

        *p = 0;
        p++;
    }
}

// FUNCTION: 0x4b7540
void CommandArgs::ShiftArgs(int n)
{
    if (count >= n) {
        count = 0;
        return;
    }
    char** end = &args[count];
    char** p = &args[n];
    char** dst = args;
    while (p != end)
        *dst++ = *p++;
    count -= n;
}
