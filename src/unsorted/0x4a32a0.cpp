// Decompiled by Sonnet 5.5. Names are provisional.
// Fills a list box of the GUI: finds the entry called `name`, stores the item
// text and count in it, makes sure its line height (+0xda) is at least one
// font line, optionally sets a selected item id, and works out how many lines
// fit (+0xbe) by taking the entry's height (+0x19) down one line per item.
// When the entry has a linked scroll bar (+0x29) the bar's group entry is
// looked up in the global GUI and refreshed, and 0x4a3ef0 is called when the
// items overflow.
//
// NOT MATCHED: 48.6%, 754 of 765 bytes. The control flow and the inlined
// helpers (name search, font step evaluated twice by a MAX macro, scroll bar
// search) follow the original, but the register assignment differs: the
// original keeps `name` in ebp, the item count in ebx (loaded after the first
// search) and the constant 0 in edi, and spills `obj->holder` to a local at
// [esp+0x10] that the second search reloads; here name lands in ebx, count in
// edi and the zero in ebp. A holder local, count copies and declaration
// changes did not move it.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a32a0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char id;                  // +0x01
    char name[0x17];                   // +0x02
    short height;                      // +0x19
    unsigned int flags;                // +0x1b
    char unknown_1f[0x29 - 0x1f];
    unsigned char scroll;              // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short items;                       // +0xc0
    char* text;                        // +0xc2
    char unknown_c6[0xd6 - 0xc6];
    int selected;                      // +0xd6
    short lineHeight;                  // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Holder_004a32a0 {
    char unknown_0[4];
    Entry_004a32a0* entries;           // +0x04
};

struct Font_004a32a0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct Class_004a32a0 {
    int group;                         // +0x00
    char unknown_04[0x14 - 0x04];
    Font_004a32a0* font;               // +0x14
    Holder_004a32a0* holder;           // +0x18
};
#pragma pack(pop)

extern Class_004a32a0* DAT_0051fba4;

int FUN_004c1450();
void* __stdcall FUN_004b7f30(void* a, int b);
void __stdcall FUN_004b6290(const char* text);
void __stdcall FUN_004a03f0(Class_004a32a0* obj, int index, int flag);
void __stdcall FUN_004a3ef0(Class_004a32a0* obj, int index);

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static inline int Step_004a32a0()
{
    if (DAT_0051fba4->font == 0)
        return FUN_004c1450();
    return *(unsigned short*)((char*)FUN_004b7f30(DAT_0051fba4->font->glyphs, 0x49) + 2) + 2;
}

static inline int FindName_004a32a0(Entry_004a32a0* entries, const char* name)
{
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

static inline int FindBar_004a32a0(Entry_004a32a0* entries, unsigned char id)
{
    int k;
    for (k = 1; k < entries->count + 1; k++) {
        if (entries[k].type == 4 && entries[k].id == id)
            return k;
    }
    return 0;
}

// FUNCTION: 0x4a32a0
void __stdcall FUN_004a32a0(Class_004a32a0* obj, char* name, char* text, int count, int selected)
{
    Entry_004a32a0* entries = obj->holder->entries;
    int idx = FindName_004a32a0(entries, name);
    Entry_004a32a0* e;
    if (idx != -1) {
        e = &entries[idx];
    } else {
        FUN_004b6290("Error in GUI layout");
        e = 0;
    }
    e->items = count;
    e->text = text;
    e->flags |= 0x10;
    e->lineHeight = MAX(e->lineHeight, Step_004a32a0() + 1);
    if (selected != 0) {
        e->selected = selected;
        e->flags |= 0x800;
    }
    e->field_bc = 0;
    e->field_ba = 0;
    int rem = e->height;
    e->field_be = count - 1;
    int line;
    if (e->lineHeight == 0)
        line = Step_004a32a0() + 1;
    else
        line = e->lineHeight;
    for (int n = count - 1; n > -1; n--) {
        rem -= line;
        if (rem < 0)
            break;
        e->field_be = n;
    }
    if (e->scroll != 0) {
        entries = obj->holder->entries;
        int j = FindName_004a32a0(entries, name);
        unsigned char id = entries[j].id;
        int k = FindBar_004a32a0(entries, id);
        if (k != -1) {
            Entry_004a32a0* bar = &entries[k];
            Class_004a32a0* g = DAT_0051fba4;
            char* barName = bar->name;
            if (g->holder) {
                int m = FindName_004a32a0(g->holder->entries, barName);
                if (m != -1)
                    FUN_004a03f0(g, m, rem < 0);
            }
            if (rem < 0)
                FUN_004a3ef0(DAT_0051fba4, k);
        }
    }
}
