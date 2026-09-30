// Decompiled by GPT-5.6-Terra, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Draws the "waiting for other players" progress bars: one bar per connected
// player, each 620/n wide, with a fill proportional to that player's percent.
//
// Partial, best 90.4% (this break-loop form). The original skips ineligible
// players and tests the ten-entry bound at the bottom:
//   0x497ec0  cmp ecx, 0xcee / jl 0x497dec        (ecx holds the byte offset)
// so the second loop is a real `for (j = 0; j < 10; j++)` with `continue`.
// Writing it that way reproduces the loop head and tail byte for byte
// (ecx as a byte-offset induction variable, spilled to [esp+0x10]), but then
// MSVC picks eax/edx instead of ecx/edx for the body's bar-drawing temporaries,
// a global ecx<->eax rotation, and the score drops to 80.3%.
// This break-loop form keeps the body's register allocation right (because the
// index never occupies ecx) but emits a break instead of a bound test and a
// stack increment instead of the ecx compare/jl, which is the 90.4% residue.
// Next step: find the source shape that gives both, likely by making the index
// operand come back through g_game so MSVC's LEA keeps ebx as base, see the
// note in 0x4848e0.cpp.
//
// Second pass (deepseek-v4.1-flash) tried: a real for-loop with `continue`,
// the same with nested ifs (no continue), a do-while, `j` declared outside the
// for, the x2 expression written as slot+x-2 / x+slot-2 / slot+(x-2) /
// (x-2)+slot / a static inline helper, and all 128 header combinations. Every
// bounded-loop form (for, do-while) fixes the head/tail exactly (do-while even
// lands at 595 bytes) but MSVC then assigns the body's first temp to eax and
// the color1 load to ecx, an ecx<->eax rotation; the unbounded break form is
// the reverse. Independent of the expression, ours also encodes the first
// `lea` as [esi+ebx-2] where the original has [ebx+esi-2] (base = slot); that
// operand order was not movable. Header scans found no improvement.
//
// Third pass (space-bunny-free) measured the rotation exactly. In every
// bounded form the loop head and tail are byte-identical to the original
// (including `mov ecx,[esp+0x10]` on the body's path only), but the whole
// rest of the function is rotated by one register in the same direction:
// original ecx -> ours eax, original edx -> ours ecx, original eax -> ours
// edx. It is a whole-function phase, not a body-local choice: the code after
// the loop (`lea ecx,[esp+0x34]` for the sprintf buffer, `mov edx,[esp+0xa4]`
// for the surface) is rotated too, while everything before the loop head is
// not. So the seed is the second loop's own basic blocks, and no source edit
// inside the body moves it. Measured, all bounded forms score 79-80%:
//   for + continue                    80.3%  (594 bytes, head/tail exact)
//   do-while (second pass)            595 bytes, same rotation
//   x + slot - 2 instead of slot + x - 2  80.3% (the lea base/index is
//       invariant to the order of the two terms, so it cannot be fixed here)
//   IV init (`int j = 0`) before/after x = 11   80.3% (the store order
//       `mov [esp+0x10],ecx` / `mov esi,0xb` is not movable either)
//   unsigned index                    79.8%
//   bound in a local `const unsigned n = 10`   79.8% (test still byte-offset)
//   aggregate rect init {10,420,0,435} 78.4%
//   one index shared by both loops    79.2%
//   percent temp inlined (no `pc`)     79.2%
//   pointer walk `q < g_game->players + 10`    74.8%  (and `q != ...`, same)
//   all 128 header sets on the bounded form     80.3% for every one of them
// The pointer-walk result is informative: a pointer induction variable is
// lowered differently and loses the `lea`/`mov` pair at the head, so the index
// form (`g_game->players[j]`) is the right one, it just needs a phase seed we
// have not found. Next idea: the seed may sit in the first loop, which is
// byte-identical here but not necessarily written the way Cavedog wrote it
// (see the note in 0x4581e0 about a load order that flips with unrelated code
// placed before it in the same file).
#include <stdio.h>

#pragma pack(push, 1)

