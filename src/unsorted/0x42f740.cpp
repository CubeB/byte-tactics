// Decompiled by Opus. Names are provisional.

void __cdecl FUN_004d85a0(int* param_1);

struct Slot_0042f740 {
    int count;                         // +0x00
    int* a;                            // +0x04
    int* b;                            // +0x08
};

struct Entry_0042f740 {
    char unknown_0[0x40];
    Slot_0042f740 slots[0x18];         // +0x40
};

#pragma pack(push, 1)
struct Game_0042f740 {
    char unknown_0[0x37e13];
    Entry_0042f740* entries;           // +0x37e13
    int entry_count;                   // +0x37e17
};
#pragma pack(pop)

extern Game_0042f740* g_game;

// FUNCTION: 0x42f740
void FUN_0042f740()
{
    if (g_game->entry_count > 0) {
        for (int i = 0; i < g_game->entry_count; i++) {
            Entry_0042f740* e = &g_game->entries[i];
            for (int j = 0; j < 0x18; j++) {
                if (e->slots[j].count > 0) {
                    FUN_004d85a0(e->slots[j].a);
                    FUN_004d85a0(e->slots[j].b);
                }
            }
        }
        FUN_004d85a0((int*)g_game->entries);
    }
    g_game->entry_count = 0;
    g_game->entries = 0;
}
