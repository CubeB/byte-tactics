// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// PARTIAL 24.0%: the prologue should be sub esp,8 (two locals) but ours uses
// push ecx; result is reloaded at every exit instead of one shared tail; the
// p->field_22 store is emitted twice in the group paths.
#pragma pack(push, 1)
struct PlayerInfo_00453010 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

struct Player_00453010 {
    int active;                        // +0x00
    int field_4;                       // +0x04
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
int __stdcall FUN_00451df0(int player, void* data, int size);
void __stdcall FUN_00452cc0(int id);
int __stdcall FUN_004530e0();

static inline unsigned char FindPlayerIndex(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (FUN_0044ffd0(i) == id)
                return i;
        }
    }
    return 10;
}

// FUNCTION: 0x453010
int __stdcall FUN_00453010(int id, unsigned char value)
{
    int result = 0;
    Player_00453010* p;
    if (FindPlayerIndex(id) == 10) {
        p = 0;
    } else {
        p = &g_game->players[FUN_0044fe40(id)];
    }

    if (p == 0)
        return 0;

    unsigned char* msg = g_game->buffer;
    msg[0] = 0x1b;
    *(int*)(msg + 1) = -1;
    msg[5] = value;

    if (p->active != 0) {
        if ((p->state == 1 || p->state == 2) && p->field_22 == 0) {
            if (p->state == 1) {
                for (int i = 0; i < 10; i++) {
                    if (g_game->players[i].active != 0
                        && (g_game->players[i].state == 1 || g_game->players[i].state == 2)) {
                        *(int*)(msg + 1) = g_game->players[i].field_4;
                        FUN_00451df0(FUN_004530e0(), msg, 6);
                        FUN_00452cc0(p->field_4);
                        g_game->players[i].field_22 = value;
                    }
                }
                result = 1;
            } else {
                *(int*)(msg + 1) = p->field_4;
                FUN_00451df0(FUN_004530e0(), msg, 6);
                FUN_00452cc0(p->field_4);
                result = 1;
            }
        } else if (p->state == 3 && p->field_22 == 0) {
            *(int*)(msg + 1) = id;
            result = FUN_00451df0(FUN_004530e0(), msg, 6);
            if (p->active != 0 && p->state == 3 && p->field_27->field_94 == 1) {
                unsigned char c = p->field_c;
                for (int i = 0; i < 10; i++) {
                    if (g_game->players[i].field_c == c) {
                        FUN_00452cc0(g_game->players[i].field_4);
                        g_game->players[i].field_22 = value;
                    }
                }
                p->field_22 = value;
            } else {
                FUN_00452cc0(p->field_4);
            }
        }
    }
    p->field_22 = value;
    return result;
}
