// Decompiled by Opus. Names are provisional.
#include <string.h>

// A table of (name, value) pairs sorted by name, case-insensitively.
struct Pair_004c4630 {
    char* name;                     // +0x0
    int value;                      // +0x4
};

struct NameLess_004c4630 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

#pragma pack(push, 1)
class Class_004c4630 {
public:
    char unknown_0[0x19];
    Pair_004c4630* first;           // +0x19
    Pair_004c4630* last;            // +0x1d

    int FindFieldValue(char* name);
};
#pragma pack(pop)

static inline Pair_004c4630* LowerBound(Pair_004c4630* first, Pair_004c4630* last, char* name)
{
    NameLess_004c4630 less;
    while (first != last) {
        Pair_004c4630* mid = first + (last - first) / 2;
        if (less(mid->name, name))
            first = mid + 1;
        else
            last = mid;
    }
    return first;
}

static inline int* Find(Class_004c4630* table, char* name)
{
    NameLess_004c4630 less;
    Pair_004c4630* it = LowerBound(table->first, table->last, name);
    if (it == table->last || less(name, it->name))
        return 0;
    return &it->value;
}

// FUNCTION: 0x4c4630
int Class_004c4630::FindFieldValue(char* name)
{
    int* p = Find(this, name);
    if (p)
        return *p;
    return 0;
}
