// Decompiled by Haiku. Names are provisional.
#include <stdio.h>

struct Class_004cb7d0 {
    char unknown_0[0xc];
    FILE* file;

    void Close();
};

// FUNCTION: 0x4cb7d0
void Class_004cb7d0::Close()
{
    if (file != NULL) {
        fclose(file);
    }
}
