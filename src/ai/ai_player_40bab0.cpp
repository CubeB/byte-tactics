// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

struct Struct_0040bab0 {
    int a, b, c;
};

#pragma pack(push, 1)
struct Game_0040bab0 {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
};
#pragma pack(pop)

extern Game_0040bab0* g_game;
extern void* DAT_005119c0[];

// FUNCTION: 0x40bab0
void __stdcall FUN_0040bab0(int index, int unused, Struct_0040bab0* out)
{
    out->a = (g_game->baseX << 16) -
             ((Struct_0040bab0*)((char*)DAT_005119c0[index] + 0x35))->a;
    out->b = 0;
    out->c = (g_game->baseX << 16) -
             ((Struct_0040bab0*)((char*)DAT_005119c0[index] + 0x35))->c;
}
