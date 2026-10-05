// Decompiled by Space Bunny Free. Names are provisional.
// Opens the TCP settings dialog (TCP.GUI) with FUN_00442050 as its handler.
// The "ADDRESS" setting gets the direct-connect address typed on the command
// line (DAT_00512d90) when there is one, otherwise the "TCPADDR" value; the
// ADDRESS gadget is then selected in the dialog.
#include <string.h>

struct Inner_004421f0 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_004421f0 {
    char unknown_0[0x18];
    Inner_004421f0* inner;             // +0x18
};

struct Dialog_004421f0 {
    int unknown_0;
    void* gadgets;                     // +0x4
    void (__stdcall* handler)(Menu_004421f0*); // +0x8
    struct Game* owner;                // +0xc
    char unknown_10[0x1c - 0x10];
    int field_1c;                      // +0x1c
};

struct Info_004421f0 {
    int unknown_0;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    void* field_10;                    // +0x10
    Info_004421f0 info;                // +0x14
    char unknown_18[0x519 - 0x18];
    Menu_004421f0 menu;                // +0x519
    char unknown_535[0x2aaf - 0x535];
    unsigned short field_2aaf;         // +0x2aaf
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;
// GLOBAL: 0x512c84
extern int DAT_00512c84;
// GLOBAL: 0x512d90
extern char DAT_00512d90[];

Dialog_004421f0* __stdcall LoadGuiLayer(Menu_004421f0* menu, const char* name, int flags);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall HAPINET_initlobbiedconnection(Info_004421f0* info);
int __stdcall FindGadgetIndex(void* gadgets, const char* name, int flag);
char* __stdcall GetGadgetText(Menu_004421f0* menu, const char* key, char* out);
int __stdcall FUN_0042f980(const char* key, char* out, int* len);
void __stdcall FUN_004a0bf0(Menu_004421f0* menu, const char* key, int value, int param_4);
void __stdcall FUN_00442050(Menu_004421f0* menu);
void __stdcall SelectGadgetByIndex(Menu_004421f0* menu, int index);
void __stdcall FUN_0049fc50(Menu_004421f0* menu, int index);
void __stdcall FUN_0049fb10(Menu_004421f0* menu, int value);
void __stdcall RenderLayer(Menu_004421f0* menu, int value);

// FUNCTION: 0x4421f0
void FUN_004421f0()
{
    Dialog_004421f0* dialog = LoadGuiLayer(&g_game->menu, "TCP.GUI", 0x800);
    dialog->handler = FUN_00442050;
    dialog->owner = g_game;
    dialog->field_1c = 0;
    FUN_004288d0(0, 0, 0, 0);
    HAPINET_initlobbiedconnection(&g_game->info);
    FindGadgetIndex(dialog->gadgets, "ADDRESS", 3);
    char* address = GetGadgetText(&g_game->menu, "ADDRESS", 0);
    int direct = DAT_00512d90[0];
    int len = 0x80;
    if (direct) {
        address[0] = 0;
        strncat(address, DAT_00512d90, len - 1);
    } else {
        if (!FUN_0042f980("TCPADDR", address, &len)) {
            address[0] = 0;
        }
    }
    FUN_004a0bf0(&g_game->menu, "ADDRESS", (int)address, 0);
    if (direct) {
        FUN_00442050(&g_game->menu);
        g_game->field_2aaf = g_game->field_2aaf ^ ((DAT_00512c84 != 0) ^ g_game->field_2aaf) & 1;
    } else {
        SelectGadgetByIndex(&g_game->menu, FindGadgetIndex(dialog->gadgets, "ADDRESS", 3));
        FUN_0049fc50(&g_game->menu, FindGadgetIndex(dialog->gadgets, "ADDRESS", 3));
        FUN_0049fb10(&g_game->menu, 1);
        RenderLayer(&g_game->menu, 0x40);
    }
}
