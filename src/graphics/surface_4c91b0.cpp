// Decompiled by Opus. Names are provisional.
// Constructor of the reference-counted string handle from a C string (see
// 0x4c9180 for the default constructor, 0x4c91a0 for the copy constructor and
// 0x4c93b0 for assignment). The handle points at the characters; the
// reference count is the int just before them. An empty or null string shares
// the global empty string, whose count is DAT_0050a778. The class is named
// after this address because data/symbols.csv maps one name per constructor
// (Class_004c91a0::Class_004c91a0 is already the copy constructor).
#include <stdlib.h>
#include <string.h>

extern int DAT_0050a778;
extern void* DAT_0050a77c;

class Class_004c91b0 {
public:
    char* ptr;

    Class_004c91b0(const char* text);
};

// FUNCTION: 0x4c91b0
Class_004c91b0::Class_004c91b0(const char* text)
{
    char* chars;
    if (text == 0 || *text == 0) {
        DAT_0050a778++;
        chars = (char*)&DAT_0050a77c;
    } else {
        int* block = (int*)malloc(strlen(text) + 1 + sizeof(int));
        *block = 1;
        chars = (char*)(block + 1);
        strcpy(chars, text);
    }
    ptr = chars;
}
