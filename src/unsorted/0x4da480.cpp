// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>
#include <string.h>

extern char DAT_0050cd38;
extern char DAT_0050ced8;
extern char DAT_0050c958;

// FUNCTION: 0x4da480
HGLOBAL FUN_004da480(int id)
{
    char* src = 0;
    int size = 0;
    switch (id) {
    case 0x66:
        src = &DAT_0050cd38;
        size = 0x1a0;
        break;
    case 0x67:
        src = &DAT_0050ced8;
        size = 0x2c0;
        break;
    case 0x81:
        src = &DAT_0050c958;
        size = 0x3e0;
        break;
    }
    if (src != 0 && size != 0) {
        HGLOBAL mem = GlobalAlloc(0x40, size);
        memcpy(mem, src, size);
        return mem;
    }
    return 0;
}
