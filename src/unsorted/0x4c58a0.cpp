// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Looks a key up in a TDF section: builds the key by prepending the current
// section name (DAT_0051fdc0), tries that first and falls back to the plain
// key. param_5 (or the empty default DAT_005119b8) is the TDF default value.
// param_1 is the open file object, its section parser at +4. The intermediate
// int is what makes MSVC 5 keep each argument in the original's register.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Class_004c48c0 {
public:
    char unknown_0[0x19];
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

class Class_004c2ea0 {
public:
    int root;                            // +0x0
    Class_004c48c0* current;             // +0x4
    int file;                            // +0x8
};

extern char DAT_0051fdc0[256];
extern char DAT_005119b8[];

// FUNCTION: 0x4c58a0
int __stdcall FUN_004c58a0(Class_004c2ea0* file, char* dst, char* key, size_t size,
                           char* def)
{
    char full[256];
    strcpy(full, DAT_0051fdc0);
    strcat(full, key);
    int r;
    if (def) {
        r = file->current->FUN_004c48c0(dst, full, size, def);
        if (r) return 1;
        return file->current->FUN_004c48c0(dst, key, size, def);
    }
    r = file->current->FUN_004c48c0(dst, full, size, DAT_005119b8);
    if (r) return 1;
    return file->current->FUN_004c48c0(dst, key, size, DAT_005119b8);
}
