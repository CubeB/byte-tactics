// Decompiled by Opus. Names are provisional.
// An integer command-line setting (e.g. "setvalue" / "-memset" in 0x4d8260):
// starts at the default, then parses the text after the switch as hex
// ("0x...") or decimal.
#include <stdio.h>

char* __cdecl FUN_004d9f60(char* name);

class Class_004da040 {
public:
    int value;                         // +0x0
    Class_004da040(char* name, int a, int def, char* sw);
};

// FUNCTION: 0x4da040
Class_004da040::Class_004da040(char* name, int a, int def, char* sw)
{
    value = def;
    char* p = FUN_004d9f60(sw);
    if (p) {
        while (*p == ' ' || *p == '\t' || *p == '=')
            p++;
        if (*p == '0' && (p[1] == 'x' || p[1] == 'X')) {
            sscanf(p, "%x", &value);
            return;
        }
        sscanf(p, "%d", &value);
    }
}
