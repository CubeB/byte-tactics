// Decompiled by Opus. Names are provisional.
// Same shape as the C runtime's abort(): report, raise SIGABRT, exit code 3.
#include <signal.h>
#include <stdlib.h>

void FUN_004d8390(void);

// FUNCTION: 0x4da0c0
void FUN_004da0c0(void)
{
    FUN_004d8390();
    raise(SIGABRT);
    _exit(3);
}
