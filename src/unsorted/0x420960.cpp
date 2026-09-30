// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ddraw.h>

class Class_00437a00 {
public:
    char unknown_0[4];
    int* field_4;                      // +0x4

    void FUN_00437a00();
};

struct Entry_00420960 {
    void* ptr;                         // +0x0
    int field_4;                       // +0x4
};

struct List_00420960 {
    unsigned short count;              // +0x0
    char unknown_2[0x26];
    Entry_00420960 entries[1];         // +0x28
};

#pragma pack(push, 1)
struct Game_00420960 {
    char unknown_0[0x1ab8f];
    List_00420960* lists[3];           // +0x1ab8f
    void* field_1ab9b;                 // +0x1ab9b
};
#pragma pack(pop)

extern Game_00420960* g_game;
extern Class_00437a00 DAT_00511f80;
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x420960
void FUN_00420960(void)
{
    if (g_game->field_1ab9b != 0) {
        DAT_00511f80.FUN_00437a00();
        for (int i = 0; i < 3; i++) {
            if (g_game->lists[i] != 0) {
                for (int j = 0; j < g_game->lists[i]->count; j++) {
                    FUN_004d85a0(g_game->lists[i]->entries[j].ptr);
                    g_game->lists[i]->entries[j].ptr = 0;
                }
                FUN_004d85a0(g_game->lists[i]);
                g_game->lists[i] = 0;
            }
        }
        FUN_004d85a0(g_game->field_1ab9b);
        g_game->field_1ab9b = 0;
    }
}
