// Decompiled by Opus. Names are provisional.
#include <string.h>

class Class_004e1a30 {
public:
    char* name;                      // +0x00
    int FUN_004e1a30(const Class_004e1a30& other);
};

// FUNCTION: 0x4e1a30
int Class_004e1a30::FUN_004e1a30(const Class_004e1a30& other)
{
    if (name != other.name && strcmp(name, other.name) < 0) {
        return 1;
    }
    return 0;
}
