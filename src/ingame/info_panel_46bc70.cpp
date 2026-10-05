// Decompiled by Opus. Names are provisional.
// Shows a message: mode 0 through AddMessage (and sets a game flag),
// mode 1 in the game's message line for half of FUN_004b6700's value.

struct Game {
    char unknown_0[0x519];
    char message[0x2bee - 0x519];      // +0x519
    unsigned short flag0 : 1;          // +0x2bee, bit 0
    unsigned short bits1 : 15;
};

extern Game* g_game;

void __stdcall AddMessage(char* param_1, int param_2, int param_3, int param_4);
int __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);
int FUN_004b6700();

// FUNCTION: 0x46bc70
int __stdcall FUN_0046bc70(char* text, int mode)
{
    int result = 1;
    if (mode == 0) {
        AddMessage(text, 0x10, 0, 10);
        g_game->flag0 = 1;
        return 1;
    }
    if (mode == 1) {
        int t = FUN_004b6700();
        result = FUN_004abd90(g_game->message, text, (int)(t * 0.5), 1, 1);
    }
    return result;
}
