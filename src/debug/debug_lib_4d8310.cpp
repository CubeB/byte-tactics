// Decompiled by Claude Opus 5.5. Names are provisional.
//
// The walk through the block is a copy `p` of the parameter (that is what
// loads `block` into eax at entry), and the leftover bytes are compared by
// two pointers stepped together under a separate count: MSVC folds `pattern`
// into the block pointer plus a constant offset, giving [eax + edx]. Indexing
// both with one `i` gives [edx + eax] instead.

// Breaks into the debugger where a block no longer holds its fill pattern:
// each whole dword must equal `fill`, and the bytes left over the first bytes
// of it.
// FUNCTION: 0x4d8310
void __cdecl FUN_004d8310(unsigned int* block, unsigned int fill, unsigned int size)
{
    unsigned int* p = block;
    while (size >= 4) {
        if (*p != fill)
            __asm int 3
        p++;
        size -= 4;
    }
    unsigned char* bytes = (unsigned char*)p;
    unsigned char* pattern = (unsigned char*)&fill;
    for (unsigned int n = size; n > 0; n--, bytes++, pattern++)
        if (*bytes != *pattern)
            __asm int 3
}
