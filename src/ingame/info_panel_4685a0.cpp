// Decompiled by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
#include <windows.h>
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

struct Unit {
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

struct Game {
    char unknown_0[0xdcb];
    unsigned char colors[16];          // +0xdcb, [15] is the text colour
    char unknown_ddb[0x14357 - 0xddb];
    Unit* units;                       // +0x14357
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

extern Game* g_game;
extern int DAT_0051e540;

int GetTextKeyColor();
void __stdcall SetTextColors(int param_1, int param_2);
void __stdcall SetFont(int param_1);
int GetFontHeight();
int FUN_004c6b60();
int __stdcall FUN_0040bb00(int player, unsigned short type);
void __stdcall FadeRectangle(void* surface, void* rect, int level);
void __stdcall DrawRectangle(void* surface, void* rect, int color);
void __stdcall FillRectangle(void* surface, void* rect, int color);
void __stdcall DrawString(void* dst, const char* text, int x, int y, int maxWidth);


static void Bar_004685a0(void* surface, Rect_004685a0* rect, int percent)
{
    unsigned char& color = g_game->colors[15];
    DrawRectangle(surface, rect, color);
    percent = (percent >= 100) ? 100 : percent;
    if (percent > 0) {
        rect->right = (rect->right - rect->left) * percent / 100 + rect->left;
        FillRectangle(surface, rect, color);
    }
}

// FUNCTION: 0x4685a0
int __stdcall DrawUnitBuilderProbe(void* surface)
{
    if (g_game->f_391b9 == 0 || g_game->f_391bd == 0)
        return 0;
    Unit* unit = &g_game->units[g_game->f_391bd];
    if ((unit->f_110 & 0x10000000) == 0 || (unit->f_110 & 0x4000) != 0
            || unit->type->types == 0) {
        g_game->f_391b3 = 0;
        g_game->f_391b7 = 0;
    }
    unsigned char* colors = g_game->colors;
    SetTextColors(colors[15], GetTextKeyColor());
    SetFont(g_game->f_391f9);
    int lineHeight = GetFontHeight() + 3;
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
    FadeRectangle(surface, &r, -0x18);
    r.right++;
    r.bottom++;
    DrawRectangle(surface, &r, colors[5]);
    char buf[0x80];
    y += 3;
    DrawString(surface, "Unit Builder Probe", 0x86, y, -1);
    y += lineHeight;
    DrawString(surface, "==================", 0x86, y, -1);
    y += lineHeight;
    sprintf(buf, "uid: %03d '%s'\n", unit->f_a8, (char*)unit->type);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineHeight;
    char* mobile = unit->type->mobile ? "MOBILE" : "BUILDING";
    char* remote;
    if (unit->player->f_0 != 0 && (unit->player->controller == 1 || unit->player->controller == 2))
        remote = "LOCAL";
    else
        remote = "REMOTE";
    sprintf(buf, "playerno: %d '%s' %s - %s\n", unit->player->player, unit->player->name, remote, mobile);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineHeight;
    sprintf(buf, "controller: %d\n\n", unit->player->controller);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineHeight;
    if (unit->player->f_0 != 0 && (unit->player->controller == 1 || unit->player->controller == 2)) {
        sprintf(buf, "Units I can build, and the probabilities:\n",
                ((unit->f_1f & 0x10) ? 'X' : '-'),
                ((unit->f_3b & 0x10) ? 'X' : '-'),
                ((unit->f_57 & 0x10) ? 'X' : '-'));
        DrawString(surface, buf, 0x86, y, -1);
        int i = 0;
        y += lineHeight;
        if (unit->type->count > 0) {
            do {
                unsigned short id = unit->type->types[i];
                int prob = FUN_0040bb00(unit->f_ff, id);
                Def_004685a0* def = &g_game->defs[id];
                Rect_004685a0 bar;
                bar.left = 0x88;
                bar.top = y + 1;
                bar.right = 0xa2;
                bar.bottom = bar.top + lineHeight - 6;
                Bar_004685a0(surface, &bar, prob);
                char* name = def->name;
                sprintf(buf, "       %3d %% - '%s'\n", prob, name);
                DrawString(surface, buf, 0x86, y, -1);
                y += lineHeight;
                i++;
            } while (i < unit->type->count);
        }
    }
    DAT_0051e540 = y;
    return 1;
}
