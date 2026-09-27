// Decompiled by Claude Opus 5.5. Names are provisional.
// Network statistics text: about once a second (more than 30 ticks), turns
// the packet and byte counters (see 0x415f40 and 0x416150) into per-second
// rates and a compression percentage, then prints the latest values.
// Match notes: the "previous" counters are all updated after the rates are
// computed (the scheduler moves the stores up again), which is what keeps the
// current counters in callee-saved registers; and it needs a header set
// (headers.py: <windows.h> <stdio.h> <math.h>).
#include <windows.h>
#include <stdio.h>
#include <math.h>
extern int DAT_00511bc0;
extern int DAT_00511bc4;
extern int DAT_00511bc8;
extern int DAT_00511c34;
extern int DAT_00511c48;
extern int DAT_00511c50;
extern int DAT_00511dc8;
extern unsigned int DAT_00511dd8;
extern int DAT_00511c40;
extern int DAT_00511c44;
extern int DAT_00511dcc;
extern int DAT_00511a4c;
extern int DAT_00511bcc;
extern int DAT_00511c24;
extern int DAT_00511c28;
extern unsigned int DAT_00511c38;
extern unsigned int DAT_00511c3c;
extern int DAT_00511a44;
extern unsigned int DAT_00511c4c;
extern unsigned int DAT_00511c30;
extern unsigned int DAT_00511dc4;
extern unsigned int DAT_00511dc0;

unsigned int __cdecl FUN_004b6340();

// FUNCTION: 0x415fa0
void __stdcall FUN_00415fa0(char* text)
{
    unsigned int now = FUN_004b6340();
    unsigned int elapsed = now - DAT_00511dd8;
    if (elapsed > 30) {
        DAT_00511dd8 = now;
        DAT_00511c38 = (DAT_00511bc0 * 30 - DAT_00511c40 * 30) / elapsed;
        int packets = DAT_00511bc4 - DAT_00511c44;
        DAT_00511c3c = packets * 30 / elapsed;
        DAT_00511c40 = DAT_00511bc0;
        DAT_00511c44 = DAT_00511bc4;
        if (packets > 0)
            DAT_00511a44 = (DAT_00511dcc + packets - DAT_00511dc8) * 100 / packets;
        else
            DAT_00511a44 = 0;
        DAT_00511dcc = DAT_00511dc8;
        DAT_00511c4c = (DAT_00511bc8 * 30 - DAT_00511a4c * 30) / elapsed;
        DAT_00511c30 = (DAT_00511c50 * 30 - DAT_00511bcc * 30) / elapsed;
        DAT_00511dc4 = (DAT_00511c34 * 30 - DAT_00511c24 * 30) / elapsed;
        DAT_00511dc0 = (DAT_00511c48 * 30 - DAT_00511c28 * 30) / elapsed;
        DAT_00511a4c = DAT_00511bc8;
        DAT_00511bcc = DAT_00511c50;
        DAT_00511c24 = DAT_00511c34;
        DAT_00511c28 = DAT_00511c48;
    }
    sprintf(text, "pS=%4d pR=%4d (S=%d/%4d, R=%d/%4d) C=%3d%%\n", DAT_00511c3c, DAT_00511c38,
            DAT_00511dc4, DAT_00511c4c, DAT_00511dc0, DAT_00511c30, DAT_00511a44);
}
