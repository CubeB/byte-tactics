// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Debug overlay: dumps the state of the unit selected by the game's "unit
// probe" cursor. Draws a fading panel, then a column of lines ("Unit State
// Probe", uid, player, controller, build time left, damage, occupy, autotarget
// weights and the mission queues) at x 0x86. The running y is also written to
// DAT_0051e540 so the next overlay stacks below this one.
#include <stdio.h>

struct Rect_004b0510 {
    int x1;
    int y1;
    int x2;
    int y2;
};

#pragma pack(push, 1)
struct Unit {
    char pad_0[0x1f];
    unsigned char f_1f;            // +0x1f
    char pad_20[0x3b - 0x20];
    unsigned char f_3b;            // +0x3b
    char pad_3c[0x57 - 0x3c];
    unsigned char f_57;            // +0x57
    char pad_58[0x5c - 0x58];
    void* f_5c;                    // +0x5c
    void* f_60;                    // +0x60
    char pad_64[0x92 - 0x64];
    char* f_92;                    // +0x92
    int* f_96;                     // +0x96
    char pad_9a[0xa8 - 0x9a];
    unsigned short f_a8;           // +0xa8
    char pad_aa[0x104 - 0xaa];
    float f_104;                   // +0x104
    short f_108;                   // +0x108
    char pad_10a[0x110 - 0x10a];
    int f_110;                     // +0x110
    char pad_114[0x118 - 0x114];
};

struct Mission_00467e50 {
    char pad_0[5];
    unsigned char state;           // +0x5
    char pad_6[0x16 - 0x6];
    void* target;                  // +0x16
    char pad_1a[0x4a - 0x1a];
    void* next;                    // +0x4a
};

struct MissionName_00467e50 {
    char pad_0[0x15];
    char* name;                    // +0x15
};

struct Class_00438830 {
    MissionName_00467e50* FUN_00438830();
};
#pragma pack(pop)

extern char* g_game;
extern int DAT_0051e540;

int GetTextKeyColor();
void __stdcall SetTextColors(int color, int font);
void __stdcall SetFont(int arg);
int GetFontHeight();
void FUN_004c6b60();
void __stdcall FadeRectangle(void* surface, Rect_004b0510* rect, int level);
void __stdcall DrawRectangle(void* surface, Rect_004b0510* rect, int color);
void __stdcall DrawString(void* surface, const char* text, int x, int y, int maxWidth);

// FUNCTION: 0x467e50
int __stdcall DrawUnitStateProbe(void* surface)
{
    char buf[0x80];
    char* names[3];
    unsigned char* colors;
    Unit* unit;
    Rect_004b0510 r;
    int lineH;
    int y;
    int prev;
    Mission_00467e50* m;

    if (*(int*)(g_game + 0x391b3) == 0 || *(unsigned short*)(g_game + 0x391b7) == 0)
        return 0;
    unit = (Unit*)(*(int*)(g_game + 0x14357)
                            + *(unsigned short*)(g_game + 0x391b7) * 0x118);
    if ((unit->f_110 & 0x10000000) == 0 || (unit->f_110 & 0x4000) != 0) {
        *(int*)(g_game + 0x391b3) = 0;
        *(unsigned short*)(g_game + 0x391b7) = 0;
    }
    colors = (unsigned char*)(g_game + 0xdcb);
    SetTextColors(colors[15], GetTextKeyColor());
    SetFont(*(int*)(g_game + 0x391f9));
    lineH = GetFontHeight() + 3;
    y = lineH * 7;
    FUN_004c6b60();
    prev = DAT_0051e540;
    r.x1 = 0x83;
    r.x2 = 0x191;
    r.y1 = y;
    r.y2 = prev == 0 ? lineH * 20 : prev;
    FadeRectangle(surface, &r, -0x18);
    r.x2++;
    r.y2++;
    DrawRectangle(surface, &r, colors[5]);
    y += 3;
    DrawString(surface, "Unit State Probe", 0x86, y, -1);
    y += lineH;
    DrawString(surface, "================", 0x86, y, -1);
    y += lineH;
    sprintf(buf, "uid: %03d/%04x '%s'\n", unit->f_a8, unit->f_a8, unit->f_92);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    sprintf(buf, "playerno: %d '%s' %s - %s\n",
            *(unsigned char*)((char*)unit->f_96 + 0x146),
            (char*)unit->f_96 + 0x2b,
            (*unit->f_96 != 0
             && (*(unsigned char*)((char*)unit->f_96 + 0x73) == 1
                 || *(unsigned char*)((char*)unit->f_96 + 0x73) == 2))
                ? "LOCAL" : "REMOTE",
            *(unsigned char*)(unit->f_92 + 0x22f) != 0 ? "MOBILE" : "BUILDING");
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    sprintf(buf, "controller: %d\n", *(unsigned char*)((char*)unit->f_96 + 0x73));
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    sprintf(buf, "buildtimeleft: %1.3f\n", unit->f_104);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    sprintf(buf, "damage: %d\n", unit->f_108);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    names[0] = "NONE";
    names[1] = "GROUND";
    names[2] = "AIR";
    sprintf(buf, "occupy: %s\n", names[unit->f_110 & 3]);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    if (*unit->f_96 != 0
        && (*(unsigned char*)((char*)unit->f_96 + 0x73) == 1
            || *(unsigned char*)((char*)unit->f_96 + 0x73) == 2)) {
        sprintf(buf, "autotarget w[pri:sec:spe]: w[%c:%c:%c]\n",
                (unit->f_1f & 0x10) ? 'X' : '-',
                (unit->f_3b & 0x10) ? 'X' : '-',
                (unit->f_57 & 0x10) ? 'X' : '-');
        DrawString(surface, buf, 0x86, y, -1);
        y += lineH;
        if (unit->f_5c != 0) {
            DrawString(surface, "Mission Q:", 0x86, y, -1);
            y += lineH;
            for (m = (Mission_00467e50*)unit->f_5c; m != 0;
                 m = (Mission_00467e50*)m->next) {
                if (m->target != 0)
                    sprintf(buf, "    '%s' state: %d  tgt: '%s'\n",
                            ((Class_00438830*)((char*)m + 4))->FUN_00438830()->name,
                            m->state, *(char**)((char*)m->target + 0x92));
                else
                    sprintf(buf, "    '%s' state: %d\n",
                            ((Class_00438830*)((char*)m + 4))->FUN_00438830()->name,
                            m->state);
                DrawString(surface, buf, 0x86, y, -1);
                y += lineH;
            }
        }
        if (unit->f_60 != 0) {
            DrawString(surface, "Background Mission Q:", 0x86, y, -1);
            y += lineH;
            for (m = (Mission_00467e50*)unit->f_60; m != 0;
                 m = (Mission_00467e50*)m->next) {
                if (m->target != 0)
                    sprintf(buf, "    '%s' state: %d  tgt: '%s'\n",
                            ((Class_00438830*)((char*)m + 4))->FUN_00438830()->name,
                            m->state, *(char**)((char*)m->target + 0x92));
                else
                    sprintf(buf, "    '%s' state: %d\n",
                            ((Class_00438830*)((char*)m + 4))->FUN_00438830()->name,
                            m->state);
                DrawString(surface, buf, 0x86, y, -1);
                y += lineH;
            }
        }
    }
    DAT_0051e540 = y;
    return 1;
}
