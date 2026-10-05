// Decompiled by Opus. Names are provisional.
// Adds an amount to the player's total (DAT_00511bc0) and, for kinds 2..44,
// bumps the per-kind count and total (the tables FUN_00415e90 resets from
// kind 1, at DAT_00511a60 and DAT_00511c60).

extern int DAT_00511bc0[];
extern int DAT_00511a58[45][2];
extern int DAT_00511c58[45][2];

// FUNCTION: 0x415ef0
void __stdcall FUN_00415ef0(unsigned char kind, int amount, int player)
{
    DAT_00511bc0[player] += amount;
    if (kind > 1 && kind < 0x2d) {
        DAT_00511a58[kind][player]++;
        DAT_00511c58[kind][player] += amount;
    }
}
