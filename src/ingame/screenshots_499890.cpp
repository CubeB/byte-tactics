// Decompiled by GPT-5.6-Terra, finished by Space Bunny Free. Names are provisional.
// The key test is a plain `unsigned char` shift in the original disassembly
// (mov cl, [eax+0x37f2f]; shr cl, 1; test cl, 1), which only comes out of a
// standalone `if` on the second bit of an `unsigned short` bitfield; the
// storage unit of that bitfield is two bytes, so the padding after it starts
// at 0x37f31. g_game+0xc is a pointer, the final test reads a byte at +0xf1
// through it.
#include <stdio.h>

class Class_00435100 {
public:
    int FUN_00435100();
};

struct Input_00499890 {
    int fields[6];
};

struct Ctx_00499890 {
    char unknown_0[0xf1];
    unsigned char flags;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0xc];
    Ctx_00499890* unknown_c;
    char unknown_10[0x519 - 0x10];
    char menu[0x10];
    char unknown_529[0x2c76 - 0x529];
    Input_00499890 selected;
    char unknown_2c8e[0x37ebe - 0x2c8e];
    unsigned short orderFlags;
    char unknown_37ec0[0x37f2f - 0x37ec0];
    unsigned short screenBitA : 1;
    unsigned short screenBitB : 1;
    char unknown_37f31[0x38a37 - 0x37f31];
    int screenshot;
    char unknown_38a3b[0x38a51 - 0x38a3b];
    unsigned short otherFlags;
    char path[0x20c];
    char unknown_38c5f[0x391e9 - 0x38c5f];
    Class_00435100* manager;
    char unknown_391ed[0x391f5 - 0x391ed];
    void (__cdecl* callback)();
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005024fc[];
extern char DAT_0050966c[];

int PeekKey();
void PopKey();
void ToggleFullScreen();
void __stdcall FUN_00491d70(int);
void __stdcall MakeDirectoryPath(char*);
int __stdcall GetTicks();
void __stdcall PeekMouseEvent(Input_00499890*);
void __stdcall UpdateMenu(void*);
void FUN_00494e70();
void PlayNextSpeech();
void __stdcall PopMouseEvent(int*);
void UpdateTimers();
void __stdcall SaveScreenshot(char*, char*);

// FUNCTION: 0x499890
void FUN_00499890()
{
    Input_00499890 first;
    Input_00499890 second;
    char path[0x100];
    int key = PeekKey();

    if (key == 0x7e) {
        if (g_game->screenBitB) {
            PopKey();
            ToggleFullScreen();
        }
    }
    if ((g_game->orderFlags & 1) && key == 0xe3) {
        PopKey();
        FUN_00491d70(0);
        g_game->orderFlags &= 0xfffe;
        if (g_game->manager->FUN_00435100() != 3)
            g_game->otherFlags &= 0xfffe;
    }
    if (key == 0xd6) {
        PopKey();
        MakeDirectoryPath(g_game->path);
        sprintf(path, DAT_005024fc, g_game->path);
        MakeDirectoryPath(path);
        SaveScreenshot(path, DAT_0050966c);
        g_game->screenshot = GetTicks();
    }
    PeekMouseEvent(&first);
    UpdateMenu(g_game->menu);
    PeekMouseEvent(&second);
    FUN_00494e70();
    PlayNextSpeech();
    if (first.fields[4] == second.fields[4])
        PopMouseEvent(g_game->selected.fields);
    else if (first.fields[4] == 0x205 || first.fields[4] == 0x202)
        g_game->selected = first;
    else
        g_game->selected = second;
    UpdateTimers();
    if (!(g_game->unknown_c->flags & 8))
        g_game->callback();
}
