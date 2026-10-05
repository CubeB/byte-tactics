// Decompiled by Opus. Names are provisional.
#include <string.h>

class NameKey {
public:
    char* name;                      // +0x00
    int FUN_004e1a30(const NameKey& other);
};

// FUNCTION: 0x4e1a30
int NameKey::FUN_004e1a30(const NameKey& other)
{
    if (name != other.name && strcmp(name, other.name) < 0) {
        return 1;
    }
    return 0;
}
