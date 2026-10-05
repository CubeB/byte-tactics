// Decompiled by GPT-6 Astra, finished by space-bunny-free, finished by deepseek-v4.1-flash,
// finished by GPT-6.1-sol, finished by Claude Opus 5.5. Names are provisional.
//
// What made this match (91.3% before):
// - Vec3::operator- is the explicit-component form the matched sibling 0x413d80
//   (same translation unit) uses. That makes the state 3 block byte exact, but
//   on its own it ties `range` and `order` at priority 130 (c2prio), and range
//   wins the tie on its +0x40 key, so order and range trade esi and edi.
// - `int ok = FUN_0041ba60(...); if (ok)` adds a candidate to a block that
//   references order, which raises order to 134 and gives it esi again (97.1%
//   with <stdlib.h>; only the six bounds adds were left).
// - The operand order of the six bounds adds (pos.x + min.x and so on) follows
//   the symbol ids, so it moves with the headers and with code-neutral
//   spellings. What puts all six in place (found by the permuter): <memory.h>
//   plus <windows.h>, the state 0 test written as two nested ifs, and an empty
//   `do {} while (0);` in UnitRef::Get(), a debug check that compiles to
//   nothing. Without the do-while no header set gets past 97.1%; without the
//   `ok` local the function drops to 73.3%.
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

struct Game {
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
    struct {
        unsigned short : 3;
        unsigned short synced : 1;
        unsigned short : 12;
    } netBits;                         // +0x38d75
};
#pragma pack(pop)


void __stdcall FUN_004a81e0(Menu_00497ce0* menu, int value);
void __stdcall FUN_0049fad0(Menu_00497ce0* menu);
void __stdcall FUN_004ab170(Menu_00497ce0* menu, int a, int b);
void __stdcall FillRectangle(void* surface, Rect_00497ce0* rect, int color);
void __stdcall FUN_004a50e0(void* surface, const char* text, int x, int y, int len, int flag);
char* __stdcall FUN_004c5740(char* s);

// FUNCTION: 0x497ce0
void __stdcall FUN_00497ce0(void* surface)
{
    int off;
    extern Game* g_game;

    FUN_004a81e0(&g_game->menu, 0x40);
    FUN_0049fad0(&g_game->menu);
    FUN_004ab170(&g_game->menu, 0, 0);

    const char* text;
    if (g_game->netBits.synced) {
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

        int x;
        int slot = 620 / countA;
        Rect_00497ce0 r;
        r.x1 = 10;
        r.y1 = 420;
        r.y2 = 435;
        off = 0;
        x = 11;
        for (; off < 10 * (int)sizeof(PlayerRec_00497ce0); off += sizeof(PlayerRec_00497ce0)) {
            PlayerRec_00497ce0* q = (PlayerRec_00497ce0*)((char*)g_game->players + off);
            if (!(q->present && (q->team == 1 || q->team == 2 || q->team == 3) && q->kind != 10))
                continue;
            {
                r.x1 = x;
                r.x2 = slot + x - 2;
                FillRectangle(surface, &r, g_game->color1);
                int pc = q->percent;
                r.x2 = r.x1 + (pc * (slot - 2)) / 100;
                FillRectangle(surface, &r, g_game->color2);
                FUN_004a50e0(surface, q->name, r.x1, 420, slot - 2, 0);
                x += slot;
            }
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