struct Rect_00497ce0 {
    int x1;
    int y1;
    int x2;
    int y2;
};

struct PlayerRec_00497ce0 {            // 0x14b bytes, array at g_game+0x1b63
    int present;                       // +0x00
    char unknown_4[0x20 - 0x4];
    unsigned char percent;             // +0x20
    char unknown_21[0x2b - 0x21];
    char name[0x73 - 0x2b];
    unsigned char team;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char kind;                // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Menu_00497ce0 {
    char unknown_0[0x18];
    void* data;                        // +0x18
};

struct Game_00497ce0 {
    char unknown_0[0x519];
    Menu_00497ce0 menu;                // +0x519
    char unknown_519[0xdcf - 0x519 - sizeof(Menu_00497ce0)];
    unsigned char color1;              // +0xdcf
    char unknown_dd0[0xdd5 - 0xdd0];
    unsigned char color2;              // +0xdd5
    char unknown_dd6[0x1b63 - 0xdd6];
    PlayerRec_00497ce0 players[10];    // +0x1b63
    char unknown_2851[0x29a4 - 0x2851];
    int loaded[10];                    // +0x29a4
    char unknown_29cc[0x38d75 - 0x29cc];
    volatile unsigned char netFlags;     // +0x38d75, volatile in the original (see 0x494e70.cpp)
};
#pragma pack(pop)

extern Game_00497ce0* g_game;

void __stdcall FUN_004a81e0(Menu_00497ce0* menu, int value);
void __stdcall FUN_0049fad0(Menu_00497ce0* menu);
void __stdcall FUN_004ab170(Menu_00497ce0* menu, int a, int b);
void __stdcall FUN_004bf6f0(void* surface, Rect_00497ce0* rect, int color);
void __stdcall FUN_004a50e0(void* surface, const char* text, int x, int y, int len, int flag);
char* __stdcall FUN_004c5740(char* s);

// FUNCTION: 0x497ce0
void __stdcall FUN_00497ce0(void* surface)
{
    FUN_004a81e0(&g_game->menu, 0x40);
    FUN_0049fad0(&g_game->menu);
    FUN_004ab170(&g_game->menu, 0, 0);

    const char* text;
    unsigned char sync = g_game->netFlags >> 3;
    if ((sync & 1) != 0) {
        text = FUN_004c5740("Synchronization complete");
    } else {
        int countA = 0;
        int countB = 0;
        for (int i = 0; i < 10; i++) {
            PlayerRec_00497ce0* p = &g_game->players[i];
            if (p->present && (p->team == 1 || p->team == 2 || p->team == 3) && p->kind != 10)
                countA++;
            if (p->present && (p->team == 1 || p->team == 2 || p->team == 3) && p->kind != 10 &&
                p->percent == 100 && g_game->loaded[i] != 0)
                countB++;
        }

        int slot = 620 / countA;
        Rect_00497ce0 r;
        r.x1 = 10;
        r.y1 = 420;
        r.y2 = 435;
        int j = 0;
        int x = 11;
        while (1) {
            PlayerRec_00497ce0* q = &g_game->players[j];
            if (!(q->present && (q->team == 1 || q->team == 2 || q->team == 3) && q->kind != 10))
                break;
            {
                r.x1 = x;
                r.x2 = slot + x - 2;
                FUN_004bf6f0(surface, &r, g_game->color1);
                int pc = q->percent;
                r.x2 = r.x1 + (pc * (slot - 2)) / 100;
                FUN_004bf6f0(surface, &r, g_game->color2);
                FUN_004a50e0(surface, q->name, r.x1, 420, slot - 2, 0);
                x += slot;
            }
            j++;
        }

        const char* pr;
        if (countB == 1)
            pr = FUN_004c5740("player ready");
        else
            pr = FUN_004c5740("players ready");
        char buf[128];
        sprintf(buf, "%s.  %i %s", FUN_004c5740("Waiting for other players"), countB, pr);
        text = buf;
    }

    FUN_004a50e0(surface, text, 10, 400, -1, 0);
}
