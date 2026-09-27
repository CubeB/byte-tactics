// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Dumps the per-unit-type data to the file named by the argument.
#include <stdio.h>

#pragma pack(push, 1)

struct UnitType_0044b230 {
    char unknown_0[0x13e];
    int field_13e;                     // +0x13e
    char unknown_142[0x249 - 0x142];
};

struct Game_0044b230 {
    char unknown_0[0x1438f];
    int field_1438f;                   // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_0044b230* field_1439b;    // +0x1439b
};

#pragma pack(pop)

extern Game_0044b230* g_game;
extern char* DAT_005129b4;

// FUNCTION: 0x44b230
void __stdcall FUN_0044b230(char* filename)
{
    FILE* f = fopen(filename, "wb+");

    int count = g_game->field_1438f - 1;
    fwrite(&count, 4, 1, f);
    count++;

    for (int i = 1; i < count; i++) {
        for (int j = 0; j < g_game->field_1438f; j++) {
            if (*(int*)(DAT_005129b4 + 0x52 + j * 0x62) == i) {
                int v = g_game->field_1439b[i].field_13e;
                fwrite(&v, 4, 1, f);
                v = *(int*)(DAT_005129b4 + 0x5a + j * 0x62);
                fwrite(&v, 4, 1, f);
                break;
            }
        }
    }

    fclose(f);
}
