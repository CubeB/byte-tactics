// Decompiled by Space Bunny Free. Names are provisional.
struct Rect_0046b900 {
    int a;                          // +0x0
    int b;                          // +0x4
    int c;                          // +0x8
    int d;                          // +0xc
};

#pragma pack(push, 1)
struct Game_0046b900 {
    char unknown_0[0x37e1f];
    int width;                      // +0x37e1f
    char unknown_37e23[0x38d89 - 0x37e23];
    int total;                      // +0x38d89
    int values[8];                  // +0x38d8d
};
#pragma pack(pop)

extern Game_0046b900* g_game;

int FUN_004c1450();
void __stdcall FUN_004bf8c0(int surface, Rect_0046b900* rect, int color);
void __stdcall FUN_004c14f0(int surface, const char* text, int x, int y, int maxWidth);
void __stdcall FUN_004bf6f0(int surface, Rect_0046b900* rect, int color);

// FUNCTION: 0x46b900
void __stdcall FUN_0046b900(int surface, const char* text, int index)
{
    Rect_0046b900 rect;
    int x = g_game->width - 0x5a;
    int c = FUN_004c1450();

    rect.a = x - 0xc8;
    rect.c = 0x27f;
    rect.b = 0x26;
    rect.d = c * 9 + 0x29;
    FUN_004bf8c0(surface, &rect, 0xff);

    int s = surface;
    int y = c * index + 0x28;
    FUN_004c14f0(s, text, x + 5, y, -1);

    int w = g_game->values[index] * 100 / g_game->total * 2;
    rect.c = x;
    rect.b = y;
    rect.a = x - w;
    rect.d = y + c;
    FUN_004bf6f0(s, &rect, index + 1);
}
