// Decompiled by Sonnet. Names are provisional.
#include <string.h>

int __stdcall ReadGameRegistryValue(const char* key, void* buf, unsigned int* size);

extern unsigned int DAT_0051e828[0x2a8];

// FUNCTION: 0x490f40
void FUN_00490f40(void)
{
    unsigned int size = 0xaa0;
    if (ReadGameRegistryValue("CDLISTS", DAT_0051e828, &size) == 0) {
        memset(DAT_0051e828, 0, sizeof(DAT_0051e828));
    }
}
