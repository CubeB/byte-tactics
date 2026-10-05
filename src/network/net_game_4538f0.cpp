// Decompiled by Claude Opus 5.5. Names are provisional.
// Click handler of the in-game chat dialog. On close (field +0x60 == -1)
// it returns to the "Previous" menu and frees DAT_00512c74; TALK sends the
// typed text from the local player (FUN_00463e50), marks the game flag and
// clears the field, then refreshes the message area; REJECT calls
// FUN_00453010(DAT_005061d8, 6).
#include <string.h>

#pragma pack(push, 1)
struct Entry_004538f0 {
    char unknown_0[0xb6];
    char text[0x15b - 0xb6];           // +0xb6
};

struct Layer_004538f0 {
    int unknown_0;
    Entry_004538f0* entries;           // +0x04
};

struct Gadget_004538f0 {
    char unknown_0[0x18];
    Layer_004538f0* layer;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Player_004538f0 {
    char unknown_0[0x14b];
};

struct Game {
    char unknown_0[0x519];
    char message[0x531 - 0x519];       // +0x519
    Layer_004538f0* layer_531;         // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_004538f0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bee - 0x2a43];
    unsigned short flag0 : 1;          // +0x2bee, bit 0
};
#pragma pack(pop)

extern Game* g_game;
extern void* DAT_00512c74;
extern int DAT_005061d8;
extern char DAT_005119b8[];

void __stdcall FUN_0047f1a0(char* name, int param_2);
void __cdecl FUN_004d85a0(void* p);
int __stdcall FUN_0049fd60(Gadget_004538f0* gadget, char* name);
Entry_004538f0* __stdcall FUN_004a0010(Entry_004538f0* entries, char* name);
void __stdcall FUN_00463e50(Player_004538f0* from, char* text, int param_3, char* to);
int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004a7190(void* menu, int index);
void __stdcall FUN_004ab0a0(void* param_1);
void __stdcall FUN_0049fa90(void* menu);
void __stdcall FUN_00453010(int param_1, int param_2);

// FUNCTION: 0x4538f0
void __stdcall FUN_004538f0(Gadget_004538f0* gadget)
{
    Entry_004538f0* entries = gadget->layer->entries;

    if (gadget->field_60 == -1) {
        FUN_0047f1a0("Previous", 0);
        if (DAT_00512c74)
            FUN_004d85a0(DAT_00512c74);
        DAT_00512c74 = 0;
        return;
    }
    if (FUN_0049fd60(gadget, "TALK")) {
        Entry_004538f0* entry = FUN_004a0010(entries, "TALK");
        if (strlen(entry->text)) {
            FUN_00463e50(&g_game->players[g_game->localPlayer], entry->text, 4, 0);
            g_game->flag0 = 1;
            strcpy(entry->text, DAT_005119b8);
        }
        FUN_004a7190(g_game->message, FUN_0049fdf0(g_game->layer_531->entries, "TALK", 3));
        FUN_004ab0a0(g_game->message);
        FUN_0049fa90(g_game->message);
        return;
    }
    if (FUN_0049fd60(gadget, "REJECT")) {
        FUN_00453010(DAT_005061d8, 6);
        return;
    }
    FUN_004ab0a0(gadget);
}
