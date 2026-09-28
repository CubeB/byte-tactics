// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>
#include <stdlib.h>

struct Entry_004c4800 {
    char* key;                       // +0x0
    char* value;                     // +0x4
};

#pragma pack(push, 1)
class Class_004c4800 {
public:
    char unknown_0[0x19];
    Entry_004c4800* first;           // +0x19
    Entry_004c4800* last;            // +0x1d

    int* FUN_004c4800(int* dst, char* key, int def);
};
#pragma pack(pop)

static inline bool Less_004c4800(const char* a, const char* b)
{
    return _strcmpi(a, b) < 0;
}

// FUNCTION: 0x4c4800
int* Class_004c4800::FUN_004c4800(int* dst, char* key, int def)
{
    Entry_004c4800* lo = first;
    Entry_004c4800* hi = last;
    while (lo != hi) {
        Entry_004c4800* mid = lo + (hi - lo) / 2;
        if (Less_004c4800(mid->key, key)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    char** p;
    if (lo == last || Less_004c4800(key, lo->key)) {
        p = 0;
    } else {
        p = &lo->value;
    }
    char* value = p ? *p : 0;
    if (value) {
        *dst = (int)(atof(value) * 65536.0);
        return dst;
    }
    *dst = def;
    return dst;
}
