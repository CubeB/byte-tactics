// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Saves the current game settings: copies the 0x53-byte block at
// g_game+0x37ee6 into the settings block at DAT_00512f18, saves two game
// flags into bits 0 and 1 of that block's trailing word, then saves the
// track number, the option byte, the current track index and the 100
// track-name characters read from the object at g_game+0x10.
#include <string.h>

class Class_004ce5a0 {
public:
    int FUN_004ce5a0();
};

class Class_004ce7e0 {
public:
    unsigned char GetCategoryOfTrack(int index);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    void* field_10;                    // +0x10
    char unknown_14[0x14281 - 0x14];
    unsigned short bit0 : 1;           // +0x14281, bit 0
    unsigned short bit1 : 1;           // +0x14281, bit 1 (mask 2)
    unsigned short bit2 : 1;           // +0x14281, bit 2 (mask 4)
    unsigned short rest : 13;
    char unknown_14283[0x1434d - 0x14283];
    unsigned char field_1434d;         // +0x1434d
    char unknown_1434e[0x37ee6 - 0x1434e];
    char field_37ee6[0x53];            // +0x37ee6
    char unknown_37f39[0x38a4b - 0x37f39];
    unsigned short field_38a4b;        // +0x38a4b
};

// The saved settings block: 0x53 bytes of state, then bits 0 and 1 of the
// word at +0x53 (0x512f6b).
struct Settings_45cde0 {
    char block[0x53];                  // +0x0
    unsigned short bit0 : 1;           // +0x53, bit 0
    unsigned short bit1 : 1;           // +0x53, bit 1
    unsigned short rest : 14;
};
#pragma pack(pop)

extern Game* g_game;
// Flags live in this struct, not a standalone global: keeps the load order.
extern Settings_45cde0 DAT_00512f18;
extern int DAT_00512f6d;
extern int DAT_00512f71;
extern int DAT_00512fd9;
extern char DAT_00512f75[];

// FUNCTION: 0x45cde0
void SaveGameSettings()
{
    memcpy(DAT_00512f18.block, (char*)g_game + 0x37ee6, 0x53);
    DAT_00512f18.bit0 = g_game->bit1;
    DAT_00512f18.bit1 = g_game->bit2;
    DAT_00512f6d = g_game->field_38a4b;
    DAT_00512f71 = g_game->field_1434d;
    DAT_00512fd9 = ((Class_004ce5a0*)g_game->field_10)->FUN_004ce5a0();
    for (int i = 0; i < 100; i++) {
        DAT_00512f75[i] = ((Class_004ce7e0*)g_game->field_10)->GetCategoryOfTrack(i);
    }
}
