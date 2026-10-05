// Decompiled by Haiku. Names are provisional.

#include <string.h>

class TdfRecord {
public:
    const char* field_0;

    void CopyRecordName(char* dest, size_t count);
};

// FUNCTION: 0x4c4420
void TdfRecord::CopyRecordName(char* dest, size_t count) {
    strncpy(dest, field_0, count);
}
