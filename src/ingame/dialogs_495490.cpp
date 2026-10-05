// Decompiled by space-bunny-free, finished by mimo-v2.6-flash. Names are provisional.
// Runs one game update per pending time step (g_game->steps), profiling each
// phase into g_game->prof.acc[]; the profile object also lives at +0x38d85 with
// total at +4, the display copy at +8 and the accumulators at +0x2c.

unsigned int __cdecl FUN_004b6560(void);

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

class Class_0046a400 {                 // frame-time profile, embedded at g_game+0x38d85
public:
    int last;                           // +0x00
    int total;                          // +0x04
    int values[9];                      // +0x08
    int acc[9];                         // +0x2c

    void FUN_0046a400(int i) {
        unsigned int t = FUN_004b6560();
        int d = t - last;
        int v = acc[i];
        v = v + d;
        acc[i] = v;
        last = t;
    }
};

struct Player_495490 {                  // 0x14b bytes
    char unknown_0[0x14b];
};

#pragma pack(push, 1)
struct Game_495490 {
    char unknown_0[0x1b63];
    Player_495490 players[10];          // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x38a3b - 0x2a43];
    int steps;                          // +0x38a3b
    char unknown_38a3f[0x38a47 - 0x38a3f];
    int ticks;                          // +0x38a47
    char unknown_38a4b[0x38d85 - 0x38a4b];
    Class_0046a400 prof;                // +0x38d85
};
#pragma pack(pop)

extern Game_495490* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

void FUN_00453d40(void);
void FUN_0048ad30(void);
void FUN_0049b720(void);
void FUN_00420f30(void);
void FUN_00464f80(void);
void FUN_00424050(void);
void FUN_00415b30(void);
void FUN_00490c40(void);
void FUN_00437de0(void);
void FUN_0041ca10(void);
void FUN_00471eb0(void);
void FUN_00466580(void);
void FUN_00428bd0(void);
void FUN_00428be0(void);
void FUN_00428bf0(void);
void FUN_00463ef0(void);
void FUN_00482130(void);
void __stdcall FUN_00457d30(Player_495490* player);

// FUNCTION: 0x495490
void __stdcall FUN_00495490(int showStats)
{
    int n = g_game->steps;

    while (n--) {
        g_game->ticks++;

        if (showStats) {
            FUN_00453d40();
            g_game->prof.FUN_0046a400(0);
        }
        FUN_0048ad30();
        g_game->prof.FUN_0046a400(1);
        FUN_0049b720();
        g_game->prof.FUN_0046a400(7);
        FUN_00420f30();
        g_game->prof.FUN_0046a400(8);
        FUN_00464f80();
        g_game->prof.FUN_0046a400(2);

        FUN_00424050();
        FUN_00415b30();
        FUN_00490c40();
        FUN_00437de0();
        FUN_0041ca10();
        g_game->prof.FUN_0046a400(8);

        FUN_00471eb0();
        g_game->prof.FUN_0046a400(6);
        FUN_00466580();
        g_game->prof.FUN_0046a400(8);

        if (showStats && DAT_00506dbc != 0) {
            FUN_00457d30(&g_game->players[g_game->localPlayer]);
            DAT_00513000.FUN_004618a0(0);
            g_game->prof.FUN_0046a400(0);
        }
    }

    FUN_00428bd0();
    FUN_00428be0();
    FUN_00428bf0();
    FUN_00463ef0();
    FUN_00482130();
    g_game->prof.FUN_0046a400(8);
}
