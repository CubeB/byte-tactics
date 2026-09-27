// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_0044b140 {
    char unknown_0[0x13e];
    int field_13e;                     // +0x13e
    char unknown_142[0x249 - 0x142];
};

struct Sub_0044b140 {
    char unknown_0[0x52];
    int id;                            // +0x52
    int field_56;                      // +0x56
    int value;                         // +0x5a
    char unknown_5e[0x62 - 0x5e];
};

struct Game_0044b140 {
    char unknown_0[0x1438f];
    int field_1438f;                   // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Entry_0044b140* field_1439b;       // +0x1439b
};
#pragma pack(pop)

extern Game_0044b140* g_game;
extern Sub_0044b140* DAT_005129b4;

// Reads pairs of ints from a binary file given by `name`. For each pair the
// first int is matched against the field at +0x13e of the 0x249-byte entries
// at g_game+0x1439b (entries are 1-based here); the second int is then stored
// in the field at +0x5a of the 0x62-byte entry of the table at DAT_005129b4
// whose +0x52 field equals the matched index.
// FUNCTION: 0x44b140
void __stdcall FUN_0044b140(char* name)
{
    FILE* f = fopen(name, "rb");
    int count;
    fread(&count, 4, 1, f);
    for (int i = 0; i < count; i++) {
        int a, b;
        fread(&a, 4, 1, f);
        fread(&b, 4, 1, f);
        int n = g_game->field_1438f;
        for (int idx = 1; idx < n; idx++) {
            if (g_game->field_1439b[idx].field_13e == a) {
                for (int j = 0; j < n; j++) {
                    if (DAT_005129b4[j].id == idx) {
                        DAT_005129b4[j].value = b;
                        break;
                    }
                }
                break;
            }
        }
    }
    fclose(f);
}
