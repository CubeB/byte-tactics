// Decompiled by Opus. Names are provisional.
// Returns the index of the first free (zero) slot of a 100-entry global
// table, or -1 when it is full.

extern int DAT_00511df0[];

// FUNCTION: 0x421150
int FUN_00421150()
{
    for (int i = 0; i < 100; i++) {
        if (DAT_00511df0[i] == 0) {
            return i;
        }
    }
    return -1;
}
