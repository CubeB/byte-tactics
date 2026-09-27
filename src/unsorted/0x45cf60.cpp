// Decompiled by Opus. Names are provisional.

struct Sub_0045cf60 {
    char unknown_0[0xcca];
    int field_cca;                     // +0xcca
};

#pragma pack(push, 1)
struct Game_0045cf60 {
    char unknown_0[0x519];
    Sub_0045cf60 sub;                  // +0x519
    char unknown_x[0x2a44 - 0x519 - sizeof(Sub_0045cf60)];
    unsigned short bit0 : 1;           // +0x2a44
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short rest : 13;
    char unknown_2a46[0x37e1b - 0x2a46];
    void* field_37e1b;                 // +0x37e1b
};
#pragma pack(pop)

extern Game_0045cf60* g_game;

void __stdcall FUN_004a81e0(Sub_0045cf60* sub, int value);
void __stdcall FUN_0049fa90(Sub_0045cf60* sub);
void __stdcall FUN_004ab170(Sub_0045cf60* sub, unsigned int* a, int* b);
void __stdcall FUN_004c69a0(void* p);
void FUN_004c63a0();

// A bitfield tested for being set gives "mov cl, [m]; shr cl, 2; test cl, 1";
// testing it for being clear ("if (!bit2) {...}") folds to "test byte ptr".
// FUNCTION: 0x45cf60
void FUN_0045cf60()
{
    if (g_game->bit2) {
        return;
    }
    FUN_004a81e0(&g_game->sub, 0x40);
    FUN_0049fa90(&g_game->sub);
    FUN_004ab170(&g_game->sub, 0, 0);
    FUN_004c69a0(g_game->field_37e1b);
    FUN_004c63a0();
}
