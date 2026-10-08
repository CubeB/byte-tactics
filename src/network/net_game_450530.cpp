// Decompiled by deepseek-v4.1-flash, retries by GPT-6.1-sol and space-bunny-free, finished by deepseek-v4.1-flash, improved by claude-sonnet-5-5, matched by Space Bunny Free. Names are provisional.
//
// Sends a 10-byte 0x21 message via SendPacketToPlayer for each player slot in state
// 1, 2 or 3 with f_146 != 10 and field_c == 0 (to = first slot whose data->flags
// has bit 0, from = first slot in state 1 or 2). State 1 sets the flag byte and
// arg = -1, state 2 sets the flag byte and arg = the local player's id, state 3
// clears the flag and arg = -1.
#pragma pack(push, 1)
struct PlayerInfo {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
};

struct Player_00450530 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0xc - 0x8];
    int field_c;                       // +0x0c
    char unknown_10[0x27 - 0x10];
    PlayerInfo* data;                  // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;               // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char f_146;               // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00450530 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char local_player;        // +0x2a42
    char unknown_2a43[0x391f1 - 0x2a43];
    int mode;                          // +0x391f1
};

struct Msg_00450530 {
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int id;                            // +0x2
    int arg;                           // +0x6
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall GetSlotDpid(unsigned char index);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
unsigned char FindHostSlot();

static inline int IsPlaying_00450530(Player_00450530* player)
{
    if (player->active == 0)
        return 0;
    if (player->state == 1 || player->state == 2)
        return 1;
    return 0;
}

static inline int GetPlayerId_00450530(unsigned char i)
{
    if (i != 10 && g_game->players[i].state != 0)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayer_00450530()
{
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].state != 0 && (g_game->players[i].data->flags & 1))
            return i;
    }
    return 10;
}

static inline int FindFrom_00450530()
{
    for (int k = 0; k < 10; k++) {
        if (IsPlaying_00450530(&g_game->players[k]))
            return g_game->players[k].id;
    }
    return -1;
}

static inline int FindTo_00450530()
{
    for (int j = 0; j < 10; j++) {
        if (g_game->players[j].data->flags & 1)
            return GetSlotDpid(j);
    }
    return -1;
}

static inline int FindToB_00450530()
{
    for (int j = 0; j < 10; j++) {
        if (g_game->players[j].data->flags & 1)
            return GetPlayerId_00450530(j);
    }
    return -1;
}

// FUNCTION: 0x450530
void SendLobbySyncRequests()
{
    // Function scope, before the mode == 6 guard: orders the stores in the three branches.
    Msg_00450530 msg;
    if (g_game->mode == 6)
        return;
    for (int i = 0; i < 10; i++) {
        Player_00450530* p = &g_game->players[i];
        if (p->active != 0
            && (p->state == 1 || p->state == 2 || p->state == 3)
            && p->f_146 != 10
            && p->field_c == 0) {
            if (IsPlaying_00450530(p) && p->state == 1) {
                Player_00450530* q = &g_game->players[FindHostSlot()];
                if (IsPlaying_00450530(q)) {
                    p->field_c = 1;
                    continue;
                }
                msg.type = 0x21;
                msg.flag = 1;
                msg.id = p->id;
                msg.arg = -1;
                if (FindHostSlot() == 10)
                    continue;
                // from and to stay call arguments, not locals.
                SendPacketToPlayer(FindFrom_00450530(), FindTo_00450530(), &msg, 10);
            }
            else if (IsPlaying_00450530(p) && p->state == 2) {
                Player_00450530* q = &g_game->players[FindHostSlot()];
                if (IsPlaying_00450530(q)) {
                    p->field_c = 1;
                    continue;
                }
                msg.type = 0x21;
                msg.flag = 1;
                msg.id = p->id;
                msg.arg = g_game->players[g_game->local_player].id;
                if (FindHostSlot() == 10)
                    continue;
                SendPacketToPlayer(FindFrom_00450530(), FindToB_00450530(), &msg, 10);
            }
            else if (p->state == 3) {
                msg.type = 0x21;
                msg.flag = 0;
                msg.id = p->id;
                msg.arg = -1;
                if (FindPlayer_00450530() == 10)
                    continue;
                // Dead branch, not real logic: the `i == 9` test is the only
                // thing that stops MSVC strength-reducing &players[i], and a
                // self-assignment is the only body it then deletes. See the
                // header note; removing these two lines costs the match.
                if (i == 9)
                    p->field_c = p->field_c;
                SendPacketToPlayer(FindFrom_00450530(), FindTo_00450530(), &msg, 10);
            }
        }
    }
}
