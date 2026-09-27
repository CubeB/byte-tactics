// Decompiled by Space Bunny Free. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_00442970 {
    char name[0x81];                   // +0x00
    char number[0x81];                 // +0x81
};

struct Gadget_00442970 {
    char unknown_0[0xba];
    short count;                       // +0xba
};

struct Holder_00442970 {
    char unknown_0[4];
    void* gadgets;                     // +0x04
};

struct Game_00442970 {
    char unknown_0[0x531];
    Holder_00442970* holder;           // +0x531
};
#pragma pack(pop)

extern Game_00442970* g_game;
extern Entry_00442970* DAT_00512988;

Gadget_00442970* __stdcall FUN_0049ff90(void* gadgets, char* name);
void __stdcall FUN_0042f960(void* key, void* buf, int value);

// The last used entry is moved to the front of the modem number list, which is
// then written to the registry under "MODEMNUMBERS".
// FUNCTION: 0x442970
void FUN_00442970(void)
{
    if (DAT_00512988 != 0) {
        short count = FUN_0049ff90(g_game->holder->gadgets, "ACCOUNTS")->count;
        if (count > 0) {
            Entry_00442970 temp;
            memcpy(&temp, &DAT_00512988[count], 0x102);
            for (int i = count; i > 0; i--)
                memcpy(&DAT_00512988[i], &DAT_00512988[i - 1], 0x102);
            memcpy(&DAT_00512988[0], &temp, 0x102);
        }
        FUN_0042f960("MODEMNUMBERS", DAT_00512988, 0x1428);
    }
}
