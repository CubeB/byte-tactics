// Decompiled by Opus, Sonnet, Space Bunny Free and Haiku. Names are provisional.
// Both toggle protection on the same game field, one read-only and one read-write.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

class Mission {
public:
    int FUN_004356c0(int param_1);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1439b];
    void* field_1439b;                 // +0x1439b
    char unknown_1439f[0x24e4a];
    Mission* field_391e9;              // +0x391e9
};
#pragma pack(pop)
extern Game* g_game;

extern int __cdecl _strcmpi(const char*, const char*);

void __cdecl ProtectBlockReadOnly(void* param_1);
void __cdecl ProtectBlockReadWrite(void* param_1);

class AIProfileParser {
public:
    char token[0x80];                   // +0
    char* field_80;                     // +0x80
    char* field_84;                     // +0x84
    char* field_88;                     // +0x88
    int errorReported;                  // +0x8c
    int field_90;                       // +0x90
    int field_94;                       // +0x94

    AIProfileParser();
    void SetBuffer(char* param_1, int param_2);
    int NextToken();
    int ReadInt();
    bool TokenEquals(char* param_1);
    int ScanToken();
    void ReportParseError(char* text);
};

// The AI profile parser: reads values and keys out of an AI profile buffer.
// FUNCTION: 0x428c60
AIProfileParser::AIProfileParser()
{
    field_80 = 0;
    field_84 = 0;
    field_88 = 0;
    errorReported = 0;
    field_90 = 0;
    field_94 = 0x102;
    token[0] = 0;
}

// FUNCTION: 0x428c90
void AIProfileParser::SetBuffer(char* param_1, int param_2)
{
    field_80 = param_1;
    field_84 = param_1 + param_2;
    field_88 = param_1;
    errorReported = 0;
    field_90 = 0;
    field_94 = 0x102;
    token[0] = 0;
}

// FUNCTION: 0x428cd0
int AIProfileParser::NextToken()
{
    if (errorReported != 0)
        return 0x102;
    if (field_90 != 0) {
        field_90 = 0;
        return field_94;
    }
    field_94 = ScanToken();
    return field_94;
}

// AI profile tokenizer. The number scan is written as
// `char* q = field_88; field_88 = q + 1;` rather than `*++field_88` or a bare
// `field_88++` statement: the two-register form (increment in edx, then a
// `mov eax, edx` copy for the load) is what MSVC emits for every pre-increment
// spelling, while going through a pointer local that is stored back to the
// member makes it increment and store in eax directly, as the original does.
// FUNCTION: 0x428d10
int AIProfileParser::ScanToken()
{
    if (errorReported != 0)
        return 0x102;

    while (isspace(*field_88) && field_88 != field_84)
        field_88++;

    if (field_88 == field_84 || *field_88 == '\0')
        return 0;

    if (ispunct(*field_88))
        return *field_88++;

    int len = 0;
    if (!isdigit(*field_88) && *field_88 != '-' && *field_88 != '.') {
        while (isalnum(*field_88) && field_88 != field_84 && len < 0x7f) {
            token[len] = *field_88;
            len++;
            field_88++;
        }
        token[len] = 0;
        if (len != 0 && len != 0x7f)
            return 0x100;
        return 0x102;
    }

    token[0] = *field_88;
    int i = 1;
    for (;;) {
        char* q = field_88;
        field_88 = q + 1;
        if (!(isdigit(*field_88) || *field_88 == '.'))
            break;
        token[i++] = *field_88;
    }
    token[i] = 0;
    return 0x101;
}

// FUNCTION: 0x428e90
int AIProfileParser::ReadInt()
{
    char buffer[256];
    int token;
    if (errorReported != 0) {
        token = 0x102;
    } else if (field_90 != 0) {
        token = field_94;
        field_90 = 0;
    } else {
        token = ScanToken();
        field_94 = token;
    }
    if (token != 0x101) {
        if (errorReported == 0) {
            sprintf(buffer, "parse error reading AI profile %s\n%s\nlast string =",
                    (char*)g_game->field_391e9->FUN_004356c0(7),
                    "expecting int", this);
        }
        errorReported = 1;
        return 0;
    }
    return atoi((char*)this);
}

// FUNCTION: 0x428f40
bool AIProfileParser::TokenEquals(char* param_1)
{
    return _strcmpi(param_1, (const char*)this) == 0;
}

// Reports (once) a parse error in an AI profile: formats the message into a
// local buffer that is never used (the output call was compiled out), then
// sets the "error reported" flag.
// FUNCTION: 0x428f60
void AIProfileParser::ReportParseError(char* text)
{
    char buffer[256];
    if (!errorReported) {
        sprintf(buffer, "parse error reading AI profile %s\n%s\nlast string =",
                (char*)g_game->field_391e9->FUN_004356c0(7), text, this);
    }
    errorReported = 1;
}

// FUNCTION: 0x428fc0
void FUN_00428fc0()
{
    ProtectBlockReadOnly(g_game->field_1439b);
}

// FUNCTION: 0x428fe0
void FUN_00428fe0()
{
    ProtectBlockReadWrite(g_game->field_1439b);
}
