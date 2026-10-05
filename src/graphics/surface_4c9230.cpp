// Decompiled by Opus. Names are provisional.
// Constructor of the reference-counted string handle (see 0x4c91b0) from the
// first len characters of a string. A null string shares the global empty
// string, whose count is g_emptyStringRefs. The class is named after this address
// because data/symbols.csv maps one name per constructor.
#include <stdlib.h>
#include <string.h>

extern int g_emptyStringRefs;
extern void* g_emptyString;

class Class_004c9230 {
public:
    char* ptr;

    Class_004c9230(const char* text, int len);
};

// FUNCTION: 0x4c9230
Class_004c9230::Class_004c9230(const char* text, int len)
{
    if (text == 0) {
        g_emptyStringRefs++;
        ptr = (char*)&g_emptyString;
    } else {
        int* block = (int*)malloc(len + 1 + sizeof(int));
        *block = 1;
        char* chars = (char*)(block + 1);
        strncpy(chars, text, len);
        chars[len] = 0;
        ptr = chars;
    }
}
