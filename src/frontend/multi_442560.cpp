// Decompiled by Space Bunny Free. Names are provisional.
// Opens the serial link dialog (SERIAL.GUI), gives the "PORTS" and "SPEEDS"
// menus a single entry each, and restores the saved values from the registry.

struct Menu_00442560;
struct Gadget_00442560;
struct Entry_00442560;

struct Menu_00442560 {
    char unknown_0[0x10];
};

struct Game {
    char unknown_0[0x519];
    Menu_00442560 menu;                 // +0x519
};

struct Gadget_00442560 {
    char unknown_0[0x4];
    void* gadgets;                      // +0x04
    void (__stdcall* handler)(void*);   // +0x08
    Game* game;                         // +0x0c
    char unknown_10[0x1c - 0x10];
    int field_1c;                       // +0x1c
};

#pragma pack(push, 1)
struct Entry_00442560 {
    char unknown_0[0xce];
    void (__stdcall* onSelect)(Menu_00442560* menu, Entry_00442560* entry);   // +0xce
};
#pragma pack(pop)

extern Game* g_game;

Gadget_00442560* __stdcall LoadGuiLayer(Menu_00442560* menu, const char* name, int flags);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
int __stdcall HAPINET_initlobbiedconnection(char* p);
void __stdcall FUN_004a32a0(Menu_00442560* menu, char* name, char* text, int value, int flag);
int __stdcall FUN_0042f980(const char* key, void* buf, unsigned int* size);
void __stdcall FUN_004a2e40(Menu_00442560* menu, char* name, int index);
Entry_00442560* __stdcall FindGadgetChecked(void* gadgets, char* name);
void __stdcall FUN_004423a0(Menu_00442560* menu, Entry_00442560* entry);
void __stdcall FUN_00442380(Menu_00442560* menu, Entry_00442560* entry);
void __stdcall FUN_004423c0(void* gadget);
void __stdcall FUN_0049fb10(Menu_00442560* menu, int value);
void __stdcall RenderLayer(Menu_00442560* menu, int value);

// FUNCTION: 0x442560
void FUN_00442560()
{
    Gadget_00442560* gadget = LoadGuiLayer(&g_game->menu, "SERIAL.GUI", 0x800);
    gadget->handler = FUN_004423c0;
    gadget->game = g_game;
    gadget->field_1c = 0;
    FUN_004288d0(0, 0, 0, 0);
    HAPINET_initlobbiedconnection((char*)g_game + 0x14);
    FUN_004a32a0(&g_game->menu, "PORTS", "COM1\0COM2\0COM3\0COM4", 4, 0);
    FUN_004a32a0(&g_game->menu, "SPEEDS", "115200\0" "57600\0" "38400\0" "19200\0" "14400\0" "9600", 6, 0);

    int value;
    unsigned int size = 4;

    if (FUN_0042f980("SERBAUD", &value, &size)) {
        FUN_004a2e40(&g_game->menu, "SPEEDS", value);
    }
    if (FUN_0042f980("SERPORT", &value, &size)) {
        FUN_004a2e40(&g_game->menu, "PORTS", value);
    }
    Entry_00442560* entry = FindGadgetChecked(gadget->gadgets, "PORTS");
    entry->onSelect = FUN_004423a0;
    FUN_004423a0(&g_game->menu, entry);
    Entry_00442560* speeds = FindGadgetChecked(gadget->gadgets, "SPEEDS");
    speeds->onSelect = FUN_00442380;
    FUN_00442380(&g_game->menu, speeds);
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}
