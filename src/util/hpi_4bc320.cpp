// Decompiled by Opus. Names are provisional.
// Gets the current directory of a drive ("C:..." or the current drive when
// null) into buf and returns buf. The drive number is computed on each path
// (`- '@'` in both branches, tail-merged into one `sub`); a single `d - '@'`
// after the if gives `add eax, -0x40` scheduled after the first push.
#include <direct.h>
#include <stddef.h>

// FUNCTION: 0x4bc320
char* __stdcall FUN_004bc320(char* drive, char* buf, int size)
{
    int n;
    if (drive == NULL)
        n = (char)(_getdrive() + '@') - '@';
    else
        n = *drive - '@';
    _getdcwd(n, buf, size);
    return buf;
}
