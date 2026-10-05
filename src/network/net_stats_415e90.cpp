// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    int field_38a47;                   // +0x38a47
};
#pragma pack(pop)

struct Pair_00419560 {
    int a;
    int b;
};

extern Game* g_game;
extern int DAT_00511bc0;
extern int DAT_00511bc4;
extern int DAT_00511bc8;
extern int DAT_00511c20;
extern int DAT_00511c34;
extern int DAT_00511c48;
extern int DAT_00511c50;
extern Pair_00419560 DAT_00511a60[44];
extern Pair_00419560 DAT_00511c60[44];

// Same reset sequence as FUN_00419560 and CmdNetStats.
// FUNCTION: 0x415e90
void ResetNetStats()
{
    DAT_00511c20 = g_game->field_38a47;
    DAT_00511bc0 = 0;
    DAT_00511bc4 = 0;
    Pair_00419560* p = DAT_00511c60;
    for (int i = 0; i < 44; i++) {
        DAT_00511a60[i].a = 0;
        DAT_00511a60[i].b = 0;
        DAT_00511c60[i].a = 0;
        p->b = 0;
        p++;
    }
    DAT_00511c34 = 0;
    DAT_00511bc8 = 0;
    DAT_00511c48 = 0;
    DAT_00511c50 = 0;
}
