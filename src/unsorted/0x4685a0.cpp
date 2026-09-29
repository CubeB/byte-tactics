// Decompiled by deepseek-v4.1-flash. Names are provisional.
// STATUS: partial, 55.2%. Debug overlay for the selected unit ("Unit Builder
// Probe"): its uid, owner, controller and, for a local player, every type it
// can build with the build probability and a small bar.
//
// WHAT MATCHES: the packed Game/Unit/UnitType/PlayerInfo layout (without
// #pragma pack(1) every offset drifts by 1..4), the 0x118 stride of Unit, the
// index computation, the string constants and their argument order, the frame
// size (buf is char[0x70]), and the overall control flow.
//
// WHAT STILL DIFFERS, all of it register/slot assignment, no source idea found
// within the timebox:
//  - the original keeps `unit` in ebp and the zero constant in ebx; this file
//    gets the opposite (unit in ebx, zero in ebp). Everything downstream that
//    names ebp or ebx follows from that one swap.
//  - the original keeps lineHeight in esi and y in edi; this file swaps them.
//  - local slots: original has colors at [esp+0x10], unit at [esp+0x14],
//    lineHeight at [esp+0x18], buf at [esp+0x44]; this file's are shifted
//    (colors [esp+0x18], unit [esp+0x40], buf lower), which is the 0x10 of
//    frame difference that remains.
#include <stdio.h>

#pragma pack(push, 1)

struct Rect_004685a0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Def_004685a0 {
    char unknown_0[0x20];
    char name[0x229];                  // +0x20
};

struct PlayerInfo_004685a0 {
    int f_0;                           // +0x0
    char unknown_4[0x2b - 4];
    char name[0x73 - 0x2b];            // +0x2b
    unsigned char controller;          // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char player;              // +0x146
    char unknown_147[0x22f - 0x147];
    unsigned char mobile;              // +0x22f
};

struct UnitType_004685a0 {
    char name[0x152];                  // +0x0
    int count;                         // +0x152
    unsigned short* types;             // +0x156
    char unknown_15a[0x22f - 0x15a];
    unsigned char mobile;              // +0x22f
};

struct Unit_004685a0 {
    char unknown_0[0x1f];
    unsigned char f_1f;                // +0x1f
    char unknown_20[0x3b - 0x20];
    unsigned char f_3b;                // +0x3b
    char unknown_3c[0x57 - 0x3c];
    unsigned char f_57;                // +0x57
    char unknown_58[0x92 - 0x58];
    UnitType_004685a0* type;           // +0x92
    PlayerInfo_004685a0* player;       // +0x96
    char unknown_9a[0xa8 - 0x9a];
    unsigned short f_a8;               // +0xa8
    char unknown_aa[0xff - 0xaa];
    unsigned char f_ff;                // +0xff
    char unknown_100[0x110 - 0x100];
    int f_110;                         // +0x110
    char unknown_114[4];
};

struct Game_004685a0 {
    char unknown_0[0xdcb];
    unsigned char colors[16];          // +0xdcb, [15] is the text colour
    char unknown_ddb[0x14357 - 0xddb];
    Unit_004685a0* units;              // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    Def_004685a0* defs;                // +0x1439b
    char unknown_1439f[0x391b3 - 0x1439f];
    int f_391b3;                       // +0x391b3
    unsigned short f_391b7;            // +0x391b7
    int f_391b9;                       // +0x391b9
    unsigned short f_391bd;            // +0x391bd
    char unknown_391bf[0x391f9 - 0x391bf];
    int f_391f9;                       // +0x391f9
};

#pragma pack(pop)

extern Game_004685a0* g_game;
extern int DAT_0051e540;

int FUN_004c13f0();
void __stdcall FUN_004c13a0(int param_1, int param_2);
void __stdcall FUN_004c1420(int param_1);
int FUN_004c1450();
int FUN_004c6b60();
int __stdcall FUN_0040bb00(int player, unsigned short type);
void __stdcall FUN_004bf4d0(void* surface, void* rect, int level);
void __stdcall FUN_004bf8c0(void* surface, void* rect, int color);
void __stdcall FUN_004bf6f0(void* surface, void* rect, int color);
void __stdcall FUN_004c14f0(void* dst, const char* text, int x, int y, int maxWidth);

