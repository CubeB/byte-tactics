// Decompiled by Opus. Names are provisional.
// Deletes the object at g_game+0x14207 and clears the pointer.

class Pathfinder {
public:
    char unknown_0[0x30];

    ~Pathfinder();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14207];
    Pathfinder* field_14207;           // +0x14207
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x44f6e0
void FUN_0044f6e0()
{
    delete g_game->field_14207;
    g_game->field_14207 = 0;
}
