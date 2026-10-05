// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Entry_004c48c0 {
    char* key;                       // +0x0
    char* value;                     // +0x4
};

#pragma pack(push, 1)
class TdfRecord {
public:
    char unknown_0[0x19];
    Entry_004c48c0* first;           // +0x19
    Entry_004c48c0* last;            // +0x1d

    int GetFieldString(char* dst, char* key, size_t size, char* def);
};
#pragma pack(pop)

static inline bool Less_004c48c0(const char* a, const char* b)
{
    return _strcmpi(a, b) < 0;
}

// FUNCTION: 0x4c48c0
int TdfRecord::GetFieldString(char* dst, char* key, size_t size, char* def)
{
    Entry_004c48c0* lo = first;
    Entry_004c48c0* hi = last;
    while (lo != hi) {
        Entry_004c48c0* mid = lo + (hi - lo) / 2;
        if (Less_004c48c0(mid->key, key)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    char** p;
    if (lo == last || Less_004c48c0(key, lo->key)) {
        p = 0;
    } else {
        p = &lo->value;
    }
    char* value = p ? *p : 0;
    if (value) {
        strncpy(dst, value, size);
        dst[size - 1] = 0;
        return 1;
    }
    if (def) {
        strcpy(dst, def);
    }
    return 0;
}
