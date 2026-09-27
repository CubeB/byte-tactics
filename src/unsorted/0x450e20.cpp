// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Player_00450e20 {
    int active;                        // +0x0
    int id;                            // +0x4
    char unknown_8[0x22 - 0x8];
    unsigned char reason;              // +0x22
    char unknown_23[0x73 - 0x23];
    char state;                        // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00450e20 {
    char unknown_0[0x14];
    char field_14[0x1b63 - 0x14];
    Player_00450e20 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char local_player;        // +0x2a42
    char unknown_2a43;
    unsigned short flags_2a44;         // +0x2a44
    char unknown_2a46[0x3923b - 0x2a46];
    unsigned short field_3923b;        // +0x3923b
    char unknown_3923d[4];
};
#pragma pack(pop)

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

extern Game_00450e20* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

void __stdcall FUN_00452cc0(int id);
void FUN_0046c190();
int __stdcall FUN_004c9f90(void* net);
void __stdcall FUN_004b4fd0(void (*callback)(int), int param);
void __stdcall FUN_004b6230(char* message);

static inline int IsPlaying(Player_00450e20* player)
{
    if (player->active == 0)
        return 0;
    if (player->state == 1 || player->state == 2)
        return 1;
    return 0;
}

// FUNCTION: 0x450e20
void FUN_00450e20()
{
    if (DAT_00506dbc != 0) {
        DAT_00513000.FUN_004618a0(1);
    }
    if (g_game->flags_2a44 & 1) {
        for (int i = 0; i < 10; i++) {
            if (IsPlaying(&g_game->players[i])) {
                FUN_00452cc0(g_game->players[i].id);
            }
        }
        FUN_0046c190();
    }
    FUN_004c9f90(g_game->field_14);
    FUN_004b4fd0(0, 0);
    g_game->field_3923b |= 4;
    int reason = g_game->players[g_game->local_player].reason;
    char* text;
    if (reason != 0) {
        switch (reason) {
        case 4:
            text = "You did not have the correct password";
            break;
        case 3:
            text = "The game is closed";
            break;
        case 5:
            text = "The game is full";
            break;
        case 6:
            text = "You have lost connection with the game";
            break;
        case 7:
            text = "You need a unit you don't have for this game";
            break;
        case 8:
            text = "You need a newer version of the game to enter";
            break;
        case 9:
            text = "No watching is allowed for this game";
            break;
        case 10:
            text = "The creator has left the game";
            break;
        default:
            text = "You were rejected from the game";
            break;
        }
    } else {
        text = 0;
    }
    FUN_004b6230(text);
}
