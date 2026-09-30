// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
//
// For each player slot in state 1, 2 or 3 with field_146 != 10 and field_c == 0
// it sends a 10-byte 0x21 message (to = first slot whose data->flags has bit 0,
// from = first slot in state 1 or 2) via FUN_00451bc0. State 1 sets the flag
// byte and arg = -1; state 2 sets the flag byte and arg = the local player's id;
// state 3 clears the flag and arg = -1. The local id lookup is the inlined
// FUN_0044ffd0 (GetPlayerId above) and the "find any in-use player" test is the
// inlined FUN_00456850 (FindPlayer above); writing them as static inline helpers
// at the sites where the original inlined them is what took this from 36.4% to
// 52.3% (it also moves g_game into edi and matches the reference counts).
//
// Re-attempt (deepseek-v4.1-flash, issue 1303): re-verified lever 1 and the
// strength-reduction question below; no variant beat 52.3%, so the body is
// unchanged. This is the allocator/scheduler wall described below.
// Retry (GPT-6.1-sol, issue 1627): tried combining the state-1/state-2 bodies,
// casting the loop index to unsigned char, using raw byte-offset addressing,
// and using players+i. The first two changes regressed; the pointer forms
// tied at 52.3%. Replaced state-2's static GetPlayerId helper with the external
// FUN_0044ffd0 call, which regressed to 42.2%; restored this best version.
//
// Still differs from the original (52.3%):
//  * The original outer player loop keeps the raw index in ebp and recomputes
//    i*0x14b from it every iteration (mov eax,ebp; shl eax,5; add eax,ebp;
//    add ecx,ebp; lea eax,[eax+eax*4]; lea esi,[ecx+eax*2+0x1b63]). MSVC 5
//    always strength-reduces this loop here: it keeps a byte offset in esi
//    instead and spills the counter to [esp+0x10/0x14], which cascades into a
//    0x14 frame instead of 0x10, msg at [esp+0x18] instead of [esp+0x14], and
//    `to` in ebp instead of edi. Tried and ruled out: pointer local vs direct
//    indexing, i/p declared outside the loop, an array reference, a char* cast,
//    a 2D char array, while/do-while/i!=10 forms, short/char/unsigned index,
//    separate ifs, nested else, a switch, and all 128 header sets from
//    tools/headers.py. See build/scratch/0x450530/.
// Tried by space-bunny-free and rejected, all scored with check.py --sym:
// removing the `p` local from the loop head and writing `g_game->players[i]`
// at each of the four guard tests (the two `lea` at the top of the loop then
// become one) drops it to 36.9%; hoisting the state into an
// `unsigned char st` local and testing st three times drops it to 43.9%, even
// though it is the shape the original's single `mov al` + three compares
// suggests. Keep `Player* p = &g_game->players[i]` and the three direct
// `p->state` tests.
//  * The state dispatch in the original is a second compare chain
//    (cmp al,1 / je A / cmp al,2 / jne ... / cmp al,1 / jne B) because its
//    branch bodies are laid out out of line; ours falls through to A, which is
//    a consequence of the same outer-loop allocation.

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
            if (p->state == 1) {
                Player_00450530* q = &g_game->players[FUN_00456850()];
                if (q->active != 0 && (q->state == 1 || q->state == 2)) {
                    p->field_c = 1;
                    continue;
                }
                msg.type = 0x21;
                msg.flag = 1;
                msg.id = p->id;
                msg.arg = -1;
                if (FUN_00456850() == 10)
                    continue;
                int to = -1;
                for (int j = 0; j < 10; j++) {
                    if (g_game->players[j].data->flags & 1) {
                        to = FUN_0044ffd0(j);
                        break;
                    }
                }
                int from = -1;
                for (int k = 0; k < 10; k++) {
                    if (g_game->players[k].active != 0
                        && (g_game->players[k].state == 1 || g_game->players[k].state == 2)) {
                        from = g_game->players[k].id;
                        break;
                    }
                }
                FUN_00451bc0(from, to, &msg, 10);
            }
            else if (p->state == 2) {
                Player_00450530* q = &g_game->players[FUN_00456850()];
                if (q->active != 0 && (q->state == 1 || q->state == 2)) {
                    p->field_c = 1;
                    continue;
                }
                msg.type = 0x21;
                msg.flag = 1;
                msg.id = p->id;
                msg.arg = g_game->players[g_game->local_player].id;
                if (FUN_00456850() == 10)
                    continue;
                int to = -1;
                for (int j = 0; j < 10; j++) {
                    if (g_game->players[j].data->flags & 1) {
                        to = GetPlayerId_00450530(j);
                        break;
                    }
                }
                int from = -1;
                for (int k = 0; k < 10; k++) {
                    if (g_game->players[k].active != 0
                        && (g_game->players[k].state == 1 || g_game->players[k].state == 2)) {
                        from = g_game->players[k].id;
                        break;
                    }
                }
                FUN_00451bc0(from, to, &msg, 10);
            }
            else if (p->state == 3) {
                msg.type = 0x21;
                msg.flag = 0;
                msg.id = p->id;
                msg.arg = -1;
                if (FindPlayer_00450530() == 10)
                    continue;
                int to = -1;
                for (int j = 0; j < 10; j++) {
                    if (g_game->players[j].data->flags & 1) {
                        to = FUN_0044ffd0(j);
                        break;
                    }
                }
                int from = -1;
                for (int k = 0; k < 10; k++) {
                    if (g_game->players[k].active != 0
                        && (g_game->players[k].state == 1 || g_game->players[k].state == 2)) {
                        from = g_game->players[k].id;
                        break;
                    }
                }
                FUN_00451bc0(from, to, &msg, 10);
            }
        }
    }
}
