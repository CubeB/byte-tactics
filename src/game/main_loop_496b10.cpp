// Decompiled by Opus. Names are provisional.

struct Sub_00496b10 {
    char unknown_0[0xa6];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Sub_00496b10 sub;                  // +0x519
    char unknown_5bf[0x2a44 - 0x519 - sizeof(Sub_00496b10)];
    unsigned short bit0 : 1;           // +0x2a44
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short bit3 : 1;
    unsigned short rest_2a44 : 12;
    char unknown_2a46[0x391f1 - 0x2a46];
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

extern Game* g_game;

void FUN_00425a90();
void __stdcall FUN_00434ab0(int param);
void FUN_004c1a40();
void __stdcall FUN_0049fa50(Sub_00496b10* p);
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
void __cdecl LeaveNetGameCallback(int param);
void FUN_00496bb0();

// FUNCTION: 0x496b10
void FUN_00496b10()
{
    FUN_00425a90();
    g_game->bit2 = 0;
    g_game->bit3 = 0;
    g_game->bit0 = 0;
    FUN_00434ab0(0);
    g_game->flag4_3923b = 0;
    g_game->flag2_3923b = 0;
    FUN_004c1a40();
    FUN_0049fa50(&g_game->sub);
    g_game->mode = 2;
    g_game->handler = FUN_00496bb0;
    SetCloseHandler(LeaveNetGameCallback, 0);
}
