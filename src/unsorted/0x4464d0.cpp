// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Handler for the CONTROL.GUI dialog: choosing a "LIVEPLYR%d" entry opens the
// reject dialog for that player, WATCHING toggles the local player's watching
// flag and republishes the GUI values, OK kicks every playing player in state
// 3 without watch permission, and any other gadget clears the current one.
//
// STILL DIFFERS (91.0%): every instruction matches except that MSVC gives the
// parameter `gui` edi and the local `info` ebp, while the original has gui in
// ebp and info in edi (7 places: the prologue load/order, the three
// `push gui`, and the two `info` bit operations). This is a register-priority
// tie in MSVC 5's allocator: adding one more loop-weighted use of `gui` flips
// the pair (a scratch with `i < 10 && gui != 0` in the LIVEPLYR loop puts gui
// in ebp), but every spelling tried that keeps the bytes identical (loop
// condition, while/goto/switch wrappers, inline getters, casts, extra locals,
// declaration-order swaps, all headers.py sets) leaves it swapped. See
// build/scratch/0x4464d0/ for the variants.
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

struct Game_004464d0 {
    char unknown_0[0x519];
    char gui[0x1b63 - 0x519];          // +0x519
    Player_004464d0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game_004464d0* g_game;

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
            FUN_004ab0a0(gui);
            return;
        }
        if (FUN_0049fd60(gui, "OK")) {
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
        } else {
            FUN_004ab0a0(gui);
        }
    }
}
