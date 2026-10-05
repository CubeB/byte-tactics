// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Console command callback: applies a logo selection to a player.

#pragma pack(push, 1)
struct PlayerData {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
};

struct Player {
    int active;                        // +0x00
    char unknown_4[0x23];
    PlayerData* data;                  // +0x27
    char unknown_2b[0x48];
    unsigned char type;                // +0x73
    char unknown_74[0xd2];
    unsigned char field_146;           // +0x146
    char unknown_147[0x4];
};

class Class_00437c80 {
public:
    void FUN_00437c80();
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_2851[0x1437b - 0x2851];
    Class_00437c80* obj;               // +0x1437b
    char unknown_1437f[0x148db - 0x1437f];
    void* logos32;                     // +0x148db
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

void __stdcall FUN_00463ca0(char* text, int param_2, int param_3, int param_4);

// FUNCTION: 0x4168d0
void __stdcall FUN_004168d0(Class_004b73e0* args)
{
    int n = args->FUN_004b73e0(1, 0);
    if (n >= 0) {
        int m = args->FUN_004b73e0(1, 0);
        unsigned int limit = 0;
        limit = *(unsigned short*)g_game->logos32;
        if (m < (int)limit) {
            unsigned char i = args->FUN_004b73e0(2, 0);
            if (i < 10) {
                Player* p = &g_game->players[i];
                if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10) {
                    int v = args->FUN_004b73e0(1, 0);
                    int j = args->FUN_004b73e0(2, 0);
                    g_game->players[j].data->field_96 = v;
                    g_game->obj->FUN_00437c80();
                    return;
                }
            }
        }
    }
    FUN_00463ca0("Invalid logo setting", 2, 0, 10);
}
