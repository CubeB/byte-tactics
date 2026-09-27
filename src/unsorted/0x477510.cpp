// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Menu_00477510;

struct Info_00477510 {
    char unknown_0[0x30];
    char name[8];                      // +0x30
};

struct Object_00477510 {
    char unknown_0[0x18];
    Info_00477510* info;               // +0x18
};

#pragma pack(push, 1)
struct Game_00477510 {
    char unknown_0[0x519];
    char menu[0x38d7f - 0x519];        // +0x519
    unsigned short flags_38d7f;        // +0x38d7f
};
#pragma pack(pop)

extern Game_00477510* g_game;

void __stdcall FUN_004a0570(Menu_00477510* menu, char* name, int value);
void __stdcall FUN_0049fa90(Menu_00477510* menu);
void FUN_00432b60();

// FUNCTION: 0x477510
void __stdcall FUN_00477510(Object_00477510* obj)
{
    if (strncmp(obj->info->name, "DRDEATH", 7) == 0) {
        if (!(g_game->flags_38d7f & 1)) {
            FUN_004a0570((Menu_00477510*)g_game->menu, "AnyMsn", 1);
            g_game->flags_38d7f |= 1;
        } else {
            FUN_004a0570((Menu_00477510*)g_game->menu, "AnyMsn", 0);
            g_game->flags_38d7f &= ~1;
        }
        FUN_00432b60();
        FUN_0049fa90((Menu_00477510*)g_game->menu);
    }
}
