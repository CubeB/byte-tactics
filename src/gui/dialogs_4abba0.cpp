// Decompiled by Opus. Names are provisional.
// Same shape as 0x4ac300: tests the current GUI entry against "CHC1" and
// "CHC2".

struct Entry_004abba0;

int __stdcall IsGadgetNamed(Entry_004abba0* entries, int i, char* name);

struct Layer_004abba0 {
    int unknown_0;
    Entry_004abba0* entries;           // +0x4
};

struct Gadget_004abba0 {
    char unknown_0[0x18];
    Layer_004abba0* layer;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int index;                         // +0x60
};

// FUNCTION: 0x4abba0
void __stdcall YesNoDialogHandler(Gadget_004abba0* gadget)
{
    Entry_004abba0* entries = gadget->layer->entries;
    int index = gadget->index;
    IsGadgetNamed(entries, index, "CHC1");
    IsGadgetNamed(entries, index, "CHC2");
}
