// Decompiled by Opus. Names are provisional.
// Opens the map view dialog (VIEWMAP.GUI) with FUN_00444ba0 as its handler.

struct Sub_00444be0 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Game_00444be0 {
    char unknown_0[0x519];
    Sub_00444be0 sub;                  // +0x519
};
#pragma pack(pop)

struct Gadget_00444be0 {
    char unknown_0[0x8];
    void (__stdcall* handler)(void*);  // +0x8
};

// GLOBAL: 0x511de8
extern Game_00444be0* g_game;

Gadget_00444be0* __stdcall FUN_004aa8f0(Sub_00444be0* sub, const char* name, int flags);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void FUN_00444a20();
void __stdcall FUN_0049fb10(Sub_00444be0* sub, int value);
void __stdcall FUN_004a81e0(Sub_00444be0* sub, int value);
void __stdcall FUN_00444ba0(void* gadget);

// FUNCTION: 0x444be0
void FUN_00444be0()
{
    FUN_004aa8f0(&g_game->sub, "VIEWMAP.GUI", 0x900)->handler = FUN_00444ba0;
    FUN_004288d0("DVIEWMAP", 0, 0, 0);
    FUN_00444a20();
    FUN_0049fb10(&g_game->sub, 1);
    FUN_004a81e0(&g_game->sub, 0x40);
}
