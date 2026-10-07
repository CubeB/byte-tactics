// Decompiled by deepseek-v4.1-flash, finished by DeepSeek V4.1 Flash, finished by LongCat 2.5 Preview Free, finished by space-bunny-free. Names are provisional.
// Rebuilds the ally GUI list: for the local player it scans all 10 player
// slots and, for each live ally other than the local player, adds a
// LIVEALLY%d entry whose value is two bytes from the local player's arrays at
// +0x108 and +0x113: (field_113 << 1) | field_108.
#include <stdio.h>

#pragma pack(push, 1)
struct PlayerInfo_00446fb0 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
    char unknown_97[0x9b - 0x97];
    unsigned char flag_9b;             // +0x9b, tested with 0x40
};

struct Player_00446fb0 {               // 0x14b bytes
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00446fb0* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char field_108[0xb];      // +0x108
    unsigned char field_113[0xb];      // +0x113
    char unknown_11e[0x140 - 0x11e];
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Sub_00446fb0 {
    char unknown_0[0x18];
    void* entry;                       // +0x18
};

struct Game {
    char unknown_0[0x519];
    Sub_00446fb0 sub;                  // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_00446fb0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall IsScreenNamed(Sub_00446fb0* obj, const char* name);
int __stdcall SetButtonStageByName(Sub_00446fb0* obj, char* name, int value);
void __stdcall FUN_0049fa90(Sub_00446fb0* obj);

__inline int IsLiveType_00446fb0(Player_00446fb0* p)
{
    if (p->type != 1 && p->type != 2 && p->type != 3)
        return 0;
    return 1;
}

// Players are passed by pointer with no local copy: keeps g_game in eax.
__inline int IsAlly_00446fb0(Player_00446fb0* p)
{
    // First active test reads a local, the second reads p->active: keeps both tests.
    int act = p->active;
    if (!act)
        return 0;
    if (p->info->flag_9b & 0x40)
        return 0;
    if (!p->active)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->field_146 == 10)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    return 1;
}

// Separate from IsAlly: runs the type chain once.
__inline int IsLive_00446fb0(Player_00446fb0* p)
{
    if (!p->active)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->field_146 == 10)
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    if (p->info->field_96 == 0xff)
        return 0;
    return 1;
}

// FUNCTION: 0x446fb0
void RebuildAllyList()
{
    // 52 bytes, not 44: sets the frame size.
    char text[52];
    // No lp local: both pointers are built from g_game->players[...] directly.
    unsigned char* a = &g_game->players[g_game->localPlayer].field_108[0];
    unsigned char* b = &g_game->players[g_game->localPlayer].field_113[0];

    if (IsScreenNamed(&g_game->sub, "ALLIES.GUI") != 0) {
        int i;
        // Pointer induction variables, not a[i] / b[i]; i < 10, not i != 10.
        for (i = 0; i < 10; ++i, ++a, ++b) {
            if (IsAlly_00446fb0(&g_game->players[i]) && i != g_game->localPlayer
                && IsLive_00446fb0(&g_game->players[i])) {
                sprintf(text, "LIVEALLY%d", i);
                SetButtonStageByName(&g_game->sub, text, (*b << 1) | *a);
            }
        }
        FUN_0049fa90(&g_game->sub);
    }
}
