// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL. Only the head of FUN_00448c70 is transcribed, down to the
// "MAPNAME" / viewmap.gui update at 0x448eb3. What still differs:
//   * the function returns early where the original carries on into the
//     FUN_004358f0 test at 0x448eb3 and the whole player-list GUI refresh;
//   * the per-player loop from 0x4491af to the end (PLAYER%d / SIDE%d /
//     ALLY%d / TEAMICONS%d / RES%d / PING%d / MEM%d / READY%d updates) is
//     not written;
//   * the scroll arithmetic at 0x448cfa and the strcpy/strcmp expansions
//     are written with the C library, not the exact expression MSVC inlined.
//
// Layout: g_game is a pointer. Player i sits at g_game + i*0x14b + 0x1b63,
// the player's info pointer at player+0x27 (so g_game + i*0x14b + 0x1b8a).
// The map-name table is g_game + 0x12ef with stride 0x48.
#include <stdio.h>
#include <string.h>

extern char* g_game;

char* __stdcall FUN_0049ff90(char* entries, const char* name);
char* __stdcall FUN_004a0180(char* entries, const char* name);
int __stdcall FUN_004a0bf0(char* gui, const char* name, const char* text, int flag);
int __stdcall FUN_004ab060(char* gui, const char* name);
int FUN_004a50b0(void);
char* __stdcall FUN_004b6af0(int list, int index);
void FUN_00445ed0(void);
void FUN_00444a20(void);

struct Class_00435c30 { char* FUN_00435c30(); };
struct Class_00435c40 { int   FUN_00435c40(); };
struct Class_00435c20 { char* FUN_00435c20(); };
struct Class_00435a20 { void  FUN_00435a20(void* p); };
struct Class_004358f0 { int   FUN_004358f0(); };

// FUNCTION: 0x448c70
void FUN_00448c70(void)
{
    unsigned int idx = *(unsigned char*)(g_game + 0x2a42);
    char* player = g_game + idx * 0x14b + 0x1b63;
    unsigned int visible = (*(unsigned char*)(*(int*)(player + 0x27) + 0x9b) & 0x20) >> 5;
    int count = 0;

    char* gadget = FUN_0049ff90(*(char**)(*(int*)(g_game + 0x531) + 4), "OUTPUT");

    unsigned int top = *(unsigned short*)(g_game + 0x2a3e);
    unsigned int bot = *(unsigned short*)(g_game + 0x2a40);
    if ((int)top < (int)bot)
        top += 0x1e;
    int diff = top - bot;
    if (diff > (short)*(short*)(gadget + 0x19) / (FUN_004a50b0() + 2)) {
        unsigned short v = *(unsigned short*)(g_game + 0x2a40) + 1;
        *(unsigned short*)(g_game + 0x2a40) = v;
        if (v >= 0x1e)
            *(unsigned short*)(g_game + 0x2a40) = 0;
    }

    if (*(unsigned short*)(g_game + 0x2a3e) != *(unsigned short*)(g_game + 0x2a40)) {
        unsigned int b = *(unsigned short*)(g_game + 0x2a40);
        do {
            strcpy(FUN_004b6af0(*(int*)(g_game + 0x2a9b), count),
                   g_game + b * 0x48 + 0x12ef);
            count++;
            b++;
            if (b == 0x1e)
                b = 0;
        } while (*(unsigned short*)(g_game + 0x2a3e) != b);
    }

    FUN_00445ed0();
    *(short*)(gadget + 0xc0) = (short)count;

    char* mapGadget = FUN_004a0180(*(char**)(*(int*)(g_game + 0x531) + 4), "MAPNAME");
    char* oldName = ((Class_00435c30*)(*(int*)(g_game + 0x391e9)))->FUN_00435c30();
    if (!((Class_00435c40*)(*(int*)(g_game + 0x391e9)))->FUN_00435c40()) {
        *(int*)(mapGadget + 0x23) = 0xc;
        FUN_004a0bf0(g_game + 0x519, "MAPNAME", "NOT SELECTED", 0);
        return;
    }

    char* curName = ((Class_00435c20*)(*(int*)(g_game + 0x391e9)))->FUN_00435c20();
    if (strcmp(mapGadget + 0xb6, curName) != 0) {
        if (FUN_004ab060(g_game + 0x519, "viewmap.gui") != 0)
            FUN_00444a20();
        else
            ((Class_00435a20*)(*(int*)(g_game + 0x391e9)))->FUN_00435a20(oldName);
    }

    // Remaining original work not transcribed; see notes above.
    (void)visible;
    return;
}
