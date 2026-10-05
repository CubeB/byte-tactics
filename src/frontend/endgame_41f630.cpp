// Decompiled by Opus. Names are provisional.

struct Menu_0041f630 {
    char unknown_0[0x1c];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_0041f630 menu;                // +0x519
    char unknown_535[0x39057 - 0x519 - sizeof(Menu_0041f630)];
    int field_39057;                   // +0x39057
};
#pragma pack(pop)

extern Game* g_game;

void FUN_004257a0();
void FUN_0041f0a0();
void FUN_0041e420();
void FUN_0041f400();
void __stdcall FUN_0049fad0(Menu_0041f630* menu);
void __stdcall FUN_0049fa90(Menu_0041f630* menu);
void __stdcall FUN_00490b30(int a);

// FUNCTION: 0x41f630
void FUN_0041f630()
{
    FUN_004257a0();
    FUN_0041f0a0();
    FUN_0041e420();
    FUN_0041f400();
    FUN_0049fad0(&g_game->menu);
    FUN_0049fa90(&g_game->menu);
    FUN_00490b30(7);
    g_game->field_39057 = 7;
}
