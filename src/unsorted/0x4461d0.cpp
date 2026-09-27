// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Handler for the "MODES" (display mode) dialog. When a mode gadget is
// selected (MODES or SELECT), it copies the selected display mode into the
// game's width/height and the local player's screen size, then applies it.
// CANCEL exits the game, OK plays the button sound. With no current gadget
// (-1) it frees the display mode list.

#pragma pack(push, 1)
struct Mode_00446310 {
    int width;                         // +0x0
    int height;                        // +0x4
    int field_8;                       // +0x8
};

struct Class_00446310 {
    int count;                         // +0x0
    Mode_00446310* modes;              // +0x4
    char unknown_8[0x14 - 0x8];
    char* available;                   // +0x14
    char unknown_18[0x20 - 0x18];
};

struct PlayerData_004461d0 {
    char unknown_0[0x8b];
    unsigned short field_8b;           // +0x8b
    unsigned short field_8d;           // +0x8d
};

struct Player_004461d0 {
    char unknown_0[0x27];
    PlayerData_004461d0* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game_004461d0 {
    char unknown_0[0x1b63];
    Player_004461d0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37f1b - 0x2a43];
    int field_37f1b;                   // +0x37f1b
    int field_37f1f;                   // +0x37f1f
};

struct Gadget_004461d0 {
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Holder_004461d0 {
    char unknown_0[4];
    Gadget_004461d0* gadgets;          // +0x4
    char unknown_8[4];
    Class_00446310* obj;               // +0xc
};

struct Gui_004461d0 {
    char unknown_0[0x18];
    Holder_004461d0* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};
#pragma pack(pop)

extern Game_004461d0* g_game;

void FUN_004d85a0(void* p);
int __stdcall FUN_0049fd60(Gui_004461d0* gui, char* name);
Gadget_004461d0* __stdcall FUN_0049ff90(Gadget_004461d0* gadgets, char* name);
void __stdcall FUN_0047f1a0(char* name, int param_2);
void __stdcall FUN_004ab0a0(Gui_004461d0* gui);
void FUN_00430f00(void);
void FUN_00450f90(void);

// FUNCTION: 0x4461d0
void __stdcall FUN_004461d0(Gui_004461d0* gui)
{
    Holder_004461d0* holder = gui->holder;
    Gadget_004461d0* gadgets = holder->gadgets;
    Class_00446310* obj = holder->obj;
    if (gui->current == -1) {
        FUN_004d85a0(obj->available);
        FUN_004d85a0(obj->modes);
        FUN_004d85a0(obj);
        return;
    }
    if (FUN_0049fd60(gui, "MODES") || FUN_0049fd60(gui, "SELECT")) {
        Player_004461d0* player = &g_game->players[g_game->localPlayer];
        Gadget_004461d0* entry = FUN_0049ff90(gadgets, "MODES");
        Mode_00446310* mode = &obj->modes[entry->selected];
        g_game->field_37f1b = mode->width;
        g_game->field_37f1f = mode->height;
        player->data->field_8b = (unsigned short)mode->width;
        player->data->field_8d = (unsigned short)mode->height;
        FUN_00450f90();
        FUN_00430f00();
        return;
    }
    if (FUN_0049fd60(gui, "CANCEL")) {
        FUN_0047f1a0("Exit", 0);
        FUN_00430f00();
        return;
    }
    if (FUN_0049fd60(gui, "OK")) {
        FUN_0047f1a0("SMLBUTTON", 0);
        FUN_00430f00();
        return;
    }
    FUN_004ab0a0(gui);
}
