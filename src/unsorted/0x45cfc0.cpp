// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Picks the GUI layout file (PREFS.GUI when the bitfield flag is set, else
// STARTOPT.GUI), loads it, sets the music gadget and the mode flag.
#include <string.h>

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Menu_0045cfc0 {
    char unknown_0[0x18];
    void* layer;                         // +0x18
};

struct Game_0045cfc0 {
    char unknown_0[0x10];
    int* field_10;                       // +0x10
    char unknown_14[0x519 - 0x14];
    Menu_0045cfc0 menu;                  // +0x519
    char unknown_535[0x2a44 - 0x535];
    // An unsigned short bitfield whose storage starts at the odd offset
    // 0x2a44: testing `prefs` alone emits mov al / shr al, 2 / test al, 1,
    // while `prefs && x` emits a direct test byte [m], 4.
    unsigned short bit0 : 1;             // +0x2a44
    unsigned short bit1 : 1;
    unsigned short prefs : 1;            // bit 2
    unsigned short bits_3_15 : 13;
    char unknown_2a46[0x37ebe - 0x2a46];
    unsigned short loaded : 1;           // +0x37ebe
    unsigned short bits_37ebe_1 : 15;
    char unknown_37ec0[0x38a51 - 0x37ec0];
    unsigned char flags_38a51;           // +0x38a51
    char unknown_38a52[0x391e9 - 0x38a52];
    Class_00435100* mode;                // +0x391e9
};
#pragma pack(pop)

extern Game_0045cfc0* g_game;

int __stdcall FUN_004aa8f0(void* obj, char* buf, int size);
void __stdcall FUN_004a1250(void* obj, char* name, int value);
void __stdcall FUN_0049fa50(void* obj);

// FUNCTION: 0x45cfc0
int __cdecl FUN_0045cfc0()
{
    char buf[256];
    if (g_game->prefs) {
        strcpy(buf, "PREFS.GUI");
        g_game->loaded |= 1;
    } else {
        strcpy(buf, "STARTOPT.GUI");
    }
    int result = FUN_004aa8f0(&g_game->menu, buf, 0x80);
    FUN_004a1250(&g_game->menu, "MUSIC", *(int*)g_game->field_10 == 0);
    FUN_0049fa50(&g_game->menu);
    if (g_game->prefs && g_game->mode->FUN_00435100() != 3) {
        g_game->flags_38a51 |= 1;
    }
    return result;
}
