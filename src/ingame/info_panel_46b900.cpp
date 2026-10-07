// Decompiled by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Draws one labelled bar: GetFontHeight returns the current text line height
// (c); the empty bar is outlined in white, the label is drawn, then the fill
// bar for g_game->values[index] is drawn at 100/total scale.
struct Rect_0046b900 {
    int a;                          // +0x0
    int b;                          // +0x4
    int c;                          // +0x8
    int d;                          // +0xc
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e1f];
    int width;                      // +0x37e1f
    char unknown_37e23[0x38d89 - 0x37e23];
    int total;                      // +0x38d89
    int values[8];                  // +0x38d8d
};
#pragma pack(pop)

extern Game* g_game;

int GetFontHeight();
void __stdcall DrawRectangle(int surface, Rect_0046b900* rect, int color);
void __stdcall DrawString(int surface, const char* text, int x, int y, int maxWidth);
void __stdcall FillRectangle(int surface, Rect_0046b900* rect, int color);

// FUNCTION: 0x46b900
void __stdcall FUN_0046b900(int surface, const char* text, int index)
{
    Rect_0046b900 rect;
    int x = g_game->width - 0x5a;
    int c = GetFontHeight();

    rect.a = x - 0xc8;
    rect.c = 0x27f;
    rect.b = 0x26;
    rect.d = c * 9 + 0x29;
    DrawRectangle(surface, &rect, 0xff);

    // No named local for c * index + 0x28: it must stay a compiler temp.
    DrawString(surface, text, x + 5, c * index + 0x28, -1);

    int w = g_game->values[index] * 100 / g_game->total * 2;
    // Stored first: any order with rect.c first changes register use.
    rect.a = x - w;
    rect.c = x;
    rect.d = c * index + 0x28;
    rect.b = rect.d;
    rect.d = rect.d + c;
    FillRectangle(surface, &rect, index + 1);
}
