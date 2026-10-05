// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x39211];
    int field_39211;                   // +0x39211
    int field_39215;                   // +0x39215
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall BuildCompoundAddress(int* a, int* b);

// FUNCTION: 0x442000
int __stdcall FUN_00442000(int unused)
{
    int a = 0;
    int b = 0;
    int result = BuildCompoundAddress(&a, &b);
    if (result >= 0) {
        g_game->field_39211 = a;
        result = 0;
        g_game->field_39215 = b;
    }
    return result;
}
