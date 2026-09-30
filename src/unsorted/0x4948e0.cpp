// Decompiled by deepseek-v4.1, finished by space-bunny-free. Names are provisional.
// Partial: 78.1% (real check.py run; 1451 bytes vs the original 1418).  The
// frame is 0xd0 in both and the layout matches up to 0x14 (maxw at 0x10, i at
// 0x14), but everything from the panel up is 4 bytes high, because this version
// gives the y local a stack slot at 0x18 while the original keeps y purely in
// ebx and starts the panel rect at 0x18 (see 0x494a61: mov [esp+0x18],eax with
// esp at the frame base, and 0x494a7d: lea edx,[esp+0x18] to pass &panel).
// That one dword is the whole remaining puzzle: it moves about forty stack
// references, and with it the draw loop's strength reduction changes too.
//
// THE ORIGINAL'S LOCAL MAP (frame base = esp after the four pushes, frame 0xd0,
// so 0x00-0x0f is unused and 0xd0-0xdf are the saved registers): 0x10 maxw,
// 0x14 i, 0x18 panel (4 dwords), 0x28 a 4-byte counter the cleanup loop alone
// writes, 0x2c dst quad, 0x4c hr rect, 0x5c src quad, 0x7c buf.  Nothing
// written in this version reproduces the counter at 0x28 or the absence of a y
// slot: see the list of attempts at the bottom.
//
// NEW THIS SESSION (all checked against the disassembly):
//  - The cleanup loop reuses the SEARCH counter as its own counter, in memory:
//    0x494dcb `mov [esp+0x28],edi` stores edi (the search index, 10 there),
//    and the latch is 0x494e33 `mov ecx,[esp+0x28] / add eax,0x14b / dec ecx
//    / mov [esp+0x28],ecx / jne`.  So the source is a pointer walk whose
//    condition is the search counter itself, and every failing test reaches that
//    latch, i.e. one && chain with the pointer bump in the latch:
//      Player* q = g_game->players;
//      do { if (checks-as-one-&&-chain) q->field_148--; q++; } while (--n);
//    build/scratch/0x4948e0/v7.cpp is that version: it is 1426 bytes (25 closer
//    than this file), it gets the pointer walk, the latch and `mov edx,edi`
//    right, and it scores 73.2% only because the frame stays 0xd0 with the
//    panel still 4 high.
//  - The src quad is initialised to only four fields, not four others: the
//    original stores 1 to 0x60 (p[0].y), 0x64 (p[1].x), 0x78 (p[3].y) and 0x6c
//    (p[2].x) at 0x494aa5-0x494ab4 and never touches p[0].x, p[1].y or p[3].x.
//    This file initialises p[0].x, p[0].y, p[3].x, p[1].y instead, which is
//    three wrong stores (v11.cpp, 77.1%).
//  - dst.p[1].x and dst.p[2].x come from panel.left + maxw, not from
//    panel.right - 6: 0x494b99 reads maxw (frame+0x10) and adds panel.left.
//  - The `Losses` label is right-aligned against panel.TOP, not panel.right.
//    At 0x494b4d esp is frame-12 (the inlined strcpy's `push 0`, maxw, y and
//    the ret 4 of FUN_004a5030), so `mov ecx,[esp+0x2c]` is frame+0x1c, which
//    is the slot 0x494a6c filled with the constant 0x20 and 0x494a98 loaded
//    into ebx as y.  `Kills` meanwhile is drawn at panel.left+2
//    (0x494ae3 `mov edx,[esp+0x1c]` = frame+0x18, then `add edx,2`).  Writing
//    panel.top there does emit the same load, but MSVC then folds the constant,
//    so this file keeps panel.right and stays 4 bytes high instead.
// Tried and worse: v7 cleanup (73.2), v7 + chained dst stores (66.2, and the
// frame drops to 0xcc), v7 + panel.left+maxw + panel.top (66.2), those three
// without the chaining (58.0), the src-quad fix on this file (77.1),
// char buf[84] instead of 100 (77.1, and the frame does not move, so the
// buffer is not what sets the frame size), y += 0x28 moved into the for
// increment clause (77.6 and 73.5 on v7, frame unchanged), the cleanup block
// turned into an explicit else (73.2).  This session: hoisting `int n;` before
// `Quad dst;` and driving the cleanup with it as a do-while pointer walk scores
// 72.3 (frame 0xd0, panel still at 0x1c, so n did NOT take a hoisted slot);
// `int y = 0x20;` drops y's slot and moves panel to 0x18 but also swaps the
// maxw/i slots and shrinks the frame to 0xcc (67.5), and that same constant y
// with the hoisted-n cleanup is 63.6 (frame 0xcc, y folded into immediates).
// So the missing slot at 0x28 is n's home, needed only when the cleanup reuses
// n as its counter with edi taken over by i, and y must stay a register-only
// (ebx) variable for the panel to land at 0x18.  Earlier sessions: draw block
// before the cleanup guard 67.5, pointer-walk cleanup counter 65.6, buf[80]
// 69.5, y declared only for the outer loop 65.2, v1..v5 66.6.

