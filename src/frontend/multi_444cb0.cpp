// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Layout_00444cb0 {
    char unknown_0[0x14];
    void* field_14;                    // +0x14
};

struct Holder_00444cb0 {
    int unknown_0;                     // +0x0
    void* entries;                     // +0x4
    char unknown_8[0xc - 8];
    Layout_00444cb0* layout;           // +0xc
};

struct Entry_00444cb0 {
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
};

struct PlayerData_00444cb0 {
    char name[0x9b];                   // +0x0
    unsigned short flags;              // +0x9b
    char unknown_9d[0xa9 - 0x9d];
    unsigned int field_a9;             // +0xa9
    char unknown_ad[0xb9 - 0xad];
};

struct Player_00444cb0 {
    int active;                        // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerData_00444cb0* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    char state;                        // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Gadget_00444cb0 {
    char unknown_0[0x18];
    Holder_00444cb0* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Game {
    char unknown_0[0x531];
    Holder_00444cb0* holder;           // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_00444cb0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x391e9 - 0x2a43];
    void* field_391e9;                 // +0x391e9
};
#pragma pack(pop)

class Class_00435a20 {
public:
    int FUN_00435a20(char* name);
};

class Class_00435c30 {
public:
    char* FUN_00435c30();
};

class Class_004373a0 {
public:
    unsigned int FUN_004373a0();
};

extern Game* g_game;
extern char* DAT_00512990;

int __stdcall IsCurrentGadgetNamed(Gadget_00444cb0* gadget, char* name);
Entry_00444cb0* __stdcall FindGadgetChecked(void* entries, char* name);
Entry_00444cb0* __stdcall FUN_004a0280(void* entries, char* name);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall FUN_0047f1a0(char* str, int flag);
void __stdcall FUN_004ab0a0(Gadget_00444cb0* gadget);
void __stdcall ReportGameEvent(int msg);
void __cdecl FUN_004d85a0(void* p);
void BroadcastPlayerInfo(void);
void UpdateNetGameInfo(void);

// FUNCTION: 0x444cb0
void __stdcall FUN_00444cb0(Gadget_00444cb0* param_1)
{
    void* entries = param_1->holder->entries;
    Layout_00444cb0* layout = param_1->holder->layout;

    if (param_1->field_60 == -1) {
        Entry_00444cb0* entry = FUN_004a0280(g_game->holder->entries, "MAPPIC");
        if (entry->text != 0) {
            FUN_004d85a0(entry->text);
            entry->text = 0;
        }
        FUN_004d85a0(layout->field_14);
        FUN_004d85a0(layout);
        FUN_004d85a0(DAT_00512990);
        DAT_00512990 = 0;
        return;
    }

    if (IsCurrentGadgetNamed(param_1, "MAPNAMES") || IsCurrentGadgetNamed(param_1, "LOAD")) {
        FUN_0047f1a0("Multi", 0);
        Entry_00444cb0* g = FindGadgetChecked(entries, "MAPNAMES");
        ((Class_00435a20*)g_game->field_391e9)->FUN_00435a20(
            FUN_004b6af0(g->text, g->selected));

        Player_00444cb0* player = &g_game->players[g_game->localPlayer];
        strcpy(player->data->name,
               ((Class_00435c30*)g_game->field_391e9)->FUN_00435c30());
        player->data->field_a9 =
            ((Class_004373a0*)g_game->field_391e9)->FUN_004373a0();

        BroadcastPlayerInfo();
        ReportGameEvent(5);
        UpdateNetGameInfo();

        for (int i = 0; i < 10; i++) {
            if (g_game->players[i].active == 0 ||
                (g_game->players[i].state != 1 && g_game->players[i].state != 2)) {
                g_game->players[i].data->flags &= 0xffdf;
            }
        }
        return;
    }

    if (IsCurrentGadgetNamed(param_1, "PREVMENU")) {
        FUN_0047f1a0("Previous", 0);
        ((Class_00435a20*)g_game->field_391e9)->FUN_00435a20(DAT_00512990);
        BroadcastPlayerInfo();
        return;
    }

    FUN_004ab0a0(param_1);
}
