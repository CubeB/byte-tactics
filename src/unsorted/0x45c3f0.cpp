// Decompiled by Space Bunny Free. Names are provisional.
#include <stdio.h>
#include <string.h>

class Class_004ce7e0 {
public:
    unsigned char FUN_004ce7e0(int param_1);
};

class Class_004ce580 {
public:
    void FUN_004ce580(int param_1);
};

struct Holder_45c3f0 {
    char unknown_0[4];
    void* entries;                    // +0x04
};

struct Menu_45c3f0 {
    char unknown_0[0x18];
    Holder_45c3f0* holder;            // +0x18
};

#pragma pack(push, 1)
struct Game_45c3f0 {
    char unknown_0[0x10];
    Class_004ce7e0* field_10;         // +0x10
    char unknown_14[0x519 - 0x14];
    Menu_45c3f0 menu;                 // +0x519
    char unknown_535[0x37f14 - 0x535];
    char f_37f14;                     // +0x37f14
    char unknown_37f15;
    char state;                       // +0x37f16
};
#pragma pack(pop)

extern Game_45c3f0* g_game;
extern int DAT_00512fe0;

int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004a1250(void* obj, char* name, int value);
int __stdcall FUN_004a1080(void* obj, char* name, int value);
void __stdcall FUN_004a0bf0(void* obj, char* name, int param_3, int param_4);
void __stdcall FUN_004a1450(void* obj, char* name, int param_3);

// FUNCTION: 0x45c3f0
void FUN_0045c3f0()
{
    char buf[12];
    Menu_45c3f0* menu = &g_game->menu;

    if (FUN_0049fdf0(menu->holder->entries, "TRACKTYPE", 1) != -1) {
        int disc = DAT_00512fe0;
        FUN_004a1250(menu, "TRACKTYPE",
                     ((g_game->f_37f14 & 1) && g_game->state == 4) ? 0 : 1);
        FUN_004a1080(menu, "TRACKTYPE", g_game->field_10->FUN_004ce7e0(disc));
        if (disc == 0)
            strcpy(buf, "NO DISC");
        else
            sprintf(buf, "%d", disc);
        FUN_004a0bf0(menu, "TRACKNUM", (int)buf, 0);
    }
    FUN_004a1450(menu, "TRACKNUM", (char)(~g_game->f_37f14) & 1);
    if (g_game->state == 3) {
        // the track number is re-read from the global here, not taken from disc
        int track = DAT_00512fe0;
        ((Class_004ce580*)g_game->field_10)->FUN_004ce580(track);
    }
}
