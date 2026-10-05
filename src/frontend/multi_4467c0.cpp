// Decompiled by Opus. Names are provisional.
// Returns whether two players are allied: alliance 5 means "no alliance".

#pragma pack(push, 1)
struct Player_004467c0 {
    char unknown_0[0x13f];
    unsigned char alliance;            // +0x13f
};
#pragma pack(pop)

// FUNCTION: 0x4467c0
bool __stdcall ArePlayersAllied(Player_004467c0* a, Player_004467c0* b)
{
    if (a->alliance == 5)
        return false;
    return a->alliance == b->alliance;
}
