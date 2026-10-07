// Decompiled by Opus. Names are provisional.
// Assignment of a C string to the reference-counted string handle (see
// 0x4c91b0 for the constructor from a C string and 0x4c93b0 for assignment
// from another handle): releases the old characters, then shares the global
// empty string or copies the text into a new block whose first int is the
// reference count.
#include <stdlib.h>
#include <string.h>

extern int g_emptyStringRefs;
extern void* g_emptyString;

class Class_004c93f0 {
public:
    char* ptr;

    Class_004c93f0* AssignText(const char* text);
};

// FUNCTION: 0x4c93f0
Class_004c93f0* Class_004c93f0::AssignText(const char* text)
{
    // The release, phrased as in the destructor body 0x4c9390.
    ((int*)ptr)[-1]--;
    int* old = (int*)ptr - 1;
    if (((int*)ptr)[-1] == 0)
        free(old);
    char* chars;
    if (text == 0 || *text == 0) {
        g_emptyStringRefs++;
        chars = (char*)&g_emptyString;
    } else {
        int* block = (int*)malloc(strlen(text) + 1 + sizeof(int));
        *block = 1;
        chars = (char*)(block + 1);
        strcpy(chars, text);
    }
    ptr = chars;
    return this;
}
