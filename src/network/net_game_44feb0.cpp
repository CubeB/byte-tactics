// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Player_0044feb0 {
    char unknown_0[0x146];
    unsigned char field_146;           // +0x146
};
#pragma pack(pop)

// FUNCTION: 0x44feb0
unsigned char __stdcall FUN_0044feb0(Player_0044feb0* player)
{
    if (player == 0) {
        return 10;
    }
    return player->field_146;
}
