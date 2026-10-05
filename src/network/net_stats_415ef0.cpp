// Decompiled by Opus. Names are provisional.
// Adds an amount to the player's total (DAT_00511bc0) and, for kinds 2..44,
// bumps the per-kind count and total (the tables ResetNetStats resets from
// kind 1, at DAT_00511a60 and DAT_00511c60).

extern int DAT_00511bc0[];
extern int g_messageCountByType[45][2];
extern int g_messageBytesByType[45][2];

// FUNCTION: 0x415ef0
void __stdcall CountMessage(unsigned char kind, int amount, int player)
{
    DAT_00511bc0[player] += amount;
    if (kind > 1 && kind < 0x2d) {
        g_messageCountByType[kind][player]++;
        g_messageBytesByType[kind][player] += amount;
    }
}
