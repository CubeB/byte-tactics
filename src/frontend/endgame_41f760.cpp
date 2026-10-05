// Decompiled by Opus. Names are provisional.
// Called through a pointer (no direct callers): when the game state at
// +0x39057 is 1, draws the image at +0x39077 onto the surface at +0x37e1b at
// the display's position, then refreshes the GUI at +0x519.

struct Sub_0041f760 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Sub_0041f760 sub;                  // +0x519
    char unknown_529[0x37e1b - 0x529];
    void* surface;                     // +0x37e1b
    char unknown_37e1f[0x39057 - 0x37e1f];
    int state;                         // +0x39057
    char unknown_3905b[0x39077 - 0x3905b];
    void* image;                       // +0x39077
};
#pragma pack(pop)

struct Display_0041f760 {
    char unknown_0[0xd4];
    int x;                             // +0xd4
    int y;                             // +0xd8
};

extern Game* g_game;

Display_0041f760* GetDisplay(void);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);
void __stdcall DrawMessages(void* surface);
void __stdcall FUN_004a9fd0(Sub_0041f760* sub);
void __stdcall FUN_004ab170(Sub_0041f760* sub, unsigned int* param_2, int* param_3);
void FUN_004c2870();
void FlipScreen();

// FUNCTION: 0x41f760
int FUN_0041f760()
{
    if (g_game->state == 1) {
        Display_0041f760* d = GetDisplay();
        DrawSurface(g_game->surface, g_game->image, d->x, d->y);
        DrawMessages(g_game->surface);
        FUN_004a9fd0(&g_game->sub);
        FUN_004ab170(&g_game->sub, 0, 0);
        FUN_004c2870();
        FlipScreen();
        return 1;
    }
    return 0;
}
