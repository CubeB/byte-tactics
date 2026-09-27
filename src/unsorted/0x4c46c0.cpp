// Decompiled by Opus. Names are provisional.
// Looks a name up in a table of (name, string) pairs sorted by name
// (case-insensitively) and returns the string as an int, or a default.
#include <stdlib.h>
#include <string.h>

struct Pair_004c46c0 {
    char* name;                     // +0x0
    char* value;                    // +0x4
};

struct NameLess_004c46c0 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

#pragma pack(push, 1)
class Class_004c46c0 {
public:
    char unknown_0[0x19];
    Pair_004c46c0* first;           // +0x19
    Pair_004c46c0* last;            // +0x1d

    int FUN_004c46c0(const char* name, int def);
};
#pragma pack(pop)

static inline Pair_004c46c0* LowerBound(Pair_004c46c0* first, Pair_004c46c0* last, const char* name)
{
    NameLess_004c46c0 less;
    while (first != last) {
        Pair_004c46c0* mid = first + (last - first) / 2;
        if (less(mid->name, name))
            first = mid + 1;
        else
            last = mid;
    }
    return first;
}

static inline char** Find(Class_004c46c0* table, const char* name)
{
    NameLess_004c46c0 less;
    Pair_004c46c0* it = LowerBound(table->first, table->last, name);
    if (it == table->last || less(name, it->name))
        return 0;
    return &it->value;
}

static inline char* GetString(Class_004c46c0* table, const char* name)
{
    char** p = Find(table, name);
    if (p)
        return *p;
    return 0;
}

// FUNCTION: 0x4c46c0
int Class_004c46c0::FUN_004c46c0(const char* name, int def)
{
    char* s = GetString(this, name);
    if (s)
        return atoi(s);
    return def;
}
