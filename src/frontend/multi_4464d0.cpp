// Decompiled by deepseek-v4.1-flash, finished by DeepSeek V4.1 Flash. Names are provisional.
// Handler for the CONTROL.GUI dialog: choosing a "LIVEPLYR%d" entry opens the
// reject dialog for that player, WATCHING toggles the local player's watching
// flag and republishes the GUI values, OK kicks every playing player in state
// 3 without watch permission, and any other gadget clears the current one.
//
// The WATCHING and OK tests are one if/else-if chain and the final
// FUN_004ab0a0(gui) is written once after it, not at the end of each path.
// MSVC duplicates that call into the WATCHING fall-through and the
// neither-gadget fall-through, and the lower source use count of `gui` is what
// lands it in ebp with `info` in edi (writing the call in both arms keeps gui
// in edi instead). The OK arm returns early so it skips the shared call.
#include <stdio.h>

struct Class_004a1080;

struct Gui_004464d0 {
    char unknown_0[0x60];
    int current;                       // +0x60
};

#pragma pack(push, 1)
struct PlayerInfo_004464d0 {
    char unknown_0[0x9b];
    unsigned short bits_9b_0 : 6;      // +0x9b
    unsigned short bit6 : 1;           // bit 6
    unsigned short watching : 1;       // bit 7
    unsigned short bits_9b_8 : 7;
    unsigned short closed : 1;         // bit 15
};

struct Player_004464d0 {
    int active;                        // +0x00
    int field_4;                       // +0x04
    char unknown_8[0x27 - 0x8];
    PlayerInfo_004464d0* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    char state;                        // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x519];
    char gui[0x1b63 - 0x519];          // +0x519
    Player_004464d0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_0049fd60(Gui_004464d0* gui, char* name);
void __stdcall FUN_00446080(int player);
void __stdcall FUN_0047f1a0(char* name, int param_2);
int __stdcall FUN_004a1080(Class_004a1080* obj, char* name, int value);
void __stdcall FUN_0049fa90(Class_004a1080* obj);
void FUN_00450f90(void);
void FUN_00451180(void);
void __stdcall FUN_00453010(int obj, int value);
void __stdcall FUN_004ab0a0(Gui_004464d0* gui);

// FUNCTION: 0x4464d0
void __stdcall FUN_004464d0(Gui_004464d0* gui)
{
    PlayerInfo_004464d0* info = g_game->players[g_game->localPlayer].info;
    if (gui->current != -1) {
        char buf[100];
        for (int i = 0; i < 10; i++) {
            sprintf(buf, "LIVEPLYR%d", i);
            if (FUN_0049fd60(gui, buf)) {
                FUN_00446080(i);
                return;
            }
        }
        if (FUN_0049fd60(gui, "WATCHING")) {
            info->watching = !info->watching;
            FUN_0047f1a0("Options", 0);
            info = g_game->players[g_game->localPlayer].info;
            FUN_004a1080((Class_004a1080*)g_game->gui, "WATCHING", info->watching);
            FUN_004a1080((Class_004a1080*)g_game->gui, "GAMEOPEN", !info->closed);
            FUN_0049fa90((Class_004a1080*)g_game->gui);
            FUN_00450f90();
        } else if (FUN_0049fd60(gui, "OK")) {
            FUN_00451180();
            FUN_0047f1a0("Options", 0);
            if (!info->watching) {
                for (int i = 0; i < 10; i++) {
                    if (g_game->players[i].active != 0) {
                        if (g_game->players[i].state == 3) {
                            if (g_game->players[i].info->bit6) {
                                FUN_00453010(g_game->players[i].field_4, 9);
                            }
                        }
                    }
                }
            }
            return;
        }
        FUN_004ab0a0(gui);
    }
}
