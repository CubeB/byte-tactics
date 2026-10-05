// Decompiled by Opus. Names are provisional.
// Network statistics: counts a packet of `size` bytes as sent or received
// (the counters reset by 0x415e90) and adds any positive overhead.

extern int DAT_00511dc8;
extern int DAT_00511c34;
extern int DAT_00511bc8;
extern int DAT_00511c48;
extern int DAT_00511c50;

// FUNCTION: 0x415f40
void __stdcall FUN_00415f40(int size, int overhead, int sent)
{
    if (overhead > 0) {
        DAT_00511dc8 += overhead;
    }
    if (sent != 0) {
        DAT_00511c34++;
        DAT_00511bc8 += size;
    } else {
        DAT_00511c48++;
        DAT_00511c50 += size;
    }
}
