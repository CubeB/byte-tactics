// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Draws the screen-space footprint of a map point: the 16x16 tile position is
// scaled to pixels, offset by the scroll and the object's height, and kind
// selects a colour from the table at g_game+0xdcb.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0xdcb];
    unsigned char colors[0x1431f - 0xdcb];
    int scroll_x;                       // +0x1431f
    int scroll_y;                       // +0x14323
};
#pragma pack(pop)

struct Size_0046b9d0 {
    short w;
    short h;
};

struct Rect_0046b9d0 {
    int x1;
    int y1;
    int x2;
    int y2;
};

extern Game* g_game;

int __stdcall FUN_00485010(void* p);
void __stdcall FUN_004bf8c0(void* surface, void* rect, int color);

// FUNCTION: 0x46b9d0
void __stdcall FUN_0046b9d0(void* surface, short* pos, Size_0046b9d0 size, int kind)
{
    unsigned char* colors = (unsigned char*)g_game + 0xdcb;
    int h = FUN_00485010(pos);
    int y1 = (pos[1] << 4) - g_game->scroll_y;
    int x1 = ((pos[0] + 8) << 4) - g_game->scroll_x;
    Rect_0046b9d0 rect;
    rect.x1 = x1;
    rect.y1 = y1 - (h >> 1) + 0x20;
    rect.x2 = x1 + (size.w << 4);
    rect.y2 = rect.y1 + (size.h << 4);
    if (kind == 4) {
        rect.x1++;
        rect.y1++;
        rect.x2--;
        rect.y2--;
    }
    FUN_004bf8c0(surface, &rect, colors[kind]);
}
