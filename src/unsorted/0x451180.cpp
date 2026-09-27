// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct PlayerInfo_451180 {
    char unknown_0[0x9b];
    unsigned short unknown_9b_0 : 4; // +0x9b
    unsigned short flag_9b_4 : 1;
    unsigned short unknown_9b_5 : 11;
};

struct Player_451180 {
    char unknown_0[0x27];
    PlayerInfo_451180* info;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game_451180 {
    char unknown_0[0x14];
    char unknown_14[0x475 - 0x14];   // +0x14
    unsigned int unknown_475_0 : 5;  // +0x475
    unsigned int flag_475_5 : 1;
    unsigned int unknown_475_6 : 26;
    char unknown_479[0x1b63 - 0x479];
    Player_451180 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;       // +0x2a42
};
#pragma pack(pop)

extern Game_451180* g_game;
extern char DAT_005119b8[];

void __stdcall FUN_00451090(char* name, int* d, int* c, int* b, int* a);
void __stdcall FUN_004c9890(void* obj, char* name, char* data, int d, int c, int b, int a);

// The flag is a bit of an unsigned short bitfield whose storage starts at the
// odd offset 0x9b (packed struct): that gives the byte load and "shr al, 4;
// test al, 1". An unsigned char bitfield folds to "test byte ptr".
// FUNCTION: 0x451180
void FUN_00451180(void)
{
    int a;
    int b;
    int c;
    int d;
    char name[32];

    FUN_00451090(name, &d, &c, &b, &a);
    if (g_game->players[g_game->localPlayer].info->flag_9b_4) {
        g_game->flag_475_5 = 1;
    }
    FUN_004c9890(g_game->unknown_14, name, DAT_005119b8, d, c, b, a);
}
