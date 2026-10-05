// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash. Names are provisional.
// MATCH (deepseek-v4.1-flash, issue #4340): 100%, 1418 bytes. What made it match:
// (1) the draw block lives INSIDE the search loop body (the match check breaks into
// it), with `if (n == 10) { cleanup }` after the loop; that gives the original's
// `je <draw>` / `jmp <guard>` / shared `cmp edi,0xa` tail with no extra test.
// (2) the dst quad is written with chained assignments (p0.x = p3.x = ...,
// p2.x = p1.x = ..., p0.y = p1.y = ..., p2.y = p3.y = ...): that reproduces the
// original's store order (p3.x,p0.x,p1.x,p2.x,p1.y,p0.y,p3.y,p2.y) AND stops MSVC
// strength-reducing y+0x25 into ebx, so y stays in ebx with no stack home and the
// frame map matches (maxw 0x10, i 0x14, panel 0x18, counter 0x28, dst 0x2c,
// hr 0x4c, src 0x5c, buf 0x7c).  The plain p0..p3 assignment order instead made
// ebx hold y+0x25 and gave y a home at 0x18, shifting every panel ref 4 bytes.
// (3) dst.p[1].x = panel.left + maxw (not panel.right - 6) for the right edge,
// and the last text x reads dst.p[1].x.
// (4) the cleanup is a pointer walk with a countdown `for (int k = n; k != 0;
// k--, q++)`: k gets the stack slot at 0x28 (which the old indexed walk did not
// have, leaving the frame 4 bytes short) and `k = n` reproduces the original's
// `mov [esp+0x28], edi` (edi == 10 there).
// (5) hr assignments in source order left, right, top, bottom (top,left,right,bottom
// emitted top first and cost the last hunk).
// SUPERSEDED notes: everything below is the history of the earlier partials.
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

struct Game {
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

extern Game* g_game;
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
        Player_004948e0* p = g_game->players;
        Quad_004948e0 dst;
        dst.p[0].x = dst.p[3].x = panel.left + 7;
        dst.p[2].x = dst.p[1].x = panel.left + maxw;
        dst.p[0].y = dst.p[1].y = y + 1;
        dst.p[2].y = dst.p[3].y = y + 0x25;

        int n;
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
            Rect_004948e0 hr;
            hr.left = panel.left + 4;
            hr.right = panel.right - 4;
            hr.top = y - 1;
            hr.bottom = y + 0x26;
            if (n == g_game->localPlayer) {
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
            FUN_004a50e0(surface, buf, dst.p[1].x - FUN_004a5030(buf) - 2,
                         dst.p[0].y + 0x14, maxw, DAT_0051e810[n]);
            y += 0x28;
            break;
        }
        if (n == 10) {
            Player_004948e0* q = g_game->players;
            for (int k = n; k != 0; k--, q++) {
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
                if (q->field_148 > i)
                    q->field_148--;
            }
        }
    }
}
