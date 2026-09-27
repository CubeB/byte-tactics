// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Sets the game selection to 0x13 (inlined body of FUN_00491c80), resets the
// input/UI state and installs the game's own handler.

struct Obj_004ab400;
struct Src_004ab400;

#pragma pack(push, 1)
struct Game_00496a60 {
    char unknown_0[0x519];
    char field_519[0x2a44 - 0x519];    // +0x519
    unsigned short bit0 : 1;           // +0x2a44
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short bit3 : 1;
    unsigned short rest_2a44 : 12;
    char unknown_2a46[0x2cbe - 0x2a46];
    signed char selected;              // +0x2cbe
    char unknown_2cbf[0x1487f - 0x2cbf];
    Src_004ab400* table[1];            // +0x1487f
    char unknown_14883[0x391f1 - 0x14883];
    int mode;                          // +0x391f1
    void (*handler)();                 // +0x391f5
    char unknown_391f9[0x3923b - 0x391f9];
    unsigned short bits_3923b : 2;     // +0x3923b
    unsigned short flag2_3923b : 1;
    unsigned short bit3_3923b : 1;
    unsigned short flag4_3923b : 1;
    unsigned short rest_3923b : 11;
};
#pragma pack(pop)

extern Game_00496a60* g_game;

void __stdcall FUN_004ab400(Obj_004ab400* p, Src_004ab400* src);
void FUN_004c2870();
void __stdcall FUN_00434ab0(int param);
void FUN_004c1a40();
void FUN_004578f0(int param);
void __stdcall FUN_004b4fd0(void (*callback)(int), int param);
void FUN_00496bb0();

// FUNCTION: 0x496a60
void FUN_00496a60()
{
    if (g_game->selected != 0x13) {
        g_game->selected = 0x13;
        FUN_004ab400((Obj_004ab400*)g_game->field_519, g_game->table[0x13]);
    }
    FUN_004c2870();
    g_game->bit2 = 0;
    g_game->bit3 = 0;
    g_game->bit0 = 0;
    FUN_00434ab0(0);
    g_game->flag4_3923b = 0;
    g_game->flag2_3923b = 0;
    FUN_004c1a40();
    g_game->mode = 2;
    g_game->handler = FUN_00496bb0;
    FUN_004b4fd0(FUN_004578f0, 0);
}
