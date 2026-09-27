// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Substring of the reference-counted string handle (see 0x4c9230 for the
// constructor from the first len characters of a string): builds the handle
// for this->ptr[start..end) into *out. Clamps start to 0 and end to the
// string length; an empty range shares the global empty string, whose count is
// DAT_0050a778. The class is named after this address.
//
// GAVE UP: bytes match except for two things a stronger model must fix.
// 1. count/end lands in esi and chars in edi, but the original has count in
//    edi and chars in esi. The allocation idiom of the matched sibling
//    0x4c9230 (int* block = (int*)malloc(n); *block = 1; char* chars =
//    (char*)(block + 1);) gives the original's `lea esi,[eax+4]; mov [eax],1`
//    here only when chars goes to esi, and that assignment needs the count in
//    edi; with this function's extra start/src/out and the inlined strlen scan
//    (which uses edi) the compiler always picks esi for the count. ~50 source
//    variants (allocation shape, declaration order, helper, statement order,
//    loop form) did not break the pair.
// 2. In the second error block the load of `out` is hoisted above the
//    increment; the original loads it after. Same register-pressure cause.
#include <stdlib.h>
#include <string.h>

extern int DAT_0050a778;
extern void* DAT_0050a77c;

class Class_004c9490 {
public:
    char* ptr;

    void FUN_004c9490(char** out, int start, int end);
};

// FUNCTION: 0x4c9490
void Class_004c9490::FUN_004c9490(char** out, int start, int end)
{
    int len = strlen(ptr);
    if (start < 0)
        start = 0;
    if (end > len)
        end = len;
    if (start >= end) {
        DAT_0050a778++;
        *out = (char*)&DAT_0050a77c;
        return;
    }
    char* src = ptr + start;
    end -= start;
    if (src == 0) {
        DAT_0050a778++;
        *out = (char*)&DAT_0050a77c;
        return;
    }
    char* chars = (char*)malloc(end + 5);
    int* block = (int*)chars;
    chars += 4;
    *block = 1;
    strncpy(chars, src, end);
    chars[end] = 0;
    *out = chars;
}
