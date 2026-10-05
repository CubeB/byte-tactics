// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

// FUNCTION: 0x4baff0
char* __stdcall FUN_004baff0(char* a, char* b, char* c)
{
    strcpy(b, a);
    char* p = b + strlen(b) - 1;
    while (p >= b) {
        if (*p == '\\')
            break;
        if (*p == '.') {
            *p = '\0';
            break;
        }
        p--;
    }
    strcat(b, ".");
    strcat(b, c);
    return b;
}
