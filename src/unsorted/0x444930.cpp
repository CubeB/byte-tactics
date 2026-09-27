// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Handler for the multiplayer side-selection dialog. When the dialog closes
// (current gadget -1) it frees the layout data; when the player picks a side
// (LOGOS/SELECT) it copies that side's byte into the local player's info.

#pragma pack(push, 1)
struct Entry_00444930 {
    char unknown_0[2];
    char name[0x10];                   // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6
    char unknown_b8[0xba - 0xb8];
    unsigned char field_ba;            // +0xba
};

struct Layout_00444930 {
    char unknown_0[0x18];
    void* field_18;                    // +0x18
    void* field_1c;                    // +0x1c
};

struct Holder_00444930 {
    char unknown_0[4];
    Entry_00444930* entries;           // +0x04
    char unknown_8[0xc - 8];
    Layout_00444930* layout;           // +0x0c
};

struct PlayerInfo_00444930 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
};

struct Player_00444930 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x04];
    PlayerInfo_00444930* info;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game_00444930 {
    char unknown_0[0x1b63];
    Player_00444930 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bee - 0x2a43];
    unsigned short flag0 : 1;          // +0x2bee, bit 0
    unsigned short bits1 : 15;
};

struct Gadget_00444930 {
    char unknown_0[0x18];
    Holder_00444930* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game_00444930* g_game;

extern "C" int __stdcall FUN_0049fd60(Gadget_00444930* gadget, char* name);
extern "C" Entry_00444930* __stdcall FUN_0049ff90(Entry_00444930* entries, char* name);
extern "C" void __stdcall FUN_0047f1a0(char* str, int flag);
extern "C" void __stdcall FUN_004ab0a0(Gadget_00444930* gadget);
extern "C" void __stdcall FUN_004526c0(int value);
void FUN_004d85a0(void* p);

// FUNCTION: 0x444930
void __stdcall FUN_00444930(Gadget_00444930* param_1)
{
    Entry_00444930* entries = param_1->holder->entries;
    Layout_00444930* layout = param_1->holder->layout;

    if (param_1->field_60 == -1) {
        FUN_004d85a0(layout->field_1c);
        FUN_004d85a0(layout->field_18);
        FUN_004d85a0(layout);
        FUN_0047f1a0("Multi", 0);
        return;
    }
    if (FUN_0049fd60(param_1, "LOGOS") || FUN_0049fd60(param_1, "SELECT")) {
        Player_00444930* player = &g_game->players[g_game->localPlayer];
        Entry_00444930* entry = FUN_0049ff90(entries, "LOGOS");
        player->info->field_96 = ((char*)layout)[entry->field_ba];
        g_game->flag0 = 1;
        FUN_004526c0(player->info->field_96);
        return;
    }
    if (!FUN_0049fd60(param_1, "Cancel"))
        FUN_004ab0a0(param_1);
}
