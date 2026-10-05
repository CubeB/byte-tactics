// Decompiled by space-bunny-free. Names are provisional.
// The status panel's scroll/refresh tick. When the space bar is held, `panel`
// moves a third of the way toward 0 (opening "Options") or toward -31
// (reopening "Panel"), so the panel slides instead of snapping. The rate limit
// DAT_0051e544 (GetTickCount() + 15) keeps one move per frame. Everything drawn
// below is positioned relative to bounds.bottom + `panel`, so the text scrolls
// with it. The three lines are drawn at fixed offsets from bounds.left (0x19,
// 0xbe, 0x17c), each sprintf'ed into the same 256-byte `buf` before it is drawn,
// so the earlier text is gone by the time the next line is written. The speed
// line is built in `num` and then appended to in place, with strlen, so the
// " (%+d)" of a second, differing speed value lands at its end.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

struct Rect_004689c0 {
    int left;
    int top;
    int right;
    int bottom;
};

class Class_004c6ae0 {
public:
    char unknown_0[0x1c];
    Rect_004689c0 field_1c;              // +0x1c
    Rect_004689c0* FUN_004c6ae0(Rect_004689c0* out);
};

struct Team_004689c0 {                  // 347 bytes
    char unknown_0[4];
    unsigned char* data;                // +0x4
    char unknown_8[347 - 8];
};

struct Player_004689c0 {                // 331 bytes
    char unknown_0[0x119];
    unsigned short field_119;           // +0x119
    char unknown_11b[331 - 0x11b];
};

struct Game {
    char unknown_0[0x521];
    int field_521;                      // +0x521
    int field_525;                      // +0x525
    char unknown_529[4];
    int field_52d;                      // +0x52d
    Team_004689c0* teams;               // +0x531
    char unknown_535[0x57d - 0x535];
    int team_index;                     // +0x57d
    char unknown_581[0x1b8e - 0x581];
    Player_004689c0 players[11];        // +0x1b8e
    char unknown_29c7[0x2a42 - 0x29c7];
    unsigned char team_number;          // +0x2a42
    char unknown_2a43[0x37e90 - 0x2a43];
    int panel;                          // +0x37e90
    void* sprite;                       // +0x37e94
    char unknown_37e98[0x37ee6 - 0x37e98];
    unsigned short max_units;           // +0x37ee6
    char unknown_37ee8[0x38a47 - 0x37ee8];
    unsigned int tick;                  // +0x38a47
    unsigned short speed;               // +0x38a4b
    unsigned short speed2;              // +0x38a4d
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e544;

void __stdcall FUN_0047f1a0(char* name, int param_2);
int __stdcall FUN_004c1b80(int key);
int FUN_004b6560();
void __stdcall FUN_004b7f90(void* dst, void* bmp, int x, int y);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004a50e0(void* surf, void* text, int x, int y, int color, int just);

// FUNCTION: 0x4689c0
void __stdcall FUN_004689c0(Class_004c6ae0* win)
{
    char buf[0x100];
    char num[0x34];
    int v = g_game->panel;
    if (DAT_0051e544 < FUN_004b6560()) {
        DAT_0051e544 = FUN_004b6560() + 15;
        if (!FUN_004c1b80(0x20) || (g_game->team_index != -1 && ((unsigned char*)g_game->teams->data)[g_game->team_index * 347] == 3)) {
            if (v < 0) {
                if (v == -31)
                    FUN_0047f1a0("Panel", 0);
                int q = (0 - v) / 3;
                if (q <= 1)
                    q = 1;
                v += q;
                if (v == 0)
                    FUN_0047f1a0("Options", 0);
                g_game->panel = v;
            }
        } else {
            if (v > -31) {
                if (v == 0)
                    FUN_0047f1a0("Panel", 0);
                int q = (v + 31) / 3;
                if (q <= 1)
                    q = 1;
                v -= q;
                if (v == -31)
                    FUN_0047f1a0("Options", 0);
            }
            g_game->panel = v;
        }
    }
    if (v == 0)
        return;
    g_game->field_52d = g_game->field_525;
    Rect_004689c0 bounds;
    win->FUN_004c6ae0(&bounds);
    int left = bounds.left;
    int bottom = bounds.bottom + v;
    FUN_004b7f90(win, g_game->sprite, left, bottom);
    unsigned int tick = g_game->tick;
    int hours = tick / 108000;
    int rest = tick - hours * 108000;
    int minutes = rest / 1800;
    int seconds = (rest - minutes * 1800) / 30;
    sprintf(buf, "%s : %02d:%02d:%02d", FUN_004c5740("Game Time"), hours, minutes, seconds);
    FUN_004a50e0(win, buf, left + 0x19, bottom + 0xa, -1, 0);
    int team = g_game->team_number;
    sprintf(buf, "%s : %d  (Max %d)", FUN_004c5740("Total Units"),
            g_game->players[team].field_119, g_game->max_units);
    FUN_004a50e0(win, buf, left + 0xbe, bottom + 0xa, -1, 0);
    if (g_game->speed2 == 10)
        sprintf(num, FUN_004c5740("Normal"));
    else
        sprintf(num, "%+d", (int)g_game->speed2 - 10);
    sprintf(buf, "%s %s", FUN_004c5740("Game Speed"), num);
    if (g_game->speed2 != g_game->speed)
        sprintf(buf + strlen(buf), " (%+d)", (int)g_game->speed - 10);
    FUN_004a50e0(win, buf, left + 0x17c, bottom + 0xa, -1, 0);
    g_game->field_52d = g_game->field_521;
}
