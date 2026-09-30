// Decompiled by Opus. Names are provisional.
// Returns whether the file dir + name exists.
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

// FUNCTION: 0x4de810
bool __cdecl FUN_004de810(char* dir, char* name)
{
    struct _stat st;
    char path[1000];
    strcpy(path, dir);
    strcat(path, name);
    return _stat(path, &st) == 0 ? true : false;
}
