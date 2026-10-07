// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// Selecting the entry with the index the caller passes: remembers the index,
// and if the entry it names is a type 3 (text) control it makes the group's
// type 7 list entry current and puts the entry's own text back into the field.
#pragma pack(push, 1)

// A window's colour table: indexed from the window pointer itself.
struct Colour_004a7830 {
    char unknown_0[0x8b2];
    unsigned char colour;              // +0x8b2
};

struct Entry_004a7830 {                // 0x15b bytes
    unsigned char type;                // +0x000
    char unknown_01[0x1f - 0x01];
    Colour_004a7830* colours;          // +0x01f
    char unknown_23[0x28 - 0x23];
    char group;                        // +0x028
    char unknown_29[0xb6 - 0x29];
    union {
        char text[0x82];               // +0x0b6 (a text control)
        short count;                   // +0x0b6 (entry 0: number of entries)
        struct {
            char pad[0x20];
            int id;                    // +0x0d6 (a list entry)
        } list;
    } data;
    short maxLength;                   // +0x138
    char unknown_13a[0x15b - 0x13a];
};

struct Holder_004a7830 {
    int unknown_0;
    Entry_004a7830* entries;           // +0x4
    char unknown_8[0x20 - 0x8];
    int field_20;                      // +0x20
};

struct Menu_004a7830 {
    char unknown_0[0x18];
    Holder_004a7830* holder;           // +0x18
    char unknown_1c[0x64 - 0x1c];
    int focus;                         // +0x64
};

#pragma pack(pop)

struct Dialog {
    int group;                         // +0x0
};

extern Dialog* g_guiContext;

int GetTextKeyColor();
void __stdcall SetTextColors(int param_1, int param_2);
void __stdcall SetFont(int param_1);
void ClearKeyQueue();
int __stdcall FUN_0049fc50(Menu_004a7830* obj, int index);
void __stdcall FUN_004ab6c0(Menu_004a7830* control, int param_2, char* text,
                            int maxLength, int clear);

// FUNCTION: 0x4a7830
void __stdcall SelectGadgetByIndex(Menu_004a7830* menu, int index)
{
    // The array is loaded twice: this copy for the type test, again inside the branch.
    Entry_004a7830* first = menu->holder->entries;
    menu->focus = -1;
    menu->holder->field_20 = index;
    if (first[menu->holder->field_20].type == 3) {
        int i = menu->holder->field_20;
        Entry_004a7830* entries = menu->holder->entries;
        Entry_004a7830* entry = &entries[i];
        int font = GetTextKeyColor();
        SetTextColors((int)((unsigned char*)entry->colours)[(int)menu + 0x8b2], font);

        int n = 0;
        int j = 1;
        for (; j < entries->data.count + 1; j++) {
            if (entries[j].type == 7) {
                if (n == entry->group) {
                    SetFont(entries[j].data.list.id);
                    break;
                }
                n++;
            }
        }
        if (j == entries->data.count + 1) {
            SetFont(g_guiContext->group);
        }

        FUN_0049fc50(menu, i);
        menu->holder->field_20 = i;
        FUN_004ab6c0(menu, i, entry->data.text, entry->maxLength, 0);
        ClearKeyQueue();
    }
}
