// Decompiled by mimo-v2.6-flash. Names are provisional.

#include <string.h>

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct PlayerInfo_00494050 {
    char unknown_0[0x9b];
    unsigned short bits_9b_0 : 6;      // +0x9b, bits 0 to 5
    unsigned short flag_9b_6 : 1;      // bit 6 (mask 0x40)
    unsigned short bits_9b_7 : 9;
};

struct Player_00494050 {               // 0x14b bytes
    int unknown_0;                     // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00494050* info;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Sub_00494050 {
    char unknown_0[0x10];
};

struct Game {
    char unknown_0[0x519];
    Sub_00494050 sub;                  // +0x519
    char unknown_529[0x1b63 - 0x529];
    Player_00494050 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bee - 0x2a43];
    unsigned short flags_2bee;         // +0x2bee
    unsigned char mode_2bf0;           // +0x2bf0
    char unknown_2bf1[0x37ebe - 0x2bf1];
    unsigned short flags_37ebe;        // +0x37ebe
    char unknown_37ec0[0x391e9 - 0x37ec0];
    Class_00435100* net;               // +0x391e9
};
#pragma pack(pop)

struct Gadget_00494050 {
    char unknown_0[4];
    void* entries;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
    Game* owner;                       // +0xc
    char unknown_10[0x20 - 0x10];
    int field_20;                      // +0x20
};

extern Game* g_game;
extern int DAT_0051f2f0;
extern char DAT_0051e788[];

Gadget_00494050* __stdcall LoadGuiLayer(Sub_00494050* sub, const char* name, int flags);
void __stdcall HandleTalkDialogEvent(void* gadget);
void __stdcall FUN_004a0bf0(Sub_00494050* sub, char* name, char* param_3, int param_4);
void __stdcall SetButtonStageByName(Sub_00494050* sub, char* name, int value);
void __stdcall FUN_004a0570(Sub_00494050* sub, char* name, int value);
int __stdcall FindGadgetIndex(void* entries, char* name, int type);
void __stdcall FUN_0049fc50(Sub_00494050* sub, int index);
void __stdcall RenderLayer(Sub_00494050* sub, int value);
void __stdcall RefreshAlliesScreen(int value);
void ResetPlayerGadgets();

// FUNCTION: 0x494050
void OpenTalkDialog()
{
    PlayerInfo_00494050* info = g_game->players[g_game->localPlayer].info;
    if (info->flag_9b_6)
        return;
    if (DAT_0051f2f0 == 0) {
        DAT_0051f2f0 = 1;
        memset(DAT_0051e788, 0, 0x81);
    }
    if (g_game->flags_37ebe & 0x800)
        return;
    int multi = (g_game->flags_2bee & 0x100)
                && g_game->net->FUN_00435100() == 3;
    Gadget_00494050* d = LoadGuiLayer(&g_game->sub,
                                       multi ? "TALK2.GUI" : "TALK.GUI",
                                       multi ? 0x800 : 0x880);
    void* entries = d->entries;
    d->handler = HandleTalkDialogEvent;
    g_game->flags_37ebe |= 4;
    FUN_004a0bf0(&g_game->sub, "TALK", DAT_0051e788, 0);
    SetButtonStageByName(&g_game->sub, "SENDTO", multi);
    if (g_game->net->FUN_00435100() != 3) {
        FUN_004a0570(&g_game->sub, "SENDTO", 0);
    } else if (multi) {
        SetButtonStageByName(&g_game->sub, "SENDTYPE", g_game->mode_2bf0);
        RefreshAlliesScreen(1);
        ResetPlayerGadgets();
    }
    FUN_0049fc50(&g_game->sub, FindGadgetIndex(entries, "TALK", 3));
    d->field_20 = FindGadgetIndex(entries, "TALK", 3);
    d->owner = g_game;
    RenderLayer(&g_game->sub, 0x40 | (multi ? 0 : 0x80));
}
