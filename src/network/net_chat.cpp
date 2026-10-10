// Decompiled by space-bunny-free, Opus, DeepSeek V4.1 Flash, deepseek-v4.1-flash, GPT-6.1-sol, deepseek-v4.1, mimo-v2.6-pro and space-bunny-alpha. Names are provisional.

#include <string.h>
#include <stdio.h>
#include <math.h>

#pragma pack(push, 1)

#include "../map/mission.h"

// One 0x48-byte message ring record. AddMessage, ScrollToNextMessageUnit and
// DrawMessages see the array at +0x12ef; ExpireOldestMessage sees only the
// record's time, which its view puts at +0 (so its array begins at +0x132f,
// the first record's time), and CycleMessageUnits sees only the flags, at
// +0x1e of an array beginning at +0x1318. All three resolve to the same
// records, so one ChatHudEntry carries every name at its true offset. The two unit
// indices overlap a byte apart and cannot share the name `unit`, so the 8-bit
// DrawMessages one is `unit_46`; DrawMessages reads it there instead.
struct ChatHudEntry {                  // 0x48 bytes
    char text[0x40];                   // +0x00
    union { unsigned int time; };      // +0x40
    unsigned short unit;               // +0x44, ScrollToNextMessageUnit's index
    union {
        char player;                   // +0x46, the sender's slot, 10 for none
        unsigned char unit_46;         // +0x46, DrawMessages' index
    };
    unsigned char flags;               // +0x47
};

#include "player.h"

struct Unit {
    char unknown_0[0x6c];
    short field_6c;                    // +0x6c
    char unknown_6e[0x74 - 0x6e];
    short field_74;                    // +0x74
    char unknown_76[0x110 - 0x76];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game {
    char unknown_0[0x519];
    char menu[0xdcb - 0x519];           // +0x519, the GUI system object
    unsigned char colors[16];          // +0xdcb
    char unknown_ddb[0x12ef - 0xddb];
    ChatHudEntry chatHudRing[30];      // +0x12ef
    char unknown_1b5f[0x1b63 - 0x1b5f];
    Player players[11];       // +0x1b63
    char unknown_299c[0x2a3e - 0x299c];
    unsigned short chatHudWriteIdx;    // +0x2a3e
    unsigned short chatHudReadIdx;     // +0x2a40
    char unknown_2a42[0x14357 - 0x2a42];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x37efe - 0x1435b];
    int mode;                          // +0x37efe
    int unit_type_mask;                // +0x37f02
    char unknown_37f06[0x37f23 - 0x37f06];
    int textScroll;                    // +0x37f23
    union {
        int textLines;                 // +0x37f27, AddMessage's ring capacity
        int max_lines;                 // +0x37f27, DrawMessages' line limit
    };
    char unknown_37f2b[0x38a47 - 0x37f2b];
    unsigned int now;                  // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* mapInfo;                  // +0x391e9
    char unknown_391ed[0x391f9 - 0x391ed];
    int font;                          // +0x391f9
};

#pragma pack(pop)

struct Rect_00464060 {
    int left, top, right, bottom;
};

extern Game* g_game;
extern char DAT_005119b8[];

void __stdcall PlaySoundByName(char* name, int param_2);
int __stdcall IsScreenNamed(void* obj, const char* name);
void __stdcall MarkChanged(void* obj);
void __stdcall MarkLayerChanged(void* obj);
void __stdcall SendChatPacket(char* param_1);
void __stdcall ReportGameChat(char* param_1);
void __stdcall CenterCameraOnPoint(int a, int b, int c);
void __stdcall SetFont(int);
void __stdcall SetTextColors(int, int);
int GetFontHeight();
void __stdcall BlitSideLogoToRect(void*, void*, Rect_00464060*, int);
void __stdcall DrawTextClipped(void*, void*, int, int, int, int);
// SendChatMessage's original translation unit declared AddMessage with int
// arguments while the function itself takes narrower widths. Keeping the int
// overload declared (it is not defined here) makes the call at 0x463e50 push
// its third argument as a full int, as the original does; without it the
// visible byte-wide definition would truncate the argument first.
void __stdcall AddMessage(char* param_1, int param_2, int param_3, int param_4);

// Appends a line to the ring buffer of 0x48-byte entries at g_game+0x12ef,
// indexed by the short at g_game+0x2a3e (wrapped at 30). If the divisor at
// g_game+0x37f27 says the head is one behind the tail, the head is advanced so
// the oldest entry is dropped.
// FUNCTION: 0x463ca0
void __stdcall AddMessage(char* text, unsigned char key, unsigned short value, char last)
{
    // Keep g_game, tail and the g_game+0x519 object written out at each use (no locals).
    if (!*text)
        return;
    if (g_game->textLines == 0)
        return;
    if ((g_game->chatHudWriteIdx + 1) % g_game->textLines == g_game->chatHudReadIdx) {
        g_game->chatHudReadIdx++;
        if (g_game->chatHudReadIdx == 30)
            g_game->chatHudReadIdx = 0;
    }
    strncpy(g_game->chatHudRing[g_game->chatHudWriteIdx].text, text, 0x40);
    g_game->chatHudRing[g_game->chatHudWriteIdx].text[0x3f] = 0;
    g_game->chatHudRing[g_game->chatHudWriteIdx].time = g_game->now;
    unsigned char c = g_game->chatHudRing[g_game->chatHudWriteIdx].flags;
    g_game->chatHudRing[g_game->chatHudWriteIdx].flags = (c ^ key) & 0xf ^ c;
    g_game->chatHudRing[g_game->chatHudWriteIdx].unit = value;
    g_game->chatHudRing[g_game->chatHudWriteIdx].player = last;
    g_game->chatHudWriteIdx++;
    if (g_game->chatHudWriteIdx == 30)
        g_game->chatHudWriteIdx = 0;
    if (last != '\n')
        PlaySoundByName("MessageArrived", 0);
    if (IsScreenNamed((char*)&g_game->menu[0], "TIMEOUT.GUI")) {
        MarkChanged((char*)&g_game->menu[0]);
        MarkLayerChanged((char*)&g_game->menu[0]);
    }
}

