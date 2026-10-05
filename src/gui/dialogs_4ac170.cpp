// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Creates the CHOICE3.GUI dialog, copies the title and three choice strings
// into the CHC1 / CHC2 / CHC3 / TITL gadget entries (each 0x15b bytes, text
// at +0xb6), clears the TITL entry's +0x13 field and installs Choice3DialogHandler
// as the dialog handler. Returns 1 if the dialog was created, else 0.
#include <string.h>

#pragma pack(push, 1)
struct Gadget_004ac170 {               // 0x15b bytes
    char unknown_0[0x13];
    short field_13;                    // +0x13
    char unknown_15[0xb6 - 0x15];
    char name[0x15b - 0xb6];           // +0xb6
};
#pragma pack(pop)

struct Inner_004ac170 {
    int unknown_0;
    Gadget_004ac170* gadgets;          // +0x4
};

struct Menu_004ac170 {
    char unknown_0[0x18];
    Inner_004ac170* inner;             // +0x18
};

struct Dialog_004ac170 {
    char unknown_0[0x8];
    void (__stdcall* handler)(Menu_004ac170*); // +0x8
};

Dialog_004ac170* __stdcall LoadGuiLayer(Menu_004ac170* menu, const char* name, int flags);
void __stdcall RenderLayer(Menu_004ac170* menu, int value);
int __stdcall FindGadgetIndex(Gadget_004ac170* gadgets, const char* name, int flag);
void __stdcall Choice3DialogHandler(Menu_004ac170* param_1);

// FUNCTION: 0x4ac170
int __stdcall OpenChoice3Dialog(Menu_004ac170* menu, const char* title, const char* choice1,
                           const char* choice2, const char* choice3)
{
    Dialog_004ac170* dialog = LoadGuiLayer(menu, "CHOICE3.GUI", 0);
    if (dialog != 0) {
        Gadget_004ac170* entries = menu->inner->gadgets;
        RenderLayer(menu, 1);
        Gadget_004ac170* e1 = &entries[FindGadgetIndex(entries, "CHC1", 1)];
        Gadget_004ac170* e2 = &entries[FindGadgetIndex(entries, "CHC2", 1)];
        Gadget_004ac170* e3 = &entries[FindGadgetIndex(entries, "CHC3", 1)];
        Gadget_004ac170* et = &entries[FindGadgetIndex(entries, "TITL", 5)];
        strcpy(e1->name, choice1);
        strcpy(e2->name, choice2);
        strcpy(e3->name, choice3);
        strcpy(et->name, title);
        et->field_13 = 0xffff;
        dialog->handler = Choice3DialogHandler;
        return 1;
    }
    return 0;
}
