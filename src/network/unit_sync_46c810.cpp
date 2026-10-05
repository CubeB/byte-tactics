// Decompiled by Opus. Names are provisional.
// Returns 4 when neither handler is active; otherwise runs 0x46c2a0 and
// passes the local player's id and `msg` to the active handlers.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a42];
    unsigned char player;              // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

extern int DAT_0051e58c;
extern int DAT_0051e590;
extern int DAT_0051e594;
extern int* DAT_0051e574;
extern int (__stdcall* DAT_0051e548)(int, int);

void FUN_0046c2a0();
int __stdcall FUN_004caa00(int arg1, int arg2);

// FUNCTION: 0x46c810
int __stdcall FUN_0046c810(int msg)
{
    if ((DAT_0051e590 != 0 && (DAT_0051e594 & 8)) || DAT_0051e58c != 0) {
        FUN_0046c2a0();
        if (DAT_0051e590 != 0 && (DAT_0051e594 & 8)) {
            if (FUN_004caa00(DAT_0051e574[g_game->player], msg))
                DAT_0051e590 = 0;
        }
        if (DAT_0051e58c != 0)
            return DAT_0051e548(DAT_0051e574[g_game->player], msg);
        return 0;
    }
    return 4;
}
