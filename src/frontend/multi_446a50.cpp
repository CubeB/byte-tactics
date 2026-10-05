// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Refreshes the "TEAMICONS%d" gadget for every active player: the name uses
// either the player index or a running icon counter, and the value comes from
// the player's alliance and how many players are still counted on that side.
// The three FUN_004a1080 calls in the switch are written out separately so
// MSVC tail-merges them and keeps the original register allocation.
#include <windows.h>

struct Class_004a1080;

int __stdcall FUN_004a1080(Class_004a1080* obj, char* name, int value);

#pragma pack(push, 1)
struct PlayerInfo_00446a50 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
};

struct Player_00446a50 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00446a50* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x13f - 0x74];
    unsigned char alliance;            // +0x13f
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00446a50 {
    char unknown_0[0x519];
    char gui[0x1b63 - 0x519];          // +0x519
    Player_00446a50 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    union {
        unsigned short flags_2a44;     // +0x2a44
        struct {
            unsigned short bit0 : 1;
            unsigned short bit1 : 1;
            unsigned short bit2 : 1;
            unsigned short rest : 13;
        } bits;
    };
    char unknown_2a46[0x2bee - 0x2a46];
    unsigned short flag0 : 1;          // +0x2bee
    unsigned short bits1 : 15;
};
#pragma pack(pop)

extern Game_00446a50* g_game;

static inline int IsPlaying_00446a50(Player_00446a50* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted_00446a50(Player_00446a50* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

static inline int CountAlliance_00446a50(int alliance)
{
    if (alliance == 5)
        return 0;
    int count = 0;
    for (int j = 0; j < 10; j++) {
        Player_00446a50* q = &g_game->players[j];
        if (g_game->bits.bit2) {
            if (q->alliance == alliance && IsPlaying_00446a50(q) && IsCounted_00446a50(q))
                count++;
        } else {
            if (q->alliance == alliance && IsPlaying_00446a50(q))
                count++;
        }
    }
    return count;
}

// FUNCTION: 0x446a50
void FUN_00446a50()
{
    int i = 0;
    int teamIcon = 0;
    char buffer[0x40];

    for (; i < 10; i++) {
        Player_00446a50* p = &g_game->players[i];
        if (IsPlaying_00446a50(p) && p->type != 4
            && (!(g_game->flags_2a44 & 4) || IsCounted_00446a50(p))
            && (!(g_game->flags_2a44 & 4) || p->info->field_96 != 0xff)) {
            if (g_game->bits.bit2) {
                wsprintfA(buffer, "TEAMICONS%d", teamIcon);
                teamIcon++;
            } else {
                wsprintfA(buffer, "TEAMICONS%d", i);
            }

            int alliance = p->alliance;
            int count = CountAlliance_00446a50(alliance);

            switch (count) {
            case 0:
                FUN_004a1080((Class_004a1080*)g_game->gui, buffer, 10);
                break;
            case 1:
                FUN_004a1080((Class_004a1080*)g_game->gui, buffer, alliance * 2 + 1);
                break;
            default:
                FUN_004a1080((Class_004a1080*)g_game->gui, buffer, alliance * 2);
                break;
            }
        }
    }
    g_game->flag0 = 1;
}
