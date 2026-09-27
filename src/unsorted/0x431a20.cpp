// Decompiled by Opus. Names are provisional.
// Frees the buffer of each of the five entries at g_game+0x3816b and clears
// the pointers.

void FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
struct Entry_00431a20 {
    int* buffer;                       // +0x0
    char unknown_4[0x22e];
};

struct Game_00431a20 {
    char unknown_0[0x3816b];
    Entry_00431a20 entries[5];         // +0x3816b
};
#pragma pack(pop)

extern Game_00431a20* g_game;

// FUNCTION: 0x431a20
void FUN_00431a20()
{
    for (int i = 0; i < 5; i++) {
        int*& buffer = g_game->entries[i].buffer;
        if (buffer) {
            FUN_004d85a0(buffer);
            buffer = 0;
        }
    }
}
