// Decompiled by Opus. Names are provisional.
// Reports (once) a parse error in an AI profile: formats the message into a
// local buffer that is never used (the output call was compiled out), then
// sets the "error reported" flag.
#include <stdio.h>

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

class AIProfileParser {
public:
    char unknown_0[0x8c];
    int errorReported;                 // +0x8c

    void ReportParseError(char* text);
};

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
