// Decompiled by space-bunny-free. Names are provisional.
// The two rectangle edges are `size + pos - 1`. MSVC 5 orders the two
// commutative loads by the shape of the address expression, not by the source
// order, so `viewRight` has to be written as an index into the four radar
// shorts and `viewBottom` through a char* to get the original's registers.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x142bb];
    int viewLeft;                     // +0x142bb
    int viewTop;                      // +0x142bf
    int viewRight;                    // +0x142c3
    int viewBottom;                   // +0x142c7
    char unknown_142cb[0x142db - 0x142cb];
    void* finalSurface;               // +0x142db
    void* mappedSurface;              // +0x142df
    void* pictureSurface;             // +0x142e3
    struct { short v[4]; } dim;       // +0x142e7: posX, posY, width, height
    short blinkTimer;                 // +0x142ef
    unsigned short blinkOn : 1;       // +0x142f1, bit 0
    unsigned short unknown_bit1 : 1;
    unsigned short mapChanged : 1;    // bit 2
    unsigned short unknown_rest : 13;
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_00507518[];
extern char DAT_00507508[];

void FUN_00466780();
void* __stdcall FUN_004c69f0(char* name, int width, int height);

// FUNCTION: 0x4669b0
void FUN_004669b0()
{
    FUN_00466780();
    g_game->finalSurface = FUN_004c69f0(DAT_00507518, g_game->dim.v[2], g_game->dim.v[3]);
    g_game->mappedSurface = FUN_004c69f0(DAT_00507508, g_game->dim.v[2], g_game->dim.v[3]);
    g_game->viewLeft = g_game->dim.v[0];
    g_game->viewTop = g_game->dim.v[1];
    g_game->viewRight = g_game->dim.v[2] + g_game->dim.v[0] - 1;
    {
        char* c = (char*)g_game;
        g_game->viewBottom = *(short*)(c + 0x142ed) + *(short*)(c + 0x142e9) - 1;
    }
    g_game->mapChanged = 1;
    g_game->blinkTimer = 7;
    g_game->blinkOn = 0;
}
