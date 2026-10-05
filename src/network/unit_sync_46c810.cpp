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
extern int g_reportFlags;
extern int* DAT_0051e574;
extern int (__stdcall* DAT_0051e548)(int, int);

void FillScoreTables();
int __stdcall RIReportGameChat(int arg1, int arg2);

// FUNCTION: 0x46c810
int __stdcall ReportGameChat(int msg)
{
    if ((DAT_0051e590 != 0 && (g_reportFlags & 8)) || DAT_0051e58c != 0) {
        FillScoreTables();
        if (DAT_0051e590 != 0 && (g_reportFlags & 8)) {
            if (RIReportGameChat(DAT_0051e574[g_game->player], msg))
                DAT_0051e590 = 0;
        }
        if (DAT_0051e58c != 0)
            return DAT_0051e548(DAT_0051e574[g_game->player], msg);
        return 0;
    }
    return 4;
}
