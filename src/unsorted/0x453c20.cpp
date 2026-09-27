// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Unless bit 0 of g_game+0x37f2f is set, looks for active players
// (state 3) that have not been heard from for field_37f31 * 30 ticks since
// the later of their last message time (+0x1c) and DAT_00512c7c. If all such
// players are on one team (+0xc), the first of them is passed to
// FUN_00453a50 (by its id at +0x4); otherwise FUN_00453a50(-1). With bit 0
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

struct Game_00453c20 {
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

extern Game_00453c20* g_game;
extern unsigned int DAT_00512c7c;

unsigned int FUN_004b6340();
void __stdcall FUN_00453a50(int value);

static inline int IsTimedOut(Player_00453c20* p, unsigned int now)
{
    if (p->active != 0 && p->state == 3) {
        unsigned int last = p->lastHeard;
        if (DAT_00512c7c > last)
            last = DAT_00512c7c;
        if (now - last > (unsigned int)(g_game->field_37f31 * 30))
            return 1;
    }
    return 0;
}

// The first loop walks a pointer (p++) and the second indexes the array;
// indexing in the first loop biases the loop pointer to +0xc instead.
// FUNCTION: 0x453c20
void FUN_00453c20()
{
    if (g_game->field_37f2f & 1)
        return;
    if (g_game->field_38a51 & 1) {
        DAT_00512c7c = FUN_004b6340();
        return;
    }
    unsigned int now = FUN_004b6340();
    int mixed = 0;
    int team = -1;
    int i;
    Player_00453c20* p = g_game->players;
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
            FUN_00453a50(q->id);
            return;
        }
    }
    FUN_00453a50(-1);
}
