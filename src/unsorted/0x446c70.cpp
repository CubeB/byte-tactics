// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Recomputes the per-player ally marks. For every active player it walks the
// players that share its alliance colour (FUN_004469c0, inlined), sets the
// corresponding bytes of field_108/field_113 and bit 1 of the player info
// flags, then keeps that bit only when at least two players share the
// alliance (CountAlliance, inlined from 0x4468c0).
//
// <windows.h> is not used, but including it is what makes MSVC emit the three
// base/index addressing modes the original has: [eax+ecx+0x1b63],
// [esi+ecx+0x13f] and [ebp+eax+0x113]. Declaring the inner search's j and k
// before the player pointer p is what puts p in the base slot of the two
// field_113/field_108 stores (the variable declared first becomes the index).
#include <windows.h>
#pragma pack(push, 1)

struct PlayerInfo_00446c70 {
    char unknown_0[0x9d];
    unsigned short flags_9d;               // +0x9d
};

struct Player_00446c70 {
    int active;                            // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00446c70* info;             // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                    // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char field_108[0x113 - 0x108];// +0x108
    unsigned char field_113[0x13f - 0x113];// +0x113
    unsigned char alliance;                // +0x13f
    int field_140;                         // +0x140
    short field_144;                       // +0x144
    unsigned char field_146;               // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00446c70 {
    char unknown_0[0x1b63];
    Player_00446c70 players[10];           // +0x1b63
    char unknown_2851[0x2a44 - 0x2851];
    unsigned short bit0 : 1;               // +0x2a44
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short rest : 13;
};
#pragma pack(pop)

extern Game_00446c70* g_game;

static inline int IsPlaying(Player_00446c70* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted(Player_00446c70* p)
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
        Player_00446c70* p = &g_game->players[i];
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

// The original's out-of-line FUN_004469c0, inlined at its only call site.
static inline int FUN_004469c0(int player, int start)
{
    Player_00446c70* players = g_game->players;
    if (start == 10)
        return -1;
    for (int i = start; i < 10; i++) {
        if ((players[i].alliance == players[player].alliance && players[i].type != 0
             && players[i].alliance != 5) || i == player)
            return i;
    }
    return -1;
}

// FUNCTION: 0x446c70
void FUN_00446c70()
{
    for (int i = 0; i < 10; i++) {
        int j;
        int k;
        Player_00446c70* p = &g_game->players[i];
        if (p->active == 0)
            continue;
        unsigned char type = p->type;
        if (type != 1 && type != 2 && type != 3)
            continue;
        if (p->field_146 == 10)
            continue;
        if (type == 4)
            continue;
        j = 0;
        while ((k = FUN_004469c0(i, j)) != -1) {
            j = k + 1;
            p->field_113[k] = 1;
            p->field_108[k] = 1;
            Player_00446c70* q = &g_game->players[k];
            q->info->flags_9d |= 2;
            p->info->flags_9d |= 2;
        }
        if (CountAlliance(p->alliance) < 2)
            p->info->flags_9d &= 0xfffd;
    }
}