// FUNCTION: 0x463e50
void __stdcall SendChatMessage(Player* from, char* text, int param_3, char* to)
{
    char buf[200];

    sprintf(buf, "<%s%s%s> %s", from->fullName, to ? "->" : DAT_005119b8,
            to ? to : DAT_005119b8, text);
    SendChatPacket(buf);
    if (g_game->mapInfo->GetGameType() == 3) {
        ReportGameChat(buf);
    }
    AddMessage(buf, param_3, 0, 10);
}

// FUNCTION: 0x463ef0
int ExpireOldestMessage()
{
    int result = 0;
    unsigned short i = g_game->chatHudReadIdx;
    if (g_game->chatHudWriteIdx != i
        && g_game->chatHudRing[i].time + (g_game->textScroll + 1) * 30 < g_game->now) {
        result = 1;
        g_game->chatHudReadIdx = i + 1;
        if (g_game->chatHudReadIdx == 30)
            g_game->chatHudReadIdx = 0;
    }
    return result;
}

// CycleMessageUnits (0x464000) stays in net_chat_464000.cpp. Its original
// translation unit saw only a prototype of ScrollToNextMessageUnit, so the
// compiler could not inline it; here the definition is in the same file and
// /Ob2 expands both calls, turning 87 bytes into 316. Putting the definition
// after the caller does not help (MSVC inlines across the whole file);
// #pragma auto_inline(off) does, but the guide allows that only in a class's
// file, so 0x464000 keeps a file of its own.

// FUNCTION: 0x463f60
int ScrollToNextMessageUnit(void)
{
    Game* g = g_game;
    int i = g->chatHudReadIdx;
    int end = g->chatHudWriteIdx;
    while (end != i) {
        ChatHudEntry* e = &g->chatHudRing[i];
        unsigned short id = e->unit;
        if (id != 0 && (e->flags & 0x10) == 0) {
            Unit* u = &g->units[id];
            if (u->flags & 0x10000000) {
                e->flags |= 0x30;
                CenterCameraOnPoint(u->field_6c, u->field_74, 1);
                return 1;
            }
        }
        i++;
        if (i == 30)
            i = 0;
    }
    return 0;
}

// FUNCTION: 0x464060
void __stdcall DrawMessages(void* surf)
{
    int max_lines = g_game->max_lines;
    // Function scope, not inside the if (id != 10) body: orders the r.top store first.
    int t;
    if (max_lines == 0)
        return;
    int i = g_game->chatHudWriteIdx;
    for (int n = 1; n < max_lines; n++) {
        if (i == g_game->chatHudReadIdx)
            break;
        i--;
        if (i < 0)
            i = 0x1d;
    }
    SetFont(g_game->font);
    int start = GetFontHeight();
    if (g_game->chatHudWriteIdx == i)
        return;
    int y = 0x34;
    while (g_game->chatHudWriteIdx != i) {
        int show;
        switch (g_game->mode) {
        case 1:
            // Cavedog never assigns show in this arm when the team field is 2,
            // so the test below reads the previous iteration's value. Kept.
            if ((g_game->chatHudRing[i].flags & 0xf) != 2)
                show = 0;
            break;
        case 2:
            show = (g_game->chatHudRing[i].flags & 0xf) != 8;
            break;
        case 3:
            if (g_game->unit_type_mask == 0) {
                switch (g_game->chatHudRing[i].flags & 0xf) {
                case 1:
                case 4:
                case 8:
                    show = 1;
                    break;
                case 2:
                case 3:
                case 5:
                case 6:
                case 7:
                    show = 0;
                    break;
                default:
                    show = 0;
                    break;
                }
            } else {
                show = 1;
            }
            break;
        default:
            show = 0;
        }
        if (show) {
            if (g_game->chatHudRing[i].flags & 0x20)
                SetTextColors(g_game->colors[10], 0xfe);
            else
                SetTextColors(g_game->colors[15], 0xfe);
            int height = 138;
            int id = g_game->chatHudRing[i].unit_46;
            if (id != 10) {
                t = (int)(GetFontHeight() * 0.8);
                Rect_00464060 r;
                r.top = y;
                r.left = 138;
                r.right = t + 138;
                r.bottom = y + t;
                height = (int)(138.0 - t * -1.5);
                BlitSideLogoToRect(surf, &g_game->players[id], &r, 0);
            }
            DrawTextClipped(surf, &g_game->chatHudRing[i], height, y, -1, 0);
            y += start;
        }
        i++;
        if (i == 0x1e)
            i = 0;
    }
}
