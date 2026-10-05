// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

#pragma pack(push, 1)
struct Rect_0041cc60 {
    int x;                             // +0x0
    int y;                             // +0x4
    int unknown_8[4];                  // +0x8
};

struct CursorState_0041cc60 {
    Rect_0041cc60 rect;                // +0x0
    int flag;                          // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24
    int field_28;                      // +0x28
};

struct Game {
    char unknown_0[0x2c76];
    Rect_0041cc60 view;                // +0x2c76
    char unknown_2c8e[0x2cc7 - 0x2c8e];
    CursorState_0041cc60 cursor;       // +0x2cc7
    char unknown_2cf3[0x142f3 - 0x2cf3];
    int field_142f3;                   // +0x142f3
    int field_142f7;                   // +0x142f7
    char unknown_142fb[0x1431f - 0x142fb];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
    char unknown_14327[0x1434b - 0x14327];
    unsigned short field_1434b;        // +0x1434b
};
#pragma pack(pop)

extern Game* g_game;

void FUN_004c2470();
int GetScreenWidth();
int GetScreenHeight();
void __stdcall SetCursorPosition(int x, int y);

// FUNCTION: 0x41cc60
void FUN_0041cc60()
{
    g_game->field_1434b = 0;
    g_game->field_142f3 = 0;
    g_game->field_142f7 = 0;
    FUN_004c2470();
    CursorState_0041cc60* cursor = &g_game->cursor;
    cursor->flag = 1;
    cursor->rect = g_game->view;
    cursor->field_24 = g_game->scroll_x / 16;
    cursor->field_28 = g_game->scroll_y / 16;
    cursor->field_1c = GetScreenWidth() / 2;
    cursor->field_20 = GetScreenHeight() / 2;
    SetCursorPosition(cursor->field_1c, cursor->field_20);
}
