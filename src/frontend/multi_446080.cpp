// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Opens the YESORNO.GUI dialog for player DAT_00505510, fills its CHOICE1 /
// CHOICE2 / TITLE fields and installs FUN_00446020 as the handler. The title
// is "Reject <player name>?".
#include <stdio.h>

struct Sub_00446080 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Player_00446080 {
    char name[0x14b];                  // +0x0
};

struct Game {
    char unknown_0[0x519];
    Sub_00446080 sub;                  // +0x519
    char unknown_529[0x1b8e - 0x529];
    Player_00446080 players[10];       // +0x1b8e
};
#pragma pack(pop)

struct Gadget_00446080 {
    char unknown_0[0x4];
    void* entries;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
    Game* owner;                       // +0xc
};

extern Game* g_game;
extern int DAT_00505510;

Gadget_00446080* __stdcall FUN_004aa8f0(Sub_00446080* sub, const char* name, int flags);
void __stdcall FUN_0049fb10(Sub_00446080* sub, int value);
void __stdcall FUN_0049fdf0(void* entries, const char* name, int type);
void __stdcall FUN_004a0bf0(Sub_00446080* sub, const char* name, const char* text, int param_4);
void __stdcall FUN_004a81e0(Sub_00446080* sub, int value);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_00446020(void* gadget);

// FUNCTION: 0x446080
void __stdcall FUN_00446080(int player)
{
    char buf[100];
    DAT_00505510 = player;
    Gadget_00446080* gadget = FUN_004aa8f0(&g_game->sub, "YESORNO.GUI", 0x100);
    if (gadget != 0) {
        FUN_0049fb10(&g_game->sub, 1);
        void* entries = gadget->entries;
        FUN_0049fdf0(entries, "CHOICE1", 1);
        FUN_0049fdf0(entries, "CHOICE2", 1);
        FUN_0049fdf0(entries, "TITLE", 5);
        FUN_004a0bf0(&g_game->sub, "CHOICE1", "Yes", 0);
        FUN_004a0bf0(&g_game->sub, "CHOICE2", "No", 0);
        sprintf(buf, "%s %s?", FUN_004c5740("Reject"),
                g_game->players[DAT_00505510].name);
        FUN_004a0bf0(&g_game->sub, "TITLE", buf, 0);
        gadget->handler = FUN_00446020;
        gadget->owner = g_game;
        FUN_0049fb10(&g_game->sub, 1);
        FUN_004a81e0(&g_game->sub, 0x40);
    }
}
