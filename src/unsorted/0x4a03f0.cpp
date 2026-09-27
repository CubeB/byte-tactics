// Decompiled by space-bunny-free. Names are provisional.
// Sets an entry of the GUI layout (compare 0x4a1200, which only writes the
// value): entry 4 is a group, so every member of the group follows, entry 2 is
// a multi-state button, so the value entry that feeds it is updated too.

#pragma pack(push, 1)
struct Entry_4a03f0 {                // 0x15b bytes
    char tag;                        // +0x00
    char subtag;                     // +0x01 (group id)
    char name[0x10];                 // +0x02
    char unknown_12[0x29 - 0x12];
    char value;                      // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                     // +0xb6 (used in entry 0)
    char unknown_b8[0x15b - 0xb8];
};

struct Layer_4a03f0 {
    char unknown_0[4];
    Entry_4a03f0* entries;           // +0x04
    char unknown_8[0x20 - 0x8];
    int selected;                    // +0x20
};

struct Menu_0041a120 {
    char unknown_0[0x18];
    Layer_4a03f0* layer;             // +0x18
    char unknown_1c[0xcca - 0x1c];
    int dirty;                       // +0xcca
};
#pragma pack(pop)

void __stdcall FUN_004a7960(Menu_0041a120* menu, int value);
void __stdcall FUN_004a03f0(Menu_0041a120* menu, int index, int value);

// FUNCTION: 0x4a03f0
void __stdcall FUN_004a03f0(Menu_0041a120* menu, int index, int value)
{
    Entry_4a03f0* entries = menu->layer->entries;
    int i;

    entries[index].value = value;

    if (entries[index].tag == 4) {
        for (i = 0; i <= entries[0].count; i++) {
            if (entries[i].tag == 1 && entries[i].subtag == entries[index].subtag) {
                entries[i].value = value;
            }
        }
    } else if (entries[index].tag == 2) {
        if (value == 0) {
            char subtag = entries[index].subtag;
            int found = 0;
            i = 1;
            for (;;) {
                if (i >= entries[0].count + 1) {
                    found = 0;
                    break;
                }
                if (entries[i].tag == 4 && entries[i].subtag == subtag) {
                    found = i;
                    break;
                }
                i++;
            }
            if (found != -1) {
                FUN_004a03f0(menu, found, 0);
            }
        }
    }

    if (value == 0 && index == menu->layer->selected) {
        FUN_004a7960(menu, 1);
    }
    menu->dirty = 1;
}
