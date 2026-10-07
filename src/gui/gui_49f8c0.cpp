// Decompiled by Opus, Haiku, Sonnet and space-bunny-free. Names are provisional.
// The first gui module (0x49f8c0 to 0x4a03f0): the GUI layout entry table
// behind a dialog's +0x18, the lookups by name, index and substring, the
// gadget name and value accessors, and the path and help text helpers.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)

struct Holder_0049f8c0;

// The 0x15b-byte GUI layout entry: entry 0 holds the entry count at +0xb6,
// the other entries hold their text there, and a control's value sits at
// +0x29.
struct Entry_0049f8c0 {
    unsigned char type;            // +0x00
    char team;                     // +0x01
    char name[0x10];               // +0x02
    char unknown_12[0x29 - 0x12];
    char value;                    // +0x29
    char unknown_2a[0x33 - 0x2a];
    char key[0xb6 - 0x33];         // +0x33, the selected entry's text
    union {
        struct {
            short count;           // +0xb6, entry 0: the entry count
            char unknown_b8[0x138 - 0xb8];
            short field_138;       // +0x138
            char unknown_13a[0x15b - 0x13a];
        };
        char text[0x15b - 0xb6];   // +0xb6, the other entries' text
    };
};

// The layer a dialog's +0x18 points at: the entry table at +4.
struct Holder_0049f8c0 {
    char unknown_0[4];
    Entry_0049f8c0* entries;       // +0x04
    char unknown_8[0x14 - 0x8];
    int field_14;                  // +0x14
    int field_18;                  // +0x18
    char unknown_1c[0x20 - 0x1c];
    int selected;                  // +0x20
};

// The GUI context at g_game's +0x519: the layer at +0x18, the current entry
// at +0x60, the focused entry at +0x64, the help text's index at +0x68, the
// 0x100-byte name buffers from +0x9b6 on, and the changed flag at +0xcca.
struct Dialog {
    char unknown_0[0x18];
    Holder_0049f8c0* holder;       // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                   // +0x60
    int focus;                     // +0x64
    int index;                     // +0x68
    int used;                      // +0x6c
    char unknown_70[0x74 - 0x70];
    int length;                    // +0x74
    char unknown_78[0xa2 - 0x78];
    int field_a2;                  // +0xa2
    char unknown_a6[0x9b6 - 0xa6];
    char path_9b6[0x100];          // +0x9b6
    char path_ab6[0x100];          // +0xab6
    char path_bb6[0x100];          // +0xbb6
    char unknown_cb6[0xcca - 0xcb6];
    int changed;                   // +0xcca
};

struct Vec6_49fd20 {
    int x;                         // +0x0
    int y;                         // +0x4
    char pad[16];                  // +0x8 .. +0x17 (unused by this function)
};

struct Obj1_49fd20 {
    char unknown_0[0x3c];
    Vec6_49fd20 data;              // +0x3c
};

struct Obj2_49fd20 {
    char unknown_0[0x13];
    short val_13;                  // +0x13
    short val_15;                  // +0x15
};

#pragma pack(pop)

void __stdcall UpdateMenu(Dialog* menu);
void __stdcall FUN_004ab0b0(void* param_1, unsigned int* param_2, int* param_3);
void FUN_004c2870();
void FlipScreen();
void __stdcall FatalError(char* path);
char* __stdcall Translate(char* text);
void __stdcall DrawButton(Dialog* param_1, int param_2);
void __stdcall FUN_004a7960(Dialog* menu, int value);

extern char DAT_005119b8[];

