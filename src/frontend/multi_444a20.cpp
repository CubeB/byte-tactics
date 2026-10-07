// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_00444a20 {
    char unknown_0[0x17];
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    char unknown_1b[0xc2 - 0x1b];
    void* field_c2;                    // +0xc2
    char unknown_c6[0x15b - 0xc6];
};

struct Holder_00444a20 {
    int unknown_0;
    Entry_00444a20* entries;           // +0x4
};

struct Sub_00444a20 {
    char unknown_0[0x18];
    Holder_00444a20* holder;           // +0x18
};

struct Game {
    char unknown_0[0x519];
    Sub_00444a20 sub;                  // +0x519
    char unknown_535[0x391e9 - 0x535];
    void* field_391e9;                 // +0x391e9
};
#pragma pack(pop)

class Mission {
public:
    int GetNameSlot(int param_1);
    int GetTranslatedName();
    int GetDescription();
};

extern Game* g_game;

int __stdcall FindGadgetIndex(Entry_00444a20* entries, char* name, int type);
void __stdcall FUN_004a0bf0(Sub_00444a20* obj, char* name, int param_3, int param_4);
Entry_00444a20* __stdcall FUN_004a0280(Entry_00444a20* entries, char* name);
void __cdecl FUN_004d85a0(void* param_1);
void* __stdcall FUN_004295b0(char* path, int* outX, int* outY);
void __stdcall ResizeRadarPicture(void* bmp, int param_2, int param_3, int param_4, int param_5);
void __stdcall FUN_0049fa90(Sub_00444a20* obj);
char* __stdcall Translate(char* text);

// FUNCTION: 0x444a20
void ShowSelectedMapInfo()
{
    int outX;
    int outY;
    char buffer[100];

    if (FindGadgetIndex(g_game->sub.holder->entries, "MAPNAME", 5) != -1) {
        FUN_004a0bf0(&g_game->sub, "MAPNAME",
                     ((Mission*)g_game->field_391e9)->GetTranslatedName(), 0);
    }

    sprintf(buffer, "%s  %s: %s",
            (char*)g_game->field_391e9 + 0xdc4,
            Translate("Players"),
            (char*)g_game->field_391e9 + 0xe44);
    FUN_004a0bf0(&g_game->sub, "SIZE", (int)buffer, 0);

    Entry_00444a20* entry = FUN_004a0280(g_game->sub.holder->entries, "MAPPIC");
    if (entry->field_c2 != 0) {
        FUN_004d85a0(entry->field_c2);
        entry->field_c2 = 0;
    }
    void* bmp = FUN_004295b0(
        (char*)((Mission*)g_game->field_391e9)->GetNameSlot(1), &outX, &outY);
    entry->field_c2 = bmp;
    if (bmp != 0) {
        ResizeRadarPicture(bmp, entry->field_17, entry->field_19, outX << 4, outY << 4);
    }

    FUN_004a0bf0(&g_game->sub, "DESCRIPTION",
                 ((Mission*)g_game->field_391e9)->GetDescription(), 0);
    FUN_0049fa90(&g_game->sub);
}
