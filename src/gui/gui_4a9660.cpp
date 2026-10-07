// Decompiled by Opus. Names are provisional.
// Closes the top GUI screen: runs its close handler, pops it off the stack,
// activates the next one and frees the old node.

struct Gui_004a9660;

struct Screen_004a9660 {
    Screen_004a9660* next;             // +0x0
    char unknown_4[0x8 - 0x4];
    int (__stdcall* handler)(Gui_004a9660*); // +0x8
    char unknown_c[0x10 - 0xc];
    unsigned int flags;                // +0x10
    int active;                        // +0x14
};

struct Gui_004a9660 {
    char unknown_0[0x18];
    Screen_004a9660* top;              // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
    int field_64;                      // +0x64
    int field_68;                      // +0x68
};

void HideSoftwareCursor();
void __stdcall RenderLayer(Gui_004a9660* gui, int value);
void ShowSoftwareCursor();
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4a9660
void __stdcall CloseTopScreen(Gui_004a9660* gui)
{
    if (gui->top) {
        unsigned int flags = gui->top->flags;
        gui->field_68 = gui->field_60 = gui->field_64 = -1;
        if (gui->top->handler)
            gui->top->handler(gui);
        HideSoftwareCursor();
        RenderLayer(gui, 2);
        ShowSoftwareCursor();
        Screen_004a9660* old = gui->top;
        gui->top = old->next;
        if (gui->top)
            gui->top->active = 1;
        FUN_004d85a0(old);
        if (flags & 0x800)
            RenderLayer(gui, 0x40);
    }
}