static inline int FindEntry(Entry_0049f8c0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x49f8c0
int __stdcall HasGadgetNamed(Dialog* obj, char* name, int unused)
{
    Entry_0049f8c0* entries = obj->holder->entries;
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: 0x49f930
void __stdcall SetGadgetName(Dialog* obj, char* name, char* text)
{
    if (obj->holder) {
        Entry_0049f8c0* entries = obj->holder->entries;
        int index = FindEntry(entries, name);
        if (index != -1)
            lstrcpynA(entries[index].name, text, 0x11);
    }
}

// FUNCTION: 0x49f9c0
void __stdcall RunWhileScreenNamed(Dialog* menu, char* name)
{
    MSG msg;
    while (1) {
        if (!menu->holder || _strnicmp(menu->holder->entries->name, name, 16) != 0)
            break;
        if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        UpdateMenu(menu);
        FUN_004ab0b0(menu->holder, 0, 0);
        FUN_004c2870();
        FlipScreen();
    }
}

// FUNCTION: 0x49fa50
void __stdcall FUN_0049fa50(Dialog* p)
{
    *(int*)((char*)p + 0xa2) = 1;
}

// FUNCTION: 0x49fa70
void __stdcall FUN_0049fa70(Dialog* obj)
{
    obj->field_a2 = 0;
}

// FUNCTION: 0x49fa90
void __stdcall FUN_0049fa90(Dialog* obj)
{
    obj->changed = 1;
}

// FUNCTION: 0x49fab0
void __stdcall FUN_0049fab0(void* param_1)
{
    *(int*)((char*)param_1 + 0xcca) = 0;
}

// FUNCTION: 0x49fad0
void __stdcall FUN_0049fad0(Dialog* obj)
{
    if (obj->holder != 0) {
        obj->holder->field_14 = 1;
    }
}

// FUNCTION: 0x49faf0
void __stdcall FUN_0049faf0(Dialog* param)
{
    param->holder->field_14 = 0;
}

// FUNCTION: 0x49fb10
void __stdcall FUN_0049fb10(int param_1, int param_2)
{
    int eax = *(int*)(param_1 + 0x18);
    if (eax != 0) {
        *(int*)(eax + 0x18) = param_2;
    }
}

// FUNCTION: 0x49fb30
int __stdcall FUN_0049fb30(Dialog* obj)
{
    if (obj->holder != 0) {
        return obj->holder->field_18;
    }
    return 0;
}

// FUNCTION: 0x49fb50
void __stdcall FUN_0049fb50(Dialog* obj, const char* dir)
{
    strncpy(obj->path_bb6, dir, 0x100);
    strcat(obj->path_bb6, "\\");
}

// FUNCTION: 0x49fba0
void __stdcall FUN_0049fba0(Dialog* obj, const char* dir)
{
    strncpy(obj->path_9b6, dir, 0x100);
    strcat(obj->path_9b6, "\\");
}

// FUNCTION: 0x49fbf0
void __stdcall FUN_0049fbf0(Dialog* obj, const char* dir)
{
    strncpy(obj->path_ab6, dir, 0x100);
    strcat(obj->path_ab6, "\\");
}

// FUNCTION: 0x49fc40
void __stdcall FUN_0049fc40(Dialog* param_1)
{
    param_1->focus = 0xffffffff;
}

// FUNCTION: 0x49fc50
int __stdcall FUN_0049fc50(Dialog* obj, int index)
{
    if (obj->focus != -1 && obj->holder->entries[obj->focus].type == 3) {
        obj->focus = -1;
    }
    if (obj->focus != -1 && obj->focus != index) {
        return 0;
    }
    obj->focus = index;
    if (index != -1 && obj->holder->entries[index].type == 3) {
        obj->length = strlen(obj->holder->entries[index].text);
    }
    return 1;
}

// FUNCTION: 0x49fcf0
int __stdcall FUN_0049fcf0(int param_1, int param_2)
{
    int result = 0;
    result = *(int*)(param_1 + 0x64) == param_2;
    return result;
}

// FUNCTION: 0x49fd10
bool __stdcall FUN_0049fd10(int param_1)
{
    return *(int*)(param_1 + 0x64) != -1;
}

// FUNCTION: 0x49fd20
void __stdcall FUN_0049fd20(Obj1_49fd20* param_1, Obj2_49fd20* param_2, Vec6_49fd20* param_3)
{
    *param_3 = param_1->data;
    param_3->x -= param_2->val_13;
    param_3->y -= param_2->val_15;
}

// FUNCTION: 0x49fd60
int __stdcall IsCurrentGadgetNamed(Dialog* gui, char* name)
{
    if (!gui->holder)
        return 0;
    if (gui->current == -1)
        return 0;
    return strcmp(gui->holder->entries[gui->current].name, name) == 0;
}

// FUNCTION: 0x49fdf0
int __stdcall FindGadgetIndex(Entry_0049f8c0* entries, char* name, int type)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x49fe60
int __stdcall FindGadgetIndexBySubstring(Entry_0049f8c0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strstr(entries[i].name, name)) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x49fed0
void __stdcall GetGadgetName(Entry_0049f8c0* entries, char* name, int index)
{
    strncpy(name, entries[index].name, 0x10);
    name[0x10] = 0;
}

// FUNCTION: 0x49ff10
Entry_0049f8c0* __stdcall FindGadgetOrNull(Entry_0049f8c0* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    return 0;
}

// FUNCTION: 0x49ff90
Entry_0049f8c0* __stdcall FindGadgetChecked(Entry_0049f8c0* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    FatalError("Error in GUI layout");
    return 0;
}

// FUNCTION: 0x4a0010
Entry_0049f8c0* __stdcall FUN_004a0010(Entry_0049f8c0* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    FatalError("Error in GUI layout");
    return 0;
}

// FUNCTION: 0x4a0090
void __stdcall UpdateHelpText(Dialog* obj)
{
    char* text = DAT_005119b8;
    if (obj->index != -1) {
        obj->used = obj->index;
        text = obj->holder->entries[obj->index].key;
    }
    int found = FindEntry(obj->holder->entries, "HELPTEXT");
    if (found != -1) {
        // re-read the entry pointer here: the original reloads it after the call
        strcpy((char*)&obj->holder->entries[found].count, Translate(text));
        obj->changed = 1;
    }
}

// FUNCTION: 0x4a0180
Entry_0049f8c0* __stdcall FUN_004a0180(Entry_0049f8c0* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    FatalError("Error in GUI layout");
    return 0;
}

// FUNCTION: 0x4a0200
Entry_0049f8c0* __stdcall FUN_004a0200(Entry_0049f8c0* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    FatalError("Error in GUI layout");
    return 0;
}

// FUNCTION: 0x4a0280
Entry_0049f8c0* __stdcall FUN_004a0280(Entry_0049f8c0* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    FatalError("Error in GUI layout");
    return 0;
}

// FUNCTION: 0x4a0300
int __stdcall IsGadgetNamed(Entry_0049f8c0* entries, int i, char* name)
{
    if (i == -1) {
        return 0;
    }
    return strncmp(entries[i].name, name, 0x10) == 0;
}

// FUNCTION: 0x4a0340
void __stdcall FUN_004a0340(Dialog* param_1, int index)
{
    Entry_0049f8c0* entries = param_1->holder->entries;
    Entry_0049f8c0* me = &entries[index];
    if (me->team != 0) {
        Entry_0049f8c0* e = &entries[1];
        for (int i = 1; i < entries->count + 1; i++, e++) {
            if (e->type == 1 && i != index && e->team == me->team
                && e->field_138 != 0) {
                e->field_138 = 0;
                DrawButton(param_1, i);
                if (param_1->holder != 0) {
                    param_1->holder->field_14 = 1;
                }
            }
        }
    }
}

// FUNCTION: 0x4a03f0
void __stdcall FUN_004a03f0(Dialog* menu, int index, int value)
{
    Entry_0049f8c0* entries = menu->holder->entries;
    int i;

    entries[index].value = value;

    if (entries[index].type == 4) {
        for (i = 0; i <= entries[0].count; i++) {
            if (entries[i].type == 1 && entries[i].team == entries[index].team) {
                entries[i].value = value;
            }
        }
    } else if (entries[index].type == 2) {
        if (value == 0) {
            char team = entries[index].team;
            int found = 0;
            i = 1;
            for (;;) {
                if (i >= entries[0].count + 1) {
                    found = 0;
                    break;
                }
                if (entries[i].type == 4 && entries[i].team == team) {
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

    if (value == 0 && index == menu->holder->selected) {
        FUN_004a7960(menu, 1);
    }
    menu->changed = 1;
}
