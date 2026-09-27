// Decompiled by Opus. Names are provisional.
#include <string.h>

class Class_004bb670 {
public:
    char unknown_0[0x18];
    char name[0x100];                  // +0x18

    void FUN_004bb670(const char* text);
};

// FUNCTION: 0x4bb670
void Class_004bb670::FUN_004bb670(const char* text)
{
    strncpy(name, text, sizeof(name));
    name[sizeof(name) - 1] = 0;
}
