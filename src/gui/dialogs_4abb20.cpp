// Decompiled by Opus. Names are provisional.
// Opens the confirmation dialog (CONFIRM.GUI) and sets its title text.
#include <string.h>

#pragma pack(push, 1)
struct Gadget_004abb20 {
    char unknown_0[0x13];
    short field_13;                    // +0x13
    char unknown_15[0xb6 - 0x15];
    char text[0x15b - 0xb6];           // +0xb6
};
#pragma pack(pop)

struct Layer_004abb20 {
    int unknown_0;
    Gadget_004abb20* gadgets;          // +0x4
};

struct Sub_004abb20;

Layer_004abb20* __stdcall FUN_004aa8f0(Sub_004abb20* sub, const char* name, int flags);
int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);

// FUNCTION: 0x4abb20
int __stdcall FUN_004abb20(Sub_004abb20* sub, char* title)
{
    Layer_004abb20* layer = FUN_004aa8f0(sub, "CONFIRM.GUI", 0);
    if (layer) {
        Gadget_004abb20* gadgets = layer->gadgets;
        int i = FUN_0049fdf0(gadgets, "TITL", 5);
        strcpy(gadgets[i].text, title);
        gadgets[i].field_13 = -1;
        return 1;
    }
    return 0;
}
