// Decompiled by Opus. Names are provisional.
// Drains the ring buffer that PopKey pops from (0 when empty).

int PopKey(void);

// FUNCTION: 0x4257c0
void FUN_004257c0(void)
{
    while (PopKey() != 0) {
    }
}
