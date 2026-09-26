// Decompiled by Opus. Names are provisional.

struct PlayerInfo_451180 {
    char unknown_0[0x98];
    unsigned int unknown_98_0 : 28;  // +0x98
    unsigned int flag_98_28 : 1;
    unsigned int unknown_98_29 : 3;
};

#pragma pack(push, 1)
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

// FUNCTION: 0x451180
void FUN_00451180(void)
{
    int a;
    int b;
    int c;
    int d;
    char name[32];

    FUN_00451090(name, &d, &c, &b, &a);
    // Best version (85.4%): the original loads only byte 0x9b and does
    // "shr al, 4; test al, 1"; this dword bitfield gives "mov eax, [+0x98];
    // shr eax, 0x1c; test al, 1", and every byte-sized form tried folds to
    // "test byte ptr [+0x9b], 0x10".
    if (g_game->players[g_game->localPlayer].info->flag_98_28) {
        g_game->flag_475_5 = 1;
    }
    FUN_004c9890(g_game->unknown_14, name, DAT_005119b8, d, c, b, a);
}
