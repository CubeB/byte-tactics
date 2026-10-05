// Decompiled by Opus. Names are provisional.
// Creates the object at g_game+0x14207 (deleted again by 0x44f6e0).

class Class_0040e9e0 {
public:
    char unknown_0[0xc9];

    Class_0040e9e0();
};

#pragma pack(push, 1)
struct Game_0044f6a0 {
    char unknown_0[0x14207];
    Class_0040e9e0* field_14207;       // +0x14207
};
#pragma pack(pop)

extern Game_0044f6a0* g_game;

// FUNCTION: 0x44f6a0
void FUN_0044f6a0()
{
    g_game->field_14207 = new Class_0040e9e0;
}
