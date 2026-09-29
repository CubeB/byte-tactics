// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Title-screen click handler: dispatches on the gadget name, SINGLE/MULTI
// set the front-end state (+0x2bc0) to 5/6, INTRO checks the CD and starts
// the movie state (clearing +0x2bbe/+0x2bbf/+0x2bc0), EXIT goes to 8 and
// Credits to 9; unknown gadgets just release themselves.
#include <stdio.h>
#include <windows.h>

class Class_004c2ea0 {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

struct Gadget_00425d80 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Display_00425d80 {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;           // +0xf0
    unsigned short fullscreen : 1;
    unsigned short rest : 14;
};

extern char* g_game;
extern void* DAT_00512298;

int FUN_00428bc0(void);
void FUN_0041d4c0();
char __stdcall FUN_0041d6a0(int param_1);
int FUN_004c1ab0(void);
void FUN_004c63a0();
void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void FUN_004d85a0(void* p);
void __stdcall FUN_0047f1a0(char* name, int param_2);
void __stdcall FUN_00491c80(int param_1);
int __stdcall FUN_0049fd60(Gadget_00425d80* gadget, char* name);
void __stdcall FUN_004ab0a0(void* param_1);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004c6890(int param_1, int param_2);
void __stdcall FUN_004c69a0(int param_1);
Display_00425d80* FUN_004b6220(void);

// FUNCTION: 0x425d80
void __stdcall FUN_00425d80(Gadget_00425d80* gadget)
{
    char buf[256];

    if (gadget->field_60 == -1) {
        FUN_004d85a0(DAT_00512298);
        return;
    }
    if (FUN_0049fd60(gadget, "SINGLE")) {
        FUN_0047f1a0("BigButton", 0);
        FUN_00491c80(0x14);
        g_game[0x2bc0] = 5;
        return;
    }
    if (FUN_0049fd60(gadget, "MULTI")) {
        FUN_0047f1a0("BigButton", 0);
        FUN_00491c80(0x14);
        FUN_0041d4c0();
        FUN_004290f0(buf, "maps", "multiplay", "tdf");
        Class_004c2ea0 obj;
        if (((Class_004c2f60*)&obj)->FUN_004c2f60(buf) != 0) {
            g_game[0x2bc0] = 6;
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c6890(0, 0);
            FUN_004c63a0();
            return;
        }
        FUN_004abd90(g_game + 0x519,
                     FUN_004c5740("Please insert the Multiplayer CD (Disc 1) and try again"),
                     200, 1, 1);
        FUN_004ab0a0(g_game + 0x519);
        return;
    }
    if (FUN_0049fd60(gadget, "INTRO")) {
        FUN_0047f1a0("smlButton", 0);
        Display_00425d80* display = FUN_004b6220();
        if (!display->fullscreen) {
            FUN_004abd90(g_game + 0x519,
                         "Debug:  You must be in full-screen mode to play a movie.",
                         200, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
            return;
        }
        if (!FUN_0041d6a0(0) && !FUN_0041d6a0(1)) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("Please insert a Total Annihilation CD and try again"),
                         200, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
            return;
        }
        FUN_00491c80(0x14);
        if (GetKeyState(0x10) < 0) {
            *(int*)(g_game + 0x39241) = 1;
        } else {
            *(int*)(g_game + 0x39241) = 0;
        }
        while (FUN_004c1ab0()) {
        }
        if (FUN_00428bc0()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    580, "c:\\cavedog\\wargame\\frontend.cpp");
            FUN_004abd90(g_game + 0x519, buf, 500, 1, 1);
        }
        g_game[0x2bbe] = 1;
        if (FUN_00428bc0()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    155, "c:\\cavedog\\wargame\\frontend.cpp");
            FUN_004abd90(g_game + 0x519, buf, 500, 1, 1);
        }
        g_game[0x2bbf] = 0;
        g_game[0x2bc0] = 0;
        return;
    }
    if (FUN_0049fd60(gadget, "EXIT")) {
        FUN_0047f1a0("exit", 0);
        FUN_00491c80(0x14);
        g_game[0x2bc0] = 8;
        return;
    }
    if (FUN_0049fd60(gadget, "Credits")) {
        FUN_0047f1a0("smlButton", 0);
        Display_00425d80* display = FUN_004b6220();
        if (!display->fullscreen) {
            FUN_004abd90(g_game + 0x519,
                         "Debug:  You must be in full-screen mode to play a movie.",
                         200, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
            return;
        }
        if (!FUN_0041d6a0(0) && !FUN_0041d6a0(1)) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("Please insert a Total Annihilation CD and try again"),
                         200, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
            return;
        }
        FUN_00491c80(0x14);
        g_game[0x2bc0] = 9;
        return;
    }
    FUN_004ab0a0(gadget);
}
