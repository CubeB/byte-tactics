// Decompiled by Opus. Names are provisional.
// Sibling of 0x451180: marks the local player's info, then reads the same
// five values and hands them to HAPINET_createnewgame.

#pragma pack(push, 1)
struct PlayerInfo_451540 {
    char unknown_0[0x97];
    unsigned short flag_97_0 : 1;    // +0x97
    unsigned short unknown_97_1 : 15;
};

struct Player_451540 {
    char unknown_0[0x27];
    PlayerInfo_451540* info;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x14];
    char unknown_14[0x1b63 - 0x14];  // +0x14
    Player_451540 players[10];       // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    short field_2a3c;                // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;       // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

void __stdcall BuildGameInfo(char* name, int* d, int* c, int* b, int* a);
void __stdcall HAPINET_createnewgame(void* obj, char* name, char* data, int d, int c, int b, int a);

// The flag is a bit of an unsigned short bitfield in the packed info struct:
// that gives "or byte ptr [m], 1"; an unsigned char or a plain byte "|= 1"
// goes through a register.
// FUNCTION: 0x451540
void CreateNetGame(void)
{
    int a;
    int b;
    int c;
    int d;
    char name[32];

    g_game->players[g_game->localPlayer].info->flag_97_0 = 1;
    BuildGameInfo(name, &d, &c, &b, &a);
    g_game->field_2a3c = 0;
    HAPINET_createnewgame(g_game->unknown_14, name, DAT_005119b8, d, c, b, a);
}
