// Decompiled by Opus. Names are provisional.
// Clears a flag in the cursor state held in the game object and warps the
// mouse back to the saved position. The state is reached through a pointer
// local, which keeps the `add eax, 0x2cc7` the original has.

void __stdcall SetCursorPosition(int x, int y);
void ShowSoftwareCursor();

#pragma pack(push, 1)
struct CursorState_0041cd20 {
    int x;                             // +0x0
    int y;                             // +0x4
    char unknown_8[0x10];
    int flag;                          // +0x18
};

struct Game {
    char unknown_0[0x2cc7];
    CursorState_0041cd20 cursor;       // +0x2cc7
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x41cd20
void EndMouseScroll()
{
    CursorState_0041cd20* cursor = &g_game->cursor;
    cursor->flag = 0;
    SetCursorPosition(cursor->x, cursor->y);
    ShowSoftwareCursor();
}
