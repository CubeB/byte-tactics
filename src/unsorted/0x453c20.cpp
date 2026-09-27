// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Player_00453c20 {
    int field_0;                        // +0x0
    int field_4;                        // +0x4
    char unknown_8[4];                  // +0x8
    int field_c;                        // +0xc
    char unknown_10[0xc];               // +0x10
    int field_1c;                       // +0x1c
    char unknown_20[0x53];              // +0x20
    char flag_73;                       // +0x73
    char unknown_74[0x14b - 0x74];      // +0x74
};

struct Game_00453c20 {
    char unknown_0[0x1b63];
    Player_00453c20 players[10];        // +0x1b63
    char unknown_2851[0x37f2f - 0x2851];
    unsigned char field_37f2f;          // +0x37f2f
    char unknown_37f30[1];              // +0x37f30
    int field_37f31;                    // +0x37f31
    char unknown_37f35[0x38a51 - 0x37f35];
    unsigned char field_38a51;          // +0x38a51
};
#pragma pack(pop)

extern Game_00453c20* g_game;
extern unsigned int DAT_00512c7c;

unsigned int FUN_004b6340();
void __stdcall FUN_00453a50(int value);

// FUNCTION: 0x453c20
void FUN_00453c20()
{
    if (g_game->field_37f2f & 1)
        return;
    if (g_game->field_38a51 & 1) {
        DAT_00512c7c = FUN_004b6340();
        return;
    }
    unsigned int now = FUN_004b6340();
    int index = -1;
    int changed = 0;
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        Player_00453c20* p = &g_game->players[i];
        if (p->field_0 != 0 && p->flag_73 == 3) {
            unsigned int m = p->field_1c;
            if (DAT_00512c7c > m)
                m = DAT_00512c7c;
            if (now - m > (unsigned int)(g_game->field_37f31 * 30)) {
                if (index < 0)
                    index = p->field_c;
                else if (index != p->field_c)
                    changed = true;
            }
        }
    }
    for (j = 0; j < 10; j++) {
        Player_00453c20* p = &g_game->players[j];
        if (p->field_0 != 0 && p->flag_73 == 3) {
            unsigned int m = p->field_1c;
            if (DAT_00512c7c > m)
                m = DAT_00512c7c;
            if (now - m > (unsigned int)(g_game->field_37f31 * 30) && !changed) {
                FUN_00453a50(p->field_4);
                return;
            }
        }
    }
    FUN_00453a50(-1);
}
