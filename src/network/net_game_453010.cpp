// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
// MATCH: select the state branch once with if/else. <iostream> gives
// the shared return block instead of duplicating it into separate arms.

#include <iostream>

#pragma pack(push, 1)
struct PlayerInfo_00453010 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

struct Player_00453010 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0xc - 8];
    int field_c;                       // +0x0c
    char unknown_10[0x22 - 0x10];
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerInfo_00453010* field_27;     // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;               // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00453010 {
    char unknown_0[0x1b63];
    Player_00453010 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - (0x1b63 + 0x14b * 10)];
    unsigned char* buffer;             // +0x2a38
};
#pragma pack(pop)

extern Game_00453010* g_game;

int __stdcall FUN_0044ffd0(unsigned char index);
unsigned char __stdcall FUN_0044fe40(int id);
int __stdcall FUN_00451df0(int player, unsigned char* data, int size);
void __stdcall FUN_00452cc0(int id);

static inline unsigned char FindIndex_00453010(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (FUN_0044ffd0(i) == id)
            return i;
    }
    return 10;
}

static inline int FindActiveId_00453010()
{
    int i = 0;
    while (1) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].state == 1 || g_game->players[i].state == 2))
            return g_game->players[i].id;
        i++;
        if (i >= 10)
            return -1;
    }
}
// FUNCTION: 0x453010
int __stdcall FUN_00453010(int id, unsigned char value)
{
    int result = 0;

    unsigned char fi;
    if (id == -1)
        fi = 10;
    else
        fi = FindIndex_00453010(id);

    Player_00453010* p;
    if (fi == 10)
        p = 0;
    else
        p = &g_game->players[FUN_0044fe40(id)];

    if (p == 0)
        return 0;

    unsigned char* msg = g_game->buffer;
    msg[0] = 0x1b;
    *(int*)(msg + 1) = -1;
    msg[5] = value;

    if (p->active != 0
        && (p->state == 1 || p->state == 2)
        && p->field_22 == 0) {
        if (p->state == 1) {
            for (int i = 0; i < 10; i++) {
                if (g_game->players[i].active != 0
                    && (g_game->players[i].state == 1 || g_game->players[i].state == 2)) {
                    *(int*)(msg + 1) = g_game->players[i].id;
                    FUN_00451df0(FindActiveId_00453010(), msg, 6);
                    FUN_00452cc0(p->id);
                    g_game->players[i].field_22 = value;
                }
            }
            result = 1;
        } else {
            *(int*)(msg + 1) = p->id;
            FUN_00451df0(FindActiveId_00453010(), msg, 6);
            FUN_00452cc0(p->id);
            result = 1;
        }
    } else if (p->active != 0 && p->state == 3 && p->field_22 == 0) {
        *(int*)(msg + 1) = id;
        result = FUN_00451df0(FindActiveId_00453010(), msg, 6);
        if (p->active != 0 && p->state == 3 && p->field_27->field_94 == 1) {
            unsigned char c = p->field_c;
            for (int i = 0; i < 10; i++) {
                if (g_game->players[i].field_c == c) {
                    FUN_00452cc0(g_game->players[i].id);
                    g_game->players[i].field_22 = value;
                }
            }
        } else {
            FUN_00452cc0(p->id);
        }
    }

    p->field_22 = value;
    return result;
}
