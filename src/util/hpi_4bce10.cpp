// Decompiled by Opus. Names are provisional.
#include <direct.h>
#include <stddef.h>

struct Class_004bce10 {
    char unknown_0[0x628];
    char cwd[0x100];                   // +0x628
};

extern Class_004bce10* GetDisplay();

// Stores the current directory of the current drive. The drive number is
// converted on each path separately (`n = d - '@'` in both branches); a
// single `d - '@'` after the if picks edx and interleaves the subtraction
// with the argument pushes.
// FUNCTION: 0x4bce10
void SaveStartDirectory()
{
    Class_004bce10* g = GetDisplay();
    char buf[12];
    char d = _getdrive() + '@';
    char* p = g->cwd;
    int n;
    if (buf != NULL)
        n = d - '@';
    else
        n = (char)(_getdrive() + '@') - '@';
    _getdcwd(n, p, 0x100);
}
