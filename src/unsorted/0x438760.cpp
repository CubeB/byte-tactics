// Decompiled by space-bunny-free. Names are provisional.
#include <string.h>

extern int _strcmpi(const char*, const char*);

#pragma pack(push, 1)
struct Entry_00438760 {
    char unknown_0[0x15];
    char* name;                        // +0x15
};
#pragma pack(pop)

extern Entry_00438760* DAT_00512344;
extern Entry_00438760* DAT_00512348;

// An order type held as its index in the sorted order-type table. Callers
// build it from a name as a by-value temporary (0x403260, 0x4118e0, ...), so
// this is its constructor.
class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

// FUNCTION: 0x438760
Class_00438760::Class_00438760(const char* name)
{
    Entry_00438760* first = DAT_00512344;
    int n = DAT_00512348 - DAT_00512344;
    for (; 0 < n; ) {
        int n2 = n / 2;
        Entry_00438760* m = first;
        m += n2;
        int less = _strcmpi(m->name, name) < 0;
        if (less)
            first = ++m, n -= n2 + 1;
        else
            n = n2;
    }
    if (first != DAT_00512348 && _strcmpi(first->name, name) == 0) {
        index = (unsigned char)(first - DAT_00512344);
        return;
    }
    index = 0;
}
