// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// <windows.h> is only here to fix the base/index operand order of the
// DAT_005129b4[i] addressing (tools/headers.py).
#include <windows.h>

#pragma pack(push, 1)
struct Record_005129b4 {
    char unknown_0[0x52];
    int field_52;                      // +0x52
    char unknown_56[0x5a - 0x56];
    int field_5a;                      // +0x5a
    int field_5e;                      // +0x5e
    char unknown_62[0x62 - 0x62];
};

struct Entry_44c220 {
    char unknown_0[0xc0];
    short count;                       // +0xc0
    char unknown_c2[0xd6 - 0xc2];
    unsigned char* bits;               // +0xd6
};

struct Holder_44c220 {
    char unknown_0[4];
    Entry_44c220* entries;             // +0x04
};

struct Item_44c220 {                   // 0x249 bytes
    char unknown_0[0x13e];
    int field_13e;                     // +0x13e
    char unknown_142[0x249 - 0x142];
};

struct Event_44c220 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    short field_8;                     // +0x08
    short field_a;                     // +0x0a
    int field_c;                       // +0x0c
};

struct Class_0046e280 {
    int FUN_0046e280(Event_44c220* event);
};

struct Game {
    char unknown_0[0x531];
    Holder_44c220* holder;             // +0x531
    char unknown_535[0x2a30 - 0x535];
    Class_0046e280* queue;             // +0x2a30
    char unknown_2a34[0x1439b - 0x2a34];
    Item_44c220* items;                // +0x1439b
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;
// GLOBAL: 0x5129b4
extern Record_005129b4* DAT_005129b4;
// GLOBAL: 0x5129c8
extern int DAT_005129c8;

Entry_44c220* __stdcall FUN_0049ff90(void* entries, const char* name);
int FUN_004b6340();
void FUN_0044c0d0();
void __stdcall FUN_0044bfd0(void* menu, int value);
void __stdcall FUN_0049fa90(void* menu);

// FUNCTION: 0x44c220
void FUN_0044c220()
{
    Event_44c220 event;
    int n = 0;
    Entry_44c220* entry = (Entry_44c220*)FUN_0049ff90(g_game->holder->entries, "PICLIST");

    if (DAT_005129c8 < FUN_004b6340()) {
        DAT_005129c8 = FUN_004b6340() + 2;
        FUN_0044c0d0();
    }

    while (g_game->queue->FUN_0046e280(&event) != 0) {
        n++;
        for (int i = 0; i < entry->count; i++) {
            if (event.field_0 == g_game->items[DAT_005129b4[i].field_52].field_13e) {
                entry->bits[i] = (event.field_a == 0);
                DAT_005129b4[i].field_5e = event.field_a;
                DAT_005129b4[i].field_5a = event.field_c;
                entry->bits[i] |= (event.field_c != 0) ? 0 : 2;
            }
        }
    }

    if (n != 0) {
        FUN_0044bfd0((char*)g_game + 0x519, 0);
        FUN_0049fa90((char*)g_game + 0x519);
    }
}
