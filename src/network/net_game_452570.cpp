// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// g_game holds ten 0x14b-byte player/unit entries at +0x1b67. Each entry has
// an int id at +0x00, a Unit* at +0x23 and a byte "in use" flag at +0x6f. The
// function returns true when no in-use entry other than the given id maps its
// Unit's +0x96 byte to the given slot.
#include <string.h>

#pragma pack(push, 1)

struct Unit_00452570 {
    char unknown_0[0x96];
    unsigned char field_96;                // +0x96
};

struct Entry_00452570 {
    int id;                                // +0x00 (g_game + 0x1b67)
    char unknown_4[0x1f];                  // +0x04
    Unit_00452570* unit;                   // +0x23 (g_game + 0x1b8a)
    char unknown_27[0x48];                 // +0x27
    unsigned char valid;                   // +0x6f (g_game + 0x1bd6)
    char unknown_70[0x14b - 0x70];         // +0x70
};

struct Game {
    char unknown_0[0x1b67];
    Entry_00452570 entries[10];            // +0x1b67
};

#pragma pack(pop)

extern Game* g_game;

// Returns the index of the first in-use entry whose id matches, else 10.
static __inline unsigned char FindSlot_00452570(int id)
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        int v;
        // Redundant guard: kept as in the original.
        if (i == 10) {
            v = -1;
        } else {
            v = g_game->entries[i].valid ? g_game->entries[i].id : -1;
        }
        if (v == id) {
            return i;
        }
    }
    return 10;
}

// FUNCTION: 0x452570
int __stdcall IsColorFree(int id, int slot)
{
    if (slot == 0xff || slot < 0 || slot >= 10) {
        return 0;
    }

    unsigned char local_c[10];
    memset(local_c, -1, 10);

    unsigned char found;
    if (id == -1) {
        found = 10;
    } else {
        found = FindSlot_00452570(id);
    }
    // Second search, result discarded: kept as in the original.
    if (found != 10 && id != -1) {
        FindSlot_00452570(id);
    }
    for (int j = 0; j < 10; j++) {
        if (g_game->entries[j].valid != 0 && g_game->entries[j].id != id &&
            g_game->entries[j].unit->field_96 != 0xff) {
            local_c[g_game->entries[j].unit->field_96] = (unsigned char)j;
        }
    }
    return local_c[slot] == 0xff;
}
