// Decompiled by Haiku. Names are provisional.

#include <string.h>

class Class_004c4420 {
public:
    const char* field_0;

    void FUN_004c4420(char* dest, size_t count);
};

// FUNCTION: 0x4c4420
void Class_004c4420::FUN_004c4420(char* dest, size_t count) {
    strncpy(dest, field_0, count);
}
