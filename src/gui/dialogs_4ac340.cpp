// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Opens the player setup dialog (INPUT.GUI), fills the TITL / INPT / CHC1 /
// CHC2 gadget texts and installs FUN_004ac300 as its handler. Returns 1 if the
// dialog was created, else 0.
#include <string.h>

#pragma pack(push, 1)
struct Gadget_004ac340 {               // 0x15b bytes
    char unknown_0[0x13];
    short field_13;                    // +0x13
    char unknown_15[0xb6 - 0x15];
    char text[0x15b - 0xb6];           // +0xb6
};
#pragma pack(pop)

struct Layer_004ac340 {
    int unknown_0;
    Gadget_004ac340* gadgets;          // +0x4
};

struct Sub_004ac340 {
    char unknown_0[0x18];
    Layer_004ac340* current;           // +0x18
};

struct Dialog_004ac340 {
    char unknown_0[0x8];
    void (__stdcall* handler)(Sub_004ac340*); // +0x8
};

Dialog_004ac340* __stdcall FUN_004aa8f0(Sub_004ac340* sub, const char* name, int flags);
int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004ac300(Sub_004ac340* sub);

// FUNCTION: 0x4ac340
int __stdcall FUN_004ac340(Sub_004ac340* sub, char* title, char* input, char* chc2, char* chc1, int unused)
{
    Dialog_004ac340* dialog = FUN_004aa8f0(sub, "INPUT.GUI", 0);
    if (dialog) {
        Gadget_004ac340* gadgets = sub->current->gadgets;
        Gadget_004ac340* g = &gadgets[FUN_0049fdf0(gadgets, "INPT", 3)];
        strcpy(g->text, input);
        g = &gadgets[FUN_0049fdf0(gadgets, "TITL", 5)];
        strcpy(g->text, title);
        g->field_13 = -1;
        Gadget_004ac340* g1 = &gadgets[FUN_0049fdf0(gadgets, "CHC1", 1)];
        Gadget_004ac340* g2 = &gadgets[FUN_0049fdf0(gadgets, "CHC2", 1)];
        strcpy(g1->text, chc1);
        strcpy(g2->text, chc2);
        dialog->handler = FUN_004ac300;
        return 1;
    }
    return 0;
}
