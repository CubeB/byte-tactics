// Decompiled by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
// Draws one labelled bar: FUN_004c1450 returns the current text line height
// (c); the empty bar is outlined in white, the label is drawn, then the fill
// bar for g_game->values[index] is drawn at 100/total scale.
//
// The temporary c*index+0x28 has no local slot of its own: the original CSEs
// the two occurrences into a temp and spills it into the dead first-argument
// slot, which is why the frame is exactly the 4 dwords of rect (the original
// has no `int y` local).
//
// Still differs from the original in the second half (93.7%, both 196 bytes):
// the original keeps the spilled temp in eax, stores it to rect.b, then does
// `add eax,edi` for rect.d, storing rect.b and rect.a before the argument
// pushes; this compiler version instead adds into edi (`add edi,eax`) and
// sinks the rect.a/rect.b stores past the pushes. All source orderings and
// expression spellings tried reproduce this scheduling, so it is left here.
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

    FUN_004c14f0(surface, text, x + 5, c * index + 0x28, -1);

    int w = g_game->values[index] * 100 / g_game->total * 2;
    rect.a = x - w;
    rect.c = x;
    rect.b = c * index + 0x28;
    rect.d = rect.b + c;
    FUN_004bf6f0(surface, &rect, index + 1);
}
