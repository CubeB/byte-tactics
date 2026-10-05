// Decompiled by Opus. Names are provisional.
// Network statistics: about once a second (more than 30 ticks), turns the
// sent and received byte counters (see 0x415f40) into per-second rates, and
// returns the latest rates. Needs a header (headers.py: <stdlib.h>) for the
// first rate's store to be scheduled after the second counter's update.
#include <stdlib.h>

extern int DAT_00511bc8;
extern int DAT_00511c50;
extern int DAT_00511c2c;
extern int DAT_00511a40;
extern unsigned int DAT_00511ddc;
extern unsigned int DAT_00511a48;
extern unsigned int DAT_00511a50;

unsigned int __cdecl FUN_004b6340();

// FUNCTION: 0x416150
void __stdcall FUN_00416150(unsigned int* sent, unsigned int* received)
{
    unsigned int now = FUN_004b6340();
    unsigned int elapsed = now - DAT_00511ddc;
    if (elapsed > 30) {
        DAT_00511ddc = now;
        DAT_00511a48 = (DAT_00511bc8 * 30 - DAT_00511c2c * 30) / elapsed;
        DAT_00511a50 = (DAT_00511c50 * 30 - DAT_00511a40 * 30) / elapsed;
        DAT_00511c2c = DAT_00511bc8;
        DAT_00511a40 = DAT_00511c50;
    }
    *sent = DAT_00511a48;
    *received = DAT_00511a50;
}
