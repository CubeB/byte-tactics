// Decompiled by Opus, finished by deepseek-v4.1-flash and GPT-6.1-sol. Names are provisional.
// Codex / GPT-6 retest in #13:
// capturing z first and reversing helper argument order did not
// preserve the position pointer in ecx and tile x in esi.
// GPT-6.1-sol retest in #1672: the baseline helper version remains best at 72.7%.
// Inlining the bounds and cell-index expression fell to 46.0%; restoring the
// helper and trying the original x/y declaration order again stayed at 72.7%.
// DeepSeek retest in #1334: the whole diff is still one allocation tie. p
// wants ecx (short lived, then reused for width) and x wants esi (long
// lived). Compiling the real preceding function 0x485070 in the same file
// (same g_game, same struct layouts) did not change it, nor did a CSE'd
// extra use of x at the GetCell call, a reference/copy of p, raw field
// access, single-declaration locals, or uninitialised cell/result locals
// (those last three drop to 68.2%). The instruction sequence is otherwise
// identical, so this is compiler register priority, not source shape.
// Returns the average of the two height bytes (+5, +6) of the map cell under
// a 16.16 fixed-point position, or -1 off the map.
// Not matched: the original keeps the position pointer in ecx and the tile x
// in esi; every rewrite tried (locals, helpers, macro, method, header sets)
// swaps them. <windows.h> only fixes the lea order of the index.
#include <windows.h>

#pragma pack(push, 1)
struct Cell_00485140 {
    char unknown_0[0x5];
    unsigned char field_5;              // +0x5
    unsigned char field_6;              // +0x6
    char unknown_7[0xd - 0x7];
};

struct Game_00485140 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00485140* cells;               // +0x14287
};
#pragma pack(pop)

struct Fixed_00485140 {
    unsigned short frac;
    short whole;
};

struct Pos_00485140 {
    Fixed_00485140 x;                   // +0x0
    Fixed_00485140 y;                   // +0x4
    Fixed_00485140 z;                   // +0x8
};

extern Game_00485140* g_game;

static inline Cell_00485140* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[g_game->width * y + x];
    return 0;
}

// FUNCTION: 0x485140
int __stdcall FUN_00485140(Pos_00485140* p)
{
    int x = p->x.whole / 16;
    int y = p->z.whole / 16;
    Cell_00485140* cell = GetCell(x, y);
    if (cell)
        return (cell->field_5 + cell->field_6) >> 1;
    return -1;
}
