// Decompiled by Opus. Names are provisional.
#include <stdio.h>

// Writes a buffer to a new file; returns the number of bytes written or -1.
// FUNCTION: 0x4bc290
int __stdcall FUN_004bc290(char* filename, void* data, int size)
{
    FILE* f = fopen(filename, "wb");
    if (f == NULL) {
        return -1;
    }
    int written = fwrite(data, 1, size, f);
    fclose(f);
    return written;
}
