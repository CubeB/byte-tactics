// Decompiled by Claude Opus 5.5. Names are provisional.
// Substring of the reference-counted string handle (see 0x4c9180 for the
// default constructor and 0x4c9230 for the constructor from the first len
// characters of a string): returns a new handle for ptr[start..end), with
// start clamped to 0 and end to the string length. The handle is returned by
// value through the hidden return pointer, and both constructors are inlined.
// The class is named after this address.
#include <stdlib.h>
#include <string.h>

extern int DAT_0050a778;
extern void* DAT_0050a77c;

class Class_004c9490 {
public:
    char* ptr;

    Class_004c9490()
    {
        DAT_0050a778++;
        ptr = (char*)&DAT_0050a77c;
    }
    Class_004c9490(const char* text, int len)
    {
        if (text == 0) {
            DAT_0050a778++;
            ptr = (char*)&DAT_0050a77c;
        } else {
            int* block = (int*)malloc(len + 1 + sizeof(int));
            *block = 1;
            char* chars = (char*)(block + 1);
            strncpy(chars, text, len);
            chars[len] = 0;
            ptr = chars;
        }
    }

    Class_004c9490 FUN_004c9490(int start, int end) const;
};

// FUNCTION: 0x4c9490
Class_004c9490 Class_004c9490::FUN_004c9490(int start, int end) const
{
    int len = strlen(ptr);
    if (start < 0)
        start = 0;
    if (end > len)
        end = len;
    if (start >= end)
        return Class_004c9490();
    return Class_004c9490(ptr + start, end - start);
}
