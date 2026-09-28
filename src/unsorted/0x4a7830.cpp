// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Selecting a text control in a dialog: remembers the index, picks the
// current id of the list entry that belongs to the control's group and puts
// the control's text back into it.
//
// Still differs (71.8%): the original loads the entry array pointer
// (`[holder+4]`) right after loading the holder, before the `focus = -1`
// store, and only then reloads the holder for the index; here it reloads the
// holder first and loads the array pointer after the index, so `edi`/`esi`/
// `ebx`/`ebp` end up swapped and the colour byte lands in `edx` rather than
// `eax`. Everything from the loop onward is otherwise identical. The sibling
// 0x4a76b0 has the same shape but a FindEntry prologue that pins the entry
// pointer in `ebx`.

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

struct Class_0051fba4 {
    int group;                         // +0x0
};

extern Class_0051fba4* DAT_0051fba4;

int FUN_004c13f0();
void __stdcall FUN_004c13a0(int param_1, int param_2);
void __stdcall FUN_004c1420(int param_1);
void FUN_004c1a40();
int __stdcall FUN_0049fc50(Menu_004a7830* obj, int index);
void __stdcall FUN_004ab6c0(Menu_004a7830* control, int param_2, char* text,
                            int maxLength, int clear);

// FUNCTION: 0x4a7830
void __stdcall FUN_004a7830(Menu_004a7830* menu, int index)
{
    menu->focus = -1;
    menu->holder->field_20 = index;
    if (menu->holder->entries[menu->holder->field_20].type == 3) {
        Entry_004a7830* entries = menu->holder->entries;
        int i = menu->holder->field_20;
        Entry_004a7830* entry = &entries[i];
        int font = FUN_004c13f0();
        FUN_004c13a0((int)((unsigned char*)entry->colours)[(int)menu + 0x8b2], font);

        int n = 0;
        int j = 1;
        for (; j < entries->data.count + 1; j++) {
            if (entries[j].type == 7) {
                if (n == entry->group) {
                    FUN_004c1420(entries[j].data.list.id);
                    break;
                }
                n++;
            }
        }
        if (j == entries->data.count + 1) {
            FUN_004c1420(DAT_0051fba4->group);
        }

        FUN_0049fc50(menu, i);
        menu->holder->field_20 = i;
        FUN_004ab6c0(menu, i, entry->data.text, entry->maxLength, 0);
        FUN_004c1a40();
    }
}
