// Decompiled by Claude Opus 5.5. Names are provisional.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004441a0 {                 // a gadget of the menu, 0x15b bytes
    short unknown_0;
    char name[0x10];                    // +0x02
    char unknown_12[0xba - 0x12];
    short field_ba;                     // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Holder_004441a0 {
    int unknown_0;
    Entry_004441a0* entries;            // +0x4
};

struct Menu_004441a0 {
    char unknown_0[0x18];
    Holder_004441a0* holder;            // +0x18
    char unknown_1c[0x60 - 0x1c];
    int selected;                       // +0x60
};

struct Conn_004441a0 {
    void* data;
    int size;
};

struct Game_004441a0 {
    char unknown_0[0x2a4b];
    void* descriptions;                 // +0x2a4b
    char unknown_2a4f[0x2a9f - 0x2a4f];
    void* guids;                        // +0x2a9f
    Conn_004441a0* conns;               // +0x2aa3
    char unknown_2aa7[0x2bc0 - 0x2aa7];
    char field_2bc0;                    // +0x2bc0
};
#pragma pack(pop)

struct LinkInfo {
    int id;             // -1: unused
    char name[32];
};

extern Game_004441a0* g_game;
extern LinkInfo DAT_005127c8[];

void __cdecl FUN_004d85a0(void* p);
int __stdcall IsCurrentGadgetNamed(void* menu, char* name);
Entry_004441a0* __stdcall FindGadgetChecked(void* entries, char* name);
int __stdcall FindGadgetIndex(void* entries, const char* name, int flag);
int __stdcall SelectConnection(int index);
void __stdcall PlaySoundByName(char* name, int flag);
void __stdcall FUN_00491c80(int value);
int __stdcall OnlineProcessButtonCommand(int button, char* message, unsigned int size);
void OnlineUnload();
char* __stdcall FUN_004c5740(char* text);
void __stdcall OpenMessageBox(void* menu, char* text, int size, int param_4, int param_5);
void __stdcall FUN_004ab0a0(void* menu);
void OpenOptionsPanel();

// The SELPROV.GUI menu's handler. A function with a try block, built without
// /GX, gives every local a stack slot, used or not: `id` and `cur` (unused, as
// in the SELGAME handler 0x4437c0's declarations) fill two of the original's
// slots. The slots are ordered scope by scope, and within a scope by a hash of
// the name (16 buckets, the later declaration first within a bucket), so the
// names `link`, `message`, `result` and `msg` are what give the original
// layout.
// FUNCTION: 0x4441a0
void __stdcall FUN_004441a0(Menu_004441a0* menu)
{
    Entry_004441a0* entries = menu->holder->entries;
    int i;
    int id;
    int cur;
    if (menu->selected == -1) {
        if (g_game->guids != 0) {
            FUN_004d85a0(g_game->guids);
            for (i = 0; i < 10; i++) {
                if (g_game->conns[i].data != 0) {
                    FUN_004d85a0(g_game->conns[i].data);
                    g_game->conns[i].data = 0;
                }
            }
            FUN_004d85a0(g_game->conns);
            FUN_004d85a0(g_game->descriptions);
            g_game->guids = 0;
            g_game->descriptions = 0;
        }
        return;
    }
    if (IsCurrentGadgetNamed(menu, "DPLAY") || IsCurrentGadgetNamed(menu, "SELECT")) {
        SelectConnection(FindGadgetChecked(entries, "DPLAY")->field_ba);
        g_game->field_2bc0 = 2;
        PlaySoundByName("BigButton", 0);
        return;
    }
    if (_strnicmp(entries[menu->selected].name, "SERVICE", 7) == 0) {
        PlaySoundByName("BigButton", 0);
        int link = atoi(entries[menu->selected].name + 7);
        char message[0x140];
        message[0] = 0;
        int result = 2;
        try {
            FUN_00491c80(0x14);
            result = OnlineProcessButtonCommand(DAT_005127c8[link].id, message, sizeof(message));
            FUN_00491c80(0x13);
        } catch (...) {
            FUN_00491c80(0x13);
        }
        if (result == 0) {
            OnlineUnload();
            exit(0);
            return;
        }
        if (message[0] == 0)
            sprintf(message, "The %s service failed or took%stoo long. Please see the readme%sfile for more information.",
                    DAT_005127c8[link].name, "\n\n", "\n\n");
        char* msg = FUN_004c5740(message);
        OpenMessageBox(menu, msg, sizeof(message), 1, 1);
        FUN_004ab0a0(menu);
        return;
    }
    if (FindGadgetIndex(entries, "PREVMENU", 0xe) == menu->selected) {
        g_game->field_2bc0 = 3;
        PlaySoundByName("Previous", 0);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "SETTINGS")) {
        PlaySoundByName("Options", 0);
        FUN_004ab0a0(menu);
        OpenOptionsPanel();
        return;
    }
    FUN_004ab0a0(menu);
}
