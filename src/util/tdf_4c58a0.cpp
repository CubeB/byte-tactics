// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Looks a key up in a TDF section: builds the key by prepending the current
// section name (g_language), tries that first and falls back to the plain
// key. param_5 (or the empty default DAT_005119b8) is the TDF default value.
// param_1 is the open file object, its section parser at +4.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class TdfRecord {
public:
    char unknown_0[0x19];
    int GetFieldString(char* dst, char* key, size_t size, char* def);
};

class TdfFile {
public:
    int root;                            // +0x0
    TdfRecord* current;                  // +0x4
    int file;                            // +0x8
};

extern char g_language[256];
extern char DAT_005119b8[];

// FUNCTION: 0x4c58a0
int __stdcall GetLocalizedString(TdfFile* file, char* dst, char* key, size_t size,
                           char* def)
{
    char full[256];
    strcpy(full, g_language);
    strcat(full, key);
    // Intermediate int keeps each argument in the original's register.
    int r;
    if (def) {
        r = file->current->GetFieldString(dst, full, size, def);
        if (r) return 1;
        return file->current->GetFieldString(dst, key, size, def);
    }
    r = file->current->GetFieldString(dst, full, size, DAT_005119b8);
    if (r) return 1;
    return file->current->GetFieldString(dst, key, size, DAT_005119b8);
}
