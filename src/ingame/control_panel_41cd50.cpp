// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Mouse scrolling: moves the view by a quarter of the mouse offset from the
// centre point, saves the new scroll position (in 16-pixel units), warps the
// OS cursor back to the centre, and ends the scroll (restoring the cursor to
// where it started) once the button is released. 0x41cc60 starts it.

struct Rect_0041cd50 {
    int x;                             // +0x0
    int y;                             // +0x4
    int flags;                         // +0x8
    int unknown_c[3];
};

struct Point_0041cd50 {
    int x;                             // +0x0
    int y;                             // +0x4
};

struct CursorState_0041cd50 {
    Rect_0041cd50 rect;                // +0x0
    int flag;                          // +0x18
    Point_0041cd50 center;             // +0x1c
    Point_0041cd50 savedScroll;        // +0x24
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2c76];
    Rect_0041cd50 view;                // +0x2c76
    char unknown_2c8e[0x2cc7 - 0x2c8e];
    CursorState_0041cd50 cursor;       // +0x2cc7
    char unknown_2cf3[0x14281 - 0x2cf3];
    unsigned short flags_14281;        // +0x14281
    char unknown_14283[0x142f1 - 0x14283];
    unsigned short flags_142f1;        // +0x142f1
    char unknown_142f3[0x1431f - 0x142f3];
    int x;                             // +0x1431f
    int y;                             // +0x14323
    int x2;                            // +0x14327
    int y2;                            // +0x1432b
};
#pragma pack(pop)

extern Game* g_game;
void __stdcall SetCursorPosition(int x, int y);
void FUN_004c2870();
void FUN_0041c3c0();

// The same scroll-and-clamp sequence is written out in 0x41c7c0, 0x41c8e0,
// 0x41d0f0 and 0x41d1f0.
static inline void ScrollTo(int x, int y)
{
    g_game->x = x;
    g_game->y = y;
    g_game->flags_142f1 |= 2;
    FUN_0041c3c0();
    g_game->x2 = g_game->x;
    g_game->y2 = g_game->y;
    g_game->flags_14281 &= 0xfff7;
}

// All field accesses go through one pointer c, so MSVC keeps each field's
// address in a register (the four lea's) and knows they do not overlap: that
// lets it load center.y before the savedScroll.y store. int& locals per field
// give the same lea's but no such knowledge. The quarter offsets need their
// own locals (dx, dy) for savedScroll to get esi/ebp and center.x the spill.
// FUNCTION: 0x41cd50
void FUN_0041cd50()
{
    CursorState_0041cd50* c = &g_game->cursor;
    Rect_0041cd50 r = g_game->view;
    int dx = (r.x - c->center.x) / 4;
    int x = (dx + c->savedScroll.x) * 16;
    int dy = (r.y - c->center.y) / 4;
    int y = (dy + c->savedScroll.y) * 16;
    ScrollTo(x, y);
    c->savedScroll.x = g_game->x / 16;
    c->savedScroll.y = g_game->y / 16;
    SetCursorPosition(c->center.x, c->center.y);
    if (!(r.flags & 2)) {
        CursorState_0041cd50* cursor = &g_game->cursor;
        cursor->flag = 0;
        SetCursorPosition(cursor->rect.x, cursor->rect.y);
        FUN_004c2870();
    }
}
