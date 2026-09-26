// Pilot functions used to validate the toolchain. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct PlayerStruct {
    char unknown[0x14B];
};

struct GameState {
    char unknown[0x1B63];
    PlayerStruct players[10];
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern GameState* g_game;

class PlayerRef {
public:
    int unknown[12];
    PlayerStruct* player;

    void Reset(unsigned char playerIndex);
};

// FUNCTION: 0x401070
void PlayerRef::Reset(unsigned char playerIndex)
{
    memset(this, 0, sizeof(*this));
    player = &g_game->players[playerIndex];
}
