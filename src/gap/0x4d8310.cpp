// Decompiled by Claude Opus 5.5. Names are provisional.
//
// Best so far 80.6%. What differs: the original loads `block` into eax at
// entry, before `push ebx`; here the compiler loads it in the first loop's
// preheader and again on the path that skips the loop (an extra jmp), and the
// byte loop addresses the pattern as [edx + eax] where the original has
// [eax + edx]. Tried: a local pointer copied from block (28.9%), the first
// loop as do/while with its own count (28.9%), a for loop, both pointers
// incremented, compare operands swapped, char and unsigned char, the byte
// loop over block directly, declarations at the top (all 28.9% to 80.6%),
// and /Os (a different shape altogether).

// Breaks into the debugger where a block no longer holds its fill pattern:
// each whole dword must equal `fill`, and the bytes left over the first bytes
// of it.
// FUNCTION: 0x4d8310
void __cdecl FUN_004d8310(unsigned int* block, unsigned int fill, unsigned int size)
{
    while (size >= 4) {
        if (*block != fill)
            __asm int 3
        block++;
        size -= 4;
    }
    unsigned char* bytes = (unsigned char*)block;
    unsigned char* pattern = (unsigned char*)&fill;
    for (unsigned int i = 0; i < size; i++)
        if (bytes[i] != pattern[i])
            __asm int 3
}
