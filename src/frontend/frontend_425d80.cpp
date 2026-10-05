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

int CodeChecksumFailed(void);
void RegisterDataArchives();
char __stdcall FindGameCdDrive(int param_1);
int PopKey(void);
void FlipScreen();
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __cdecl FUN_004d85a0(void* p);
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall FUN_00491c80(int param_1);
int __stdcall IsCurrentGadgetNamed(Gadget_00425d80* gadget, char* name);
void __stdcall FUN_004ab0a0(void* param_1);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FillSurface(int param_1, int param_2);
void __stdcall SetOffscreenSurface(int param_1);
Display_00425d80* GetDisplay(void);

// FUNCTION: 0x425d80
void __stdcall HandleMainMenuClick(Gadget_00425d80* gadget)
{
    char buf[256];

    if (gadget->field_60 == -1) {
        FUN_004d85a0(DAT_00512298);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "SINGLE")) {
        PlaySoundByName("BigButton", 0);
        FUN_00491c80(0x14);
        g_game[0x2bc0] = 5;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "MULTI")) {
        PlaySoundByName("BigButton", 0);
        FUN_00491c80(0x14);
        RegisterDataArchives();
        BuildDataPath(buf, "maps", "multiplay", "tdf");
        Class_004c2ea0 obj;
        if (((Class_004c2f60*)&obj)->FUN_004c2f60(buf) != 0) {
            g_game[0x2bc0] = 6;
            SetOffscreenSurface(*(int*)(g_game + 0x37e1b));
            FillSurface(0, 0);
            FlipScreen();
            return;
        }
        OpenMessageBox(g_game + 0x519,
                     FUN_004c5740("Please insert the Multiplayer CD (Disc 1) and try again"),
                     200, 1, 1);
        FUN_004ab0a0(g_game + 0x519);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "INTRO")) {
        PlaySoundByName("smlButton", 0);
        Display_00425d80* display = GetDisplay();
        if (!display->fullscreen) {
            OpenMessageBox(g_game + 0x519,
                         "Debug:  You must be in full-screen mode to play a movie.",
                         200, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
            return;
        }
        if (!FindGameCdDrive(0) && !FindGameCdDrive(1)) {
            OpenMessageBox(g_game + 0x519,
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
        while (PopKey()) {
        }
        if (CodeChecksumFailed()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    580, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
        }
        g_game[0x2bbe] = 1;
        if (CodeChecksumFailed()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    155, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
        }
        g_game[0x2bbf] = 0;
        g_game[0x2bc0] = 0;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "EXIT")) {
        PlaySoundByName("exit", 0);
        FUN_00491c80(0x14);
        g_game[0x2bc0] = 8;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "Credits")) {
        PlaySoundByName("smlButton", 0);
        Display_00425d80* display = GetDisplay();
        if (!display->fullscreen) {
            OpenMessageBox(g_game + 0x519,
                         "Debug:  You must be in full-screen mode to play a movie.",
                         200, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
            return;
        }
        if (!FindGameCdDrive(0) && !FindGameCdDrive(1)) {
            OpenMessageBox(g_game + 0x519,
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
