// Gui: the GUI system object (Thaldren's GuiRoot, 0xcf6 bytes) that sits at
// g_game+0x519: the fonts, the layer stack above it (+0x18), the cursor
// sprites and animation, the last mouse event, the index of the hot gadget
// (+0x60), the colour remap, the GUI file paths and the changed flag. The one
// declaration of the type, for the files that define or read it.
//
// The members are plain data and Layer is the only type named: every type or
// forward declaration the header adds moves the symbol ids of the small views
// it replaces (docs/c2-regalloc.md), and those views match in narrow windows.
// The files that read the pointers behind the members (the font tables, the
// cursor animation, the mouse event) keep their own view of them.
#ifndef GUI_H
#define GUI_H

#pragma pack(push, 1)

struct Layer;

struct Gui {
    int font;                           // +0x00
    void* gaf;                          // +0x04
    void* values[3];                    // +0x08, the font tables
    void* language;                     // +0x14, the font table in use
    Layer* layer;                       // +0x18
    int cursorSharedFrame;              // +0x1c
    int cursorDefaultFrame;             // +0x20
    int cursorHoverFrame;               // +0x24
    void* src_28;                       // +0x28, the hover cursor animation
    void* src_2c;                       // +0x2c, the active cursor animation
    unsigned short cursorIndex;         // +0x30, the cursor animation reference
    unsigned short cursorValue;         // +0x32
    unsigned char cursorKind;           // +0x34
    char unknown_35[3];
    void* cursorSrc;                    // +0x38
    int pointX;                         // +0x3c, the last mouse event
    int pointY;                         // +0x40
    unsigned int pointKeyFlags;         // +0x44
    int pointTick;                      // +0x48
    int pointMessage;                   // +0x4c
    int pointDoubleClick;               // +0x50
    int mouseKeyFlags;                  // +0x54
    int clickMode;                      // +0x58
    unsigned int cursorFlags;           // +0x5c, bit 0 set while the cursor animates
    int hotGadgetIndex;                 // +0x60
    // Must stay a second field after hotGadgetIndex (dialogs.cpp).
    int focus;                          // +0x64
    int hoverGadgetIndex;               // +0x68
    int prevHoverGadget;                // +0x6c
    int clearQuickKeys;                 // +0x70
    int cursor;                         // +0x74
    int knobDragging;                   // +0x78
    char unknown_7c[0x96 - 0x7c];
    int time;                           // +0x96
    int animTimer;                      // +0x9a
    int inputEnabled;                   // +0x9e
    int dirty;                          // +0xa2 (nonzero = selection active)
    char unknown_a6[0x8b2 - 0xa6];
    unsigned char colours[0x100];       // +0x8b2
    char unknown_9b2[0x9b6 - 0x9b2];
    char str_9b6[0x100];                // +0x9b6
    char str_ab6[0x100];                // +0xab6
    char str_bb6[0x110];                // +0xbb6
    int pathsReady;                     // +0xcc6
    int changed;                        // +0xcca
    int clickStatusCache;               // +0xcce
    int fallback;                       // +0xcd2
    char cachedBgName;                  // +0xcd6
    char unknown_cd7[0xcf6 - 0xcd7];
};

#pragma pack(pop)

#endif
