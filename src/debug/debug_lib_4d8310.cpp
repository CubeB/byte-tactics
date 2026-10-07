// Decompiled by Claude Opus 5.5. Names are provisional.

// Breaks into the debugger where a block no longer holds its fill pattern:
// each whole dword must equal `fill`, and the bytes left over the first bytes
// of it.
// FUNCTION: 0x4d8310
void __cdecl CheckFillPattern(unsigned int* block, unsigned int fill, unsigned int size)
{
    // Walk a copy of block, not the parameter itself.
    unsigned int* p = block;
    while (size >= 4) {
        if (*p != fill)
            __asm int 3
        p++;
        size -= 4;
    }
    // Leftover bytes: two pointers stepped together under a separate count, not one index.
    unsigned char* bytes = (unsigned char*)p;
    unsigned char* pattern = (unsigned char*)&fill;
    for (unsigned int n = size; n > 0; n--, bytes++, pattern++)
        if (*bytes != *pattern)
            __asm int 3
}
