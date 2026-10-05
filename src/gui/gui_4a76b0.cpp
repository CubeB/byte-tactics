// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// Selects the menu entry named `name` (16 bytes of its name at +0x02 of the
// 0x15b-byte entry, so entry i's name is at entries + i*0x15b + 2). When the
// selected entry is type 3 it makes its group's type-7 entry current and
// refreshes its edit field. FindEntry is inlined from the same helper as
// 0x4a1810/0x4a30c0; the group loop is FUN_004a1810's body inlined.
//
// The type==3 body is the inlined helper DoSelect. The call passes a fresh
// `menu->layer->entries` and a fresh read of `menu->layer->field_20` (the
// store to field_20 above invalidates both, so the compiler reloads them into
// esi and ebx), while the type==3 test itself still reads the local `entries`
// and the field. That split is what makes the original's register choices:
// esi for the index, ebx reloaded from layer+4 rather than kept, and the test's
// `add ebx,esi` addressing that destroys the dead local.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a76b0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x02 - 0x01];
    char name[0x10];                   // +0x02
    char unknown_12[0x1f - 0x12];
    int field_1f;                      // +0x1f
    char unknown_23[0x28 - 0x23];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0xd6 - 0xb6];        // +0xb6
    } u;
    int id;                            // +0xd6
    char unknown_da[0x138 - 0xda];
    short field_138;                   // +0x138
    char unknown_13a[0x15b - 0x13a];
};

struct Layer_004a76b0 {
    int unknown_0;
    Entry_004a76b0* entries;           // +0x04
    char unknown_08[0x20 - 0x08];
    int field_20;                      // +0x20
};

struct Menu_004a76b0 {
    char unknown_00[0x18];
    Layer_004a76b0* layer;             // +0x18
    char unknown_1c[0x64 - 0x1c];
    int focus;                         // +0x64
};
#pragma pack(pop)

struct Class_0051fba4 {
    int group;                         // +0x00
};

extern Class_0051fba4* DAT_0051fba4;

int GetTextKeyColor();
void __stdcall SetTextColors(int colour, int font);
void __stdcall SetFont(int id);
int __stdcall FUN_0049fc50(Menu_004a76b0* menu, int index);
void __stdcall FUN_004ab6c0(Menu_004a76b0* menu, int index, char* text, int maxLength, int clear);
void FUN_004c1a40();

static inline int FindEntry(Entry_004a76b0* entries, char* name)
{
    for (int i = 1; i < entries->u.count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

static inline void DoSelect(Menu_004a76b0* menu, Entry_004a76b0* entries, int sel)
{
    Entry_004a76b0* entry = &entries[sel];
    SetTextColors(((unsigned char*)entry->field_1f)[(int)menu + 0x8b2], GetTextKeyColor());
    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entry->group) {
                SetFont(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1)
        SetFont(DAT_0051fba4->group);
    FUN_0049fc50(menu, sel);
    menu->layer->field_20 = sel;
    FUN_004ab6c0(menu, sel, entry->u.text, entry->field_138, 0);
    FUN_004c1a40();
}

// FUNCTION: 0x4a76b0
void __stdcall FUN_004a76b0(Menu_004a76b0* menu, char* name)
{
    Entry_004a76b0* entries = menu->layer->entries;
    int index = FindEntry(entries, name);
    if (index != -1) {
        menu->focus = -1;
        menu->layer->field_20 = index;
        if (entries[menu->layer->field_20].type == 3) {
            DoSelect(menu, menu->layer->entries, menu->layer->field_20);
        }
    }
}
