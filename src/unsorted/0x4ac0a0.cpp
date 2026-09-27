// Decompiled by Opus. Names are provisional.
// Opens the "file does not exist" dialog (NOTEXIST.GUI), puts the name in
// its NAME gadget and installs FUN_004ac080 as its handler; compare 0x4abb20.
#include <string.h>

#pragma pack(push, 1)
struct Gadget_004ac0a0 {
    char unknown_0[0x13];
    short field_13;                    // +0x13
    char unknown_15[0xb6 - 0x15];
    char text[0x15b - 0xb6];           // +0xb6
};
#pragma pack(pop)

struct Layer_004ac0a0 {
    int unknown_0;
    Gadget_004ac0a0* gadgets;          // +0x4
};

struct Sub_004ac0a0 {
    char unknown_0[0x18];
    Layer_004ac0a0* current;           // +0x18
};

struct Dialog_004ac0a0 {
    char unknown_0[0x8];
    void (__stdcall* handler)(Sub_004ac0a0*); // +0x8
};

Dialog_004ac0a0* __stdcall FUN_004aa8f0(Sub_004ac0a0* sub, const char* name, int flags);
int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004ac080(Sub_004ac0a0* sub);

// FUNCTION: 0x4ac0a0
int __stdcall FUN_004ac0a0(Sub_004ac0a0* sub, char* name)
{
    Dialog_004ac0a0* dialog = FUN_004aa8f0(sub, "NOTEXIST.GUI", 0);
    if (dialog) {
        Gadget_004ac0a0* gadgets = sub->current->gadgets;
        int i = FUN_0049fdf0(gadgets, "NAME", 5);
        strcpy(gadgets[i].text, name);
        gadgets[i].field_13 = -1;
        dialog->handler = FUN_004ac080;
        return 1;
    }
    return 0;
}
