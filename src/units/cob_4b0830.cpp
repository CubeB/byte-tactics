// Decompiled by Opus. Names are provisional.
// Looks a name up in the table at +8 and claims a channel slot for its index
// (StartThread, see 0x4b0a10.cpp); -1 when there is no table.
#include <string.h>

struct NameTable_004b0830 {
    char unknown_0[4];
    int count;                         // +0x4
    char unknown_8[0x1c - 0x8];
    char** names;                      // +0x1c
};

class Class_004b07c0 {
public:
    char unknown_0[8];
    NameTable_004b0830* table;         // +0x8

    int FindScript(const char* name);
};

class CobScript {
public:
    int StartThread(int id);
};

class Class_004b0830 {
public:
    char unknown_0[8];
    NameTable_004b0830* table;         // +0x8

    int StartThreadByName(const char* name);
};

// The name lookup at 0x4b07c0 (the function just before this one in the
// original file), defined here so /Ob2 inlines it as the original did. With
// only a static inline helper, and no function compiled before this one, MSVC
// gives the loop guard its own copy of the "-1" call instead of sharing the
// loop exit.
// FUNCTION: 0x4b07c0
int Class_004b07c0::FindScript(const char* name)
{
    for (int i = 0; i < table->count; i++) {
        if (strcmp(name, table->names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4b0830
int Class_004b0830::StartThreadByName(const char* name)
{
    if (table == 0) {
        return -1;
    }
    return ((CobScript*)this)->StartThread(((Class_004b07c0*)this)->FindScript(name));
}
