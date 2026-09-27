// Decompiled by Opus. Names are provisional.
// Closes a file record: closes its FILE (if open), then frees its buffer at
// +0x8 and the record itself.
#include <stdio.h>

struct File_004be070 {
    FILE* file;                        // +0x0
    int unknown_4;
    int* buffer;                       // +0x8
};

void FUN_004d85a0(int* param_1);

// FUNCTION: 0x4be070
void __stdcall FUN_004be070(File_004be070* f)
{
    if (f != 0) {
        if (f->file != 0)
            fclose(f->file);
        FUN_004d85a0(f->buffer);
        FUN_004d85a0((int*)f);
    }
}
