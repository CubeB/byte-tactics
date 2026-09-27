// Decompiled by space-bunny-free. Names are provisional.
// Sets each player's two camera floats (player +0x8c and +0x98) from the game
// mode: mode 1 copies the net player's stored floats, mode 2 the per-slot
// integers converted to float, mode 3 the colour-index player's info values
// times 100 (read from players[index] but stored into players[i], as the
// original does, so a player with a colour index other than its own slot
// gets the other player's view position).
//
// Still differs (68.4%): one register-allocation decision about the players
// offset induction variable. The original keeps i*0x14b in its stack slot
// ([esp+0x14]), loading it into ecx at the top of the loop for
// `lea esi,[eax+ecx+0x1b63]` and into edx at the bottom to add 0x14b, so
// esi holds the player pointer and the loop starts with
// `xor ebp,ebp` (the slots IV) initialising both memory IVs. This build
// promotes the offset IV into esi and coalesces the player pointer into the
// same register, which also reorders the g_game load in case 3 and the flag
// load at the loop top, and makes three jumps shorter. Tried without luck:
// pointer declared in/out of the loop, `int`/`long`/`unsigned`/`short` loop
// counters, while/do forms, if/else instead of the switch, net or slot
// pointer locals, 1-D vs 2-D vs raw-offset address forms, `&arr[i]` vs
// `arr + i`, storing through players[i] in one case at a time, an unused
// compare on the pointer, and 0..400 dummy externs before and inside the
// function (the "compiler state" trick). Frame size, slot order and every
// other instruction already match.
#pragma pack(push, 1)

class Class_00435100 {
public:
    char unknown_0[0xd5c];
    float pos_x[10];                   // +0xd5c
    float pos_y[10];                   // +0xd84
    int FUN_00435100();
};

struct PlayerInfo_00465e30 {
    char unknown_0[0xa1];
    unsigned short x;                  // +0xa1
    unsigned short y;                  // +0xa3
};

struct Player_00465e30 {
    char unknown_0[0x27];
    PlayerInfo_00465e30* info;         // +0x27
    char unknown_2b[0x8c - 0x2b];
    float x;                           // +0x8c
    char unknown_90[0x98 - 0x90];
    float y;                           // +0x98
    char unknown_9c[0x14b - 0x9c];
};

struct Slot_00465e30 {
    int unknown_0[3];
    int field_c;                       // +0xc
    int field_10;                      // +0x10
    int unknown_14;
};

struct Game_00465e30 {
    char unknown_0[0x1b63];
    Player_00465e30 players[10];       // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Slot_00465e30* slots;              // +0x29a0
    char unknown_29a4[0x38d6b - 0x29a4];
    int field_38d6b;                   // +0x38d6b
    char unknown_38d6f[0x391e9 - 0x38d6f];
    Class_00435100* net;               // +0x391e9
};

#pragma pack(pop)

extern Game_00465e30* g_game;

unsigned char FUN_00456850();
void __stdcall FUN_00496e90(Player_00465e30* player, int x, int y);

// FUNCTION: 0x465e30
void FUN_00465e30()
{
    for (int i = 0; i < 10; i++) {
        Player_00465e30* player = &g_game->players[i];
        if (g_game->field_38d6b == 0) {
            switch (g_game->net->FUN_00435100()) {
            case 1:
                FUN_00496e90(player, (int)g_game->net->pos_x[i],
                             (int)g_game->net->pos_y[i]);
                player->x = g_game->net->pos_y[i];
                player->y = g_game->net->pos_x[i];
                break;
            case 2:
                player->x = (float)g_game->slots[i].field_10;
                player->y = (float)g_game->slots[i].field_c;
                break;
            case 3: {
                int index = FUN_00456850();
                if (index == 10)
                    index = i;
                Player_00465e30* other = &g_game->players[index];
                player->x = other->info->x * 100;
                player->y = other->info->y * 100;
                break;
            }
            }
        }
    }
}
