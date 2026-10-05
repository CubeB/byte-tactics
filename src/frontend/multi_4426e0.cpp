// Decompiled by Space Bunny Free. Names are provisional.
#include <string.h>

// 0x102-byte records: a name and a number string.
struct Entry_004426e0 {
    char name[0x81];                 // +0x00
    char number[0x81];               // +0x81
};

// GUI layout entry, as returned by FUN_0049ff90.
struct Layout_004426e0 {
    char unknown_0[0xba];
    short player;                    // +0xba
};

// Object with the layout entry array at +4.
struct Table_004426e0 {
    int unknown_0;
    Layout_004426e0* entries;        // +0x4
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    void* menu;                      // +0x519
    char unknown_51d[0x531 - 0x51d];
    Table_004426e0* table;           // +0x531
};
#pragma pack(pop)

extern Entry_004426e0* DAT_00512988;
extern char* DAT_0051298c;
extern Game* g_game;

Layout_004426e0* __stdcall FUN_0049ff90(Layout_004426e0* entries, char* name);
void __stdcall FUN_004a32a0(void* menu, char* name, char* text,
                            int count, int flag);
void __stdcall FUN_004a2e40(void* menu, char* name, int player);

// FUNCTION: 0x4426e0
void FUN_004426e0(void)
{
    char* buffer = DAT_0051298c;
    *buffer = 0;
    for (int i = 0; i < 20; i++) {
        strcpy(buffer, DAT_00512988[i].name);
        buffer += strlen(DAT_00512988[i].name) + 1;
    }
    Layout_004426e0* entry = FUN_0049ff90(g_game->table->entries, "ACCOUNTS");
    int player = entry->player;
    FUN_004a32a0((char*)g_game + 0x519, "ACCOUNTS", DAT_0051298c, 20, 0);
    FUN_004a2e40((char*)g_game + 0x519, "ACCOUNTS", player);
}
