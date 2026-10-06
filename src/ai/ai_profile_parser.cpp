// Decompiled by Opus, Sonnet, Space Bunny Free and Haiku. Names are provisional.

#include <stdio.h>
#include <stdlib.h>

class Class_00428d10 {
public:
    int ScanToken();
};

class Mission {
public:
    int FUN_004356c0(int param_1);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Mission* field_391e9;              // +0x391e9
};
#pragma pack(pop)
extern Game* g_game;

extern int __cdecl _strcmpi(const char*, const char*);

extern Game* g_game;

class AIProfileParser {
public:
    char flag;                          // +0
    char unknown_1[0x7f];
    char* field_80;                     // +0x80
    char* field_84;                     // +0x84
    char* field_88;                     // +0x88
    int errorReported;                       // +0x8c
    int field_90;                       // +0x90
    int field_94;                       // +0x94

    AIProfileParser();
    void SetBuffer(char* param_1, int param_2);
    int NextToken();
    int ReadInt();
    bool TokenEquals(char* param_1);
    void ReportParseError(char* text);
};

// The AI profile parser: reads values and keys out of an AI profile buffer.
// AIProfileParser and Class_00428d10 are two views of one object (the token
// scanner at 0x428d10 is called through `this` at offset 0), so the four
// members are one class.
// FUNCTION: 0x428c60
AIProfileParser::AIProfileParser()
{
    field_80 = 0;
    field_84 = 0;
    field_88 = 0;
    errorReported = 0;
    field_90 = 0;
    field_94 = 0x102;
    flag = 0;
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
    flag = 0;
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
    field_94 = ((Class_00428d10*)this)->ScanToken();
    return field_94;
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
        token = ((Class_00428d10*)this)->ScanToken();
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
