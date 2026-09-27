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

class Class_00438760 {
public:
    char* FUN_00438760(char* name);
};

// FUNCTION: 0x438760
char* Class_00438760::FUN_00438760(char* name)
{
    char* result = (char*)this;
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
        *result = (unsigned char)(first - DAT_00512344);
        return result;
    }
    *result = 0;
    return result;
}
