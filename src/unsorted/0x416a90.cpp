// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_00416a90 {
    char unknown_0[0x1427f];
    unsigned char field_1427f;         // +0x1427f
};
#pragma pack(pop)

extern Game_00416a90* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

// FUNCTION: 0x416a90
void __stdcall FUN_00416a90(Class_004b73e0* args)
{
    g_game->field_1427f = args->FUN_004b73e0(1, 0);
}