#include <string.h>

#pragma pack(push, 1)

struct Rect_004948e0 {
    int left;
    int top;
    int right;
    int bottom;
};

struct Point_004948e0 {
    int x;
    int y;
};

struct Quad_004948e0 {
    Point_004948e0 p[4];
};

struct Team_004948e0 {
    char unknown_0[4];
    unsigned char* data;               // +0x4
};

struct PlayerData_004948e0 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
    char unknown_97[0x9b - 0x97];
    unsigned char field_9b;            // +0x9b
};

struct Player_004948e0 {               // 0x14b bytes
    int field_0;                       // +0x0
    char unknown_4[0x27 - 4];
    PlayerData_004948e0* data;         // +0x27
    char name[0x48];                   // +0x2b
    unsigned char field_73;            // +0x73
    char unknown_74[0xfc - 0x74];
    short field_fc;                    // +0xfc
    short field_fe;                    // +0xfe
    char unknown_100[0x104 - 0x100];
    short field_104;                   // +0x104
    short field_106;                   // +0x106
    char unknown_108[0x140 - 0x108];
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x148 - 0x147];
    unsigned char field_148;           // +0x148
    char unknown_149[0x14b - 0x149];
};

struct Game_004948e0 {
    char unknown_0[0x531];
    Team_004948e0* teams;              // +0x531
    char unknown_535[0x57d - 0x535];
    int team_index;                    // +0x57d
    char unknown_581[0x1b63 - 0x581];
    Player_004948e0 players[10];       // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x148db - 0x2a43];
    int field_148db;                   // +0x148db
    char unknown_148df[0x37ef6 - 0x148df];
    int field_37ef6;                   // +0x37ef6
    char unknown_37efa[0x37f06 - 0x37efa];
    unsigned char field_37f06;         // +0x37f06
};
#pragma pack(pop)

extern Game_004948e0* g_game;
extern unsigned char DAT_0051f2c8[10];
extern unsigned char DAT_0051e810[10];
extern int DAT_0051f2d8;
extern int DAT_0051f2f4;

unsigned int FUN_004b6340();
int FUN_004b6700();
void __stdcall FUN_0047f1a0(char* name, int param_2);
int __stdcall FUN_004c1b80(int key);
void __stdcall FUN_004bf4d0(void* surface, Rect_004948e0* rect, int level);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style);
int __stdcall FUN_004a5030(char* text);
char* __stdcall FUN_004c5740(char* name);
void* __stdcall FUN_004b7f30(void* glyphs, int c);
void __stdcall FUN_004c7580(void* surface, void* pic, Quad_004948e0* dst, Quad_004948e0* src);
int __cdecl sprintf(char* buf, char* fmt, ...);

