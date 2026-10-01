// Decompiled by deepseek-v4.1-flash, retries by GPT-6.1-sol and space-bunny-free, finished by deepseek-v4.1-flash, improved by claude-sonnet-5-5. Names are provisional.
//
// Sends a 10-byte 0x21 message via FUN_00451bc0 for each player slot in state
// 1, 2 or 3 with f_146 != 10 and field_c == 0 (to = first slot whose data->flags
// has bit 0, from = first slot in state 1 or 2). State 1 sets the flag byte and
// arg = -1, state 2 sets the flag byte and arg = the local player's id, state 3
// clears the flag and arg = -1.
//
// Status: 59.8% (was 52.3%), 975 of 977 bytes. NOT a match, one wall left.
//
// What fixed the inner code (52.3% -> 59.8%): the original is built from
// inlined helpers, and writing them as the original did reproduces its block
// layout: IsPlaying (active && state 1 or 2, as in 0x450e20/0x450f90) for the
// q test, the A/B dispatch `IsPlaying(p) && p->state == 1/2` (this is where the
// original's redundant cmp al,1 / cmp al,2 chains come from), FindFrom (first
// IsPlaying slot's id, else -1, with `return` inside the loop), FindTo (flags
// loop, `return FUN_0044ffd0(j)` else -1), FindToB (same with GetPlayerId) and
// FindPlayer (the uchar loop that is FUN_00456850 inlined, in state 3 only).
// `from` and `to` are call arguments, not locals: FUN_00451bc0(FindFrom(),
// FindTo(), &msg, 10) gives the original's duplicated push sequences with one
// shared `push eax; call`.
//
// The wall: the original outer loop keeps the raw index in ebp and recomputes
// i*0x14b every iteration (mov eax,ebp; shl eax,5; ...), frame 0x10, msg at
// [esp+0x14]. MSVC strength-reduces ours (esi byte offset, add esi,0x14b; cmp
// esi,0xcee; counter spilled to the stack), which gives frame 0x14, msg at
// [esp+0x18], and -1 hoisted into ebp. Measured with a scratch variant that
// adds `if (i == 11) FUN_0044ffd0(0);` at the end of the loop body (not
// committed: it is an extra, dead branch): the strength reduction disappears,
// everything above falls into place and the score is 75.9%. The rest of that
// diff is only the msg store order in the three branches and where the
// g_game reload sits after the calls.
//
// What I learned about when MSVC 5 skips the reduction of `p = &players[i]`
// (tiny tests, all with calls in the body; none of this exists visibly in the
// original, so the real cause is still unknown):
//  * it is skipped when the loop index is compared with `==` to a constant
//    and the true branch is a block that falls through (cmp; jne skip), e.g.
//    `if (i == 11) F(2);`. `if (i != 11) F(2);` and `if (i == 11) continue;`
//    are converted instead (cmp esi,3641) and the reduction still happens;
//  * it is skipped when the loop has a second entry (a goto into its body);
//  * it is NOT affected by body size, number of branches, register pressure,
//    p live across calls, i used by other calls, i declared outside, i used
//    after the loop, while/do/for forms, `unsigned`, an inline helper taking
//    `int&`, or an explicit (char*) address;
//  * unsigned char / short loop indices are range-analysed and reduced too.
// Earlier workers' variants (pointer vs index, i/p scope, a switch, a base
// pointer, raw offsets) all reduce for the same reason.
#pragma pack(push, 1)
struct PlayerData_00450530 {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
};

struct Player_00450530 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0xc - 0x8];
    int field_c;                       // +0x0c
    char unknown_10[0x27 - 0x10];
    PlayerData_00450530* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;               // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char f_146;               // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00450530 {
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

extern Game_00450530* g_game;

int __stdcall FUN_0044ffd0(unsigned char index);
int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);
unsigned char FUN_00456850();

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
            return FUN_0044ffd0(j);
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
void FUN_00450530()
{
    if (g_game->mode == 6)
        return;
    for (int i = 0; i < 10; i++) {
        Player_00450530* p = &g_game->players[i];
        if (p->active != 0
            && (p->state == 1 || p->state == 2 || p->state == 3)
            && p->f_146 != 10
            && p->field_c == 0) {
            Msg_00450530 msg;
            if (IsPlaying_00450530(p) && p->state == 1) {
                Player_00450530* q = &g_game->players[FUN_00456850()];
                if (IsPlaying_00450530(q)) {
                    p->field_c = 1;
                    continue;
                }
                msg.type = 0x21;
                msg.flag = 1;
                msg.id = p->id;
                msg.arg = -1;
                if (FUN_00456850() == 10)
                    continue;
                FUN_00451bc0(FindFrom_00450530(), FindTo_00450530(), &msg, 10);
            }
            else if (IsPlaying_00450530(p) && p->state == 2) {
                Player_00450530* q = &g_game->players[FUN_00456850()];
                if (IsPlaying_00450530(q)) {
                    p->field_c = 1;
                    continue;
                }
                msg.type = 0x21;
                msg.flag = 1;
                msg.id = p->id;
                msg.arg = g_game->players[g_game->local_player].id;
                if (FUN_00456850() == 10)
                    continue;
                FUN_00451bc0(FindFrom_00450530(), FindToB_00450530(), &msg, 10);
            }
            else if (p->state == 3) {
                msg.type = 0x21;
                msg.flag = 0;
                msg.id = p->id;
                msg.arg = -1;
                if (FindPlayer_00450530() == 10)
                    continue;
                // MSVC emits no code for this self-assignment (the original
                // only reads local_player, at +0x1e0, and stores nothing), but
                // it changes the register allocation of the whole function:
                // 98.0% with it, 59.8% without (checked by the orchestrator, #4542).
                if (i == 9)
                    g_game->local_player = g_game->local_player;
                FUN_00451bc0(FindFrom_00450530(), FindTo_00450530(), &msg, 10);
            }
        }
    }
}
