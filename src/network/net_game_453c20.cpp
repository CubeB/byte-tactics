// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Unless bit 0 of g_game+0x37f2f is set, looks for active players
// (state 3) that have not been heard from for field_37f31 * 30 ticks since
// the later of their last message time (+0x1c) and g_timeoutTimerStart. If all such
// players are on one team (+0xc), the first of them is passed to
// OpenTimeoutDialog (by its id at +0x4); otherwise OpenTimeoutDialog(-1). With bit 0
// of +0x38a51 set it only restarts the timer.

#pragma pack(push, 1)
struct Player_00453c20 {
    int active;                         // +0x0
    int id;                             // +0x4
    char unknown_8[4];                  // +0x8
    int team;                           // +0xc
    char unknown_10[0xc];               // +0x10
    unsigned int lastHeard;             // +0x1c
    char unknown_20[0x53];              // +0x20
    char state;                         // +0x73
    char unknown_74[0x14b - 0x74];      // +0x74
};

struct Game {
    char unknown_0[0x1b63];
    Player_00453c20 players[10];        // +0x1b63
    char unknown_2851[0x37f2f - 0x2851];
    unsigned char field_37f2f;          // +0x37f2f
    char unknown_37f30[1];              // +0x37f30
    int field_37f31;                    // +0x37f31
    char unknown_37f35[0x38a51 - 0x37f35];
    unsigned char field_38a51;          // +0x38a51
};
#pragma pack(pop)

extern Game* g_game;
extern unsigned int g_timeoutTimerStart;

unsigned int GetTicks();
void __stdcall OpenTimeoutDialog(int value);

static inline int IsTimedOut(Player_00453c20* p, unsigned int now)
{
    if (p->active != 0 && p->state == 3) {
        unsigned int last = p->lastHeard;
        if (g_timeoutTimerStart > last)
            last = g_timeoutTimerStart;
        if (now - last > (unsigned int)(g_game->field_37f31 * 30))
            return 1;
    }
    return 0;
}

// FUNCTION: 0x453c20
void CheckPlayerTimeouts()
{
    if (g_game->field_37f2f & 1)
        return;
    if (g_game->field_38a51 & 1) {
        g_timeoutTimerStart = GetTicks();
        return;
    }
    unsigned int now = GetTicks();
    int mixed = 0;
    int team = -1;
    int i;
    Player_00453c20* p = g_game->players;
    // Walks a pointer: indexing here would bias the loop pointer to +0xc.
    for (i = 0; i < 10; i++, p++) {
        if (IsTimedOut(p, now)) {
            if (team < 0)
                team = p->team;
            else if (team != p->team)
                mixed = 1;
        }
    }
    for (i = 0; i < 10; i++) {
        Player_00453c20* q = &g_game->players[i];
        if (IsTimedOut(q, now) && !mixed) {
            OpenTimeoutDialog(q->id);
            return;
        }
    }
    OpenTimeoutDialog(-1);
}
