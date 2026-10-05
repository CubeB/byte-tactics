// Decompiled by Opus. Names are provisional.
// Opens a dialog whose GUI file name is kept in the game object, with
// FUN_00494890 as its handler and the game object as its owner.

struct Sub_00495200 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Game_00495200 {
    char unknown_0[0x519];
    Sub_00495200 sub;                  // +0x519
    char unknown_529[0x37ea0 - 0x529];
    char guiName[0x20];                // +0x37ea0
};
#pragma pack(pop)

struct Gadget_00495200 {
    char unknown_0[0x8];
    void (__stdcall* handler)(Gadget_00495200*); // +0x8
    Game_00495200* owner;              // +0xc
};

// GLOBAL: 0x511de8
extern Game_00495200* g_game;

Gadget_00495200* __stdcall FUN_004aa8f0(Sub_00495200* sub, const char* name, int flags);
void __stdcall FUN_00494890(Gadget_00495200* gadget);

// FUNCTION: 0x495200
void FUN_00495200()
{
    Gadget_00495200* gadget = FUN_004aa8f0(&g_game->sub, g_game->guiName, 0x20);
    gadget->handler = FUN_00494890;
    gadget->owner = g_game;
}
