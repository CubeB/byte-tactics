// Layer (Thaldren's GuiLayout): one 0x40-byte menu screen, the record
// LoadGuiLayer returns and the stack behind Gui+0x18: the gadget array, the
// close and key handlers, the flags and focus state, the key state and the
// background surface. The one declaration of the record for the files that
// open or walk a screen; the types behind the pointers stay private to their
// own files. Nothing is included: one forward declaration.
#ifndef LAYER_H
#define LAYER_H

struct Gadget;

#pragma pack(push, 1)

struct Layer {
    Layer* next;                           // +0x00, the previous screen in the stack
    Gadget* entries;                       // +0x04, the gadget array
    void (__stdcall* handler)(void*);      // +0x08, the close or click handler
    void* data;                            // +0x0c, the handler's own record
    unsigned int flags;                    // +0x10
    int dirty;                             // +0x14, nonzero: redraw the screen
    int keyboardInput;                     // +0x18
    void (__stdcall* cb1c)();              // +0x1c
    int current;                           // +0x20, the focused entry, -1 for none
    void* surface;                         // +0x24, the screen's draw surface
    char text[0xe];                        // +0x28, the key state
    char lastKey;                          // +0x36
    int clickMode;                         // +0x37
    void (__stdcall* textHandler)(void*);  // +0x3b, the key handler
};

#pragma pack(pop)

#endif
