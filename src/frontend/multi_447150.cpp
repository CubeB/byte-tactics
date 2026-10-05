// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>

#pragma pack(push, 1)
struct Info_00447150 {
    char unknown_0[0x9d];
    unsigned short flags_9d;           // +0x9d
};

struct Player_00447150 {
    int active;                        // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x27 - 0x8];
    Info_00447150* info;               // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allies[10];          // +0x108
    char unknown_112[0x13f - 0x112];
    unsigned char alliance;            // +0x13f
    char unknown_140[0x146 - 0x140];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Layer_00447150 {
    int unknown_0;
    void* entries;                     // +0x4
};

struct Gadget_00447150 {
    char unknown_0[0x18];
    Layer_00447150* layer;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Message_00447150 {
    char unknown_0[0x1b63 - 0x519];
};

struct Game {
    char unknown_0[0x519];
    Message_00447150 message;          // +0x519
    Player_00447150 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37ebe - 0x2a43];
    unsigned short field_37ebe;        // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall IsCurrentGadgetNamed(void* gadget, char* name);
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall SetAlliance(int, int, unsigned char, int);
char* __stdcall FUN_004c5740(char* text);
void __stdcall SendChatMessage(void* from, char* text, int param_3, void* to);
void FUN_00446fb0();
void __stdcall DrawButton(void* sub, int param_2);
int __stdcall FindGadgetIndex(void* entries, const char* name, int flag);
int __stdcall GetButtonStage(void* gadget, int index);
void __stdcall FUN_004ab0a0(void* gadget);
void BroadcastPlayerInfo();

// FUNCTION: 0x447150
void __stdcall FUN_00447150(Gadget_00447150* gadget)
{
    void* entries = gadget->layer->entries;
    char buf[100];

    if (gadget->field_60 == -1) {
        g_game->field_37ebe &= 0xffdf;
        return;
    }

    int i = 0;
    Player_00447150* local = &g_game->players[g_game->localPlayer];

    for (; i < 10; i++) {
        sprintf(buf, "LIVEALLY%d", i);
        Player_00447150* p = &g_game->players[i];
        if (IsCurrentGadgetNamed(gadget, buf) && p->active
            && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            PlaySoundByName("Options", 0);
            SetAlliance(local->field_4, p->field_4, local->allies[i] ^= 1, 0);
            char* verb = local->allies[i] ? "allied with" : "broke alliance with";
            sprintf(buf, " %s %s", FUN_004c5740(verb),
                    (char*)g_game + 0x1b8e + i * 0x14b);
            SendChatMessage(local, buf, 4, 0);
            FUN_00446fb0();
            DrawButton(&g_game->message, gadget->field_60);
        }
    }

    if (IsCurrentGadgetNamed(gadget, "VICTORY")) {
        PlaySoundByName("Options", 0);
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OK")) {
        PlaySoundByName("Options", 0);
        int old = (local->info->flags_9d >> 1) & 1;
        int index = FindGadgetIndex(entries, "VICTORY", 1);
        unsigned int value = GetButtonStage(gadget, index);
        local->info->flags_9d = (local->info->flags_9d & 0xfffd) | ((value & 1) << 1);
        if (old != ((local->info->flags_9d >> 1) & 1))
            BroadcastPlayerInfo();
    } else {
        FUN_004ab0a0(gadget);
    }
}
