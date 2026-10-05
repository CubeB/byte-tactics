// Decompiled by space-bunny-free. Names are provisional.
// Builds the player tables: three arrays of ten, then for each slot its player
// block, that player's allies block, a scoreboard, the scoreboard's ppScores
// block and a zeroed scores block, every block allocated through FUN_004d83b0
// with a name wsprintfA formats from the slot number. Any allocation that
// comes back null tears the lot down again through FUN_0046c190 and reports 1,
// so the slot loop only records the failure in a flag and breaks; the
// epilogue tests that flag.
// Both loops are `while (1)` with the test as an early `break`, not a `for`:
// /O2 rotates a `for` and gives a bottom test, the original tests at the top.
// The pointer walk in the inner loop must be three separate statements
// (store, byte offset, pointer step) or /O2 turns the pointer into an
// induction variable and emits `add ecx,4` with a `[ecx-4]` store.
// The inner loop writes 0x48 bytes of pointers into the 0x24 byte ppScores
// block, the original's bug: 18 pointers of 4 bytes into a 36 byte block, so
// 36 bytes land past the end of the allocation.
#include <string.h>
#include <windows.h>

struct PlayerInfo_0046bce0 {     // 0x18 bytes
    char unknown_0[0x14];
    void* allies;                 // +0x14
};

struct ScoreBoard_0046bce0 {     // 0xc bytes
    char unknown_0[8];
    void* ppScores;               // +0x8
};

extern PlayerInfo_0046bce0** DAT_0051e574;
extern ScoreBoard_0046bce0** DAT_0051e57c;
extern char** DAT_0051e550;

void FUN_0046c190();
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

// FUNCTION: 0x46bce0
int FUN_0046bce0()
{
    char name[64];
    int failed;
    int i;

    DAT_0051e574 = (PlayerInfo_0046bce0**)FUN_004d83b0("PlayersArray", 0x28);
    DAT_0051e57c = (ScoreBoard_0046bce0**)FUN_004d83b0("ScoreBoardsArray", 0x28);
    DAT_0051e550 = (char**)FUN_004d83b0("ScoresArray", 0x28);
    if (DAT_0051e574 == 0)
        goto failed;
    if (DAT_0051e57c == 0)
        goto failed;
    if (DAT_0051e550 == 0)
        goto failed;
    memset(DAT_0051e574, 0, 0x28);
    memset(DAT_0051e57c, 0, 0x28);
    memset(DAT_0051e550, 0, 0x28);
    failed = 0;
    i = 0;
    while (1) {
        if (i >= 10)
            break;
        wsprintfA(name, "PlayerInfo%d", i);
        DAT_0051e574[i] = (PlayerInfo_0046bce0*)FUN_004d83b0(name, 0x18);
        if (DAT_0051e574[i] == 0) {
            failed = 1;
            break;
        }
        wsprintfA(name, "Allies%d", i);
        DAT_0051e574[i]->allies = FUN_004d83b0(name, 0x28);
        if (DAT_0051e574[i]->allies == 0) {
            failed = 1;
            break;
        }
        wsprintfA(name, "ScoreBoard%d", i);
        DAT_0051e57c[i] = (ScoreBoard_0046bce0*)FUN_004d83b0(name, 0xc);
        if (DAT_0051e57c[i] == 0) {
            failed = 1;
            break;
        }
        wsprintfA(name, "ppScores%d", i);
        DAT_0051e57c[i]->ppScores = FUN_004d83b0(name, 0x24);
        if (DAT_0051e57c[i]->ppScores == 0) {
            failed = 1;
            break;
        }
        memset(DAT_0051e57c[i]->ppScores, 0, 0x24);
        wsprintfA(name, "Scores%d", i);
        DAT_0051e550[i] = (char*)FUN_004d83b0(name, 0x48);
        if (DAT_0051e550[i] == 0) {
            failed = 1;
            break;
        }
        memset(DAT_0051e550[i], 0, 0x48);
        int** slot = (int**)DAT_0051e57c[i]->ppScores;
        int j = 0;
        while (1) {
            if (j >= 0x48)
                break;
            *slot = (int*)(DAT_0051e550[i] + j);
            j += 8;
            slot++;
        }
        i++;
    }
    if (!failed)
        return 0;
failed:
    FUN_0046c190();
    return 1;
}
