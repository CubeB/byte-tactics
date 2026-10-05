// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

// FUNCTION: 0x4e1700
char IsPentiumOrBetter(void)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    if (info.dwProcessorType == 0x182 || info.dwProcessorType == 0x1e6)
        return 0;
    return 1;
}
