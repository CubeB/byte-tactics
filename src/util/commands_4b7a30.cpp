// Decompiled by Opus. Names are provisional.
// Runs a block of command lines: splits the text at newlines, parses each line
// into a command object (Tokenize), substitutes "%N" arguments from vars
// (SubstituteArgs) and executes it (ExecuteCommand); returns the OR of the results.

// Command arguments (0xd4 bytes); see 0x4b74f0.cpp.
class Class_004b74f0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    void SubstituteArgs(Class_004b74f0* other);
};

class CommandArgs {
public:
    char unknown_0[0xd0];
    int field_d0;
    CommandArgs* InitArgs();
    void Tokenize(char* start, char* end);
};

int __stdcall ExecuteCommand(Class_004b74f0* cmd, int param_2);

// The newline search at 0x4b7a10 (the function just before this one in the
// original file), defined here so /Ob2 inlines it as the original did.
// FUNCTION: 0x4b7a10
int __stdcall FindLineEnd(char* param_1, int param_2)
{
    for (int i = 0; i < param_2; i++) {
        if (param_1[i] == '\n') {
            return i;
        }
    }
    return param_2;
}

// FUNCTION: 0x4b7a30
int __stdcall ExecuteCommandText(char* text, int len, Class_004b74f0* vars, int param_4)
{
    int result = 0;
    Class_004b74f0 cmd;
    ((CommandArgs*)&cmd)->InitArgs();
    while (len > 0) {
        int n = FindLineEnd(text, len);
        if (n > len)
            break;
        char* end = text + n;
        ((CommandArgs*)&cmd)->Tokenize(text, end);
        cmd.SubstituteArgs(vars);
        result |= ExecuteCommand(&cmd, param_4);
        text = end + 1;
        len -= n + 1;
    }
    return result;
}
