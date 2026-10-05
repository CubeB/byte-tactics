// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Looks up the "PANEL" gadget in the menu's entry table (entry 0 holds the
// count as a short at +0xb6). When the game flag at +0x37ebe is set the panel
// layout grows by 0x96, and if there is no PANEL entry yet a cleared one is
// appended: type 0xb, x = 0x80, its width shrunk by x, height copied from the
// table, named "PANEL", and the table's +0xc4 field copied into it.
// The append reads the count into a short, sign-extends it once for the
// element address and once more for the stored count; indexing `entries[i]`
// at every field (rather than a local pointer) is what reproduces the
// original's scheduling of the string setup.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0045ce80 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1;
    char name[0x11];                   // +0x02
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    char unknown_1b[0x29 - 0x1b];
    unsigned char field_29;            // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                       // +0xb6 (entry 0 only)
    char unknown_b8[0xc4 - 0xb8];
    int field_c4;                      // +0xc4
    char unknown_c8[0x15b - 0xc8];
};

struct Holder_0045ce80 {
    char unknown_0[4];
    Entry_0045ce80* entries;           // +0x04
};

struct Game {
    char unknown_0[0x531];
    Holder_0045ce80* holder;           // +0x531
    char unknown_535[0x37ebe - 0x535];
    unsigned char flags;               // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_0049fdf0(Entry_0045ce80* entries, char* name, int type);

// FUNCTION: 0x45ce80
void FUN_0045ce80()
{
    Entry_0045ce80* entries = g_game->holder->entries;
    int index = FUN_0049fdf0(entries, "PANEL", 0xe);
    if (g_game->flags & 1) {
        entries->width += 0x96;
        if (index == -1) {
            short c = entries->count;
            int i = c;
            i++;
            c++;
            entries->count = c;
            memset(&entries[i], 0, sizeof(Entry_0045ce80));
            entries[i].type = 0xb;
            entries[i].x = 0x80;
            entries[i].width = entries->width;
            entries[i].y = 0;
            entries[i].width -= entries[i].x;
            entries[i].height = entries->height;
            strcpy(entries[i].name, "PANEL");
            entries[i].field_29 = 1;
            entries[i].field_c4 = entries->field_c4;
        }
    }
}
