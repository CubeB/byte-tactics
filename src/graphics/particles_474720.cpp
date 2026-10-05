// Decompiled by Sonnet. Names are provisional.

int __stdcall FUN_00485070(void* param_1);

struct Game
{
    char unknown_0[0x1427f];
    unsigned char byte_1427f; // +0x1427f
};

extern Game* g_game;

class Class_00474720
{
public:
    char unknown_0[4];
    char sub_4[0x40 - 4]; // +0x4, embedded struct passed to FUN_00485070
    int field_40;         // +0x40

    int FUN_00474720(int param_1);
};

// FUNCTION: 0x474720
int Class_00474720::FUN_00474720(int param_1)
{
    if (param_1 <= field_40) {
        int r = FUN_00485070(sub_4);
        if (r < g_game->byte_1427f)
            return 0;
    }
    return 1;
}
