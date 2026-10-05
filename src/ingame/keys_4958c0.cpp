// Decompiled by Opus. Names are provisional.

struct Struct_004958c0 {
    int unknown_0;
    int value;                       // +0x4
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    char unknown_519[0x18];          // +0x519
    Struct_004958c0* unknown_531;    // +0x531
    char unknown_535[0x2cc3 - 0x535];
    unsigned char unknown_2cc3;      // +0x2cc3
    char unknown_2cc4[2];
    unsigned char flags_2cc6;        // +0x2cc6
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_0049fe60(int value, char* name);
void __stdcall FUN_004a6a40(void* obj, int index);

// The else branch is the body of FUN_00495860.
// FUNCTION: 0x4958c0
void __stdcall FUN_004958c0(int set)
{
    int index;

    if (set) {
        g_game->flags_2cc6 |= 0x20;
        return;
    }
    g_game->unknown_2cc3 = 1;
    g_game->flags_2cc6 &= 0xdf;
    index = FUN_0049fe60(g_game->unknown_531->value, "STOP");
    if (index != -1) {
        FUN_004a6a40(g_game->unknown_519, index);
    }
}
