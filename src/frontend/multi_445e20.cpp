// Decompiled by Opus. Names are provisional.
// Sets the value of the named gadget of a menu (see 0x445e50).

struct Holder_00445e20 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_00445e20 {
    char unknown_0[0x18];
    Holder_00445e20* holder;           // +0x18
};

struct Gadget_00445e20;

Gadget_00445e20* __stdcall FUN_004a0200(void* data, char* key);
void __stdcall FUN_0045b9b0(Gadget_00445e20* gadget, int value);

// FUNCTION: 0x445e20
void __stdcall FUN_00445e20(Menu_00445e20* menu, char* name, int value)
{
    Gadget_00445e20* gadget = FUN_004a0200(menu->holder->gadgets, name);
    FUN_0045b9b0(gadget, value);
}
