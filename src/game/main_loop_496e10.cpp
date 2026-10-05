// Decompiled by Opus. Names are provisional.
// Copies a settings block into the game: the first value to +0x37ef6 and
// three flags into bits 2, 0 and 1 of the word at +0x14281.

struct Settings_00496e10 {
    int value;                         // +0x0
    int flag_4;                        // +0x4
    int flag_8;                        // +0x8
    int flag_c;                        // +0xc
};

#pragma pack(push, 1)
struct Game_00496e10 {
    char unknown_0[0x14281];
    unsigned short bit0 : 1;           // +0x14281, bit 0
    unsigned short bit1 : 1;           // bit 1
    unsigned short bit2 : 1;           // bit 2
    unsigned short rest : 13;
    char unknown_14283[0x37ef6 - 0x14283];
    int value_37ef6;                   // +0x37ef6
};
#pragma pack(pop)

extern Game_00496e10* g_game;

// FUNCTION: 0x496e10
void __stdcall FUN_00496e10(Settings_00496e10* s)
{
    g_game->value_37ef6 = s->value;
    g_game->bit2 = s->flag_c;
    g_game->bit0 = s->flag_4;
    g_game->bit1 = s->flag_8;
}
