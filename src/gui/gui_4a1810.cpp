// Decompiled by space-bunny-free. Names are provisional.
// Selects the entry of type 7 whose group number (the number of type-7 entries
// before it) matches the group of entry `index`, and makes that entry's id the
// current one. Returns the entry's number, or -1 when there is no such entry.

#pragma pack(push, 1)
struct Entry_004a1810 {               // 0x15b bytes
    unsigned char type;               // +0x00
    char unknown_01[0x28 - 0x01];
    char group;                       // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                      // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xd6 - 0xb8];
    int id;                           // +0xd6
    char unknown_da[0x15b - 0xda];
};
#pragma pack(pop)

struct Dialog {
    int group;                        // +0x00
    char unknown_04[0x14 - 0x04];
};

extern Dialog* g_guiContext;

void __stdcall SetFont(int id);

// FUNCTION: 0x4a1810
int __stdcall SelectFontForEntry(Entry_004a1810* entries, int index)
{
    int n = 0;
    int i = 1;
    for (; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entries[index].group) {
                SetFont(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1) {
        SetFont(g_guiContext->group);
        i = -1;
    }
    return i;
}
