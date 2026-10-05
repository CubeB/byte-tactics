// Decompiled by Opus. Names are provisional.
// Returns true if some alliance (0..4) contains every counted player, i.e.
// the players still in the game are all on one side.

#pragma pack(push, 1)
struct Player_004468c0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x13f - 0x74];
    unsigned char alliance;            // +0x13f
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004468c0 players[10];       // +0x1b63
    char unknown_2851[0x2a44 - 0x2851];
    unsigned short bit0 : 1;           // +0x2a44
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short rest : 13;
};
#pragma pack(pop)

extern Game* g_game;

// With both calls in one expression MSVC calls the later-declared one first.
int CountHumanPlayers();
int CountComputerPlayers();

static inline int IsPlaying(Player_004468c0* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted(Player_004468c0* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

static inline int CountAlliance(int alliance)
{
    if (alliance == 5)
        return 0;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player_004468c0* p = &g_game->players[i];
        if (g_game->bit2) {
            if (p->alliance == alliance && IsPlaying(p) && IsCounted(p))
                count++;
        } else {
            if (p->alliance == alliance && IsPlaying(p))
                count++;
        }
    }
    return count;
}

// FUNCTION: 0x4468c0
char FUN_004468c0()
{
    int total = CountComputerPlayers() + CountHumanPlayers();
    for (int alliance = 0; alliance < 5; alliance++) {
        if (CountAlliance(alliance) == total)
            return 1;
    }
    return 0;
}
