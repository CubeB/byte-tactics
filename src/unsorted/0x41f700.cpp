// Decompiled by Opus. Names are provisional.
// Opens the CD check dialog (CDCHECK.GUI) with FUN_0041f680 as its handler.

struct Sub_0041f700 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Game_0041f700 {
    char unknown_0[0x519];
    Sub_0041f700 sub;                  // +0x519
};
#pragma pack(pop)

struct Gadget_0041f700 {
    char unknown_0[0x8];
    int (__stdcall* handler)(void*);   // +0x8
};

extern Game_0041f700* g_game;

Gadget_0041f700* __stdcall FUN_004aa8f0(Sub_0041f700* sub, const char* name, int flags);
void __stdcall FUN_004c22d0(int param);
void __stdcall FUN_0049fb10(Sub_0041f700* sub, int value);
void __stdcall FUN_004a81e0(Sub_0041f700* sub, int value);
int __stdcall FUN_0041f680(void* gadget);

// FUNCTION: 0x41f700
void FUN_0041f700()
{
    FUN_004aa8f0(&g_game->sub, "CDCHECK.GUI", 0x101)->handler = FUN_0041f680;
    FUN_004c22d0(1);
    FUN_0049fb10(&g_game->sub, 1);
    FUN_004a81e0(&g_game->sub, 0x40);
}
