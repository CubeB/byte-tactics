// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (timeboxed, 4247-byte function). Measured: check.py = 14.2%,
// original 4247 bytes, ours 766 bytes. Only the leading "debug stats" arm is
// written here; it is the branch taken when g_game->f_3923b has bits 0 and 1
// set, and it ends with an early `ret 4` at 0x46aba0. The huge main body
// after 0x46aba3 (the per-unit order panel / HUD walk, ~3400 bytes) is NOT
// written yet.
// Exact remaining differences: (1) the main body is missing; (2) our local
// frame is 0x68 bytes, the original's is 0x23c, so the prologue (`sub esp`),
// every frame offset and the saved-register push order differ; (3) MSVC here
// hoists the g_game load after the memset and picks a different register for
// the player index (edx vs ecx), because our reduced function has no
// downstream pressure. The debug arm's control flow and all 7 sprintf rows
// are transcribed from the disassembly and line up structurally.
//
// Key structural notes for the next worker:
//  * __stdcall, one stack arg (void* surface), ret 4, no frame pointer (ebp is
//    a general register used as the glyph y offset).
//  * local frame 0x23c bytes; a 60-byte struct at frame+0x24 is zeroed with
//    rep stosd 0xf (likely a zero-initialised local struct), then later filled
//    and memcmp'd (rep cmpsb 0x3c) against g_game+0x37e60, and copied there
//    (rep movsd 0xf) when different.
//  * g_game is at 0x511de8. PlayerInfo stride 0x14b at g_game+0x1b63, data
//    pointer at PlayerInfo+0x27 (g_game+0x1b8a+0x14b*i).
//  * the glyph loop: FUN_004b6710() - 0x20 is the row y; FUN_004b7f30(
//    *(int*)(g_game + 0x14833 + k*4), 0) returns a record whose word at +0 is
//    the advance and signed words at +4/+6 are x/y offsets; FUN_004b7f90(
//    surface, rec, x + rec[4], y + rec[6]) draws it; x += rec[0], loop while
//    x < g_game->f_37e1f.
//  * the text rows use FUN_004c13f0() (current line height),
//    FUN_004c13a0(color, font), FUN_004c1450() (line height) and
//    FUN_004c14f0(surface, buf, x, y, -1).
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Game_0046a860 {
    char unknown_0[0xc];
    char* obj_c;                              // +0xc
    char unknown_10[0x581 - 0x10];
    int f_581;                                // +0x581
    char unknown_585[0xdcb - 0x585];
    unsigned char colors[16];                 // +0xdcb
    char unknown_ddb[0x1cbe - 0xddb];
    int f_1cbe;                               // +0x1cbe
    char unknown_1cc2[0x1e09 - 0x1cc2];
    int f_1e09;                               // +0x1e09
    char unknown_1e0d[0x1f54 - 0x1e0d];
    int f_1f54;                               // +0x1f54
    char unknown_1f58[0x2a43 - 0x1f58];
    unsigned char f_2a43;                     // +0x2a43
    char unknown_2a44[0x2c8e - 0x2a44];
    short f_2c8e;                             // +0x2c8e
    short f_2c90;                             // +0x2c90
    char unknown_2c92[0x2cba - 0x2c92];
    unsigned short f_2cba;                    // +0x2cba
    unsigned short f_2cbc;                    // +0x2cbc
    char unknown_2cbe[0x14233 - 0x2cbe];
    int f_14233;                              // +0x14233
    char unknown_14237[0x14287 - 0x14237];
    int f_14287;                              // +0x14287
    char unknown_1428b[0x1431f - 0x1428b];
    int f_1431f;                              // +0x1431f
    int f_14323;                              // +0x14323
    char unknown_14327[0x14353 - 0x14327];
    int f_14353;                              // +0x14353
    char f_14357[0x10];                       // +0x14357 (OrderEntry array base)
    int f_14367;                              // +0x14367
    char unknown_1436b[0x147a7 - 0x1436b];
    int f_147a7;                              // +0x147a7
    char unknown_147ab[0x37e1f - 0x147ab];
    int f_37e1f;                              // +0x37e1f
    int f_37e23;                              // +0x37e23
    char unknown_37e27[0x37e60 - 0x37e27];
    char f_37e60[0x3c];                       // +0x37e60
    char unknown_37e9c[0x38a3b - 0x37e9c];
    int f_38a3b;                              // +0x38a3b
    char unknown_38a3f[0x38a47 - 0x38a3f];
    int f_38a47;                              // +0x38a47
    char unknown_38a4b[0x391f9 - 0x38a4b];
    int f_391f9;                              // +0x391f9
    char unknown_391fd[0x3923b - 0x391fd];
    unsigned short f_3923b;                   // +0x3923b
};
#pragma pack(pop)