// FUNCTION: 0x4685a0
int __stdcall FUN_004685a0(void* surface)
{
    if (g_game->f_391b9 == 0 || g_game->f_391bd == 0)
        return 0;
    Unit_004685a0* unit = &g_game->units[g_game->f_391bd];
    if ((unit->f_110 & 0x10000000) == 0 || (unit->f_110 & 0x4000) != 0
            || unit->type->types == 0) {
        g_game->f_391b3 = 0;
        g_game->f_391b7 = 0;
    }
    unsigned char* colors = g_game->colors;
    FUN_004c13a0(colors[15], FUN_004c13f0());
    FUN_004c1420(g_game->f_391f9);
    int lineHeight = FUN_004c1450() + 3;
    int y = lineHeight * 3;
    FUN_004c6b60();
    Rect_004685a0 r;
    r.left = 0x83;
    r.right = 0x191;
    r.top = y;
    if (DAT_0051e540 == 0)
        r.bottom = lineHeight * 20;
    else
        r.bottom = DAT_0051e540;
    FUN_004bf4d0(surface, &r, -0x18);
    r.right++;
    r.bottom++;
    FUN_004bf8c0(surface, &r, colors[5]);
    char buf[0x70];
    y += 3;
    FUN_004c14f0(surface, "Unit Builder Probe", 0x86, y, -1);
    y += lineHeight;
    FUN_004c14f0(surface, "==================", 0x86, y, -1);
    UnitType_004685a0* type = unit->type;
    PlayerInfo_004685a0* player = unit->player;
    y += lineHeight;
    sprintf(buf, "uid: %03d '%s'\n", unit->f_a8, (char*)type);
    FUN_004c14f0(surface, buf, 0x86, y, -1);
    char* mobile = type->mobile ? "MOBILE" : "BUILDING";
    char* remote;
    if (player->controller == 1 || player->controller == 2)
        remote = "LOCAL";
    else
        remote = "REMOTE";
    y += lineHeight;
    sprintf(buf, "playerno: %d '%s' %s - %s\n", player->player, player->name, remote, mobile);
    FUN_004c14f0(surface, buf, 0x86, y, -1);
    y += lineHeight;
    sprintf(buf, "controller: %d\n\n", player->controller);
    FUN_004c14f0(surface, buf, 0x86, y, -1);
    y += lineHeight;
    if (player->f_0 != 0 && (player->controller == 1 || player->controller == 2)) {
        sprintf(buf, "Units I can build, and the probabilities:\n",
                ((unit->f_1f & 0x10) ? 'X' : '-'),
                ((unit->f_3b & 0x10) ? 'X' : '-'),
                ((unit->f_57 & 0x10) ? 'X' : '-'));
        FUN_004c14f0(surface, buf, 0x86, y, -1);
        int i = 0;
        if (type->count > 0) {
            y += lineHeight;
            do {
                unsigned short id = type->types[i];
                int prob = FUN_0040bb00(unit->f_ff, id);
                unsigned char& color = colors[15];
                Rect_004685a0 bar;
                bar.left = 0x88;
                bar.top = y + 1;
                bar.right = 0xa2;
                bar.bottom = bar.top + lineHeight - 6;
                FUN_004bf8c0(surface, &bar, color);
                int pct = prob;
                if (pct >= 100)
                    pct = 100;
                if (pct > 0) {
                    bar.right = (bar.right - bar.left) * pct / 100 + bar.left;
                    FUN_004bf6f0(surface, &bar, color);
                }
                char* name = (char*)g_game->defs + id * 0x249 + 0x20;
                sprintf(buf, "       %3d %% - '%s'\n", prob, name);
                FUN_004c14f0(surface, buf, 0x86, y, -1);
                y += lineHeight;
                i++;
            } while (i < type->count);
        }
    }
    DAT_0051e540 = y;
    return 1;
}
