// Decompiled by Opus. Names are provisional.
// Runs a block of command lines: splits the text at newlines, parses each line
// into a command object (FUN_004b7440), substitutes "%N" arguments from vars
// (FUN_004b74f0) and executes it (FUN_004b7900); returns the OR of the results.

// Command arguments (0xd4 bytes); see 0x4b74f0.cpp.
class Class_004b74f0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    void FUN_004b74f0(Class_004b74f0* other);
};

class Class_004b73b0 {
public:
    char unknown_0[0xd0];
    int field_d0;
    Class_004b73b0* FUN_004b73b0();
};

class Class_004b7440 {
public:
    void FUN_004b7440(char* start, char* end);
};

int __stdcall FUN_004b7900(Class_004b74f0* cmd, int param_2);

// The newline search at 0x4b7a10 (the function just before this one in the
// original file), defined here so /Ob2 inlines it as the original did.
// FUNCTION: 0x4b7a10
int __stdcall FUN_004b7a10(char* param_1, int param_2)
{
    for (int i = 0; i < param_2; i++) {
        if (param_1[i] == '\n') {
            return i;
        }
    }
    return param_2;
}

// FUNCTION: 0x4b7a30
int __stdcall FUN_004b7a30(char* text, int len, Class_004b74f0* vars, int param_4)
{
    int result = 0;
    Class_004b74f0 cmd;
    ((Class_004b73b0*)&cmd)->FUN_004b73b0();
    while (len > 0) {
        int n = FUN_004b7a10(text, len);
        if (n > len)
            break;
        char* end = text + n;
        ((Class_004b7440*)&cmd)->FUN_004b7440(text, end);
        cmd.FUN_004b74f0(vars);
        result |= FUN_004b7900(&cmd, param_4);
        text = end + 1;
        len -= n + 1;
    }
    return result;
}
