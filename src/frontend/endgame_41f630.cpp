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

void BlankScreen();
void OpenEndMissionScreen();
void FillEndGameStatistics();
void EnableEndMissionButtons();
void __stdcall FUN_0049fad0(Menu_0041f630* menu);
void __stdcall FUN_0049fa90(Menu_0041f630* menu);
void __stdcall SetGameMode(int a);

// FUNCTION: 0x41f630
void ShowEndMissionScreen()
{
    BlankScreen();
    OpenEndMissionScreen();
    FillEndGameStatistics();
    EnableEndMissionButtons();
    FUN_0049fad0(&g_game->menu);
    FUN_0049fa90(&g_game->menu);
    SetGameMode(7);
    g_game->field_39057 = 7;
}