struct PlayerInfo_0046a860 {
    char unknown_0[0x27];
    char* data;                               // +0x27
};

struct Glyph_0046a860 {
    unsigned short advance;                   // +0
    unsigned short unknown_2;
    short dx;                                 // +4
    short dy;                                 // +6
};

// Generic 0x118-stride array element used by the order readout.
struct OrderEntry_0046a860 {
    char unknown_0[0x110];
    int orders;                               // +0x110
};

extern Game_0046a860* g_game;

int FUN_004b6710();
char* FUN_004b7f30(int index, int unused);
void __stdcall FUN_004b7f90(void* surface, Glyph_0046a860* glyph, int x, int y);
void __stdcall FUN_004c1420(int color);
int FUN_004c13f0();
void __stdcall FUN_004c13a0(int color, int font);
int FUN_004c1450();
void __stdcall FUN_004c14f0(void* surface, const char* text, int x, int y, int maxWidth);

// FUNCTION: 0x46a860
void __stdcall FUN_0046a860(void* surface)
{
    int delta = g_game->f_37e23 - g_game->f_147a7;
    char state[0x3c];
    char buf[100];

    memset(state, 0, sizeof(state));

    if ((g_game->f_3923b & 1) && (g_game->f_3923b & 2)) {
        int x = 0x81;
        PlayerInfo_0046a860* p = (PlayerInfo_0046a860*)
            ((char*)g_game + 0x1b63 + g_game->f_2a43 * 0x14b);
        unsigned char k = *(unsigned char*)(p->data + 0x95);
        do {
            int base = FUN_004b6710();
            Glyph_0046a860* glyph = (Glyph_0046a860*)
                FUN_004b7f30(*(int*)((char*)g_game + 0x14833 + k * 4), 0);
            FUN_004b7f90(surface, glyph, x + glyph->dx, base - 0x20 + glyph->dy);
            x += glyph->advance;
        } while (x < g_game->f_37e1f);

        FUN_004c1420(g_game->f_391f9);
        FUN_004c13a0(0x53, FUN_004c13f0());
        int y = g_game->f_37e23 - FUN_004c1450() - 1;
        sprintf(buf, "PFSTATE %d, PFABLE %d\n",
                *(int*)(g_game->obj_c + 0x9c),
                *(unsigned char*)(g_game->obj_c + 0xf0) & 1);
        FUN_004c14f0(surface, buf, 0x82, y, -1);

        if (g_game->f_2cba != 0) {
            int v = ((OrderEntry_0046a860*)
                ((char*)g_game + 0x14357 + g_game->f_2cba * 0x118))->orders;
            sprintf(buf, "MOVEORD: %d FIREORD: %d\n",
                    (v >> 0x12) & 3, (v >> 0x14) & 3);
            FUN_004c14f0(surface, buf, 0x108, y, -1);
        }

        sprintf(buf, "DELTATIME: %d\n", g_game->f_38a3b);
        FUN_004c14f0(surface, buf, 0x190, y, -1);
        sprintf(buf, "GAMETIME: %d\n", g_game->f_38a47);
        FUN_004c14f0(surface, buf, 0x208, y, -1);

        y -= 0x10;
        sprintf(buf, "X: %d  Y: %d\n", g_game->f_1431f, g_game->f_14323);
        FUN_004c14f0(surface, buf, 0x82, y, -1);
        sprintf(buf, "UNITS %d\\%d\n", g_game->f_14353, g_game->f_14367);
        FUN_004c14f0(surface, buf, 0x108, y, -1);
        sprintf(buf, "PACKETS: %d %d %d\n",
                g_game->f_1cbe, g_game->f_1e09, g_game->f_1f54);
        FUN_004c14f0(surface, buf, 0x190, y, -1);

        int h = g_game->f_14233 * g_game->f_2c90 + g_game->f_2c8e;
        sprintf(buf, "XYH: %d %d %d\n", g_game->f_2c8e, g_game->f_2c90,
                *(unsigned char*)(h * 13 + g_game->f_14287 + 4));
        FUN_004c14f0(surface, buf, 0x208, y, -1);
        return;
    }

    (void)delta;
}
