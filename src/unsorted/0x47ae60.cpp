// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 2947-byte skirmish setup dialog handler. Reproduces the entry
// sequence and the "Start"/"PrevMenu" dispatch only; the long
// strcmp(control text, "...") chain and the per-branch bodies are unwritten.
// Frame is sub esp,0x70 with buf at [esp+0x40]; the inline strcmp loops in
// the original (0x47b138 onward, 2-byte unrolled, sbb/sbb) mean the source
// used plain strcmp() against string literals.
#include <string.h>
#include <stdlib.h>

struct Sub_0047ae60 {
    char unknown_0[4];                 // +0x0
    void* field_4;                     // +0x4 hwnd/window handle
    char unknown_8[0x37 - 8];
    int field_37;                      // +0x37
};

struct Gadget_0047ae60 {
    char unknown_0[0x18];
    Sub_0047ae60* field_18;            // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60 control id
};

class Class_00435a20 {
public:
    int FUN_00435a20(void* param_1);
};

class Class_00437300 {
public:
    int FUN_00437300();
};

extern char* g_game;                   // 0x511de8

int FUN_00435a20(void* param_1);
void __stdcall FUN_0047f1a0(char* name, int param_2);
char __stdcall FUN_0041d6a0(int param_1);
void FUN_0041d4c0();
void __stdcall FUN_00491c80(int param_1);
void __stdcall FUN_004ab0a0(void* param_1);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);
char* __stdcall FUN_004c5740(char* text);
int __stdcall FUN_0049fd60(Gadget_0047ae60* gadget, char* name);
void __stdcall FUN_0049fed0(void* hwnd, char* buf, int id);
void FUN_004797e0(int param_1);
int FUN_00479760();
void FUN_0047a760();
void FUN_0041da30();
void FUN_00430f00();
void FUN_0047aaf0();

// FUNCTION: 0x47ae60
void __stdcall FUN_0047ae60(Gadget_0047ae60* gadget)
{
    char buf[0x30];
    char* setup;
    int count;
    int c2;
    int c1;
    int n;

    if (gadget->field_60 == -1) {
        return;
    }
    FUN_0049fed0(gadget->field_18->field_4, buf, gadget->field_60);
    n = atoi(&buf[strlen(buf) - 1]);
    setup = *(char**)(g_game + 0x29a0);
    *(int*)(setup + 0x224) = n;
    buf[strlen(buf) - 1] = 0;

    if (FUN_0049fd60(gadget, "Start")) {
        FUN_0047f1a0("BigButton", 0);
        if (!FUN_0041d6a0(1)) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("Please insert the Multiplayer CD (Disc 1) and try again"),
                         0xc8, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
        }
        FUN_0041d4c0();

        n = 0;
        count = *(int*)(g_game + 0x38d81);
        if (count > 0) {
            int* p = *(int**)(g_game + 0x29a0);
            do {
                if (*p == 2) {
                    n++;
                }
                p += 6;
            } while (--count);
        }
        *(short*)(g_game + 0x2a3c) = n + 1;

        if (((Class_00435a20*)*(void**)(g_game + 0x391e9))->FUN_00435a20(
                (void*)(*(int*)(g_game + 0x29a0) + 0x11c)) == 0) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("The terrain for the selected map does not exist."),
                         0x1e0, 1, 1);
            FUN_004ab0a0(gadget);
            return;
        }

        c2 = 0;
        count = *(int*)(g_game + 0x38d81);
        if (count > 0) {
            int* p = *(int**)(g_game + 0x29a0);
            do {
                if (*p == 2) {
                    c2++;
                }
                p += 6;
            } while (--count);
        }
        if (c2 < 1) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("There must be at least one player and one computer opponent"),
                         0x1e0, 1, 1);
            FUN_004ab0a0(gadget);
            return;
        }

        c1 = 0;
        if (count > 0) {
            int* p = *(int**)(g_game + 0x29a0);
            do {
                if (*p == 1) {
                    c1++;
                }
                p += 6;
            } while (--count);
        }
        if (c1 < 1) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("There must be at least one player and one computer opponent"),
                         0x1e0, 1, 1);
            FUN_004ab0a0(gadget);
            return;
        }

        if ((int)(unsigned short)*(short*)(g_game + 0x2a3c) >
            ((Class_00437300*)*(void**)(g_game + 0x391e9))->FUN_00437300()) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("There are too many players enabled for this map"),
                         0x1e0, 1, 1);
            FUN_004ab0a0(gadget);
            return;
        }

        if (FUN_00479760() != 0) {
            FUN_004abd90(g_game + 0x519,
                         FUN_004c5740("All players may not be in the same allied group."),
                         0x1e0, 1, 1);
            FUN_004ab0a0(gadget);
            return;
        }

        c2 = 0;
        count = *(int*)(g_game + 0x38d81);
        if (count > 0) {
            int* p = *(int**)(g_game + 0x29a0);
            int i = count;
            do {
                if (*p == 2) {
                    c2++;
                }
                p += 6;
            } while (--i);
        }
        c1 = 0;
        if (count > 0) {
            int* p = *(int**)(g_game + 0x29a0);
            do {
                if (*p == 1) {
                    c1++;
                }
                p += 6;
            } while (--count);
        }
        *(short*)(g_game + 0x2a3c) = c1 + c2;

        FUN_0047a760();
        FUN_0041da30();
        FUN_00430f00();
        *(char*)(g_game + 0x2bc0) = 2;
        FUN_00491c80(0x14);
        return;
    }

    if (FUN_0049fd60(gadget, "PrevMenu")) {
        FUN_0047f1a0("Previous", 0);
        FUN_00491c80(0x14);
        *(char*)(g_game + 0x2bc0) = 3;
        return;
    }

    if (strcmp(buf, "Player") == 0) {
        FUN_0047f1a0("Skirmish", 0);
        FUN_004797e0(1);
        FUN_004ab0a0(gadget);
        return;
    }

    FUN_004ab0a0(gadget);
}
