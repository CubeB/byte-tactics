// Decompiled by Opus. Names are provisional.
// Returns 1 when the GUI's current gadget (index at +0x60, -1 for none) has
// the given name.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0049fd60 {
    char unknown_0[2];
    char name[0x10];                   // +0x02
    char unknown_12[0x15b - 0x12];
};
#pragma pack(pop)

struct Holder_0049fd60 {
    char unknown_0[4];
    Entry_0049fd60* entries;           // +0x04
};

struct Gui_0049fd60 {
    char unknown_0[0x18];
    Holder_0049fd60* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};

// FUNCTION: 0x49fd60
int __stdcall FUN_0049fd60(Gui_0049fd60* gui, char* name)
{
    if (!gui->holder)
        return 0;
    if (gui->current == -1)
        return 0;
    return strcmp(gui->holder->entries[gui->current].name, name) == 0;
}
