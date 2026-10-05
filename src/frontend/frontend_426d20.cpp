// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Game_00426d20 {
    char unknown_0[0x2a44];
    unsigned short bit0 : 1;                 // +0x2a44
    unsigned short bit1 : 1;
    unsigned short rest : 14;
    char unknown_2a46[0x2bbe - 0x2a46];
    unsigned char field_2bbe;                // +0x2bbe
    unsigned char field_2bbf;                // +0x2bbf
    unsigned char field_2bc0;                // +0x2bc0
};
#pragma pack(pop)

extern Game_00426d20* g_game;
extern char DAT_00511fb8[];

int FUN_00428bc0(void);
int FUN_00450d80(void);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x426d20
int FUN_00426d20(void)
{
    char buf[256];

    if (FUN_00450d80()) {
        g_game->bit0 = 1;
        return 1;
    }
    strncpy(DAT_00511fb8, "An error occurred trying to use this service", 0xf9);
    if (FUN_00428bc0()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                956, "c:\\cavedog\\wargame\\frontend.cpp");
        FUN_004abd90((char*)g_game + 0x519, buf, 500, 1, 1);
    }
    g_game->field_2bbe = 0xf;
    if (FUN_00428bc0()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                155, "c:\\cavedog\\wargame\\frontend.cpp");
        FUN_004abd90((char*)g_game + 0x519, buf, 500, 1, 1);
    }
    g_game->field_2bbf = 0;
    g_game->field_2bc0 = 0;
    if (FUN_00428bc0()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                957, "c:\\cavedog\\wargame\\frontend.cpp");
        FUN_004abd90((char*)g_game + 0x519, buf, 500, 1, 1);
    }
    g_game->field_2bbf = 0;
    g_game->field_2bc0 = 0;
    return 0;
}