// FUNCTION: 0x4948e0
void __stdcall FUN_004948e0(void* surface)
{
    if (DAT_0051f2f4 < (int)FUN_004b6340()) {
        DAT_0051f2f4 = FUN_004b6340() + 1;
        for (int i = 0; i < 10; i++) {
            if (DAT_0051f2c8[i] > 0)
                DAT_0051f2c8[i] -= 2;
            if (DAT_0051e810[i] > 0)
                DAT_0051e810[i] -= 2;
        }
    }

    if (!(g_game->field_37f06 & 0x80)
        && (FUN_004c1b80(0x20) == 0
            || (g_game->team_index != -1
                && ((unsigned char*)g_game->teams->data)[g_game->team_index * 0x15b] == 3))) {
        if (DAT_0051f2d8 <= 0)
            return;
        if (DAT_0051f2d8 == 0x7d)
            FUN_0047f1a0("Panel", 0);
        int q = DAT_0051f2d8 / 4;
        if (q <= 1)
            q = 1;
        DAT_0051f2d8 -= q;
        if (DAT_0051f2d8 <= 0) {
            DAT_0051f2d8 = 0;
            FUN_0047f1a0("Options", 0);
        }
    } else if (DAT_0051f2d8 < 0x7d) {
        if (DAT_0051f2d8 == 0)
            FUN_0047f1a0("Panel", DAT_0051f2d8);
        int q = (0x7d - DAT_0051f2d8) / 4;
        if (q <= 1)
            q = 1;
        DAT_0051f2d8 += q;
        if (DAT_0051f2d8 >= 0x7d) {
            DAT_0051f2d8 = 0x7d;
            FUN_0047f1a0("Options", 0);
        }
    }

    Rect_004948e0 panel;
    panel.left = FUN_004b6700() - DAT_0051f2d8;
    panel.top = 0x20;
    panel.right = panel.left + 0x7d;
    panel.bottom = g_game->numPlayers * 0x28 + 0x2e;
    FUN_004bf4d0(surface, &panel, -0x18);

    panel.right = panel.left + 0x7d;
    int y = panel.top;
    int maxw = panel.right - panel.left - 6;
    char buf[100];
    Quad_004948e0 src;
    src.p[0].x=1; src.p[0].y=1; src.p[3].x=1; src.p[1].y=1;
    strcpy(buf, FUN_004c5740("Kills"));
    FUN_004a50e0(surface, buf, panel.left + 2, y, maxw, 0);
    strcpy(buf, FUN_004c5740("Losses"));
    FUN_004a50e0(surface, buf, panel.right - FUN_004a5030(buf) - 2, y, maxw, 0);
    y += 0xf;

    for (int i = 0; i < (int)g_game->numPlayers; i++) {
        int n;
        Quad_004948e0 dst;
        dst.p[0].x = panel.left + 7;
        dst.p[0].y = y + 1;
        dst.p[1].x = panel.right - 6;
        dst.p[1].y = y + 1;
        dst.p[2].x = panel.right - 6;
        dst.p[2].y = y + 0x25;
        dst.p[3].x = panel.left + 7;
        dst.p[3].y = y + 0x25;

        Player_004948e0* p = g_game->players;
        for (n = 0; n < 10; n++, p++) {
            if (p->field_0 == 0)
                continue;
            unsigned char c = p->field_73;
            if (c != 1 && c != 2 && c != 3)
                continue;
            if (p->field_146 == 0xa)
                continue;
            if (p->field_144 == 0 && p->field_140 != 0)
                continue;
            if (p->data->field_9b & 0x40)
                continue;
            if (p->field_148 != i)
                continue;
            break;
        }
        if (n == 10) {
            for (int j = 0; j < 10; j++) {
                Player_004948e0* q = &g_game->players[j];
                if (q->field_0 == 0)
                    continue;
                unsigned char c = q->field_73;
                if (c != 1 && c != 2 && c != 3)
                    continue;
                if (q->field_146 == 0xa)
                    continue;
                if (q->field_144 == 0 && q->field_140 != 0)
                    continue;
                if (q->data->field_9b & 0x40)
                    continue;
                if ((unsigned)q->field_148 > (unsigned)i)
                    q->field_148--;
            }
            continue;
        }

        if (n == g_game->localPlayer) {
            Rect_004948e0 hr;
            hr.left = panel.left + 4;
            hr.top = y - 1;
            hr.right = panel.right - 4;
            hr.bottom = y + 0x26;
            FUN_004bf4d0(surface, &hr, 0x1f);
            FUN_004bf4d0(surface, &hr, 0x14);
        }
        unsigned short* frame = (unsigned short*)FUN_004b7f30(
            (void*)g_game->field_148db, p->data->field_96);

        src.p[1].x = frame[0] - 1;
        src.p[2].x = frame[0] - 1;
        src.p[2].y = frame[1] - 1;
        src.p[3].y = frame[1] - 1;
        FUN_004c7580(surface, frame, &dst, &src);

        FUN_004a50e0(surface, p->name, dst.p[0].x + 2, dst.p[0].y + 5, maxw, 0);
        int kills = g_game->field_37ef6 == 2 ? p->field_104 : p->field_fc;
        sprintf(buf, "%d", kills);
        FUN_004a50e0(surface, buf, dst.p[0].x + 2, dst.p[0].y + 0x14, maxw,
                     DAT_0051f2c8[n]);
        int losses = g_game->field_37ef6 == 2 ? p->field_106 : p->field_fe;
        sprintf(buf, "%d", losses);
        FUN_004a50e0(surface, buf, dst.p[2].x - FUN_004a5030(buf) - 2,
                     dst.p[0].y + 0x14, maxw, DAT_0051e810[n]);
        y += 0x28;
    }
}
