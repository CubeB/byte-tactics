// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Opens the CONTROL.GUI dialog (with FUN_004464d0 as its handler) when the
// local player's info does not have bit 6 set, then sets the WATCHING and
// GAMEOPEN controls from that player's flags.

struct Class_004a1080;
struct Class_0049fa90;

int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, int value);
void __stdcall FUN_0049fa90(Class_0049fa90* obj);
void __stdcall FUN_0049fb10(Class_0049fa90* obj, int value);
void __stdcall RenderLayer(Class_0049fa90* obj, int value);
void __stdcall FUN_00447380(int value);
void __stdcall FUN_004464d0(void* gadget);

#pragma pack(push, 1)
struct PlayerInfo_004466b0 {
    char unknown_0[0x9b];
    unsigned short bits_9b_0 : 6;      // bits 0..5
    unsigned short bit6 : 1;           // bit 6
    unsigned short watching : 1;       // bit 7
    unsigned short bits_9b_8 : 7;      // bits 8..14
    unsigned short closed : 1;         // bit 15
    char unknown_9d[0x14b - 0x9d];
};

struct Player_004466b0 {
    char unknown_0[0x27];
    PlayerInfo_004466b0* info;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x519];
    char gui[0x1b63 - 0x519];          // +0x519
    Player_004466b0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};

struct Gadget_004466b0 {
    char unknown_0[0x8];
    void (__stdcall* handler)(void*);  // +0x8
    int field_c;                       // +0xc
};
#pragma pack(pop)

extern Game* g_game;

Gadget_004466b0* __stdcall LoadGuiLayer(char* sub, const char* name, int flags);

// FUNCTION: 0x4466b0
void FUN_004466b0()
{
    PlayerInfo_004466b0* info = g_game->players[g_game->localPlayer].info;
    if (info->bit6) {
        return;
    }
    Gadget_004466b0* gadget = LoadGuiLayer(g_game->gui, "CONTROL.GUI", 0x800);
    gadget->handler = FUN_004464d0;
    gadget->field_c = (int)g_game;
    FUN_00447380(1);
    info = g_game->players[g_game->localPlayer].info;
    SetButtonStageByName((Class_004a1080*)g_game->gui, "WATCHING", info->watching);
    SetButtonStageByName((Class_004a1080*)g_game->gui, "GAMEOPEN", !info->closed);
    FUN_0049fa90((Class_0049fa90*)g_game->gui);
    FUN_0049fb10((Class_0049fa90*)g_game->gui, 1);
    RenderLayer((Class_0049fa90*)g_game->gui, 0x40);
}
