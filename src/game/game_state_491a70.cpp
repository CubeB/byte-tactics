// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

struct Display_00491a70 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
};

#pragma pack(push, 1)
struct Game_00491a70 {
    char unknown_0[0xc];
    Display_00491a70* field_c;         // +0xc
    char unknown_10[0x37e1b - 0x10];
    int field_37e1b;                   // +0x37e1b
    int field_37e1f;                   // +0x37e1f
    int field_37e23;                   // +0x37e23
};
#pragma pack(pop)

extern Game_00491a70* g_game;
extern const char DAT_005091d4[];      // "OFFSCREEN"

int FUN_004b6700();
int FUN_004b6710();
void __stdcall FUN_004b5940(int x, int y);
void __cdecl FUN_004d85a0(int param_1);
void __stdcall FUN_004c61f0(int param_1);
void FUN_004c62c0();
int __stdcall FUN_004c69f0(const char* name, int width, int height);
void __stdcall FUN_004c69a0(int param_1);

// FUNCTION: 0x491a70
void FUN_00491a70()
{
    g_game->field_37e1f = 0x280;
    g_game->field_37e23 = 0x1e0;
    if (FUN_004b6700() != 0x280 || FUN_004b6710() != 0x1e0) {
        FUN_004d85a0(g_game->field_37e1b);
        g_game->field_37e1b = 0;
        FUN_004c61f0(0);
        FUN_004c62c0();
        SetWindowPos(g_game->field_c->hwnd, 0, 0, 0, 0x280, 0x1e0, 4);
        FUN_004b5940(0x280, 0x1e0);
        g_game->field_37e1b = FUN_004c69f0(DAT_005091d4, g_game->field_37e1f, g_game->field_37e23);
        FUN_004c61f0(g_game->field_37e1b);
        FUN_004c69a0(g_game->field_37e1b);
    }
}
