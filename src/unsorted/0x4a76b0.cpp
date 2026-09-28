// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Selects the menu entry named `name` (16 bytes of its name at +0x02 of the
// 0x15b-byte entry, so entry i's name is at entries + i*0x15b + 2). When the
// selected entry is type 3 it makes its group's type-7 entry current and
// refreshes its edit field. FindEntry is inlined from the same helper as
// 0x4a1810/0x4a30c0; the group loop is FUN_004a1810's body inlined.
//
// Partial, 84.9%: the original reloads layer+0x20 once into esi before the
// type==3 test and uses that one register for the check, the entry address
// and the later calls; this compiler splits the reload into eax (used by the
// check) and keeps the FindEntry result in esi (used by the entry address),
// and it does not reload `entries` (mov ebx,[ecx+4]) before the entry address.
// Every attempt to force one variable/register produced worse code.
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

int FUN_004c13f0();
void __stdcall FUN_004c13a0(int colour, int font);
void __stdcall FUN_004c1420(int id);
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

// FUNCTION: 0x4a76b0
void __stdcall FUN_004a76b0(Menu_004a76b0* menu, char* name)
{
    Entry_004a76b0* entries = menu->layer->entries;
    int index = FindEntry(entries, name);
    if (index != -1) {
        menu->focus = -1;
        menu->layer->field_20 = index;
        if (entries[menu->layer->field_20].type == 3) {
            Entry_004a76b0* entry = &entries[index];
            FUN_004c13a0(((unsigned char*)entry->field_1f)[(int)menu + 0x8b2], FUN_004c13f0());
            int n = 0;
            int i = 1;
            for (; i < entries->u.count + 1; i++) {
                if (entries[i].type == 7) {
                    if (n == entry->group) {
                        FUN_004c1420(entries[i].id);
                        break;
                    }
                    n++;
                }
            }
            if (i == entries->u.count + 1)
                FUN_004c1420(DAT_0051fba4->group);
            FUN_0049fc50(menu, index);
            menu->layer->field_20 = index;
            FUN_004ab6c0(menu, index, entry->u.text, entry->field_138, 0);
            FUN_004c1a40();
        }
    }
}
