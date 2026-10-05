// Decompiled by Opus. Names are provisional.
// Opens the help dialog (HELP.GUI) with FUN_0045fac0 as its handler.

struct Sub_0045fb30 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Sub_0045fb30 sub;                  // +0x519
};

struct Info_0045fb30 {
    char unknown_0[0xb6];
    short field_b6;                    // +0xb6
};
#pragma pack(pop)

struct Gadget_0045fb30 {
    char unknown_0[0x4];
    Info_0045fb30* info;               // +0x4
    int (__stdcall* handler)(void*);   // +0x8
};

extern Game* g_game;
extern int DAT_00512ef0;

Gadget_0045fb30* __stdcall FUN_004aa8f0(Sub_0045fb30* sub, const char* name, int flags);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall FUN_0045f8c0(Sub_0045fb30* sub, int a, int b);
void __stdcall FUN_0049fb10(Sub_0045fb30* sub, int value);
void __stdcall FUN_004a81e0(Sub_0045fb30* sub, int value);
int __stdcall FUN_0045fac0(void* gadget);

// FUNCTION: 0x45fb30
void FUN_0045fb30()
{
    Gadget_0045fb30* g = FUN_004aa8f0(&g_game->sub, "HELP.GUI", 0x1881);
    g->handler = FUN_0045fac0;
    FUN_004288d0("dhelp", 0, 0, 0);
    DAT_00512ef0 = g->info->field_b6;
    FUN_0045f8c0(&g_game->sub, 0, 0x11);
    FUN_0049fb10(&g_game->sub, 1);
    FUN_004a81e0(&g_game->sub, 0x40);
}
