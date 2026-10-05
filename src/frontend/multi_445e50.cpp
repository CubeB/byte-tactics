// Decompiled by Opus. Names are provisional.
// Sets up the gadget with the given name in the game's menu (if it exists),
// then hands the gadget's index to the callback.

struct Holder_00445e50 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_00445e50 {
    char unknown_0[0x18];
    Holder_00445e50* holder;           // +0x18
};

struct Game;

typedef void (__stdcall* Callback_00445e50)(Menu_00445e50* menu, int index);

#pragma pack(push, 2)
struct Gadget_00445e50 {
    char unknown_0[0x13c];
    int field_13c;                     // +0x13c
    short field_140;                   // +0x140
    short unknown_142;
    Callback_00445e50 callback;        // +0x144
    char unknown_148[2];
    Game* game;                        // +0x14a
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_00445e50 menu;                // +0x519
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FindGadgetIndex(void* gadgets, const char* name, int flag);
Gadget_00445e50* __stdcall FUN_004a0200(void* data, char* key);
void __stdcall FUN_0045b9b0(Gadget_00445e50* gadget, int value);
void __stdcall FUN_0049fa90(Menu_00445e50* menu);

// FUNCTION: 0x445e50
void __stdcall FUN_00445e50(char* name, int param_2, int param_3, Callback_00445e50 callback)
{
    Menu_00445e50* menu = &g_game->menu;
    void* gadgets = menu->holder->gadgets;
    int index = FindGadgetIndex(gadgets, name, 0xe);
    if (index != -1) {
        Gadget_00445e50* gadget = FUN_004a0200(gadgets, name);
        gadget->field_13c = param_2;
        gadget->callback = callback;
        gadget->field_140 = param_3;
        FUN_0045b9b0(gadget, gadget->field_140);
        gadget->game = g_game;
    }
    callback(menu, index);
    FUN_0049fa90(menu);
}
