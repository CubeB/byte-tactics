// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Frees the game's entry table at +0x147af (each entry's pointer at +0x40),
// then the table itself, clears the count and the pointer, zeroes a second
// count-based array at +0x14907 and six scalars below it.

#pragma pack(push, 1)
struct Entry_0042a010 {
    char unknown_0[0x40];
    int* field_40;                     // +0x40
};

struct Game {
    char unknown_0[0x147ab];
    int count;                         // +0x147ab
    Entry_0042a010* entries;           // +0x147af
    char unknown_1[0x148ef - 0x147b3]; // +0x147b3
    int field_148ef;                   // +0x148ef
    int field_148f3;                   // +0x148f3
    int field_148f7;                   // +0x148f7
    int field_148fb;                   // +0x148fb
    int field_148ff;                   // +0x148ff
    int field_14903;                   // +0x14903
    int array_14907[1];                // +0x14907
    char unknown_2[0x37f39 - 0x1490b]; // +0x1490b
    int count2;                        // +0x37f39
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x42a010
void FreeAnimFiles()
{
    int i;
    for (i = 0; i < g_game->count; i++) {
        FUN_004d85a0(g_game->entries[i].field_40);
    }
    FUN_004d85a0((int*)g_game->entries);
    g_game->count = 0;
    g_game->entries = 0;
    for (i = 0; i < g_game->count2; i++) {
        g_game->array_14907[i] = 0;
    }
    g_game->field_148ef = g_game->field_148f3 = g_game->field_148f7 =
        g_game->field_148fb = g_game->field_148ff = g_game->field_14903 = 0;
}
