// Decompiled by Opus. Names are provisional.
// Strips the directory from a path in place ("a\\b\\c.txt" -> "c.txt");
// compare 0x4bb120, which keeps the directory instead.
#include <string.h>

// FUNCTION: 0x4bb150
char* __stdcall StripPath(char* path)
{
    int len = strlen(path);
    int i = len - 1;
    while (i >= 0 && path[i] != '\\')
        i--;
    // Two indices into the same array, not pointers: keeps the offset addressing.
    int j = i + 1;
    int k = 0;
    do {
        path[k] = path[j];
        k++;
    } while (path[j++] != 0);
    return path;
}
