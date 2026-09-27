// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// PARTIAL, 82.8%: same 315 bytes, but one instruction is scheduled two slots
// late. The original issues `mov edx, [edi]` (the refY argument) between
// `add eax, edx` and `sar eax, 4` of the viewY store; ours issues it after
// `mov [esi], eax` / `mov eax, [esp+0x10]` and just before the pushes. The
// declaration order (viewY/refY before the y computation, viewX/refX after it)
// is what makes viewX live in ebp from before the struct copy and spills refX;
// reordering the tail, copying the args to locals or adding headers does not
// move that load. Everything else matches byte for byte.
// Recentres the view from the saved screen rect, then warps the OS cursor back
// to the reference point and clears the scrolling state.

struct Rect_0041cd50 {
    int x;                             // +0x0
    int y;                             // +0x4
    int unknown_8[4];
};

struct Point_0041cd50 {
    int x;                             // +0x0
    int y;                             // +0x4
};

struct Cursor_0041cd50 {
    int x;                             // +0x0
    int y;                             // +0x4
    char unknown_8[0x10];
    int flag;                          // +0x18
};

#pragma pack(push, 1)
struct Game_0041cd50 {
    char unknown_0[0x2c76];
    Rect_0041cd50 view;                // +0x2c76
    char unknown_2c8e[0x2cc7 - 0x2c8e];
    Cursor_0041cd50 cursor;            // +0x2cc7
    Point_0041cd50 ref;                // +0x2ce3
    Point_0041cd50 viewPos;            // +0x2ceb
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

extern Game_0041cd50* g_game;
void __stdcall FUN_004c22f0(int x, int y);
void FUN_004c2870();
void FUN_0041c3c0();

// FUNCTION: 0x41cd50
void FUN_0041cd50()
{
    Rect_0041cd50 r = g_game->view;
    int& viewY = g_game->viewPos.y;
    int& refY = g_game->ref.y;
    int y = ((r.y - refY) / 4 + viewY) * 16;
    int& viewX = g_game->viewPos.x;
    int& refX = g_game->ref.x;
    int x = ((r.x - refX) / 4 + viewX) * 16;
    g_game->x = x;
    g_game->y = y;
    g_game->flags_142f1 |= 2;
    FUN_0041c3c0();
    g_game->x2 = g_game->x;
    g_game->y2 = g_game->y;
    g_game->flags_14281 &= 0xfff7;
    viewX = g_game->x / 16;
    viewY = g_game->y / 16;
    FUN_004c22f0(refX, refY);
    if (!(r.unknown_8[0] & 2)) {
        Cursor_0041cd50* cursor = &g_game->cursor;
        cursor->flag = 0;
        FUN_004c22f0(cursor->x, cursor->y);
        FUN_004c2870();
    }
}
