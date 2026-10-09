// Decompiled by Opus, Sonnet, Space Bunny Free and Haiku. Names are provisional.
// Both toggle protection on the same game field, one read-only and one read-write.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../map/mission.h"

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1439b];
    void* unitDefs;                    // +0x1439b
    char unknown_1439f[0x24e4a];
    Mission* mapInfo;                  // +0x391e9
};
#pragma pack(pop)
extern Game* g_game;

extern int __cdecl _strcmpi(const char*, const char*);

void __cdecl ProtectBlockReadOnly(void* param_1);
void __cdecl ProtectBlockReadWrite(void* param_1);

class AIProfileParser {
public:
    char token[0x80];                   // +0
    char* bufferStart;                  // +0x80
    char* bufferEnd;                    // +0x84
    char* cursor;                       // +0x88
    int errorReported;                  // +0x8c
    int tokenPending;                   // +0x90
    int tokenType;                      // +0x94

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
    bufferStart = 0;
    bufferEnd = 0;
    cursor = 0;
    errorReported = 0;
    tokenPending = 0;
    tokenType = 0x102;
    token[0] = 0;
}

// FUNCTION: 0x428c90
void AIProfileParser::SetBuffer(char* param_1, int param_2)
{
    bufferStart = param_1;
    bufferEnd = param_1 + param_2;
    cursor = param_1;
    errorReported = 0;
    tokenPending = 0;
    tokenType = 0x102;
    token[0] = 0;
}

// FUNCTION: 0x428cd0
int AIProfileParser::NextToken()
{
    if (errorReported != 0)
        return 0x102;
    if (tokenPending != 0) {
        tokenPending = 0;
        return tokenType;
    }
    tokenType = ScanToken();
    return tokenType;
}

// AI profile tokenizer.
// FUNCTION: 0x428d10
int AIProfileParser::ScanToken()
{
    if (errorReported != 0)
        return 0x102;

    while (isspace(*cursor) && cursor != bufferEnd)
        cursor++;

    if (cursor == bufferEnd || *cursor == '\0')
        return 0;

    if (ispunct(*cursor))
        return *cursor++;

    int len = 0;
    if (!isdigit(*cursor) && *cursor != '-' && *cursor != '.') {
        while (isalnum(*cursor) && cursor != bufferEnd && len < 0x7f) {
            token[len] = *cursor;
            len++;
            cursor++;
        }
        token[len] = 0;
        if (len != 0 && len != 0x7f)
            return 0x100;
        return 0x102;
    }

    token[0] = *cursor;
    int i = 1;
    for (;;) {
        // Increment through a local, not `++cursor`: the stores differ.
        char* q = cursor;
        cursor = q + 1;
        if (!(isdigit(*cursor) || *cursor == '.'))
            break;
        token[i++] = *cursor;
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
    } else if (tokenPending != 0) {
        token = tokenType;
        tokenPending = 0;
    } else {
        token = ScanToken();
        tokenType = token;
    }
    if (token != 0x101) {
        if (errorReported == 0) {
            sprintf(buffer, "parse error reading AI profile %s\n%s\nlast string =",
                    (char*)g_game->mapInfo->GetNameSlot(7),
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
                (char*)g_game->mapInfo->GetNameSlot(7), text, this);
    }
    errorReported = 1;
}

// FUNCTION: 0x428fc0
void ProtectUnitDefsReadOnly()
{
    ProtectBlockReadOnly(g_game->unitDefs);
}

// FUNCTION: 0x428fe0
void ProtectUnitDefsReadWrite()
{
    ProtectBlockReadWrite(g_game->unitDefs);
}
