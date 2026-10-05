// Decompiled by Sonnet. Names are provisional.

int __stdcall GetGroundHeight(void* param_1);

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
    char sub_4[0x40 - 4]; // +0x4, embedded struct passed to GetGroundHeight
    int field_40;         // +0x40

    int IsExpired(int param_1);
};

// FUNCTION: 0x474720
int Class_00474720::IsExpired(int param_1)
{
    if (param_1 <= field_40) {
        int r = GetGroundHeight(sub_4);
        if (r < g_game->byte_1427f)
            return 0;
    }
    return 1;
}
