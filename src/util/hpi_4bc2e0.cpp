// Decompiled by Opus. Names are provisional.
// Writes the current drive letter ("C") into buf as a string.
#include <direct.h>

// FUNCTION: 0x4bc2e0
void __stdcall GetCurrentDriveLetter(char* buf)
{
    buf[0] = _getdrive() + '@';
    buf[1] = 0;
}
