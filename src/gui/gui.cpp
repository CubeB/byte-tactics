// Decompiled by Opus, Haiku, Sonnet, Sonnet 5.5, space-bunny-free, GPT-5.6-Terra,
// muse-spark-1.3-free, deepseek-v4.1-flash, GPT-6.1-sol, claude-opus-5-5,
// Claude Opus 5.5, deepseek-v4.1, DeepSeek V4.1 Flash, Space Bunny Free,
// Fledge Alpha Free, GPT-6, Fable 5.1, claude-sonnet-5-5, Claude Sonnet 5.5,
// GPT-6-Luna and LongCat 2.5 Preview Free.
// Names are provisional.
//
// The gui module (0x49f8c0 to 0x4aa8e0): the GUI layout entry table behind a
// dialog's +0x18, the gadget entry accessors (text, value, status, stage, rect
// and flag setters), the list gadget's drawing, scrolling and input, the GUI
// layout engine that draws and handles the gadgets of a screen's 0x15b-byte
// entry table, and the helpers that render and close screens and switch the
// current GUI context. 0x4a0880 (SetGadgetText) and 0x4a3ef0 (DrawSlider)
// keep their own files: each only matches at its own file's symbol count.
//
// <windows.h> and <math.h> are only for their symbol ids: DrawButton,
// RenderLayer and the list steps match only at this symbol count.
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ddraw.h>
#include <iostream>
#include <math.h>

struct GafEntry;

// g_guiContext is the GUI context; its +0x14 layer holds the glyph table
// (Language/Font/List in the individual file views).
struct List_004a32a0 {
    char unknown_0[0xc];
    union {
        GafEntry* glyphs;              // +0x0c
        GafEntry* field_0c;            // +0x0c
    };
};

struct Entry_004a32a0;

struct Holder_004a32a0 {
    int unknown_0;
    Entry_004a32a0* entries;           // +0x04
};

struct Root_004a32a0 {                 // g_guiContext
    union { int current; int group; int fontId; };  // +0x00
    char unknown_04[0x14 - 0x04];
    union {
        List_004a32a0* language;       // +0x14
        List_004a32a0* font;           // +0x14
        List_004a32a0* list;           // +0x14
    };
    Holder_004a32a0* holder;           // +0x18
};

extern Root_004a32a0* g_guiContext;

// The inclusive bounding rectangle the gadget helpers fill: left/top at the
// top left, right/bottom at the bottom right.
struct Rect {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};


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
void __stdcall BlitLayers(void* param_1, unsigned int* param_2, int* param_3);
void ShowSoftwareCursor();
void FlipScreen();
void __stdcall FatalError(char* path);
char* __stdcall Translate(char* text);
void __stdcall DrawButton(Dialog* param_1, int param_2);
void __stdcall SelectAdjacentGadget(Dialog* menu, int value);

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
        BlitLayers(menu->holder, 0, 0);
        ShowSoftwareCursor();
        FlipScreen();
    }
}

// FUNCTION: 0x49fa50
void __stdcall EnableKeyCommands(Dialog* p)
{
    *(int*)((char*)p + 0xa2) = 1;
}

// FUNCTION: 0x49fa70
void __stdcall DisableKeyCommands(Dialog* obj)
{
    obj->field_a2 = 0;
}

// FUNCTION: 0x49fa90
void __stdcall MarkChanged(Dialog* obj)
{
    obj->changed = 1;
}

// FUNCTION: 0x49fab0
void __stdcall ClearChanged(void* param_1)
{
    *(int*)((char*)param_1 + 0xcca) = 0;
}

// FUNCTION: 0x49fad0
void __stdcall MarkLayerChanged(Dialog* obj)
{
    if (obj->holder != 0) {
        obj->holder->field_14 = 1;
    }
}

// FUNCTION: 0x49faf0
void __stdcall ClearLayerChanged(Dialog* param)
{
    param->holder->field_14 = 0;
}

// FUNCTION: 0x49fb10
void __stdcall SetKeyboardInput(int param_1, int param_2)
{
    int eax = *(int*)(param_1 + 0x18);
    if (eax != 0) {
        *(int*)(eax + 0x18) = param_2;
    }
}

// FUNCTION: 0x49fb30
int __stdcall GetKeyboardInput(Dialog* obj)
{
    if (obj->holder != 0) {
        return obj->holder->field_18;
    }
    return 0;
}

// FUNCTION: 0x49fb50
void __stdcall SetFontsPath(Dialog* obj, const char* dir)
{
    strncpy(obj->path_bb6, dir, 0x100);
    strcat(obj->path_bb6, "\\");
}

// FUNCTION: 0x49fba0
void __stdcall SetGuiPath(Dialog* obj, const char* dir)
{
    strncpy(obj->path_9b6, dir, 0x100);
    strcat(obj->path_9b6, "\\");
}

// FUNCTION: 0x49fbf0
void __stdcall SetAnimsPath(Dialog* obj, const char* dir)
{
    strncpy(obj->path_ab6, dir, 0x100);
    strcat(obj->path_ab6, "\\");
}

// FUNCTION: 0x49fc40
void __stdcall ClearFocus(Dialog* param_1)
{
    param_1->focus = 0xffffffff;
}

// FUNCTION: 0x49fc50
int __stdcall TrySetFocus(Dialog* obj, int index)
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
int __stdcall IsFocused(int param_1, int param_2)
{
    int result = 0;
    result = *(int*)(param_1 + 0x64) == param_2;
    return result;
}

// FUNCTION: 0x49fd10
bool __stdcall HasFocus(int param_1)
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
Entry_0049f8c0* __stdcall FindGadgetChecked_B(Entry_0049f8c0* entries, char* name)
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
Entry_0049f8c0* __stdcall FindGadgetChecked_C(Entry_0049f8c0* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    FatalError("Error in GUI layout");
    return 0;
}

// FUNCTION: 0x4a0200
Entry_0049f8c0* __stdcall FindGadgetChecked_D(Entry_0049f8c0* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    FatalError("Error in GUI layout");
    return 0;
}

// FUNCTION: 0x4a0280
Entry_0049f8c0* __stdcall FindGadgetChecked_E(Entry_0049f8c0* entries, char* name)
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
void __stdcall ClearPeerStatus(Dialog* param_1, int index)
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
void __stdcall SetGadgetActive(Dialog* menu, int index, int value)
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
                SetGadgetActive(menu, found, 0);
            }
        }
    }

    if (value == 0 && index == menu->holder->selected) {
        SelectAdjacentGadget(menu, 1);
    }
    menu->changed = 1;
}


// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
struct Pad_p2_0 { int field; };

#pragma pack(push, 1)

// The 0x15b-byte GUI layout entry: entry 0 holds the entry count at +0xb6 and
// the surface at +0xbc, the other entries hold their text there, and the
// selected entry's text lines sit at +0x136.
struct Entry_004a04f0 {
    unsigned char type;                // +0x00
    unsigned char group;               // +0x01
    char name[0x10];                   // +0x02
    char unknown_12[0x13 - 0x12];
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    int flags;                         // +0x1b
    int state;                         // +0x1f
    char unknown_23[0x27 - 0x23];
    char field_27;                     // +0x27
    char group_28;                     // +0x28
    char value;                        // +0x29
    char unknown_2a[0xb6 - 0x2a];
    union {
        struct {
            short count;               // +0xb6 (entry 0 holds the entry count)
            char unknown_b8[0xbc - 0xb8];
            union {
                unsigned short flag_bc;    // +0xbc, bit 0 set by type 12
                int surface;               // +0xbc (entry 0 holds the surface)
            };
            char unknown_c0[0xd6 - 0xc0];
            int id;                        // +0xd6
            char unknown_da[0x136 - 0xda];
            unsigned char count_136;       // +0x136, the text line count
            unsigned char field_137;       // +0x137, the button stage
            short field_138;               // +0x138
            char field_13a;                // +0x13a
            char unknown_13b[0x13c - 0x13b];
            union {
                unsigned short flag;       // +0x13c, bit 0 set by type 1
                struct {
                    unsigned short flag_bit : 1;
                    unsigned short unknown_13c_1 : 15;
                };
            };
            char unknown_13e[0x148 - 0x13e];
            unsigned int flag_148;         // +0x148, bit 0 set by type 5
            char unknown_14c[0x157 - 0x14c];
            int field_157;                 // +0x157
        };
        char text[0x15b - 0xb6];       // +0xb6
    };
};

// The layer a dialog's +0x18 points at: the entry table at +4. In 0x4a0e00
// the +0 is a second layer whose entries are searched.
struct Holder_004a04f0 {
    Holder_004a04f0* unknown_0;        // +0x00
    Entry_004a04f0* entries;           // +0x04
};

// The GUI context at g_game's +0x519: the group at +0, the layer at +0x18,
// the current entry at +0x64, the text length at +0x74 and the changed flag
// at +0xcca.
struct Dialog_004a04f0 {
    int group;                         // +0x00
    char unknown_04[0x18 - 0x04];
    Holder_004a04f0* holder;           // +0x18
    char unknown_1c[0x64 - 0x1c];
    int current;                       // +0x64
    char unknown_68[0x74 - 0x68];
    int length;                        // +0x74
    char unknown_78[0xcca - 0x78];
    int changed;                       // +0xcca
};

#pragma pack(pop)

void __stdcall SetGadgetActive(Dialog_004a04f0* menu, int index, int value);
void __stdcall ClearPeerStatus(Dialog_004a04f0* obj, int index);
char* __stdcall Translate(char* text);
void __stdcall FatalError(char* path);
void __stdcall DrawLitRectangle(int surface, Rect* rect, int level);
void __stdcall SetFont(int id);
int __cdecl tolower(int c);


static inline int FindEntry(Entry_004a04f0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

static inline Entry_004a04f0* GetEntry(Dialog_004a04f0* obj, int index)
{
    return obj->holder->entries + index;
}

static inline Entry_004a04f0* GetEntries(Dialog_004a04f0* obj)
{
    return obj->holder->entries;
}

static inline char* GetData(Dialog_004a04f0* obj)
{
    return (char*)obj->holder->entries;
}

// FUNCTION: 0x4a04f0
char __stdcall GetGadgetActiveByName(Dialog_004a04f0* obj, char* name)
{
    Entry_004a04f0* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i == -1) {
        return -1;
    }
    return entries[i].value;
}

// FUNCTION: 0x4a0570
void __stdcall SetGadgetActiveByName(Dialog_004a04f0* obj, char* name, int param_3)
{
    if (obj->holder) {
        int index = FindEntry(obj->holder->entries, name);
        if (index != -1)
            SetGadgetActive(obj, index, param_3);
    }
}

// The two redundant type re-tests (cmp dl,dl at 0x4a064c and cmp dl,1 at 0x4a0679)
// come from separate if statements that re-test entry->type; the tolower calls
// are compared directly, with no named temporaries.
// FUNCTION: 0x4a05e0
void __stdcall AssignQuickKey(Dialog_004a04f0* obj, int index)
{
    Entry_004a04f0* entries;
    Entry_004a04f0* scan;
    char* text;
    int length;
    Entry_004a04f0* entry;
    int i;
    int j;

    if (index == -1)
        return;

    entries = obj->holder->entries;
    entry = entries + index;
    if (entry->type == 1) {
        if ((entry->flags & 0x10000) != 0)
            return;
    }
    if (entry->type == 5 && strlen(&entry->text[0x136 - 0xb6]) == 0)
        return;
    if (entry->type != 5 && entry->type != 1)
        return;

    if (entry->type == 1 && entry->text[0x136 - 0xb6] != 0) {
        entry->text[0x13a - 0xb6] = 0;
        return;
    }
    if (entry->type == 1) {
        if (strlen(entry->text) == 0)
            return;
        entry->text[0x13a - 0xb6] = 0;
        text = entry->text;
    } else if (entry->type == 5) {
        text = entry->text;
        entry->text[0x147 - 0xb6] = 0;
    }

    length = strlen(text);
    if (length == 0)
        return;

    for (i = 0; i < length; i++) {
        if (text[i] != ' ') {
            for (j = 0; j <= entries->count; j++) {
                scan = entries + j;
                if (scan->type == 1) {
                    if (tolower((signed char)scan->text[0x13a - 0xb6]) == tolower((signed char)text[i]))
                        break;
                } else if (scan->type == 5) {
                    if (tolower((signed char)scan->text[0x147 - 0xb6]) == tolower((signed char)text[i]))
                        break;
                }
            }
            if (j > entries->count) {
                if (entry->type == 1) {
                    entry->text[0x13a - 0xb6] = text[i];
                    return;
                }
                if (entry->type == 5) {
                    entry->text[0x147 - 0xb6] = text[i];
                    return;
                }
                return;
            }
        }
    }
}

// FUNCTION: 0x4a07d0
void __stdcall SetGadgetTextByName(Dialog_004a04f0* obj, char* name, char* text)
{
    if (obj->holder) {
        Entry_004a04f0* entries = obj->holder->entries;
        int index = FindEntry(entries, name);
        if (index != -1) {
            strncpy(entries[index].text, text, 0x80);
            obj->changed = 1;
            AssignQuickKey(obj, index);
        }
    }
}

// 0x4a0880 keeps its own file: the joined file's include set and prelude
// put the entries pointer in eax instead of ecx.
struct Object_004a0880;
void __stdcall SetGadgetText(Object_004a0880* obj, int index, char* text);

// FUNCTION: 0x4a08f0
void __stdcall TranslateGadgetItems(Dialog_004a04f0* obj, int index)
{
    Entry_004a04f0* entry = GetEntry(obj, index);
    char temp[0x80];
    char* dst = temp;
    char* src = entry->text;
    int i = 0;
    while (i < entry->count_136) {
        strcpy(dst, Translate(src));
        dst += strlen(dst) + 1;
        src += strlen(src) + 1;
        i++;
    }
    memcpy(entry->text, temp, 0x80);
}

// FUNCTION: 0x4a09c0
void __stdcall SetTranslatedText(Dialog_004a04f0* context, int index, char* source, int value)
{
    if (index == -1 || context->holder == 0)
        return;
    Entry_004a04f0* entries = context->holder->entries;
    char* text = Translate(source);

    switch (entries[index].type) {
    case 5:
        strncpy(entries[index].text, text, 0x80);
        if (entries[index].count_136 != 0)
            AssignQuickKey(context, index);
        break;
    case 3:
        if (text != 0) {
            strcpy(context->holder->entries[index].text, text);
            if (context->current == index)
                context->length = strlen(text);
        }
        if (value != 0)
            entries[index].field_138 = (short)value;
        break;
    case 1:
        strncpy(entries[index].text, text, 0x80);
        AssignQuickKey(context, index);
        if (entries[index].count_136 != 0) {
            char* p = entries[index].text;
            while (*p != 0) {
                if (*p == '|')
                    *p = 0;
                p++;
            }
            Entry_004a04f0* entry = &context->holder->entries[index];
            char temp[0x80];
            char* dst = temp;
            char* src = entry->text;
            int i = 0;
            while (i < entry->count_136) {
                strcpy(dst, Translate(src));
                dst += strlen(dst) + 1;
                src += strlen(src) + 1;
                i++;
            }
            memcpy(entry->text, temp, 0x80);
        }
        break;
    }
    context->changed = 1;
}

// FUNCTION: 0x4a0bf0
void __stdcall SetTranslatedTextByName(Dialog_004a04f0* obj, char* name, char* param_3, int param_4)
{
    if (obj->holder != 0) {
        int index = FindEntry(obj->holder->entries, name);
        if (index != -1) {
            SetTranslatedText(obj, index, param_3, param_4);
        }
    }
}

// Finds a gadget by name (as in 0x4a0570), stores a value into it and marks
// the object as changed.
// FUNCTION: 0x4a0c70
void __stdcall SetGadgetColorByName(Dialog_004a04f0* obj, char* name, int value)
{
    if (obj->holder) {
        Entry_004a04f0* entries = obj->holder->entries;
        int index = FindEntry(entries, name);
        if (index != -1) {
            entries[index].state = value;
            obj->changed = 1;
        }
    }
}

// Looks a gadget entry up by name (the FindEntry of 0x4a0c70 and 0x4a0bf0) and
// returns the text at entry + 0xb6, but only for entry types 1, 3 and 5 (the
// type byte at +0, read through a switch). With a non-zero third argument the
// text is copied there as well; the return value is the text either way, 0 when
// the entry is missing or of another type.
// FUNCTION: 0x4a0d00
char* __stdcall GetGadgetText(Dialog_004a04f0* obj, char* name, char* buf)
{
    char* desc = 0;
    if (obj->holder == 0) {
        FatalError("Internal error");
    }
    Entry_004a04f0* entries = obj->holder->entries;
    int index = FindEntry(entries, name);
    if (index != -1) {
        switch (entries[index].type) {
        case 1:
        case 3:
        case 5:
            desc = entries[index].text;
            break;
        }
        if (desc != 0 && buf != 0) {
            strcpy(buf, desc);
        }
    }
    // Single return at the end: keeps the local in memory.
    return desc;
}

// Sets the text of the GUI entry named `name`: entries of type 1 and 5 take
// strncpy of 0x80 bytes, type 3 a plain strcpy, and for type 3, when the entry
// is the current one, the text length is stored. Then the list is marked
// changed. The entry is looked up in `holder->unknown_0->entries` but written
// through `holder->entries` for type 3, the two chains the machine code takes.
//
// Suspected original bug: the lookup uses `holder->unknown_0->entries` while
// the type 3 write goes to `holder->entries[index]`, so the text can be stored
// into a different array than the one that was searched. The function itself
// walks both chains (the two-level one at the top, the three-level one inside
// case 3), which is the evidence.
// FUNCTION: 0x4a0e00
void __stdcall SetControlTextByName(Dialog_004a04f0* obj, char* name, char* text)
{
    // Loaded before the null test on purpose: the original loads the entries
    // pointer before the `je`.
    Entry_004a04f0* entries = obj->holder->unknown_0->entries;
    // Guards are early returns, not nested ifs.
    if (obj->holder->unknown_0 == 0)
        return;
    int index = FindEntry(entries, name);
    if (index == -1)
        return;
    switch (entries[index].type) {
    case 3:
        strcpy(obj->holder->entries[index].text, text);
        if (obj->current == index)
            obj->length = strlen(text);
        break;
    case 1:
        strncpy(entries[index].text, text, 0x80);
        break;
    case 5:
        strncpy(entries[index].text, text, 0x80);
        break;
    }
    obj->changed = 1;
}

// FUNCTION: 0x4a0f30
int __stdcall GetGadgetStatus(Dialog_004a04f0* obj, int index)
{
    return GetEntries(obj)[index].field_138;
}

// Looks up the gadget entry by name (the lookup of 0x49fdf0, inlined) and
// returns its field 0x137 when the entry's state is 1, otherwise -1 (compare
// 0x4a0ff0).
// FUNCTION: 0x4a0f60
int __stdcall GetButtonStageByName(Dialog_004a04f0* obj, char* name)
{
    Entry_004a04f0* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i != -1 && entries[i].type == 1) {
        return entries[i].field_137;
    }
    return -1;
}

// Returns field 0x137 of entry `index` (0x15b-byte entries) when that entry's
// state is 1, otherwise -1. The entries pointer is loaded into a local before
// the index multiply; indexing through the full chain loads it afterwards.
// FUNCTION: 0x4a0ff0
int __stdcall GetButtonStage(Dialog_004a04f0* obj, int index)
{
    Entry_004a04f0* entries = obj->holder->entries;
    if (entries[index].type == 1) {
        return entries[index].field_137;
    }
    return -1;
}

// FUNCTION: 0x4a1030
int __stdcall SetButtonStage(Dialog_004a04f0* obj, int index, char value)
{
    Entry_004a04f0* entries = obj->holder->entries;
    if (entries[index].type == 1) {
        entries[index].field_137 = value;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4a1080
int __stdcall SetButtonStageByName(Dialog_004a04f0* obj, char* name, char value)
{
    Entry_004a04f0* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i != -1) {
        entries[i].field_137 = value;
        return 1;
    }
    return 0;
}

// Finds a gadget entry by name (the lookup of 0x4a1080, inlined), stores the
// given value in the entry's field 0x138, flags the list as changed and, when
// the value is not zero, tells the list to lay the entry out (0x4a0340).
// FUNCTION: 0x4a1110
int __stdcall SetGadgetStatusByName(Dialog_004a04f0* obj, char* name, int value)
{
    Entry_004a04f0* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i != -1) {
        entries[i].field_138 = value;
        obj->changed = 1;
        if (value) {
            ClearPeerStatus(obj, i);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4a11c0
void __stdcall SetGadgetStatus(Dialog_004a04f0* param_1, int param_2, short param_3)
{
    *(short*)(GetData(param_1) + param_2 * 347 + 0x138) = param_3;
    ClearPeerStatus(param_1, param_2);
}

// FUNCTION: 0x4a1200
void __stdcall SetGrayedOut(Dialog_004a04f0* obj, int index, int value)
{
    GetEntries(obj)[index].flag_bit = value;
    obj->changed = 1;
}

// Finds a gadget by name and sets its flag bit (see 0x4a1080 and 0x4a1200).
// FUNCTION: 0x4a1250
void __stdcall SetGrayedOutByName(Dialog_004a04f0* obj, char* name, int value)
{
    Entry_004a04f0* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i != -1) {
        entries[i].flag_bit = value;
    }
}

// Sets a flag of the layout entry `index` (0x15b-byte entries, entry 0 holds
// the count at +0xb6). Type 4 sets field 0x157 and then copies the flag to
// every type-1 entry with the same byte at +0x01 whose flags word has both
// bits 0x1800. The other types write one flag bit. The `or`/`and` in type 2
// and the `& 1` merges come from the source's explicit masks.
// FUNCTION: 0x4a12e0
void __stdcall SetGadgetGrayedOut(Dialog_004a04f0* obj, int index, int value)
{
    Entry_004a04f0* entries = obj->holder->entries;
    if (index == -1) {
        return;
    }
    Entry_004a04f0* e = &entries[index];
    switch (e->type) {
    case 4: {
        e->field_157 = value;
        unsigned char group = e->group;
        for (int i = 1; i < entries->count + 1; i++) {
            if (entries[i].type == 1 && entries[i].group == group && (entries[i].flags & 0x1800)) {
                entries[i].flag = (entries[i].flag & 0xfffe) | (value & 1);
            }
        }
        break;
    }
    case 1:
        e->flag = (e->flag & 0xfffe) | (value & 1);
        break;
    case 2:
        if (value) {
            e->flags |= 0x100;
        } else {
            e->flags &= 0xfffffeff;
        }
        break;
    case 5:
        e->flag_148 = (e->flag_148 & 0xfffffffe) | (value & 1);
        break;
    case 12:
        e->flag_bc = (e->flag_bc & 0xfffe) | (value & 1);
        break;
    default:
        break;
    }
}

// Looks up a menu entry by name and, when found, passes its index to
// SetGadgetGrayedOut; byte-identical to 0x4a14c0.
// FUNCTION: 0x4a1450
void __stdcall SetGadgetGrayedOutByName(Dialog_004a04f0* obj, char* name, int param_3)
{
    int index = FindEntry(obj->holder->entries, name);
    if (index != -1)
        SetGadgetGrayedOut(obj, index, param_3);
}

// Looks up a menu entry by name and, when found, passes its index to
// SetGadgetGrayedOut; compare 0x4a0570 and 0x4a1530.
// FUNCTION: 0x4a14c0
void __stdcall FUN_004a14c0(Dialog_004a04f0* obj, char* name, int param_3)
{
    int index = FindEntry(obj->holder->entries, name);
    if (index != -1)
        SetGadgetGrayedOut(obj, index, param_3);
}

// FUNCTION: 0x4a1530
void __stdcall SetQuickKeyByName(Dialog_004a04f0* obj, char* name, char value)
{
    int i = FindEntry(obj->holder->entries, name);
    obj->holder->entries[i].field_13a = value;
    obj->changed = 1;
}

// FUNCTION: 0x4a15c0
void __stdcall GetGadgetRectByIndex(char* param_1, int param_2, Rect* param_3)
{
    char* e = param_1 + param_2 * 0x15b;
    if (*e == 0) {
        param_3->left = 0;
        param_3->top = 0;
    } else {
        param_3->left = *(short*)(e + 0x13);
        param_3->top = *(short*)(e + 0x15);
    }
    param_3->right = *(short*)(e + 0x17) - 1 + param_3->left;
    param_3->bottom = *(short*)(e + 0x19) - 1 + param_3->top;
}

// Fills the bounding rectangle of a gadget entry: the entry's position and
// size when it is active, or a degenerate rectangle at the origin otherwise.
// FUNCTION: 0x4a1630
void __stdcall GetGadgetRect(Entry_004a04f0* entry, Rect* rect)
{
    if (entry->type == 0) {
        rect->left = 0;
        rect->top = 0;
    } else {
        rect->left = entry->x;
        rect->top = entry->y;
    }
    rect->right = entry->width + rect->left - 1;
    rect->bottom = entry->height + rect->top - 1;
}

// FUNCTION: 0x4a1680
void __stdcall GetGadgetScreenRect(char* param_1, int param_2, Rect* param_3)
{
    char* e = param_1 + param_2 * 0x15b;
    param_3->left = *(short*)(e + 0x13);
    param_3->top = *(short*)(e + 0x15);
    if (*e != 0) {
        param_3->left += *(short*)(param_1 + 0x13);
        param_3->top += *(short*)(param_1 + 0x15);
    }
    param_3->right = *(short*)(e + 0x17) - 1 + param_3->left;
    param_3->bottom = *(short*)(e + 0x19) - 1 + param_3->top;
}

// Marks GUI entry `index` as the selected one and outlines it: every entry of
// type 3 is cleared first, then the selected one is given state 30 and, unless
// its type rules it out, a box is drawn around it in six shrinking steps.
// FUNCTION: 0x4a16f0
void __stdcall DrawGadgetSelectionBox(Dialog_004a04f0* obj, int index, int param_3)
{
    Entry_004a04f0* entries = obj->holder->entries;
    int i;
    // Declared up here, used only at the bottom: in this scope MSVC keeps the
    // entry field loads in source order instead of hoisting them together.
    Rect rect;
    int level;
    int step;
    obj->changed = 1;
    for (i = 1; i <= entries->count; i++) {
        if (entries[i].type == 3) {
            entries[i].state = 0;
        }
    }
    if (entries[index].type == 3) {
        entries[index].state = 0x1e;
        return;
    }
    if (entries[index].type == 5) {
        return;
    }
    if (entries[index].type == 2) {
        return;
    }
    {
        Entry_004a04f0* e = &entries[index];
        if (e->type == 0) {
            rect.left = 0;
            rect.top = 0;
        } else {
            rect.left = e->x;
            rect.top = e->y;
        }
        rect.right = e->width + rect.left - 1;
        rect.bottom = e->height + rect.top - 1;
        level = 0x1f;
        for (step = 0; step < 6; step++) {
            rect.left--;
            rect.top--;
            rect.right++;
            rect.bottom++;
            DrawLitRectangle(obj->holder->entries->surface, &rect, level);
            level += -3 - step;
        }
    }
}

// Selects the entry of type 7 whose group number (the number of type-7 entries
// before it) matches the group of entry `index`, and makes that entry's id the
// current one. Returns the entry's number, or -1 when there is no such entry.
// FUNCTION: 0x4a1810
int __stdcall SelectFontForEntry(Entry_004a04f0* entries, int index)
{
    int n = 0;
    int i = 1;
    for (; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entries[index].group_28) {
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

// Counts the type-8 entries up to the one numbered by entries[index].field_27.
// Both paths return 0 in the original, although the caller tests the result.
// FUNCTION: 0x4a18c0
int __stdcall FindBackgroundCell(Entry_004a04f0* entries, int index)
{
    int n = 0;
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 8) {
            if (n == entries[index].field_27) {
                return 0;
            }
            n++;
        }
    }
    return 0;
}

// FUNCTION: 0x4a1920
int __stdcall IsPointInRect(Rect* r, int px, int py)
{
    if (px >= r->left && px <= r->right && py >= r->top && py <= r->bottom) {
        return 1;
    }
    return 0;
}

struct Obj_004a1950 {
    char unknown_0[4];
    int field_4;
};

// FUNCTION: 0x4a1950
int __stdcall FUN_004a1950(Obj_004a1950* param_1, int param_2) {
    return param_2 < param_1->field_4;
}

struct Struct_004a1970 {
    char unknown_0[0xc];
    int field_c;                       // +0xc
};

// FUNCTION: 0x4a1970
int __stdcall FUN_004a1970(Struct_004a1970* obj, int value)
{
    return value > obj->field_c;
}


#pragma pack(push, 1)
struct Entry_004a1990 {
    unsigned char type;                // +0x0
    unsigned char id;                  // +0x1
    char unknown_2[0xb6 - 0x2];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// FUNCTION: 0x4a1990
int __stdcall FindAssocListBox(Entry_004a1990* entries, int index)
{
    unsigned char id = entries[index].id;
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 2 && entries[i].id == id) {
            return i;
        }
    }
    return 0;
}
#pragma pack(push, 1)
struct Entry_004a19f0 {
    unsigned char type;                // +0x0
    unsigned char id;                  // +0x1
    char unknown_2[0xb6 - 0x2];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// FUNCTION: 0x4a19f0
int __stdcall FindAssocSlider(Entry_004a19f0* entries, int index)
{
    unsigned char id = entries[index].id;
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 4 && entries[i].id == id) {
            return i;
        }
    }
    return 0;
}
#pragma pack(push, 1)
struct Entry_004a1a50 {
    unsigned char type;                // +0x0
    unsigned char id;                  // +0x1
    char unknown_2[0xb6 - 0x2];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// FUNCTION: 0x4a1a50
int __stdcall FindAssocTextInput(Entry_004a1a50* entries, int index)
{
    unsigned char id = entries[index].id;
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 3 && entries[i].id == id) {
            return i;
        }
    }
    return 0;
}
// Redraws the rectangle of GUI entry `index` on the list's surface (entry 0
// holds the surface at +0xbc); an entry of type 0 is placed at (0, 0).

struct Surface_004a1ab0;

#pragma pack(push, 1)
struct Entry_004a1ab0 {                // 0x15b bytes
    unsigned char type;                // +0x0
    char unknown_1[0x13 - 0x1];
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    char unknown_1b[0xbc - 0x1b];
    Surface_004a1ab0* surface;         // +0xbc (only meaningful in entry 0)
    char unknown_c0[0x15b - 0xc0];
};
#pragma pack(pop)

struct Holder_004a1ab0 {
    char unknown_0[4];
    Entry_004a1ab0* entries;           // +0x4
};

struct Dialog_4a1ab0 {
    char unknown_0[0x18];
    Holder_004a1ab0* holder;           // +0x18
};

struct Rect_004a1ab0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

void __stdcall GrayRectangle(Surface_004a1ab0* dst, Rect_004a1ab0* rect);
void __stdcall FadeRectangle(Surface_004a1ab0* dst, Rect_004a1ab0* rect, int level);

// FUNCTION: 0x4a1ab0
void __stdcall RedrawGadgetRect(Dialog_4a1ab0* obj, int index)
{
    Entry_004a1ab0* entries = obj->holder->entries;
    Entry_004a1ab0* e = &entries[index];
    Rect_004a1ab0 rect;
    if (e->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = e->x;
        rect.top = e->y;
    }
    rect.right = e->width + rect.left - 1;
    rect.bottom = e->height + rect.top - 1;
    GrayRectangle(entries->surface, &rect);
    FadeRectangle(entries->surface, &rect, -0x14);
}
// Draws one entry of a list gadget: the frame, then either the text rows
// (flags 0x10) or the cell rows (flags 0x20/0x80).
//
// Known original quirks kept as they are (docs/bugs.md): a selected cell row
// reads cell->width/height even when the cell pointer is null (0x4a2233,
// 0x4a224c), and both arms of `holder->field_20 == index` draw with 0x1e.
// Needed next to <stdio.h>; <windows.h> instead is worse.

#pragma pack(push, 1)

struct Entry_004a1b40 {                 // 0x15b bytes
    unsigned char type;                 // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    int colours;                        // +0x1f
    char unknown_23[0x28 - 0x23];
    char tab;                           // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                        // +0xb6 (entry 0)
    short unknown_b8;
    short field_ba;                     // +0xba
    union {
        void* surface;                  // +0xbc (entry 0)
        short field_bc;                 // +0xbc
    };
    short field_c0;                     // +0xc0
    char* text;                         // +0xc2
    int field_c6;                       // +0xc6
    char unknown_ca[0xd6 - 0xca];
    char* field_d6;                     // +0xd6
    short field_da;                     // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Holder_004a1b40 {
    char unknown_00[4];
    Entry_004a1b40* entries;            // +0x04
    char unknown_08[0x10 - 0x08];
    int field_10;                       // +0x10
    int field_14;                       // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                       // +0x20
    void* surface;                      // +0x24
};

struct Dialog_4a1b40 {
    char unknown_00[0x18];
    Holder_004a1b40* holder;            // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colour[0x8be - 0x8b2]; // +0x8b2
    unsigned char colour_8be;           // +0x8be
    char unknown_8bf[0xcd2 - 0x8bf];
    void* fallback;                     // +0xcd2
};

struct Glyph_004a1b40 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
};

struct Language_004a1b40 {
    char unknown_0[0xc];
    unsigned short* glyphs;             // +0xc
};

struct LanguageRoot_004a1b40 {
    int current;                        // +0x0
    char unknown_04[0x14 - 0x04];
    Language_004a1b40* language;        // +0x14
};

struct Rect_004a1b40 {
    int left;
    int top;
    int right;
    int bottom;
};

struct Point_004a1b40 { int x; int y; };
struct Quad_004a1b40 { Point_004a1b40 points[4]; };

struct Cell_004a1b40 {
    unsigned short width;               // +0x00
    unsigned short height;              // +0x02
    char unknown_04[0x10 - 0x04];
    int field_10;                       // +0x10
    char unknown_14[0x18 - 0x14];
};

struct Item_004a1b40 {
    char unknown_0[0x28];
    Cell_004a1b40* cell;                // +0x28
};

struct Surface {
    void GetClipRect(Rect_004a1b40* rect);
    void SetClipRect(Rect_004a1b40 rect);
};

#pragma pack(pop)

void __stdcall DrawListboxFrame(Dialog_4a1b40* obj, int index, void* bmp);
void __stdcall CopySurfaceRect(void* dst, void* src, Rect_004a1b40* rect, Rect_004a1b40* pos);
void __stdcall SetFont(int id);
int GetFont();
int __stdcall GetTextWidth(int font, char* text);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall SetTextColors(int colour, int font);
int GetTextKeyColor();
char* __stdcall SkipTextLines(char* text, int line);
void __stdcall DrawTextClipped(void* surface, char* text, int x, int y, int maxw,
                            int style);
int __stdcall DrawWrappedText(void* surface, char* text, int x, int y, int maxw,
                            int rem, int style);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2,
                            int colour);
void __stdcall FadeRectangle(void* surface, Rect_004a1b40* rect, int id);
void __stdcall DrawFrameQuad(void* surface, void* bitmap, Quad_004a1b40* dst,
                            Quad_004a1b40* src);

// Must stay an inlined helper: the glyph width is measured through it.
static inline int Measure_004a1b40(char* text)
{
    int width = 0;
    if (0 == text)
        return 0;
    if (!g_guiContext->language)
        return GetTextWidth(GetFont(), text);
    char* q = text;
    while (*q) {
        char ch = *q;
        Glyph_004a1b40* glyph = (Glyph_004a1b40*)GetGafFrame(
            g_guiContext->language->glyphs, (unsigned char)ch);
        if (0 != glyph)
            width += glyph->width;
        q++;
    }
    return width;
}

static inline int LineHeight_004a1b40()
{
    if (0 == g_guiContext->language)
        return GetFontHeight();
    return ((Glyph_004a1b40*)GetGafFrame(g_guiContext->language->glyphs, 0x49))->height + 2;
}

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
struct Pad_a1b40_0 { int field; };
struct Pad_a1b40_1 { int field; };
struct Pad_a1b40_2 { int field; };
struct Pad_a1b40_3 { int field; };
struct Pad_a1b40_4 { int field; };
struct Pad_a1b40_5 { int field; };
struct Pad_a1b40_6 { int field; };
struct Pad_a1b40_7 { int field; };
struct Pad_a1b40_8 { int field; };
struct Pad_a1b40_9 { int field; };
struct Pad_a1b40_10 { int field; };
struct Pad_a1b40_11 { int field; };
struct Pad_a1b40_12 { int field; };
struct Pad_a1b40_13 { int field; };
struct Pad_a1b40_14 { int field; };
struct Pad_a1b40_15 { int field; };
struct Pad_a1b40_16 { int field; };
extern int Pad_a1b40_e0;
extern int Pad_a1b40_e1;
extern int Pad_a1b40_e2;
extern int Pad_a1b40_e3;

// FUNCTION: 0x4a1b40
void __stdcall DrawListBox(Dialog_4a1b40* obj, int index)
{
    unsigned char font;
    int yoff;
    int xx;
    Rect_004a1b40 bounds;
    int xw;
    int flag = 0;
    if (0 != obj->holder)
        obj->holder->field_14 = 1;
    Holder_004a1b40* holder = obj->holder;
    Entry_004a1b40* entries;
    entries = obj->holder->entries;
    Entry_004a1b40* me = &entries[index];
    int h = me->h;
    void* surface;
    GetGadgetRect((Entry_004a04f0*)&entries[index], (Rect*)&bounds);
    surface = holder->surface;
    if (surface == 0)
        surface = obj->fallback;
    if (surface == 0 && !(holder->field_10 & 0x80))
        DrawListboxFrame(obj, index, surface);
    else if (surface != 0)
        CopySurfaceRect(entries->surface, surface, &bounds, &bounds);
    int lh = LineHeight_004a1b40();
    int step;
    if (me->field_da == 0)
        step = lh + 1;
    else
        step = me->field_da;
    unsigned int flags;
    flags = me->flags;
    if ((flags & 0x10) && me->text && 0 != me->field_c0) {
        // Separate from cellRect: one shared rect changes the spill homes.
        Rect_004a1b40 rowRect;
        int i;
        int t = 0;
        // Loop test and not-found test both use count + 1.
        for (i = 1; i < entries->count + 1; i++) {
            if (7 == entries[i].type) {
                if (t == me->tab) {
                    SetFont((int)entries[i].field_d6);
                    break;
                }
                t++;
            }
        }
        if (i == entries->count + 1) { SetFont(g_guiContext->current); }
        GetFont();
        font = GetTextKeyColor();
        char* q = SkipTextLines(me->text, me->field_bc);
        int line = 0;
        int y = me->field_bc;
        yoff = 0;
        while (1) {
            rowRect.left = 2 + bounds.left;
            rowRect.right = me->w + rowRect.left - 2;
            rowRect.top = bounds.top + yoff + 2;
            rowRect.bottom = rowRect.top + step;
            int w = Measure_004a1b40(q);
            int col = obj->colour[me->colours];
            if (me->field_d6 == 0 || me->field_d6[y] != 1) {
                if (*q == '&') {
                    if (q[1] == 'G')
                        flag = 1;
                    q += 2;
                }
            } else {
                flag = 1;
            }
            int ty = rowRect.top;
            int f = me->flags;
            if ((f & 1) != 0) {
                xx = rowRect.left;
                xw = rowRect.right - rowRect.left + 1;
            } else if ((4 & f) != 0) {
                xw = w;
                xx = rowRect.right - w;
            } else if (2 & f) {
                xx = (rowRect.left + rowRect.right - w) / 2;
                if (xx < rowRect.left)
                    xx = rowRect.left;
                xw = rowRect.right - xx + 1;
            }
            if (me->field_da > 6 + LineHeight_004a1b40())
                DrawWrappedText(entries->surface, q, xx, ty, xw, bounds.bottom - bounds.top, 0);
            else
                DrawTextClipped(entries->surface, q, xx, ty, xw, 0);
            q = SkipTextLines(q, 1);
            if (flag) {
                flag = 0;
                FadeRectangle(entries->surface, &rowRect, -0x13);
                FadeRectangle(entries->surface, &rowRect, -0x14);
                FadeRectangle(entries->surface, &rowRect, -0x15);
                FadeRectangle(entries->surface, &rowRect, -0x16);
            } else if (!(me->flags & 0x100) && me->field_ba == line + me->field_bc && me->field_c0) {
                if (obj->holder->field_20 == index)
                    FadeRectangle(entries->surface, &rowRect, 0x1e);
                else
                    FadeRectangle(entries->surface, &rowRect, 0x1e);
            } else {
                SetTextColors(col, font);
            }
            line += 1;
            yoff += step;
            ++y;
            h -= step;
            // Tests h >= lh first.
            if (h >= lh) {
                if (line + me->field_bc >= me->field_c0)
                    return;
            } else {
                break;
            }
        }
    } else if (flags & 0xa0) {
        Item_004a1b40** colPtr;
        Cell_004a1b40* cellPtr;
        Rect_004a1b40 cellRect;
        Rect_004a1b40 clip;
        unsigned int bp = (flags >> 7) & 1;
        void* surf = entries->surface;
        ((Surface*)surf)->GetClipRect(&clip);
        ((Surface*)surf)->SetClipRect(bounds);
        int k = me->field_bc;
        if (!bp) {
            colPtr = &((Item_004a1b40**)me->field_c6)[k];
        } else {
            // colPtr is zeroed only in this arm.
            colPtr = 0;
            cellPtr = &((Cell_004a1b40*)me->field_c6)[k];
        }
        int yy = bounds.top + 2;
        bounds.left += 2;
        int y = step + yy;
        for (;;) {
            Cell_004a1b40* cell;
            if (0 != bp) {
                cell = cellPtr;
                cellPtr++;
            } else {
                cell = (*colPtr)->cell;
            }
            if (cell != 0 && cell->field_10 != 0) {
                Quad_004a1b40 dst;
                Quad_004a1b40 src;
                dst.points[3].x = bounds.left;
                src.points[0].x = 1;
                src.points[0].y = 1;
                src.points[3].x = 1;
                src.points[1].y = 1;
                dst.points[0].x = bounds.left;
                dst.points[1].x = bounds.right;
                dst.points[2].x = bounds.right;
                dst.points[3].y = y - 1;
                dst.points[2].y = y - 1;
                src.points[1].x = cell->width - 1;
                src.points[2].x = cell->width - 1;
                dst.points[1].y = yy;
                dst.points[0].y = yy;
                src.points[2].y = cell->height - 1;
                src.points[3].y = cell->height - 1;
                DrawFrameQuad(surf, cell, &dst, &src);
                // Stores go x pair, then y pair.
                cellRect.left = dst.points[0].x;
                cellRect.right = dst.points[1].x;
                cellRect.top = dst.points[0].y;
                cellRect.bottom = dst.points[2].y;
                unsigned char v = me->field_d6[k];
                if (1 & v) {
                    FadeRectangle(surf, &cellRect, -0x14);
                } else if ((2 & v) != 0) {
                    DrawLine(surf, cellRect.left + 1, cellRect.bottom - 1, cellRect.right - 2, 1 + cellRect.top, obj->colour_8be);
                    DrawLine(surf, 2 + cellRect.left, cellRect.bottom - 1, cellRect.right - 1, cellRect.top + 1, obj->colour_8be);
                    DrawLine(surf, 1 + cellRect.left, cellRect.top + 2, cellRect.right - 1, cellRect.bottom - 2, obj->colour_8be);
                    DrawLine(surf, cellRect.left + 2, cellRect.top + 2, cellRect.right - 2, cellRect.bottom - 2, obj->colour_8be);
                }
            }
            // A selected row reads cell->width/height even when cell is null
            // (docs/bugs.md).
            if (!(me->flags & 0x100) && me->field_ba == k) {
                Rect_004a1b40 hl;
                hl.left = bounds.left;
                hl.top = yy;
                hl.right = bounds.left + cell->width - 1;
                hl.bottom = yy + cell->height - 1;
                FadeRectangle(surf, &hl, 0x14);
            }
            // k increments before the colPtr step.
            k++;
            if (!bp)
                colPtr++;
            yy += step;
            y += step;
            if (yy >= bounds.bottom || k >= me->field_c0)
                break;
        }
        ((Surface*)surf)->SetClipRect(clip);
    }
}
#pragma pack(push, 1)
struct Entry_004a23b0 {
    char unknown_0[0x13];
    short x;                    // +0x13
    short y;                    // +0x15
    short w;                    // +0x17
    short h;                    // +0x19
    unsigned char flags;        // +0x1b
    char unknown_1c[0x140 - 0x1c];
    short off;                  // +0x140
    short size;                 // +0x142
    char unknown_144[0x15b - 0x144];
};
#pragma pack(pop)

// FUNCTION: 0x4a23b0
void __stdcall GetSliderRects(Entry_004a23b0* base, int index, int* r1, int* r2)
{
    Entry_004a23b0* e = base + index;
    r1[0] = e->x;
    r1[1] = e->y;
    r1[2] = r1[0] + e->w;
    r1[3] = r1[1] + e->h;
    if (e->flags & 1) {
        int left = r1[0] + e->off + 1;
        r2[0] = left;
        r2[1] = r1[1] + 1;
        r2[2] = left + e->size;
        r2[3] = r2[1] + e->h - 2;
    } else {
        r2[0] = r1[0] + 1;
        r2[1] = r1[1] + e->off + 2;
        r2[2] = r2[0] + e->w - 2;
        r2[3] = r2[1] + e->size;
    }
}
// Draws one gadget entry's three glyphs (start, repeated middle, end) across
// the span [x, x + w] on the surface of the entry table. Same entry table as
// 0x4a23b0 (fields x/y/w/h at 0x13..0x19) and 0x4a0f30 (holder at +0x18).
// Needed only for compiler state: changes how the loop test's sum is formed.

struct Glyph_004a2480 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

#pragma pack(push, 1)
struct Entry_004a2480 {
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    char unknown_1b[0xbc - 0x1b];
    void* surface;                     // +0xbc
    char unknown_c0[0x13a - 0xc0];
    unsigned short* glyphs;            // +0x13a
    char unknown_13e[0x15b - 0x13e];
};
#pragma pack(pop)

struct Holder_004a2480 {
    char unknown_0[4];
    Entry_004a2480* entries;           // +0x4
};

#pragma pack(push, 1)
struct Class_004a2480 {
    char unknown_0[0x18];
    Holder_004a2480* holder;           // +0x18
};
#pragma pack(pop)

char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrame(void* surface, void* image, int x, int y);

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
struct Pad_a2480_0 { int field; };
extern int Pad_a2480_e0;
extern int Pad_a2480_e1;
extern int Pad_a2480_e2;
extern int Pad_a2480_e3;
extern int Pad_a2480_e4;

// FUNCTION: 0x4a2480
void __stdcall DrawGafBar(Class_004a2480* param_1, int index)
{
    Entry_004a2480* base = param_1->holder->entries;
    void* surface = base->surface;
    Entry_004a2480* e = &base[index];
    unsigned short* glyphs = e->glyphs;
    Glyph_004a2480* glyph = (Glyph_004a2480*)GetGafFrame(glyphs, 0);
    int x = e->x;
    int y = e->y + e->h / 2;
    if (glyph != 0)
        y -= glyph->height / 2;
    else
        y = e->y;
    int limit = e->x + e->w;
    if (glyph != 0)
        DrawFrame(surface, glyph, x, y);
    x += glyph->width;
    Glyph_004a2480* mid = (Glyph_004a2480*)GetGafFrame(glyphs, 1);
    if (x + mid->width < limit) {
        do {
            DrawFrame(surface, mid, x, y);
            x += mid->width;
        } while (x + mid->width < limit);
    }
    Glyph_004a2480* last = (Glyph_004a2480*)GetGafFrame(glyphs, 2);
    DrawFrame(surface, last, limit - last->width, y);
}
#pragma pack(push, 1)
struct Glyph_004a2580 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Entry_004a2580 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    unsigned char flags;               // +0x1b
    char unknown_1c[0x28 - 0x1c];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        struct {
            short count;               // +0xb6 (entry 0)
            char head_pad[0xbc - 0xb8];
            void* surface;             // +0xbc (entry 0)
            char head_tail[0x13c - 0xc0];
        } head;
        char text[0x13c - 0xb6];       // +0xb6
    } u;
    int field_13c;                     // +0x13c
    short off;                         // +0x140
    short size;                        // +0x142
    char unknown_144[0x14a - 0x144];
    int field_14a;                     // +0x14a
    unsigned short* glyphs;            // +0x14e
    unsigned char field_152;           // +0x152
    char unknown_153[0x157 - 0x153];
    int field_157;                     // +0x157
};
#pragma pack(pop)

struct Holder_004a2580 {
    char unknown_0[4];
    Entry_004a2580* entries;           // +0x04
};

struct Object_004a2580 {
    char unknown_0[0x18];
    Holder_004a2580* holder;           // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char field_8b2;           // +0x8b2
    char unknown_8b3[0x8c1 - 0x8b3];
    unsigned char field_8c1;           // +0x8c1
    char unknown_8c2[0x8c3 - 0x8c2];
    unsigned char field_8c3;           // +0x8c3
    char unknown_8c4[0x8c6 - 0x8c4];
    unsigned char field_8c6;           // +0x8c6
};

struct Font_004a2580 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct Dialog_4a2580 {
    int group;                         // +0x00
    char unknown_04[0x14 - 0x04];
    Font_004a2580* font;               // +0x14
};

void __stdcall SetFont(int id);
void __stdcall GetSliderRects(Entry_004a2580* base, int index, int* r1, int* r2);
void __stdcall DrawRaisedBox(void* surface, int* r, int a, int b, int c);
void __stdcall DrawSunkenBox(void* surface, int* r, int a, int b, int c);
char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrame(void* surface, void* glyph, int x, int y);
int GetTextKeyColor();
void __stdcall SetTextColors(int a, int b);
int GetFont();
void __stdcall GetTextWidth(Font_004a2580* font, char* text);
int GetFontHeight();
void __stdcall DrawString(void* surface, char* text, int x, int y, int maxw);
void __stdcall GrayRectangle(void* surface, void* rect);
void __stdcall FadeRectangle(void* surface, void* rect, int a);

static inline void* Surface_004a2580(Object_004a2580* o)
{
    return o->holder->entries->u.head.surface;
}

#include <setjmp.h>

// FUNCTION: 0x4a2580
void __stdcall DrawSliderBar(Object_004a2580* obj, int index)
{
    Entry_004a2580* entries = obj->holder->entries;
    Entry_004a2580* e = &entries[index];
    void* surface = Surface_004a2580(obj);
    // One glyph pointer for every fetch, and one limit in the w<h arm: frame slot order.
    Glyph_004a2580* g;

    int n = 0;
    int i = 1;
    for (; i < entries->u.head.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == e->group) {
                SetFont(*(int*)((char*)&entries[i] + 0xd6));
                break;
            }
            n++;
        }
    }
    if (i == entries->u.head.count + 1)
        SetFont(g_guiContext->group);

    int r1[4];
    int r2[4];
    GetSliderRects(entries, index, r1, r2);

    unsigned short* gl = e->glyphs;
    if (gl == 0) {
        DrawRaisedBox(surface, r1, obj->field_8b2, obj->field_8c3, obj->field_8c6);
        DrawSunkenBox(surface, r2, obj->field_8b2, obj->field_8c3, obj->field_8c6);
    } else {
        short w = e->w;
        short h = e->h;
        if (w < h) {
            void* surf = Surface_004a2580(obj);
            int y = e->y;
            int x = e->x;
            int limit = y + e->h - 1;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152);
            if (g != 0)
                DrawFrame(surf, g, x, y);
            y += g->height;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 1);
            while (y + g->height <= limit) {
                DrawFrame(surf, g, x, y);
                y += g->height;
            }
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 2);
            DrawFrame(surf, g, x, limit - g->height + 1);
            x += g->width / 2;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 3);
            x -= g->width / 2;
            int ybase = e->off + e->y + 3;
            // The clamps use the <windows.h> min() macro, pulled in by <ddraw.h>.
            int lc = min(e->h - 6, e->size);
            limit = lc + ybase - 1;
            int t = e->h + e->y - 4;
            limit = min(limit, t);
            if (ybase > limit - lc + 1)
                ybase = limit - lc + 1;
            DrawFrame(surf, g, x, ybase);
            lc -= g->height;
            ybase += g->height;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 4);
            while (ybase <= limit - g->height) {
                DrawFrame(surf, g, x, ybase);
                ybase += g->height;
                lc -= g->height;
            }
            DrawFrame(surf, g, x, limit - g->height);
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 5);
            DrawFrame(surf, g, x, limit - g->height + 1);
        } else {
            void* surf = Surface_004a2580(obj);
            int x = e->x;
            int y = e->y;
            int limit = x + e->w - 1;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152);
            if (g != 0)
                DrawFrame(surf, g, x, y);
            x += g->width;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 1);
            while (x + g->width <= limit) {
                DrawFrame(surf, g, x, y);
                x += g->width;
            }
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 2);
            DrawFrame(surf, g, limit - g->width + 1, y);
            y += g->height / 2;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 3);
            y -= g->height / 2;
            int a = e->off + e->x + 3;
            a = min(a, limit - g->width - 2);
            DrawFrame(surf, g, a, y);
        }
    }

    if (e->flags & 4) {
        int cur = GetTextKeyColor();
        SetTextColors(obj->field_8c1, cur);
        char buf[0x10];
        // Original bug, kept as it is: the copy at 0x4a2a26 (strlen with repne
        // scasb, then rep movsd/rep movsb) is unbounded and the source field runs
        // from entry+0xb6 to the end of the 0x15b-byte entry, so a label longer
        // than 15 characters runs off the 0x10-byte buffer into r1, r2 and the
        // saved registers.
        if (e->u.text[0] != 0) {
            strcpy(buf, e->u.text);
        } else if (e->field_13c != 0) {
            int num = (int)((float)e->off * e->field_13c / (e->w - e->size));
            _itoa(num, buf, 10);
        } else if (e->flags & 8) {
            _itoa(e->off + 1, buf, 10);
        } else {
            _itoa(e->off, buf, 10);
        }
        char* text = buf;
        int total = 0;
        if (text != 0) {
            if (g_guiContext->font == 0) {
                GetTextWidth((Font_004a2580*)GetFont(), text);
            } else {
                char* p = text;
                for (; *p != 0; p++) {
                    unsigned char c = *p;
                    g = (Glyph_004a2580*)GetGafFrame((unsigned short*)g_guiContext->font->glyphs, c);
                    if (g != 0)
                        total += g->width;
                }
            }
        }
        if (g_guiContext->font == 0)
            GetFontHeight();
        else
            GetGafFrame((unsigned short*)g_guiContext->font->glyphs, 0x49);
        DrawString(surface, buf, e->x + e->w + 2, e->y + 4, -1);
    }

    if ((e->flags & 0x10) || e->field_157 != 0) {
        int rect[4];
        if (e->type == 0) {
            rect[0] = 0;
            rect[1] = 0;
        } else {
            rect[0] = e->x;
            rect[1] = e->y;
        }
        rect[2] = e->w + rect[0] - 1;
        rect[3] = e->h + rect[1] - 1;
        GrayRectangle(entries->u.head.surface, rect);
        FadeRectangle(entries->u.head.surface, rect, -0x14);
    }
}
#pragma pack(push, 1)
struct Gadget_004a2be0 {               // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x19 - 0x01];      // +0x01
    short field_19;                    // +0x19
    int field_1b;                      // +0x1b (dword)
    char unknown_1f[0xb6 - 0x1f];      // +0x1f
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];      // +0xb8
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char* field_c2;                    // +0xc2
    char unknown_c6[0xd6 - 0xc6];      // +0xc6
    int id;                            // +0xd6
    char unknown_d8[0xda - 0xd8];      // +0xd8
    short field_da;                    // +0xda
    char unknown_dc[0x136 - 0xdc];     // +0xdc
    short field_136;                   // +0x136
    char unknown_138[0x140 - 0x138];   // +0x138
    short field_140;                   // +0x140
    char unknown_142[0x15b - 0x142];   // +0x142
};
#pragma pack(pop)

struct Holder_004a2be0 {
    int current;                       // +0x00
    Gadget_004a2be0* entries;          // +0x04
    char unknown_08[0x14 - 0x08];
    void* list;                        // +0x14
};

struct Dialog_4a2be0 {
    char unknown_0[0x18];
    Holder_004a2be0* holder;           // +0x18
};

void __stdcall DrawListBox(Dialog_4a2be0* param_1, int param_2);
void __stdcall DrawSliderBar(Dialog_4a2be0* param_1, int param_2);
void __stdcall DrawTextInput(Dialog_4a2be0* param_1, int param_2);
char* __stdcall SkipTextLines(char* text, int line);

// FUNCTION: 0x4a2be0
void __stdcall SyncAssocGadgets(Dialog_4a2be0* param_1, int param_2)
{
    Gadget_004a2be0* entries = param_1->holder->entries;
    int i = 1;
    char* me = (char*)entries + param_2 * 0x15b;
    int type = *(unsigned char*)me;
    int field_1b = *(int*)(me + 0x1b);
    for (; i < (short)entries->count + 1; i++) {
        // Built fresh inside the loop body: strength reduction then rotates the preheader.
        char* entry = (char*)entries + i * 0x15b + 0x140;
        if (i != param_2) {
            if (entry[-0x13f] == me[1]) {
                switch ((unsigned char)entry[-0x140]) {
                case 2:
                    if (type == 2) {
                        *(short*)(entry - 0x84) = *(short*)(me + 0xbc);
                        *(short*)(entry - 0x86) = *(short*)(me + 0xba);
                        DrawListBox(param_1, i);
                    } else if (type == 4) {
                        int esi_val;
                        if (*(unsigned char*)(entry - 0x125) & 0x20) {
                            esi_val = *(short*)(me + 0x136) / (*(short*)(entry - 0x82) + 1);
                        } else {
                            esi_val = 0;
                        }
                        short rows = *(short*)(entry - 0x66);
                        if (rows != 0) {
                            int edx_val = *(short*)(entry - 0x80) -
                                *(short*)(entry - 0x127) / rows;
                            int eax_val = *(short*)(me + 0x140) + esi_val;
                            int result = (int)((float)edx_val * eax_val /
                                (*(short*)(me + 0x136) - 1));
                            *(short*)(entry - 0x84) = result;
                        }
                        DrawListBox(param_1, i);
                    }
                    break;
                case 3:
                    if (type == 2 && field_1b & 8) {
                        char* line = SkipTextLines(*(char**)(me + 0xc2), *(short*)(me + 0xba));
                        strcpy(entry - 0x8a, line);
                        DrawTextInput(param_1, i);
                    }
                    break;
                case 4:
                    if (type == 2) {
                        if (*(short*)(me + 0xc0) > 1) {
                            int result;
                            if (*(short*)(me + 0xbe) != 0) {
                                short scale = *(short*)(me + 0xbc);
                                short height = *(short*)(entry - 0xa);
                                short count = *(short*)(me + 0xbe);
                                // Split into a named ratio: fixes the x87 operand-staging order.
                                float ratio = (float)scale * height;
                                result = (int)(ratio / count);
                            } else {
                                result = 0;
                            }
                            if (*(short*)entry != result) {
                                *(short*)entry = result;
                                DrawSliderBar(param_1, i);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}
// FindKind returns zero on a miss, so the rescale then uses entry zero.
// <iostream> and <math.h> are needed: they restore the shared floating-point tail.

#pragma pack(push, 1)
struct Entry_004a2e40 { // 0x15b bytes
    unsigned char type; // +0x00
    unsigned char kind; // +0x01
    char name[0x10];    // +0x02
    char unknown_12[0x19 - 0x12];
    short field_19; // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group; // +0x28
    char unknown_29[0xb6 - 0x29];
    short count; // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba; // +0xba
    short field_bc; // +0xbc
    short field_be; // +0xbe
    char unknown_c0[0xd6 - 0xc0];
    int id; // +0xd6
    char unknown_da[0x136 - 0xda];
    short field_136; // +0x136
    char unknown_138[0x140 - 0x138];
    short field_140; // +0x140
    char unknown_142[0x15b - 0x142];
};
#pragma pack(pop)

struct List_004a2e40 {
    char unknown_0[0x0c];
    unsigned short* glyphs; // +0x0c
};

struct Holder_004a2e40 {
    int current;             // +0x00
    Entry_004a2e40* entries; // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a2e40* list; // +0x14
};

#pragma pack(push, 1)
struct Dialog_4a2e40 {
    char unknown_00[0x18];
    Holder_004a2e40* holder; // +0x18
    char unknown_1c[0xcca - 0x1c];
    int field_cca; // +0xcca
};
#pragma pack(pop)

void __stdcall SetFont(int id);
char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFontHeight();

static inline int FindEntry(Entry_004a2e40* entries, char* name) {
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

static inline int FindKind(Entry_004a2e40* entries, unsigned char kind) {
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 4 && entries[i].kind == kind)
            return i;
    }
    return 0;
}

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
struct Pad_a2e40_0 { int field; };
struct Pad_a2e40_1 { int field; };
struct Pad_a2e40_2 { int field; };
struct Pad_a2e40_3 { int field; };
struct Pad_a2e40_4 { int field; };
extern int Pad_a2e40_e0;
extern int Pad_a2e40_e1;
extern int Pad_a2e40_e2;

// FUNCTION: 0x4a2e40
void __stdcall SetListBoxScrollByName(Dialog_4a2e40* param_1, char* param_2, int param_3) {
    Entry_004a2e40* entries = param_1->holder->entries;
    int found = FindEntry(entries, param_2);
    if (found == -1)
        return;

    Entry_004a2e40* me = &entries[found];
    me->field_ba = param_3;

    int n = 0;
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                SetFont(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1)
        SetFont(g_guiContext->current);

    int size;
    if (g_guiContext->list == 0)
        size = GetFontHeight();
    else
        size = *(unsigned short*)(GetGafFrame(g_guiContext->list->glyphs, 0x49) + 2) + 2;
    int step = (me->field_19 - 2) / (size + 1);
    short last = me->field_bc;
    short sel = me->field_ba;
    if (sel > step + last - 1 || sel < last) {
        if (me->field_be != 0)
            me->field_bc = sel;
        if (me->field_bc > me->field_be)
            me->field_bc = me->field_be;
        Entry_004a2e40* peer = &entries[FindEntry(entries, param_2)];
        unsigned char pkind = peer->kind;
        Entry_004a2e40* e3 = &entries[FindKind(entries, pkind)];
        // Keep the float conversions: the original uses integer-memory FPU multiply/divide.
        float q = (float)e3->field_136 * me->field_bc / me->field_be;
        if ((float)e3->field_140 != q)
            e3->field_140 = (short)q;
    }
    param_1->field_cca = 1;
}
// Refreshes GUI entry `index` (0x15b-byte entries, entry 0 holds the count at
// +0xb6): clears the two words at +0xba/+0xbc, selects the entry of type 7
// whose group number matches this entry's group and makes its id current, then
// quantises this entry's height (+0x19) down to a multiple of the current font
// line step and stamps the current time into +0xb6.

#pragma pack(push, 1)
struct Entry_004a30c0 {                // 0x15b bytes
    unsigned char type;               // +0x00
    char unknown_01[0x19 - 0x01];
    short height;                     // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group;                       // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                      // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                   // +0xba
    short field_bc;                   // +0xbc
    char unknown_be[0xd6 - 0xbe];
    int id;                           // +0xd6
    char unknown_da[0x15b - 0xda];
};
#pragma pack(pop)

struct Holder_004a30c0 {
    char unknown_0[4];
    Entry_004a30c0* entries;           // +0x04
};

struct Class_004a30c0 {
    char unknown_0[0x18];
    Holder_004a30c0* holder;           // +0x18
};

struct Font_004a30c0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct Dialog_4a30c0 {
    int group;                         // +0x00
    char unknown_04[0x14 - 0x04];
    Font_004a30c0* font;               // +0x14
};

void __stdcall SetFont(int id);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
unsigned int __cdecl GetTicks();
// FUNCTION: 0x4a30c0
void __stdcall ResetListBox(Class_004a30c0* obj, int index)
{
    Entry_004a30c0* entries = obj->holder->entries;
    Entry_004a30c0* e = &entries[index];
    e->field_bc = 0;
    e->field_ba = 0;
    int n = 0;
    int i = 1;
    for (; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == e->group) {
                SetFont(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1) {
        SetFont(g_guiContext->group);
    }
    int step;
    if (g_guiContext->font == 0) {
        step = GetFontHeight();
    } else {
        step = *(unsigned short*)((char*)GetGafFrame(g_guiContext->font->glyphs, 0x49) + 2) + 2;
    }
    int h = e->height;
    e->height = h - h % (step + 2);
    *(int*)&e->count = GetTicks();
}
void* __cdecl GameAllocIgnoreTag(const char* name, unsigned int size);
char* __stdcall SkipTextLines(char* text, int n);
void __stdcall TruncateTextWithEllipsis(int a1, char* text, int a3, int a4, int a5);

// Builds a block of NUL-terminated item strings from the text blob at a3
// (one name per line) and copies it back over a3. Each line is truncated to
// 100 bytes, expanded by TruncateTextWithEllipsis (marker glyphs) and appended in place.
// FUNCTION: 0x4a31c0
void __stdcall TruncateListItems(int a1, int a2, char* a3, int a4)
{
    char* items = (char*)GameAllocIgnoreTag("SCROLLITEMS", *(int*)(a3 - 0x44));
    char* out = items;
    for (int i = 0; i < a4; i++) {
        char buf[100];
        strncpy(buf, SkipTextLines(a3, i), 100);
        TruncateTextWithEllipsis(a1, buf, a2, -1, 0);
        strcpy(out, buf);
        out += strlen(buf);
        *out = 0;
        out++;
    }
    memcpy(a3, items, out - items);
}
// The missing-name path calls the fatal-error routine FatalError, which
// exits the process. Its following null-entry accesses are unreachable.

#pragma pack(push, 1)
struct Entry_004a32a0 { // 0x15b bytes
    unsigned char type; // +0x00
    unsigned char kind; // +0x01, matched by the type-4 search
    char name[0x10];    // +0x02
    char unknown_12[0x19 - 0x12];
    short height; // +0x19, the rows are fitted into this
    int flags;    // +0x1b
    char unknown_1f[0x29 - 0x1f];
    unsigned char f_29; // +0x29, gates the whole second half
    char unknown_2a[0xb6 - 0x2a];
    short count; // +0xb6, entry 0 holds the entry count
    char unknown_b8[0xba - 0xb8];
    short f_ba;  // +0xba
    short f_bc;  // +0xbc
    short first; // +0xbe, first row that still fits
    short num;   // +0xc0, the row count
    int bitmap;  // +0xc2
    char unknown_c6[0xd6 - 0xc6];
    int id;     // +0xd6
    short f_da; // +0xda, the line height
    char unknown_dc[0x15b - 0xdc];
};
#pragma pack(pop)

struct Glyph_004a32a0 {
    unsigned short width;
    unsigned short height; // +0x02
};

struct Dialog_4a32a0 {
    char unknown_00[0x18];
    Holder_004a32a0* holder; // +0x18
};

void __stdcall FatalError(char* msg);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall SetGadgetActive(Root_004a32a0* menu, int index, int value);
// Keeps its own file, src/gui/gui_4a3ef0.cpp: DrawSlider only
// matches at that file's symbol count.
void __stdcall DrawSlider(Root_004a32a0* param_1, int param_2);

// The line height of one row: the default font height, or the height of the
// glyph for 'I' plus two. Written out three times in the caller because the
// original evaluates it again in the second arm of the +0xda minimum.
static inline int FontHeight_004a32a0() {
    if (g_guiContext->language == 0)
        return GetFontHeight();
    return (int)((Glyph_004a32a0*)GetGafFrame(g_guiContext->language->glyphs, 0x49))->height + 2;
}

// The entry search of 0x4a0180, 0x4a0200, 0x4a0280 and 0x4a35a0.
static inline int FindName_004a32a0(Entry_004a32a0* entries, char* name) {
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// The type-4 entry search; returns 0 when nothing matches.
static inline int FindType_004a32a0(Entry_004a32a0* entries, unsigned char kind) {
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 4 && entries[i].kind == kind)
            return i;
    }
    return 0;
}

// FUNCTION: 0x4a32a0
void __stdcall ConfigureListBoxByName(Dialog_4a32a0* param_1, char* name, int bitmap, int count, int flag) {
    Holder_004a32a0* holder = param_1->holder;
    Entry_004a32a0* entries = holder->entries;
    int index = FindName_004a32a0(entries, name);
    Entry_004a32a0* me;
    if (index != -1) {
        me = &entries[index];
    } else {
        FatalError("Error in GUI layout");
        me = 0;
    }
    me->num = (short)count;
    me->bitmap = bitmap;
    me->flags |= 0x10;
    me->f_da =
        (short)((me->f_da > FontHeight_004a32a0() + 1) ? (int)me->f_da : FontHeight_004a32a0() + 1);
    if (flag != 0) {
        me->id = flag;
        me->flags |= 0x800;
    }
    me->f_bc = 0;
    me->f_ba = 0;
    // Initialised before the me->first store: sets the final stack-store order.
    int remain = me->height;
    me->first = (short)(count - 1);
    int step;
    if (me->f_da == 0) {
        step = FontHeight_004a32a0() + 1;
    } else {
        step = me->f_da;
    }
    for (int j = count - 1; j > -1; j--) {
        remain -= step;
        if (remain < 0)
            break;
        me->first = (short)j;
    }
    if (me->f_29 == 0)
        return;
    int i2 = FindName_004a32a0(holder->entries, name);
    Entry_004a32a0* list = holder->entries;
    unsigned char kind = list[i2].kind;
    int found = FindType_004a32a0(list, kind);
    if (found == -1)
        return;
    char* text = (char*)&holder->entries[found].name;
    // Captured in a local before the name search, and passed on to SetGadgetActive.
    Root_004a32a0* root = g_guiContext;
    if (root->holder != 0) {
        int j2 = FindName_004a32a0(root->holder->entries, text);
        if (j2 != -1) {
            SetGadgetActive(root, j2, remain < 0);
        }
    }
    if (remain < 0) {
        // A fresh global lookup, not root.
        DrawSlider(g_guiContext, found);
    }
}
// Finds the GUI layout entry named `name` (0x15b-byte entries whose entry 0
// stores the entry count as a short at +0xb6) and gives it the row array
// `rows` and its count: +0xc6 gets the array, +0xc0 the count, flags +0x1b
// gets bit 7, and +0xbe the index of the topmost row that still fits in the
// entry's height (+0x19) when the row heights are summed from the bottom.
// Each row is 0x18 bytes with an unsigned short height at +0x2; when the
// entry's +0xda is non-zero it overrides the row height with its own value.
// A missing entry is reported and then treated as a null entry (the stores
// that follow then go through a null pointer, as in the original).

#pragma pack(push, 1)
struct Entry_004a35a0 {                // 0x15b-byte GUI entry
    char unknown_0[0x2];
    char name[0x10];                   // +0x02
    char unknown_12[0x19 - 0x12];
    short height;                      // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6 (entry 0 holds the entry count)
    char unknown_b8[0xba - 0xb8];
    short f_ba;                        // +0xba
    short f_bc;                        // +0xbc
    short first;                       // +0xbe
    short num;                         // +0xc0
    char unknown_c2[0xc6 - 0xc2];
    int f_c6;                          // +0xc6
    char unknown_ca[0xda - 0xca];
    short f_da;                        // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Row_004a35a0 {
    char unknown_0[2];
    unsigned short height;             // +0x2
    char unknown_4[0x18 - 4];
};

struct Table_004a35a0 {
    int unknown_0;
    Entry_004a35a0* entries;           // +0x4
};
#pragma pack(pop)

void __stdcall FatalError(const char* msg);

static inline int FindEntry(Entry_004a35a0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a35a0
void __stdcall SetGadgetRows(Table_004a35a0* table, char* name,
                            Row_004a35a0* rows, int count)
{
    Entry_004a35a0* entries = table->entries;
    int index = FindEntry(entries, name);
    Entry_004a35a0* e;
    if (index != -1) {
        e = &entries[index];
    } else {
        FatalError("Error in GUI layout");
        e = 0;
    }
    e->f_c6 = (int)rows;
    // Computed right after the f_c6 store and before the num store.
    Row_004a35a0* row = rows + count - 1;
    e->num = (short)count;
    e->flags |= 0x80;
    e->f_bc = 0;
    e->f_ba = 0;
    e->first = (short)(count - 1);
    int remain = e->height;
    // The test is i > -1, not i >= 0.
    for (int i = count - 1; i > -1; i--, row--) {
        // An if/else with one remain -= per arm, not one remain -= cond ? a : b.
        if (e->f_da != 0) remain -= e->f_da; else remain -= row->height;
        if (remain < 0)
            break;
        e->first = (short)i;
    }
}
// Finds the GUI layout entry whose name matches `name`. The entry table holds
// 0x15b-byte entries whose entry 0 stores the entry count as a short at +0xb6;
// entries are searched from 1. A missing entry is reported and then treated as
// a null entry. The caller supplies an array of item pointers and its count:
// the entry gets the array, the count, and the index of the first item that
// still fits in the entry's field +0x19 when the item heights are summed from
// the bottom of the array. Item heights are the unsigned short at offset +2 of
// the object found through each item's field +0x28.

#pragma pack(push, 1)
struct Entry_004a36a0 {                // 0x15b bytes
    char unknown_0[2];
    char name[0x10];                   // +0x02
    char unknown_12[0x19 - 0x12];
    short field_19;                    // +0x19
    int flags_1b;                      // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6 (entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char unknown_c2[0xc6 - 0xc2];
    int field_c6;                      // +0xc6
    char unknown_ca[0x15b - 0xca];
};
#pragma pack(pop)

struct Table_004a36a0 {
    char unknown_0[4];
    Entry_004a36a0* entries;           // +0x4
};

struct Item_004a36a0 {
    char unknown_0[0x28];
    int field_28;                      // +0x28
};

void __stdcall FatalError(char* path);

static inline int FindEntry(Entry_004a36a0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a36a0
void __stdcall SetGadgetItems(Table_004a36a0* table, char* name, int* items, int count)
{
    Entry_004a36a0* entries = table->entries;
    int i = FindEntry(entries, name);
    Entry_004a36a0* e;
    if (i != -1) {
        e = &entries[i];
    } else {
        FatalError("Error in GUI layout");
        e = 0;
    }
    // field_c0 is stored before field_c6: keeps the tail's store order.
    e->field_c0 = (short)count;
    e->field_c6 = (int)items;
    int v = e->field_19;
    e->field_bc = 0;
    e->field_ba = 0;
    e->flags_1b |= 0x20;
    int* p = &items[count - 1];
    e->field_be = (short)(count - 1);
    // Decrements stay in the body with no for-increment, and the test is j > -1.
    for (int j = count - 1; j > -1; ) {
        Item_004a36a0* item = (Item_004a36a0*)*p;
        unsigned short w = *(unsigned short*)(item->field_28 + 2);
        v -= w;
        if (v < 0) {
            break;
        }
        e->field_be = (short)j;
        j--;
        p--;
    }
}
#pragma pack(push, 1)
struct Gadget_004a3780 {               // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01
    char unknown_02[0x13 - 0x02];
    short field_13;                    // +0x13
    short field_15;                    // +0x15
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x28 - 0x1f];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0)
        int field_b6;                  // +0xb6 (the scroll repeat timer)
    };
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char* field_c2;                    // +0xc2
    void* field_c6;                    // +0xc6
    char unknown_ca[0xce - 0xca];
    void (__stdcall* field_ce)(void*, void*);  // +0xce
    char unknown_d2[0xd6 - 0xd2];
    int field_d6;                      // +0xd6
    short field_da;                    // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct List_004a3780 {
    char unknown_0[0xc];
    unsigned short* field_0c;          // +0x0c
};

struct Holder_004a3780 {
    int current;                       // +0x00
    Gadget_004a3780* entries;          // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a3780* list;               // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                      // +0x20
};

struct Rect_004a3780 { int x0, y0, x1, y1; };

struct Point_004a3780 {                // 0x18 bytes, copied with rep movsd x6
    int x;                             // +0x00
    int y;                             // +0x04
    int unknown_08[4];                 // +0x08
};

struct Object_004a3780 {
    char unknown_00[0x18];
    Holder_004a3780* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a3780 point;              // +0x3c
    char unknown_54[0x60 - 0x54];
    int field_60;                      // +0x60
    int focus;                         // +0x64
    char unknown_68[0xcca - 0x68];
    int field_cca;                     // +0xcca
};
#pragma pack(pop)

extern char DAT_00502a20[];

void __stdcall SetFont(int id);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
char* __stdcall SkipTextLines(char* text, int n);
unsigned int __cdecl GetTicks();
int __stdcall IsDoubleClickMessage(Object_004a3780* obj, unsigned char buttons);
int __stdcall IsMouseButtonMessage(Object_004a3780* obj, unsigned char buttons);
int __stdcall HasMouseKeyFlags(Object_004a3780* obj, unsigned int mask);
void __stdcall SetClickMode(Object_004a3780* obj, int param_2);
void __stdcall TrySetFocus(Object_004a3780* obj, int index);
void __stdcall DrawListBox(Object_004a3780* obj, int index);
void __stdcall SyncAssocGadgets(Object_004a3780* obj, int index);

struct Row_004a3780 {
    short unknown_0;
    unsigned short height;             // +0x02
    char unknown_4[0x18 - 0x4];
};

struct Item_004a3780 {
    char unknown_0[0x28];
    Row_004a3780* row;                 // +0x28
};

struct Glyph_004a3780 {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
};

static inline int LineHeight_004a3780()
{
    if (0 == g_guiContext->list)
        return GetFontHeight();
    return ((Glyph_004a3780*)GetGafFrame(g_guiContext->list->field_0c, 0x49))->height + 2;
}

// FUNCTION: 0x4a3780
int __stdcall HandleListBoxInput(Object_004a3780* obj, int index, int param_3)
{
    if (obj->field_60 != -1)
        return 0;
    Gadget_004a3780* entries = obj->holder->entries;
    Gadget_004a3780* me = &entries[index];
    int orig_sel = me->field_ba;
    if (me->field_c0 == 0)
        return 0;
    Rect_004a3780 r;
    unsigned int flags;
    // Called on &entries[index], not me: with me the y1 lea operands swap.
    GetGadgetRect((Entry_004a04f0*)&entries[index], (Rect*)&r);
    int n = 0;
    r.y0 += 2;
    r.y1 -= 3;
    Point_004a3780 point = obj->point;
    point.x -= entries[0].field_13;
    point.y -= entries[0].field_15;
    int i;
    for (i = 1; i < entries[0].count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                SetFont(entries[i].field_d6);
                break;
            }
            n++;
        }
    }
    if (i == entries[0].count + 1)
        SetFont(g_guiContext->current);

    int size = LineHeight_004a3780();
    short da = me->field_da;
    int span = (da != 0) ? da : size + 1;
    int step = (me->field_19 - 2) / span;
    // The SkipTextLines results go through this s.
    char* s;

    if (IsDoubleClickMessage(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            // skip0 sits at the end of the in-rect block: reloads point.x on this edge only.
            if (me->field_c0 == 0) goto skip0;
            if (!(me->flags & 0x200))
                goto ret1;
            me->field_ba = (point.y - r.y0) / span + me->field_bc;
            if (me->field_ba < 0)
                goto above;
            if (me->field_ba - me->field_bc > step - 1)
                me->field_ba = me->field_bc + step - 1;
            if (me->field_ba >= me->field_c0 - 1)
                me->field_ba = me->field_c0 - 1;
            s = SkipTextLines(me->field_c2, me->field_ba);
            if (strncmp(DAT_00502a20, s, 2) != 0)
                goto ret1;
            me->field_ba = orig_sel;
            return 0;
skip0:;
        }
    } else if (IsMouseButtonMessage(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            TrySetFocus(obj, index);
            SetClickMode(obj, 1);
        }
    } else if (IsMouseButtonMessage(obj, 2)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            TrySetFocus(obj, index);
            SetClickMode(obj, 2);
        }
    }

    if (obj->focus != index)
        goto end;
    if (!HasMouseKeyFlags(obj, 3))
        obj->focus = -1;
    if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
        obj->holder->field_20 = index;
        flags = me->flags;
        if (flags & 0x10) {
            // Stores straight into me->field_ba and tests the field; clamps written plainly.
            me->field_ba = (point.y - r.y0) / span + me->field_bc;
            if (me->field_ba >= 0) {
                if (me->field_ba - me->field_bc > step - 1)
                    me->field_ba = me->field_bc + step - 1;
                if (me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                if (me->field_ba < 0)
                    me->field_ba = 0;
                if (flags & 0x200) {
                    s = SkipTextLines(me->field_c2, me->field_ba);
                    if (strncmp(DAT_00502a20, s, 2) == 0)
                        me->field_ba = orig_sel;
                }
                for (i = 1; i <= entries[0].count; i++) {
                    if (entries[i].type == 2 && entries[i].kind == me->kind)
                        // min() for the sync clamp.
                        entries[i].field_ba = min(entries[i].field_c0 - 1, me->field_ba);
                }
            } else {
                me->field_ba = orig_sel;
            }
        } else if (flags & 0x20 | 0x80) {
            // Original bug, kept: `(flags & 0x20) | 0x80` is always true
            // (and ecx,0x20 / or cl,0x80 / test cl,cl at 0x4a3c29).
            int flag8 = (flags >> 7) & 1;
            Row_004a3780* fixed;
            Item_004a3780** ip;
            if (flag8)
                fixed = &((Row_004a3780*)me->field_c6)[me->field_bc];
            else
                ip = &((Item_004a3780**)me->field_c6)[me->field_bc];
            // k is declared after the pointer choice, not before the if (flag8).
            int k = me->field_bc;
            // remain stays declared before n2.
            int remain = point.y - r.y0 - 2;
            int n2 = 0;
            for (;;) {
                Row_004a3780* row = flag8 ? fixed : (*ip)->row;
                if (me->field_da != 0)
                    remain -= span;
                else
                    remain -= row->height;
                if (remain <= 0) {
                    me->field_ba = n2 + me->field_bc;
                    break;
                }
                if (flag8)
                    fixed++;
                else
                    ip++;
                n2++;
                k++;
                if (k > me->field_c0 - 1)
                    break;
            }
        }
        if (orig_sel != me->field_ba) {
            DrawListBox(obj, index);
            if (me->field_ce)
                me->field_ce(obj, me);
        }
        if (me->flags & 0x40) {
            // Two labels in this order: the return block's live-range split depends on it.
above:
ret1:
            return 1;
        }
        obj->field_cca = 1;
    } else if (point.y < r.y0) {
        if (me->field_bc > 0 && me->field_b6 < (int)GetTicks()) {
            me->field_b6 = GetTicks() + 2;
            if (me->field_ba > me->field_bc)
                me->field_ba = me->field_bc;
            me->field_bc--;
            me->field_ba--;
            short sel = me->field_ba;
            if (me->field_c2 != 0) {
                s = SkipTextLines(me->field_c2, sel < 0 ? 0 : sel);
                if (strncmp(DAT_00502a20, s, 2) == 0)
                    me->field_ba = orig_sel;
            }
            goto finish;
        }
        if (me->field_ba > 0) {
            me->field_ba = 0;
            goto finish;
        }
    } else if (point.y > r.y1) {
        if (me->field_bc < me->field_be && me->field_b6 < (int)GetTicks()) {
            me->field_b6 = GetTicks() + 2;
            me->field_bc++;
            me->field_ba = me->field_bc + step - 1;
            if (me->field_c2 != 0) {
                s = SkipTextLines(me->field_c2, me->field_ba);
                if (strncmp(DAT_00502a20, s, 2) == 0)
                    me->field_ba = orig_sel;
            }
            // Shared by both scroll tails via goto: MSVC 5 does not merge return blocks.
finish:
            DrawListBox(obj, index);
            SyncAssocGadgets(obj, index);
        }
    }
end:
    return obj->field_60 != -1;
}
// Clears three fields of GUI entry i (0x15b-byte entries). The stores sit in
// an inline helper taking the entry pointer; a local pointer to the entry
// loads the entries pointer before the index multiply instead of after it.

#pragma pack(push, 1)
struct Entry_004a3eb0 {
    char unknown_0[0x140];
    short field_140;                   // +0x140
    char unknown_142[2];
    int field_144;                     // +0x144
    char unknown_148[2];
    int field_14a;                     // +0x14a
    char unknown_14e[0x15b - 0x14e];
};
#pragma pack(pop)

struct Table_004a3eb0 {
    char unknown_0[4];
    Entry_004a3eb0* entries;           // +0x4
};

struct Dialog_4a3eb0 {
    char unknown_0[0x18];
    Table_004a3eb0* table;             // +0x18
};

static inline void ClearEntry(Entry_004a3eb0* e)
{
    e->field_140 = 0;
    e->field_144 = 0;
    e->field_14a = 0;
}

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
struct Pad_a3eb0_0 { int field; };
struct Pad_a3eb0_1 { int field; };
struct Pad_a3eb0_2 { int field; };
extern int Pad_a3eb0_e0;
extern int Pad_a3eb0_e1;
extern int Pad_a3eb0_e2;
extern int Pad_a3eb0_e3;

// FUNCTION: 0x4a3eb0
void __stdcall FUN_004a3eb0(Dialog_4a3eb0* obj, int i)
{
    ClearEntry(&obj->table->entries[i]);
}
// The list gadget's scroll
// bar thumb: while the entry has the focus, either drag the offset (+0x140)
// with the mouse or step it by one when the mouse is outside the thumb, then
// clamp it to 0..field_136-1 and, if it changed, mark the holder dirty,
// redraw (DrawSliderBar, SyncAssocGadgets) and call the entry's callback. Without
// the focus, a left or right press inside the gadget takes the focus, and a
// press on the thumb starts a drag.

#pragma pack(push, 1)
struct Entry_004a4170 {                // 0x15b bytes, the table of 0x4a23b0
    char unknown_00[0x13];
    short x1;                          // +0x13
    short y1;                          // +0x15
    char unknown_17[0x1b - 0x17];
    unsigned char flags;               // +0x1b, bit 1 = vertical, 0x10 = dead
    char unknown_1c[0x136 - 0x1c];
    short field_136;                   // +0x136, largest usable offset
    char unknown_138[0x140 - 0x138];
    short off;                         // +0x140, the scroll offset
    char unknown_142[0x144 - 0x142];
    int (__stdcall *cb)(void*, int);   // +0x144, called when off changed
    char unknown_148[0x14a - 0x148];
    int field_14a;                     // +0x14a, cb's second argument
    char unknown_14e[0x157 - 0x14e];
    int field_157;                     // +0x157, entry is being dragged
};
#pragma pack(pop)

struct Holder_004a4170 {
    char unknown_00[4];
    Entry_004a4170* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14, set when off changed
};

struct Point_004a4170 {                // 24 bytes, copied with rep movsd
    int x;
    int y;
    int unknown_08[4];
};

struct Object_004a4170 {
    char unknown_00[0x18];
    Holder_004a4170* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a4170 point;              // +0x3c, the mouse, table relative
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64, -1 when nothing has the focus
    char unknown_68[0x78 - 0x68];
    int field_78;                      // +0x78, non-zero while dragging
    Point_004a4170 saved;              // +0x7c, the mouse when the drag began
    short field_94;                    // +0x94, off when the drag began
};

void __stdcall TrySetFocus(Object_004a4170* obj, int index);
void __stdcall GetSliderRects(Entry_004a4170* base, int index, int* r1, int* r2);
void __stdcall DrawSliderBar(Object_004a4170* obj, int index);
void __stdcall SyncAssocGadgets(Object_004a4170* obj, int index);
int __stdcall IsMouseButtonMessage(Object_004a4170* obj, unsigned char buttons);
int __stdcall HasMouseKeyFlags(Object_004a4170* obj, unsigned int mask);
void __stdcall SetClickMode(Object_004a4170* obj, int param_2);

static inline void OffsetChanged_004a4170(Object_004a4170* obj, int index, Entry_004a4170* e, int old)
{
    if (e->off > e->field_136 - 1)
        e->off = e->field_136 - 1;
    if (e->off < 0)
        e->off = 0;
    if (e->off == old)
        return;
    if (obj->holder)
        obj->holder->field_14 = 1;
    DrawSliderBar(obj, index);
    SyncAssocGadgets(obj, index);
    if (e->cb)
        e->cb(obj, e->field_14a);
}

// FUNCTION: 0x4a4170
void __stdcall HandleSliderInput(Object_004a4170* obj, int index)
{
    Entry_004a4170* entries = obj->holder->entries;
    Entry_004a4170* e = &entries[index];
    if (e->flags & 0x10)
        return;
    if (e->field_157)
        return;

    Point_004a4170 p = obj->point;
    p.x -= entries->x1;
    p.y -= entries->y1;
    int r1[4];
    int r2[4];
    GetSliderRects(entries, index, r1, r2);

    if (obj->focus == index) {
        if (!HasMouseKeyFlags(obj, 3)) {
            obj->focus = -1;
            obj->field_78 = 0;
        }
        // Inline OffsetChanged ends both arms: the duplicated zero uses keep 0 in EDX.
        if (obj->field_78) {
            int old = e->off;
            if (e->flags & 1)
                e->off = obj->field_94 - obj->saved.x + p.x;
            else
                e->off = obj->field_94 - obj->saved.y + p.y;
            OffsetChanged_004a4170(obj, index, e, old);
        } else {
            // Per-path stores to e->off (not one after the if/else): keeps the 16-bit loads.
            int old = e->off;
            if (e->flags & 1) {
                if (p.x < r2[0])
                    e->off--;
                else if (p.x > r2[2])
                    e->off++;
            } else {
                if (p.y < r2[1])
                    e->off--;
                else if (p.y > r2[3])
                    e->off++;
            }
            OffsetChanged_004a4170(obj, index, e, old);
        }
        return;
    }

    if (obj->field_78)
        return;
    if (IsMouseButtonMessage(obj, 1)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        TrySetFocus(obj, index);
        SetClickMode(obj, 1);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
        return;
    }
    if (IsMouseButtonMessage(obj, 2)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        TrySetFocus(obj, index);
        SetClickMode(obj, 2);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
    }
}
int __cdecl tolower(int);
int __cdecl toupper(int);

#pragma pack(push, 1)
struct Entry_0049fc50 {                // 0x15b bytes
    unsigned char type;                // +0x0
    char unknown_1[0x13 - 0x1];
    short x1;                          // +0x13
    short y1;                          // +0x15
    short x2;                          // +0x17
    short y2;                          // +0x19
    unsigned char flags;               // +0x1b
    char unknown_2[0xb6 - 0x1c];
    char text[0x15b - 0xb6];           // +0xb6
};
#pragma pack(pop)

struct Holder_0049fc50 {
    int unknown_0;
    Entry_0049fc50* entries;           // +0x4
};

struct Point_0049fc50 {
    int x;
    int y;
    int unknown_8[4];
};

#pragma pack(push, 1)
struct Object_0049fc50 {
    char unknown_0[0x18];
    Holder_0049fc50* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_0049fc50 point;              // +0x3c to +0x50
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64
    char unknown_68[0xcc6 - 0x68];
    int field_cc6;                     // +0xcc6
};
#pragma pack(pop)

int __stdcall TrySetFocus(Object_0049fc50* obj, int index);
int __stdcall IsMouseButtonMessage(Object_0049fc50* obj, unsigned char buttons);
int __stdcall HasMouseKeyFlags(Object_0049fc50* obj, unsigned int mask);
void __stdcall SetClickMode(Object_0049fc50* obj, int param_2);
int PopKey(void);
int __stdcall IsKeyDown(int key);

// FUNCTION: 0x4a4440
int __stdcall HandleLabelInput(Object_0049fc50* obj, int index, char key)
{
    Entry_0049fc50* entries = obj->holder->entries;
    Entry_0049fc50* entry = &entries[index];
    struct Rect_0049fc50 { int x1, y1, x2, y2; } rect;
    Point_0049fc50 point;
    int rel_x, rel_y;

    if (entry->text[0x147 - 0xb6] == 0 && (entry->flags & 0x10))
        return 0;

    if (entry->type == 0) {
        rect.x1 = 0;
        rect.y1 = 0;
    } else {
        rect.x1 = entry->x1;
        rect.y1 = entry->y1;
    }
    rect.x2 = entry->x2 + rect.x1 - 1;
    rect.y2 = entry->y2 + rect.y1 - 1;

    memcpy(&point, &obj->point, 24);

    rel_x = point.x - entries->x1;
    rel_y = point.y - entries->y1;

    if (IsMouseButtonMessage(obj, 1)) {
        if (rel_x < rect.x1 || rel_x > rect.x2 || rel_y < rect.y1 || rel_y > rect.y2)
            goto fail;
        TrySetFocus(obj, index);
        SetClickMode(obj, 1);
    } else if (IsMouseButtonMessage(obj, 2)) {
        if (rel_x < rect.x1 || rel_x > rect.x2 || rel_y < rect.y1 || rel_y > rect.y2)
            goto fail;
        TrySetFocus(obj, index);
        SetClickMode(obj, 2);
    }

fail:
    if (obj->focus != index)
        goto check_queue;
    if (HasMouseKeyFlags(obj, 3))
        goto check_queue;
    obj->focus = -1;
    if (rel_x < rect.x1 || rel_x > rect.x2 || rel_y < rect.y1 || rel_y > rect.y2)
        goto check_queue;
    return 1;

check_queue:
    if (obj->focus != -1) {
        Entry_0049fc50* e = &entries[obj->focus];
        if (e->type == 3 && !IsKeyDown(0xfb))
            return 0;
    }

final_check:
    if (obj->field_cc6 != 1 || *(int*)&key == 0)
        return 0;
    if ((char)tolower(entry->text[0x147 - 0xb6]) != key &&
        (char)toupper(entry->text[0x147 - 0xb6]) != key)
        return 0;
    PopKey();
    return 1;
}
// Sets entry i's end time to now plus its duration.

#pragma pack(push, 1)
struct Entry_004a4620 {
    char unknown_0[0xc2];
    unsigned int duration;             // +0xc2
    unsigned int end;                  // +0xc6
    char unknown_ca[0x15b - 0xca];
};
#pragma pack(pop)

struct Table_004a4620 {
    char unknown_0[4];
    Entry_004a4620* entries;           // +0x4
};

struct Dialog_4a4620 {
    char unknown_0[0x18];
    Table_004a4620* table;             // +0x18
};

unsigned int __cdecl GetTicks();

#include <time.h>

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
extern int Pad_a4620_e0;

// FUNCTION: 0x4a4620
void __stdcall ScheduleGadgetEndTick(Dialog_4a4620* obj, int i)
{
    Entry_004a4620* e = &obj->table->entries[i];
    e->end = GetTicks() + e->duration;
}
#pragma pack(push, 1)
struct Entry_004a4660 {
    unsigned char type;
    char unknown_01[0x13 - 1];
    short x;
    short y;
    short w;
    short h;
    char unknown_1b[0x1f - 0x1b];
    int color1;
    int color2;
    char unknown_27[0xba - 0x27];
    int number;
    char unknown_be[0xd2 - 0xbe];
    int showText;
    char unknown_d6[0x15b - 0xd6];
};
#pragma pack(pop)

struct Holder_004a4660 {
    char unknown_0[4];
    Entry_004a4660 *entries;
};

struct Dialog_4a4660 {
    char unknown_0[0xc];
    void *surface;
    int unknown_10;
    void *oldSurface;
    Holder_004a4660 *holder;
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char color1;
    char unknown_8b3[0x8c3 - 0x8b3];
    unsigned char color2;
    char unknown_8c4[2];
    unsigned char color3;
    char unknown_8c7[0xcd2 - 0x8c7];
    void *fallbackSurface;
};

struct Rect_004a4660 { int left, top, right, bottom; };
struct Glyph_004a4660 { unsigned short width, height; };
struct Language_004a4660 { char unknown_0[0xc]; unsigned short *glyphs; };
struct LanguageRoot_004a4660 { char unknown_0[0x14]; Language_004a4660 *language; };

void __stdcall LockScreen(void *);
void __stdcall FillBevelBox(void *, Rect_004a4660 *, unsigned int, unsigned int, unsigned int);
void __stdcall FillRectangle(void *, Rect_004a4660 *, int);
void __stdcall DrawTextClipped(void *, char *, int, int, int, int);
char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFont();
int __stdcall GetTextWidth(int, char *);
int GetFontHeight();
void __stdcall UnlockScreen(void *);

// Kept as a separate static inline: its zero is hoisted into edi at the prologue.
static inline int Measure_004a4660(char *text)
{
    int width = 0;
    char *p = text;
    if (p == 0)
        return 0;
    if (g_guiContext->language == 0)
        return GetTextWidth(GetFont(), text);
    char *q = text;
    while (*q != 0) {
        char ch = *q;
        Glyph_004a4660 *glyph = (Glyph_004a4660 *)GetGafFrame(
            g_guiContext->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++q;
    }
    return width;
}

// FUNCTION: 0x4a4660
void __stdcall DrawProgressBarGadget(Dialog_4a4660 *obj, int index)
{
    Entry_004a4660 *entries = obj->holder->entries;
    Entry_004a4660 *entry = (Entry_004a4660 *)((char *)entries + index * 0x15b);
    obj->oldSurface = obj->surface;

    void *surface = *(void **)((char *)entries + 0xbc);
    if (surface == 0)
        surface = *(void **)((char *)obj + 0xcd2);
    LockScreen(surface);

    Rect_004a4660 rect;
    rect.left = entry->x;
    rect.top = entry->y;
    rect.right = entry->w + entry->x;
    rect.bottom = entry->h + entry->y;
    FillBevelBox(surface, &rect, obj->color1, obj->color2, obj->color3);

    rect.left += 2;
    rect.top += 2;
    rect.right -= 2;
    rect.bottom -= 2;
    FillRectangle(surface, &rect, *(int *)((char *)entry + 0x23));

    float scale = (float)*(int *)((char *)entry + 0xba) / *(int *)((char *)entry + 0xb6);
    rect.right = (int)(scale * (entry->w - 4)) + rect.left;
    FillRectangle(surface, &rect, *(int *)((char *)entry + 0x1f));

    if (entry->showText != 0) {
        char text[20];
        _itoa(entry->number, text, 10);
        int width = Measure_004a4660(text);
        int height;
        if (g_guiContext->language == 0) {
            height = GetFontHeight();
        } else {
            Glyph_004a4660 *glyph = (Glyph_004a4660 *)GetGafFrame(
                g_guiContext->language->glyphs, 0x49);
            height = glyph->height + 2;
        }
        DrawTextClipped(surface, text,
            (entry->w / 2 - width / 2) + entry->x,
            (entry->h / 2 - height / 2) + entry->y, -1, 0);
    }

    obj->oldSurface = *(void **)((char *)obj + 8);
    UnlockScreen(surface);
}
// Advances GUI entry i's animated value by its step each time its interval
// elapses, until it reaches its maximum, then redraws the entry.
// Needs a header (any of <windows.h>, <stdio.h>, ...: tools/headers.py) for
// the table pointer to be loaded between the steps of the index multiply.

#pragma pack(push, 1)
struct Entry_004a4890 {
    char unknown_0[0xba];
    int value;                         // +0xba
    int max;                           // +0xbe
    int interval;                      // +0xc2
    int next;                          // +0xc6
    float step;                        // +0xca
    int active;                        // +0xce
    char unknown_d2[0x15b - 0xd2];
};
#pragma pack(pop)

struct Table_004a4890 {
    char unknown_0[4];
    Entry_004a4890* entries;           // +0x4
};

struct Dialog_4a4890 {
    char unknown_0[0x18];
    Table_004a4890* table;             // +0x18
};

unsigned int __cdecl GetTicks();
void __stdcall DrawProgressBarGadget(Dialog_4a4890* obj, int i);

// FUNCTION: 0x4a4890
void __stdcall TickProgressBar(Dialog_4a4890* obj, int i)
{
    Entry_004a4890* e = &obj->table->entries[i];
    if (e->active && e->value < e->max) {
        if ((int)GetTicks() > e->next) {
            e->value += (int)e->step;
            if (e->value > e->max) {
                e->value = e->max;
                e->active = 0;
            }
            e->next = GetTicks() + e->interval;
        }
        DrawProgressBarGadget(obj, i);
    }
}
struct ElemArray_4a4930 {
    char unknown_0[4];
    char* base;               // +4
};

struct GameState_4a4930 {
    char unknown_0[0x18];
    ElemArray_4a4930* arr;    // +0x18
};

// FUNCTION: 0x4a4930
void __stdcall ResetProgressBar(GameState_4a4930* param_1, int index)
{
    char* e = param_1->arr->base + index * 0x15b;
    *(int*)(e + 0xb6) = 0;
    *(int*)(e + 0xbe) = 0;
    *(int*)(e + 0xc2) = 0;
    *(short*)(e + 0xc6) = 0;
}
// Draws one gadget entry: builds the entry's bounding rect and a destination
// quad, then either blits a texture (field_be via GetGafFrame, or field_c2)
// onto it, or fills the rect with the colour at obj+0x8b9.

#pragma pack(push, 1)
struct Entry_004a4980 {
    unsigned char type;               // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                          // +0x13
    short y;                          // +0x15
    short w;                          // +0x17
    short h;                          // +0x19
    char unknown_1b[0xbc - 0x1b];
    union {
        void* surface;                // +0xbc (entry 0 only)
        char unknown_bc[0xc2 - 0xbc]; // +0xbc to +0xc1
    };
    void* field_c2;                   // +0xc2
    short field_c6;                   // +0xc6
    char unknown_c8[0x15b - 0xc8];
};
#pragma pack(pop)

struct Holder_004a4980 {
    char unknown_0[4];
    Entry_004a4980* entries;           // +0x04
};

#pragma pack(push, 1)
struct Dialog_4a4980 {
    char unknown_0[0x18];
    Holder_004a4980* holder;           // +0x18
    char unknown_1c[0x8b9 - 0x1c];
    unsigned char field_8b9;           // +0x8b9
};
#pragma pack(pop)

struct Point_004a4980 {
    int x;
    int y;
};

struct Quad_004a4980 {
    Point_004a4980 p[4];
};

struct Rect_004a4980 {
    int x1;
    int y1;
    int x2;
    int y2;
};

struct Frame_004a4980 {
    unsigned short w;                 // +0x00
    unsigned short h;                 // +0x02
    short field_4;                    // +0x04
    short field_6;                    // +0x06
    char unknown_8;                   // +0x08
    unsigned char field_9;            // +0x09
};

char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrame(void* surface, void* frame, int x, int y);
void __stdcall FillRectangle(void* surface, Rect_004a4980* rect, int color);
void __stdcall DrawFrameQuad(void* surf, void* entry, Quad_004a4980* dst, Quad_004a4980* src);

// FUNCTION: 0x4a4980
void __stdcall DrawHotspot(Dialog_4a4980* obj, int index)
{
    Entry_004a4980* entries = obj->holder->entries;
    Entry_004a4980* e = &entries[index];

    Rect_004a4980 rect;
    if (e->type == 0) {
        rect.x1 = 0;
        rect.y1 = 0;
    } else {
        rect.x1 = e->x;
        rect.y1 = e->y;
    }
    rect.x2 = e->w + rect.x1 - 1;
    rect.y2 = e->h + rect.y1 - 1;

    Quad_004a4980 dst;
    dst.p[0].x = rect.x1;
    dst.p[3].x = rect.x1;
    dst.p[0].y = rect.y1;
    dst.p[1].x = rect.x2;
    dst.p[1].y = rect.y1;
    dst.p[2].x = rect.x2;
    dst.p[2].y = rect.y2;
    dst.p[3].y = rect.y2;

    Quad_004a4980 src;
    src.p[0].x = 1;
    src.p[0].y = 1;
    src.p[3].x = 1;
    src.p[1].y = 1;

    void* field_be = *(void**)((char*)e + 0xbe);
    if (field_be != 0) {
        Frame_004a4980* result = (Frame_004a4980*)GetGafFrame(field_be, e->field_c6);
        if (result != 0) {
            src.p[1].x = result->w - 1;
            src.p[2].x = result->w - 1;
            src.p[2].y = result->h - 1;
            src.p[3].y = result->h - 1;
            if (result->field_9 == 0) {
                DrawFrameQuad(*(void**)((char*)entries + 0xbc), result, &dst, &src);
                return;
            }
            DrawFrame(*(void**)((char*)entries + 0xbc), result, result->field_4 + rect.x1, result->field_6 + rect.y1);
            return;
        }
    } else if (e->field_c2 != 0) {
        src.p[1].x = ((Frame_004a4980*)e->field_c2)->w - 1;
        src.p[2].x = ((Frame_004a4980*)e->field_c2)->w - 1;
        src.p[2].y = ((Frame_004a4980*)e->field_c2)->h - 1;
        src.p[3].y = ((Frame_004a4980*)e->field_c2)->h - 1;
        DrawFrameQuad(*(void**)((char*)entries + 0xbc), e->field_c2, &dst, &src);
    } else {
        FillRectangle(*(void**)((char*)entries + 0xbc), &rect, obj->field_8b9);
    }
}
// GUI hit test for menu entry `index` (0x15b-byte entries in the object's table
// at +0x18 -> +4). The entry's rectangle comes from its header (x,y,width,
// height; a type-0 header uses origin 0,0), the entry may have a callback at
// +0xb6, and bit 0 of +0xc8 enables mouse handling. A left click (or a right
// click when there is no left) selects the entry and stores 1/2 via
// SetClickMode; when the entry was already focused and HasMouseKeyFlags says no
// button of mask 3 is down, focus is cleared. Returns 1 when the click landed
// inside the rectangle of the entry whose focus was just cleared.
#pragma pack(push, 1)

struct Dialog_4a4b50;
struct Point_004a4b50 { int x, y; };

struct Entry_004a4b50 {                       // 0x15b bytes
    unsigned char type;                       // +0x0
    char unknown_1[0x13 - 0x1];
    short x;                                  // +0x13
    short y;                                  // +0x15
    short width;                              // +0x17
    short height;                             // +0x19
    char unknown_1b[0xb6 - 0x1b];
    void (__stdcall* callback)(Dialog_4a4b50*, Entry_004a4b50*);          // +0xb6
    char unknown_ba[0xc8 - 0xba];
    unsigned char flags;                      // +0xc8
    char unknown_c9[0x15b - 0xc9];
};

struct Table_004a4b50 {
    char unknown_0[4];
    Entry_004a4b50* entries;                  // +0x4
};

struct Dialog_4a4b50 {
    char unknown_0[0x18];
    Table_004a4b50* table;                    // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a4b50 pos;                       // +0x3c
    char unknown_44[0x64 - 0x44];
    int focus;                                // +0x64
};
#pragma pack(pop)

extern int __stdcall IsMouseButtonMessage(Dialog_4a4b50* obj, unsigned char buttons);
extern int __stdcall HasMouseKeyFlags(Dialog_4a4b50* obj, unsigned int mask);
extern void __stdcall SetClickMode(Dialog_4a4b50* obj, int value);
extern int __stdcall TrySetFocus(Dialog_4a4b50* obj, int index);

// FUNCTION: 0x4a4b50
int __stdcall HandleHotspotInput(Dialog_4a4b50* obj, int index)
{
    Entry_004a4b50* entries = obj->table->entries;
    // Entries are indexed as entries[index], not through a stored pointer.
    // 16-byte stack struct: gives the frame its size.
    struct Rect { int left, top, right, bottom; } r;
    if (entries[index].type == 0) {
        r.left = 0;
        r.top = 0;
    } else {
        r.left = entries[index].x;
        r.top = entries[index].y;
    }
    r.right = entries[index].width - 1 + r.left;
    r.bottom = entries[index].height - 1 + r.top;
    if (entries[index].callback)
        entries[index].callback(obj, &entries[index]);
    if (entries[index].flags & 1) {
        if (IsMouseButtonMessage(obj, 1)) {
            // Position copied into a local Point before each hit test.
            Point_004a4b50 p = obj->pos;
            if (p.x >= r.left && p.x <= r.right && p.y >= r.top && p.y <= r.bottom) {
                TrySetFocus(obj, index);
                SetClickMode(obj, 1);
            }
        } else if (IsMouseButtonMessage(obj, 2)) {
            Point_004a4b50 p = obj->pos;
            if (p.x >= r.left && p.x <= r.right && p.y >= r.top && p.y <= r.bottom) {
                TrySetFocus(obj, index);
                SetClickMode(obj, 2);
            }
        }
        if (obj->focus == index && !HasMouseKeyFlags(obj, 3)) {
            obj->focus = -1;
            // Read through a pointer so the two loads stay after the focus store.
            Point_004a4b50* pp = &obj->pos;
            int py2 = pp->y;
            int px2 = pp->x;
            if (px2 >= r.left && px2 <= r.right && py2 >= r.top && py2 <= r.bottom)
                return 1;
        }
    }
    return 0;
}
// Draws one side of a GUI entry's rectangle when bit 0 of param_3 is set; the
// side is chosen by bits 0/1/2 of the entry's flags and the colour comes from
// the index `(int)obj + 0x8b2` into the entry's colour table at +0x1f.

#pragma pack(push, 1)
struct Entry_004a4c90 {                // 0x15b bytes
    char type;                         // +0x00
    char unknown_1[0x12];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int flags;                         // +0x1b
    unsigned char* colours;            // +0x1f
    char unknown_23[0xbc - 0x23];
    void* surface;                     // +0xbc
    char unknown_c0[0x15b - 0xc0];
};
#pragma pack(pop)

struct Holder_004a4c90 {
    char unknown_0[4];
    Entry_004a4c90* entries;           // +0x4
};

struct Dialog_4a4c90 {
    char unknown_0[0x18];
    Holder_004a4c90* holder;           // +0x18
};

struct Rect_004a4c90 {
    int x1, y1, x2, y2;
};

// The colour parameter must be int, not unsigned char: forces a zero-extending load.
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2,
                            int color);

static inline void FillRect_004a4c90(Entry_004a4c90* e, Rect_004a4c90* r)
{
    if (e->type == 0) {
        r->x1 = 0;
        r->y1 = 0;
    } else {
        r->x1 = e->x;
        r->y1 = e->y;
    }
    r->x2 = e->w - 1 + r->x1;
    r->y2 = e->h - 1 + r->y1;
}

// FUNCTION: 0x4a4c90
void __stdcall DrawLineGadget(Dialog_4a4c90* obj, int index, unsigned char param_3)
{
    Entry_004a4c90* entries = obj->holder->entries;
    Entry_004a4c90* e = (Entry_004a4c90*)((char*)entries + index * 0x15b);
    Rect_004a4c90 rect;
    FillRect_004a4c90(e, &rect);
    if (param_3 & 1) {
        if (e->flags & 1)
            DrawLine(entries->surface, rect.x1, rect.y1, rect.x2, rect.y1,
                         e->colours[(int)obj + 0x8b2]);
        else if (e->flags & 2)
            DrawLine(entries->surface, rect.x1, rect.y1, rect.x1, rect.y2,
                         e->colours[(int)obj + 0x8b2]);
        else if (e->flags & 4)
            DrawLine(entries->surface, rect.x1, rect.y1, rect.x2, rect.y2,
                         e->colours[(int)obj + 0x8b2]);
    }
}
// Draws one list-gadget
// entry: makes the entry's language current, fills or blits its rectangle,
// draws its text, and when the entry has the focus draws the text cursor (a
// vertical line) after the text up to the cursor position.
//
// The +0x1f field is an int colour index into the object's colour table at
// +0x8b2 (`obj->colours[entry->colours]`). The byte saved, zeroed and restored
// around the width measurement is `text[obj->cursor]`, the character at the
// cursor, so the measured width is that of the text before the cursor.

#pragma pack(push, 1)
struct Entry_004a4d70 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    unsigned char align;               // +0x1b
    char unknown_1c[0x1f - 0x1c];
    int colours;                       // +0x1f, index into Class::colours
    char unknown_23[0x28 - 0x23];
    char tab;                          // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0xbc - 0xb6];        // +0xb6
    } b6;
    void* surface;                     // +0xbc
    char unknown_c0[0xd6 - 0xc0];
    int language;                      // +0xd6
    char unknown_da[0x15b - 0xda];
};

struct List_004a4d70 {
    char unknown_0[0x0c];
    unsigned short* glyphs;            // +0x0c
};

struct Holder_004a4d70 {
    int current;                       // +0x00
    Entry_004a4d70* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a4d70* language;           // +0x14
    char unknown_18[0x24 - 0x18];
    void* surface;                     // +0x24
};

struct Dialog_4a4d70 {
    char unknown_00[0x18];
    Holder_004a4d70* holder;           // +0x18
    char unknown_1c[0x64 - 0x1c];
    int focus;                         // +0x64
    char unknown_68[0x74 - 0x68];
    int cursor;                        // +0x74
    char unknown_78[0x8b2 - 0x78];
    unsigned char colours[0xcd2 - 0x8b2]; // +0x8b2
    void* fallback;                    // +0xcd2
};

struct Rect_004a4d70 { int left, top, right, bottom; };
struct Glyph_004a4d70 { unsigned short width, height; };

struct LanguageRoot_004a4d70 {
    int current;                       // +0x00
    char unknown_04[0x14 - 0x04];
    List_004a4d70* language;           // +0x14
};
#pragma pack(pop)

void __stdcall SetFont(int id);
int GetFont();
int __stdcall GetTextWidth(int font, char* text);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall SetTextColors(int colour, int font);
int GetTextKeyColor();
int __stdcall DrawListboxFrame(Dialog_4a4d70* obj, int index, void* bmp);
void __stdcall CopySurfaceRect(void* dst, void* src, Rect_004a4d70* rect, int* pos);
int __stdcall FillRectangle(void* surface, Rect_004a4d70* rect, int colour);
void __stdcall DrawTextClipped(void* surface, char* text, int x, int y, int maxw, int style);
// The colour parameter must be int: forces the zero extension of the colour load.
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2,
                            int colour);

static inline Glyph_004a4d70* GetGlyph_004a4d70(unsigned char c)
{
    return (Glyph_004a4d70*)GetGafFrame(g_guiContext->language->glyphs, c);
}

static inline int Measure_004a4d70(char* text)
{
    int width = 0;
    char* p = text;
    if (p == 0)
        return 0;
    if (g_guiContext->language == 0)
        return GetTextWidth(GetFont(), text);
    char* q = text;
    while (*q != 0) {
        char ch = *q;
        Glyph_004a4d70* glyph = GetGlyph_004a4d70(ch);
        if (glyph != 0)
            width += glyph->width;
        ++q;
    }
    return width;
}

// FUNCTION: 0x4a4d70
void __stdcall DrawTextInput(Dialog_4a4d70* obj, int index)
{
    Entry_004a4d70* entries = obj->holder->entries;
    int i = 1;
    int t = 0;
    for (; i < entries->b6.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entries[index].tab) {
                SetFont(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries->b6.count + 1)
        SetFont(g_guiContext->current);

    Entry_004a4d70* me = &entries[index];

    Rect_004a4d70 rect;
    if (me->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = me->x;
        rect.top = me->y;
    }
    rect.right = me->w + rect.left - 1;
    rect.bottom = me->h + rect.top - 1;

    if (me->align & 1) {
        FillRectangle(entries->surface, &rect, obj->colours[0]);
    } else {
        void* surface = obj->holder->surface;
        if (surface == 0)
            surface = obj->fallback;
        if (surface == 0) {
            DrawListboxFrame(obj, index, 0);
        } else {
            CopySurfaceRect(entries->surface, surface, &rect, (int*)&rect);
        }
    }

    SetTextColors(obj->colours[me->colours], GetTextKeyColor());
    rect.top += 3;
    // The style is read as entries[index].colours rather than me->colours:
    // sharing one load with the colour read above swaps the SIB registers of
    // that read (see the 0x4a4d70 entry in docs/field-notes.md, Part 5).
    DrawTextClipped(entries->surface, me->b6.text, rect.left, rect.top,
                 rect.right - rect.left, entries[index].colours);

    if (index == obj->focus) {
        char* at = &me->b6.text[obj->cursor];
        char save = *at;
        *at = 0;
        int w = Measure_004a4d70(me->b6.text);
        *at = save;
        int height;
        if (g_guiContext->language == 0)
            height = GetFontHeight();
        else
            // Via GetGlyph, like every glyph fetch: keeps the width in esi.
            height = GetGlyph_004a4d70(0x49)->height + 2;
        int x = rect.left + w;
        // Arguments stay expressions; only x is a local.
        DrawLine(entries->surface, x, rect.top, x, height + rect.top, obj->colours[9]);
    }
}
// The glyph of a character in the current font (see 0x4a5030).

struct Font_004a5010 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a5010 {
    char unknown_0[0x14];
    Font_004a5010* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
// FUNCTION: 0x4a5010
void* __stdcall GetCharGlyph(unsigned char c)
{
    return GetGafFrame(g_guiContext->font->glyphs, c);
}
// Width of a string in pixels: the sum of the glyph widths of the current
// font, or GetTextWidth's measurement when no font is loaded.

struct Font_004a5030 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a5030 {
    char unknown_0[0x14];
    Font_004a5030* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFont();
int __stdcall GetTextWidth(int param_1, unsigned char* text);

// FUNCTION: 0x4a5030
int __stdcall GetTextPixelWidth(unsigned char* text)
{
    int width = 0;
    if (text == 0)
        return 0;
    if (g_guiContext->font == 0)
        return GetTextWidth(GetFont(), text);
    // Index text[i] and hold the character in its own local, not a walked pointer.
    for (int i = 0; text[i]; i++) {
        unsigned char c = text[i];
        unsigned short* glyph = (unsigned short*)GetGafFrame(g_guiContext->font->glyphs, c);
        if (glyph)
            width += *glyph;
    }
    return width;
}
// Line height of the current font: the height of the 'I' glyph plus 2, or
// GetFontHeight's value when no font is loaded.

struct Glyph_004a50b0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a50b0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a50b0 {
    char unknown_0[0x14];
    Font_004a50b0* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFontHeight();

// FUNCTION: 0x4a50b0
int GetFontLineHeight()
{
    if (g_guiContext->font == 0) {
        return GetFontHeight();
    }
    return ((Glyph_004a50b0*)GetGafFrame(g_guiContext->font->glyphs, 'I'))->height + 2;
}
struct Glyph_004a50e0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a50e0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a50e0 {
    char unknown_0[0x14];
    Font_004a50e0* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrame(void* surface, void* glyph, int x, int y);
void __stdcall DrawFrameLit(void* surface, void* glyph, int x, int y, int style);
void __stdcall DrawString(void* surface, char* text, int x, int y, int maxWidth);

// FUNCTION: 0x4a50e0
void __stdcall DrawTextClipped(void* surface, char* text, int x, int y, int maxw, int style)
{
    if (g_guiContext->font == 0) {
        DrawString(surface, text, x, y, -1);
        return;
    }
    unsigned char* s = (unsigned char*)text;
    while (*s) {
        if (*s >= ' ') {
            unsigned char c = *s;
            Glyph_004a50e0* g = (Glyph_004a50e0*)GetGafFrame(g_guiContext->font->glyphs, c);
            if (g) {
                if (maxw != -1 && (int)g->width > maxw)
                    return;
                if (*s != ' ') {
                    if (style == 0)
                        DrawFrame(surface, g, x, y);
                    else
                        DrawFrameLit(surface, g, x, y, style);
                }
                if (maxw != -1) {
                    maxw -= g->width;
                    if (maxw < 0)
                        return;
                }
                x += g->width;
            }
        }
        s++;
    }
}
// Started by Space Bunny Free (partial, 80.1%); finished by deepseek-v4.1-flash.

struct Glyph_004a51d0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a51d0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a51d0 {
    char unknown_0[0x14];
    Font_004a51d0* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFont();
int __stdcall GetTextWidth(int a, char* text);
int GetFontHeight();
void __stdcall DrawTextClipped(char* dest, char* text, int p3, int x, int maxw, int style);

static inline int LineHeight_004a50b0()
{
    if (g_guiContext->font == 0)
        return GetFontHeight();
    return (int)((Glyph_004a51d0*)GetGafFrame(g_guiContext->font->glyphs, 'I'))->height + 2;
}

static inline int Measure(char* word, int t)
{
    if (word == 0)
        return t;
    if (g_guiContext->font == 0)
        return GetTextWidth(GetFont(), word);
    for (char* n = word; *n; n++) {
        unsigned char ch = *n;
        unsigned short* g = (unsigned short*)GetGafFrame(g_guiContext->font->glyphs, ch);
        if (g)
            t += *g;
    }
    return t;
}

// FUNCTION: 0x4a51d0
int __stdcall DrawWrappedText(char* p2, char* text, int p4, int y, int maxw, int rem, int p7)
{
    int last = 0;
    int w;
    int k = 0;
    int i = 0;
    int len = strlen(text);
    while (text[k] != 0) {
        while (i != len && text[i] != ' ' && text[i] != '\r')
            i++;
        char saved = text[i];
        char* word = text + k;
        text[i] = 0;
        w = 0;
        w = Measure(word, w);
        if (w > maxw) {
            text[i] = saved;
            i = last;
            saved = text[i];
            text[i] = 0;
        } else if (saved != '\r' && i != len) {
            last = i;
            text[i] = saved;
            i++;
            continue;
        }
        DrawTextClipped(p2, word, p4, y, maxw, p7);
        text[i] = saved;
        y += LineHeight_004a50b0() + 2;
        rem -= LineHeight_004a50b0() + 2;
        if (text[i] == 0)
            goto done;
        k = i + 1;
        if (rem <= 0)
            goto done;
        last = k;
        i = k;
    }
done:
    return y;
}


#pragma pack(push, 1)

struct Glyph {                          // 8 bytes, returned by GetGafFrame
    unsigned short width;               // +0x00
    unsigned short height;              // +0x02
    short xoff;                         // +0x04
    short yoff;                         // +0x06
};

struct GafEntry {                       // 4 bytes: frame table header
    unsigned short count;               // +0x00
    unsigned short unknown_2;           // +0x02
};

struct Gui;

struct Gadget {                         // 0x15b bytes, one GUI list entry
    unsigned char type;                 // +0x00
    unsigned char team;                 // +0x01
    char name[0x11];                    // +0x02 (strncpy 0x10)
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    int colours;                        // +0x1f
    int image;                          // +0x23
    char unknown_27;
    signed char tab;                    // +0x28
    unsigned char field_29;             // +0x29
    char unknown_2a;
    void* archive;                      // +0x2b
    GafEntry* gaf;                      // +0x2f
    char helpKey[0xb4 - 0x33];          // +0x33
    unsigned char resourceFlags;        // +0xb4
    char unknown_b5;
    union {
        short count;                    // +0xb6 (entry 0 only)
        char text[0x80];                // +0xb6
        struct {                        // entry 0
            char unknown_b6[2];
            void* saveUnder;            // +0xb8, the SAVE UNDER bitmap
            void* surface;              // +0xbc, the GUI SURFACE bitmap
            void* archive;              // +0xc0
            GafEntry* background;       // +0xc4
        } assets;
        struct {                        // type 12
            char unknown_b6[2];
            Glyph* glyph;               // +0xb8
            unsigned char flag;         // +0xbc
        } frame;
        struct {                        // type 2
            int sortKey;                // +0xb6
            short field_ba;             // +0xba
            short field_bc;             // +0xbc
            short field_be;             // +0xbe
            short field_c0;             // +0xc0
            char* field_c2;             // +0xc2
            char unknown_c6[4];
            GafEntry* gaf;              // +0xca
            void (__stdcall* callback)(Gui*, Gadget*);  // +0xce
            char unknown_d2[4];
            union {
                int language;           // +0xd6
                void* filebuf;          // +0xd6, buffer type 7/8 loads
            };
            short scroll;               // +0xda
        } list;
        struct {                        // entry 0
            char unknown_b6[0x16];
            char choice[0x10];          // +0xcc
            char choice2[0x10];         // +0xdc
        } names;
        struct {                        // type 6
            int f_b6;
            int f_ba;
            int f_be;
            int f_c2;
            short f_c6;
        } t6;
        struct {                        // type 13
            char unknown_b6[4];
            int field_ba;               // +0xba
            int field_be;               // +0xbe
            int field_c2;               // +0xc2
            int field_c6;               // +0xc6
            float field_ca;             // +0xca
            int field_ce;               // +0xce
            int field_d2;               // +0xd2
        } anim;
    } u;
    union {                             // +0x136
        short field_136;
        struct {
            unsigned char stage;        // +0x136
            unsigned char stageIndex;   // +0x137
        };
    };
    short field_138;                    // +0x138 (the text length limit of a text input)
    union {                             // +0x13a
        struct {
            unsigned char field_13a;
            unsigned char field_13b;
            unsigned char field_13c;
            char unknown_13d;
        };
        GafEntry* inputGaf;
    };
    union {                             // +0x13e
        struct {
            char unknown_13e[0x142 - 0x13e];
            short sliderThumb;          // +0x142
            char unknown_144[0x14e - 0x144];
            GafEntry* sliderGaf;        // +0x14e
            unsigned char sliderStyle;  // +0x152
        };
        struct {
            char unknown_13e_b[2];
            short field_140;            // +0x140
            char unknown_142_b[2];
            void (__stdcall* callback)(Gui*, int);      // +0x144
            short unknown_148;
            int callbackArg;            // +0x14a
        };
        struct {
            char unknown_13e_c[0x147 - 0x13e];
            unsigned char field_147;    // +0x147
            unsigned char field_148;    // +0x148
        };
    };
    char unknown_153[0x157 - 0x153];
    int field_157;                      // +0x157
};

struct Layer {                          // a screen on the stack
    Layer* next;                        // +0x00
    Gadget* entries;                    // +0x04
    void (__stdcall* handler)(Gui*);    // +0x08
    char unknown_0c[4];
    unsigned int flags;                 // +0x10
    int dirty;                          // +0x14
    int keyboardInput;                  // +0x18
    void (__stdcall* cb1c)();           // +0x1c
    int current;                        // +0x20 (entry index, -1 for none)
    void* surface;                      // +0x24
    char text[0xe];                     // +0x28
    char lastKey;                       // +0x36
    char unknown_37[0x3b - 0x37];
    void (__stdcall* cb3b)(Gui*);       // +0x3b
};

struct Language {
    char unknown_0[0xc];
    GafEntry* glyphs;                   // +0x0c
};

struct Point {                          // 24 bytes, copied with rep movsd
    int x;
    int y;
    int unknown_08[4];
};

struct Gui {
    int font;                           // +0x00
    void* gaf;                          // +0x04
    Language* values[3];                // +0x08
    Language* language;                 // +0x14
    Layer* layer;                       // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point point;                        // +0x3c
    char unknown_54[0x60 - 0x54];
    int hotGadgetIndex;                 // +0x60
    int focus;                          // +0x64
    int hoverGadgetIndex;               // +0x68
    int prevHoverGadget;                // +0x6c
    int clearQuickKeys;                 // +0x70
    char unknown_74[0x78 - 0x74];
    int knobDragging;                   // +0x78
    char unknown_7c[0x96 - 0x7c];
    int time;                           // +0x96
    int animTimer;                      // +0x9a
    int inputEnabled;                   // +0x9e
    int dirty;                          // +0xa2 (nonzero = selection active)
    char unknown_a6[0x8b2 - 0xa6];
    unsigned char colours[0x100];       // +0x8b2
    char unknown_9b2[0x9b6 - 0x9b2];
    char str_9b6[0x100];                // +0x9b6
    char str_ab6[0x100];                // +0xab6
    char str_bb6[0x110];                // +0xbb6
    int pathsReady;                     // +0xcc6
    int changed;                        // +0xcca
    int clickStatusCache;               // +0xcce
    int fallback;                       // +0xcd2
    char cachedBgName;                  // +0xcd6
};

#pragma pack(pop)

extern char DAT_00502a20[];
extern char DAT_005119b8[];
extern int DAT_0051fbac;
extern int DAT_0051fbb0;
extern int DAT_0051fbb4;

int GetScreenWidth();
int GetScreenHeight();
unsigned int __cdecl GetTicks();
int PeekKey();
int PopKey();
int __stdcall IsKeyDown(int key);
void ClearKeyQueue();
void HideSoftwareCursor();
void ShowSoftwareCursor();
int __cdecl tolower(int c);
int __cdecl toupper(int c);

void __stdcall SetFont(int id);
int GetFont();
int __stdcall GetTextWidth(int font, char* text);
int GetFontHeight();
void __stdcall SetTextColors(int colour, int font);
int GetTextKeyColor();
Glyph* __stdcall GetGafFrame(GafEntry* table, int index);
GafEntry* __stdcall FindGafEntry(void* gaf, const char* name);
void* __stdcall LoadGaf(char* path);
char* __stdcall ChangeExtension(char* out, char* in, const char* ext);
long __stdcall HAPI_FileLengthByName(char* path);
char* __stdcall HAPI_LoadFile(char* name, int* size);
char* __stdcall Translate(char* key);
char* __stdcall GetGadgetText(Gui* menu, char* name, char* buf);
char* __stdcall SkipTextLines(char* text, int line);

void* __stdcall AllocSurface(char* name, int width, int height);
void __stdcall FreeSurface(void* surface);
void __stdcall DrawSurface(void* dst, void* bmp, int x, int y);
void __stdcall DrawString(void* surface, char* text, int x, int y, int maxw);
int __stdcall FillRectangle(void* surface, Rect* rect, int colour);
void __stdcall GrayRectangle(void* surface, Rect* rect);
void __stdcall FadeRectangle(void* surface, Rect* rect, int level);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int colour);
void __stdcall DrawFrame(void* surface, Glyph* glyph, int x, int y);
void __stdcall DrawFrameLit(void* surface, Glyph* glyph, int x, int y, int style);
void __stdcall FillBevelBox(void* surface, Rect* rect, unsigned int a, unsigned int b, unsigned int c);
void __stdcall FillBevelBoxDarkFirst(void* surface, Rect* rect, unsigned int a, unsigned int b, unsigned int c);
void __stdcall DrawTextClipped(void* surface, char* text, int x, int y, int maxw, int style);
int __stdcall DrawWrappedText(void* surface, char* text, int x, int y, int maxw, int rem, int style);

void __stdcall AssignQuickKey(Gui* obj, int index);
void __stdcall ClearPeerStatus(Gui* obj, int index);
void __stdcall DrawGadgetSelectionBox(Gui* obj, int index, int param3);
void __stdcall DrawSliderBar(Gui* obj, int index);
void __stdcall SyncAssocGadgets(Gui* obj, int index);
void __stdcall SetListBoxScrollByName(Gui* obj, char* name, int line);
void __stdcall DrawListBox(Gui* obj, int index);
void __stdcall DrawSlider(Gui* obj, int index);
void __stdcall DrawTextInput(Gui* obj, int index);
void __stdcall DrawListboxFrame(Gui* obj, int index, void* bmp);
void __stdcall DrawProgressBarGadget(Gui* obj, int index);
void __stdcall DrawHotspot(Gui* obj, int index);
void __stdcall DrawLineGadget(Gui* obj, int index, unsigned int param3);
int __stdcall HandleLabelInput(Gui* obj, int index, int key);
int __stdcall HandleHotspotInput(Gui* obj, int index);
int __stdcall TrySetFocus(Gui* obj, int index);
int __stdcall IsMouseButtonMessage(Gui* obj, unsigned char buttons);
int __stdcall HasMouseKeyFlags(Gui* obj, unsigned int mask);
void __stdcall SetClickMode(Gui* obj, int value);
void __stdcall CommitTextEdit(Gui* obj, int index, char* text, int maxLength, int clear);
void __stdcall UpdateCursorAndMouse(Gui* obj);
void __stdcall SetCursorHover(Gui* obj, int inside);
int __stdcall HandleListBoxInput(Gui* obj, int index, int key);
void __stdcall HandleSliderInput(Gui* obj, int index);
int __stdcall HandleTextEditKey(Gui* obj, int index, int key);
void __cdecl GameFreeThunk(void* p);
int __stdcall SelectFontForEntry(Gadget* entries, int index);

// The real GetTextPixelWidth (0x4a5030), which /Ob2 inlines into its callers here.
static inline int GetTextPixelWidth(char* text)
{
    int width = 0;
    if (text == 0)
        return 0;
    if (g_guiContext->language == 0)
        return GetTextWidth(GetFont(), text);
    char* p = text;
    while (*p != 0) {
        char ch = *p;
        Glyph* glyph = GetGafFrame(g_guiContext->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++p;
    }
    return width;
}

static inline int LineHeight()
{
    if (g_guiContext->language == 0)
        return GetFontHeight();
    Glyph* glyph = GetGafFrame(g_guiContext->language->glyphs, 0x49);
    return glyph->height + 2;
}

// The same height without the named glyph local; DrawLabel needs this form.
static inline int LineHeightDirect()
{
    if (g_guiContext->language == 0)
        return GetFontHeight();
    return (int)GetGafFrame(g_guiContext->language->glyphs, 0x49)->height + 2;
}

static inline int FindEntry(Gadget* entries, char* name)
{
    for (int i = 1; i < entries->u.count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// Walks the entry list of a layout object looking for the n-th tab stop
// (entries whose +0x00 byte is 7), sets the language from that entry, computes
// the line height, then lays the entry's text out right aligned (+0x1b bit 2),
// centred (bit 1) or left at its measured width (bit 0), writing the new x,
// width and line height back into the entry. +0xb6 is a union (count on entry
// 0, NUL terminated text elsewhere); +0x1b is a 4-byte field.
// FUNCTION: 0x4a53c0
void __stdcall LayoutLabelText(Gui* obj, int index)
{
    Gadget* entries = obj->layer->entries;
    Gadget* entry = &entries[index];
    int i;
    int t = 0;
    for (i = 1; i < entries[0].u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entry->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            t++;
        }
    }
    if (i == entries[0].u.count + 1)
        SetFont(g_guiContext->fontId);
    int x = !entry->type ? 0 : entry->x;
    // Real variable declared here, assigned in each arm, one shared tail stores it.
    int nx = x;
    int lh;
    if (g_guiContext->language == 0)
        lh = GetFontHeight();
    else
        lh = GetGafFrame(g_guiContext->language->glyphs, 0x49)->height + 2;
    if (entry->flags & 4) {
        nx = entry->w + x;
        nx -= GetTextPixelWidth(entry->u.text);
    } else if (entry->flags & 2) {
        nx = entry->w / 2 + x;
        int half = GetTextPixelWidth(entry->u.text) / 2;
        nx -= half;
        // No named local for the width: it changes how lh is loaded.
        entry->w = (short)(half * 2);
    } else if (entry->flags & 1)
        entry->w = (short)GetTextPixelWidth(entry->u.text);
    // Stored via entries[index], not entry: pins the store order of x, w and h.
    entries[index].x = (short)nx;
    entry->h = (short)lh;
}

// FUNCTION: 0x4a56b0
void __stdcall DrawLabel(Gui* obj, int index)
{
    obj->language = obj->values[1];
    Gadget* entries = obj->layer->entries;

    int i = 1;
    int t = 0;
    for (; i < entries[0].u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entries[index].tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            t++;
        }
    }
    if (i == entries[0].u.count + 1) {
        SetFont(g_guiContext->fontId);
        i = -1;
    }


    if (entries[index].x == -1)
        entries[index].x = (short)((entries[0].w - GetTextPixelWidth(entries[index].u.text)) / 2);

    Rect rect;
    if (entries[index].type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = entries[index].x;
        rect.top = entries[index].y;
    }
    rect.right = entries[index].w + rect.left - 1;
    rect.bottom = entries[index].h + rect.top - 1;

    if (entries[index].image != 0)
        FillRectangle(entries->u.assets.surface, &rect, obj->colours[entries[index].image]);
    int nx = rect.left;
    Gadget* entry = &entries[index];

    if (entry->flags & 4) {
        nx = entry->w + rect.left;
        nx -= GetTextPixelWidth(entry->u.text);
    } else if (entry->flags & 2) {
        nx = entry->w / 2 + rect.left;
        int half = GetTextPixelWidth(entry->u.text) / 2;
        nx -= half;
    }

    if (i != -1 && (entries[index].flags & 8)) {
        SetTextColors(obj->colours[0], GetTextKeyColor());
        DrawString(entries->u.assets.surface, entries[index].u.text, nx + 1, rect.top + 3, -1);
    }

    SetTextColors(entries[index].colours, GetTextKeyColor());

    if (i == -1) {
        int lh = LineHeightDirect();
        if (rect.bottom - rect.top > lh * 2)
            DrawWrappedText(entries->u.assets.surface, entries[index].u.text, nx, rect.top,
                         rect.right - rect.left + 1,
                         rect.bottom - rect.top + 1, entries[index].colours);
        else
            DrawTextClipped(entries->u.assets.surface, entries[index].u.text, nx, rect.top,
                         rect.right - rect.left + 1, entries[index].colours);
    } else {
        DrawString(entries->u.assets.surface, entries[index].u.text, nx, rect.top, -1);
    }

    if (entries[index].field_148 & 1) {
        Gadget* entries2 = obj->layer->entries;
        Rect rect2;
        if (entries2[index].type == 0) {
            rect2.left = 0;
            rect2.top = 0;
        } else {
            rect2.left = entries2[index].x;
            rect2.top = entries2[index].y;
        }
        rect2.right = entries2[index].w + rect2.left - 1;
        rect2.bottom = entries2[index].h + rect2.top - 1;
        GrayRectangle(entries2->u.assets.surface, &rect2);
        FadeRectangle(entries2->u.assets.surface, &rect2, -0x14);
    } else {
        unsigned char c = entries[index].field_147;
        if (c != 0) {
            char pat[2];
            pat[0] = (char)c;
            pat[1] = 0;
            char buf[0x80];
            strcpy(buf, entries[index].u.text);
            char* p = strstr(buf, pat);
            if (p != 0) {
                int y = rect.top;
                *p = 0;
                // Both x positions accumulate in x0, with x1 copied off it.
                int x0 = rect.left;
                x0 += GetTextPixelWidth(buf);
                int x1 = x0;
                x0 += GetTextPixelWidth(pat);
                int lh1 = LineHeightDirect();
                int lh2 = LineHeightDirect();
                DrawLine(obj->layer->entries->u.assets.surface, x1, lh2 + y - 1, x0 - 1,
                             lh1 + y - 1, obj->colours[2]);
            }
        }
    }

    // One tail after both arms of the field_148 if/else, not a copy per exit.
    obj->language = obj->values[0];
}

// FUNCTION: 0x4a5d30
void __stdcall SetCurrentFont(Gui* p, int index)
{
    p->language = p->values[index];
}

// FUNCTION: 0x4a5d50
int __stdcall TruncateGadgetText(Gui* menu, int index)
{
    Gadget* entries = menu->layer->entries;
    char* text = GetGadgetText(menu, entries[index].name, 0);
    if (text == 0)
        return 0;
    if (entries[index].type == 5 ||
        (entries[index].type == 1 && (entries[index].flags & 0x8000) != 0))
        menu->language = menu->values[1];
    while (1) {
        int w = GetTextPixelWidth(text);
        if (w <= entries[index].w - 6) {
            menu->language = menu->values[0];
            return w;
        }
        if (strlen(text) == 0)
            continue;
        text[strlen(text) - 1] = 0;
    }
}

// Draws GUI layout entry `index`: it turns on the manager's redraw flag, then
// blits the entry's glyph at the entry's rectangle offset, with the extra
// style argument when the entry's +0x1f count is positive, and finally fades
// the same rectangle when entry flag +0xbc has bit 0 set. The glyph's own x/y
// come from the loaded glyph's +4/+6.
//
// The +0xbc field is a union because entry 0 holds the
// destination surface pointer there while every other entry holds flag bits.
// FUNCTION: 0x4a5e50
void __stdcall DrawGadgetGlyph(Gui* obj, int index)
{
    if (obj->layer != 0)
        obj->layer->dirty = 1;
    Gadget* entries = obj->layer->entries;
    Gadget* e = &entries[index];
    Glyph* glyph = e->u.frame.glyph;
    if (glyph == 0)
        return;
    // One struct local, not four scalars: keeps right/bottom stored and the frame size.
    Rect rect;
    if (e->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = e->x;
        rect.top = e->y;
    }
    rect.right = e->w + rect.left - 1;
    rect.bottom = e->h + rect.top - 1;
    int count = e->colours;
    if (count > 0) {
        DrawFrameLit(entries->u.assets.surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top, count);
    } else {
        DrawFrame(entries->u.assets.surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top);
    }
    if (e->u.frame.flag & 1) {
        FadeRectangle(entries->u.assets.surface, &rect, -0x1c);
    }
}

// Draws one list-gadget entry: its glyph or frame, then the text (left,
// right, centred, or centred with an underlined hotkey letter, flags 1/4/2/0x20).
// Needed with the named glyph local in LineHeight: sets the flags 0x20 registers.
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
struct Pad_a5f40_0 { int field; };

// FUNCTION: 0x4a5f40
void __stdcall DrawButton(Gui* menu, int index)
{
    char* p;
    char key2[2];
    void* surface;
    Gadget* me;
    // Declaration order matters: rect before flagy and t, textw last.
    Rect rect;
    int x;
    int width;
    int border;
    int flagy;
    char* text;
    int pass;
    int saved;
    char buf[0x80];
    int y, i;
    int t;
    int textw;

    border = 0;
    if (menu->layer != 0)
        menu->layer->dirty = 1;
    Gadget* entries = menu->layer->entries;
    me = &entries[index];
    if (me->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = me->x;
        rect.top = me->y;
    }
    rect.right = me->w + rect.left - 1;
    rect.bottom = rect.top + me->h - 1;
    if (me->flags & 0x8000)
        menu->language = menu->values[1];

    // Own counter, not t: it shares the slot of t.
    int tab = 0;
    for (i = 1; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (tab == me->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            tab++;
        }
    }
    if (i == entries->u.count + 1)
        SetFont(g_guiContext->fontId);

    textw = TruncateGadgetText(menu, index);
    surface = entries->u.assets.surface;
    if (me->gaf != 0) {
        Glyph* glyph;
        if (me->field_13c & 1) {
            if (me->flags & 0x100) {
                glyph = GetGafFrame(me->gaf, me->gaf->count - 1);
            } else if (me->stage != 0) {
                glyph = GetGafFrame(me->gaf, me->stageIndex);
                border = 1;
            } else if (me->flags & 0x1800) {
                glyph = GetGafFrame(me->gaf, me->field_13b);
                border = 1;
            } else {
                int val = me->gaf->count - 1;
                if (me->field_138 + 2 < val)
                    val = me->field_138 + 2;
                glyph = GetGafFrame(me->gaf, val + me->field_13b);
                if (!(me->flags & 0x80))
                    border = 1;
            }
        } else {
            if (me->field_138 != 0 && (unsigned short)me->gaf->count > (unsigned short)me->stage) {
                if (me->stage != 0)
                    glyph = GetGafFrame(me->gaf, me->gaf->count - 2);
                else
                    glyph = GetGafFrame(me->gaf, me->field_13b + me->field_138);
            } else if (me->stage != 0)
                glyph = GetGafFrame(me->gaf, me->stageIndex);
            else
                glyph = GetGafFrame(me->gaf, me->field_13b);
        }
        if (glyph != 0) {
            if (me->colours != 0)
                DrawFrameLit(surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top, me->colours);
            else
                DrawFrame(surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top);
        }
    } else {
        if (me->field_13c & 1) {
            FillBevelBox(surface, &rect, menu->colours[0], menu->colours[0x13], menu->colours[0x13]);
        } else if (me->field_138 != 0) {
            FillBevelBox(surface, &rect, menu->colours[0], menu->colours[0x11], menu->colours[0x14]);
        } else {
            FillBevelBoxDarkFirst(surface, &rect, menu->colours[0], menu->colours[0x11], menu->colours[0x14]);
        }
    }

    t = 0;
    flagy = 0;
    if (me->field_138 != 0) {
        t = 1;
        flagy = 1;
    }
    text = me->u.text;
    pass = 0;
    do {
        if (me->field_138 != 0)
            SetTextColors(menu->colours[0], GetTextKeyColor());
        else
            SetTextColors(menu->colours[me->colours], GetTextKeyColor());

        p = text;
        if (me->stage != 0) {
            for (unsigned int k = me->stageIndex; k != 0; k--) {
                while (*p != 0)
                    p++;
                p++;
            }
        }

        y = (rect.bottom - LineHeight() - rect.top) / 2 + flagy + rect.top;
        if (me->flags & 0x8000)
            menu->language = menu->values[1];

        if (me->flags & 1) {
            DrawTextClipped(surface, p, t + rect.left + 3, y,
                         rect.right - rect.left + 1, 0);
        } else if (me->flags & 4) {
            x = rect.right - textw - 3;
            if (x < rect.left)
                x = rect.left;
            DrawTextClipped(surface, p, x, y, rect.right - rect.left + 1, 0);
        } else if (me->flags & 2) {
            x = (rect.right - textw - rect.left) / 2 + t;
            x += rect.left + 1;
            if (me->field_13a == 0 || (me->field_13c & 1)) {
                DrawTextClipped(surface, p, x, y, 1 + (rect.right - rect.left), 0);
            } else {
                // Declared in this block: key1[1] = 0 hoists into the inlined strcpy.
                char key1[2];
                // Each hotkey branch declares its own found: one frame slot.
                char* found;
                key1[0] = me->field_13a;
                width = rect.right - rect.left + 1;
                strcpy(buf, p);
                key1[1] = 0;
                found = strstr(buf, key1);
                if (found != 0) {
                    GetFont();
                    strcpy(buf, p);
                    *found = 0;
                    DrawTextClipped(surface, buf, x, y, width, 0);
                    x += GetTextPixelWidth(buf);
                    saved = x;
                    if (me->field_138 != 0)
                        SetTextColors(menu->colours[0], GetTextKeyColor());
                    else
                        SetTextColors(menu->colours[me->colours], GetTextKeyColor());
                    DrawTextClipped(surface, key1, x, y, width, 0);
                    x += GetTextPixelWidth(key1);
                    if (me->field_138 != 0) {
                        DrawLine(surface, saved, LineHeight() + y - 1,
                                     x - 1, LineHeight() + y - 1,
                                     menu->colours[0]);
                    } else {
                        DrawLine(surface, saved, LineHeight() + y - 1,
                                     x - 1, LineHeight() + y - 1,
                                     menu->colours[2]);
                    }
                    if (me->field_138 != 0)
                        SetTextColors(menu->colours[0], GetTextKeyColor());
                    else
                        SetTextColors(menu->colours[me->colours], GetTextKeyColor());
                    DrawTextClipped(surface, found + 1, x, y, width, 0);
                } else {
                    DrawTextClipped(surface, p, x, y, width, 0);
                }
            }
        } else if (me->flags & 0x20) {
            int xb;
            char* found;
            xb = (rect.right - textw - rect.left) / 2 + t;
            xb += rect.left + 1;
            int ys = flagy - LineHeight();
            ys += rect.bottom - 4;
            if (me->field_13a != 0 && (found = strchr(p, (signed char)me->field_13a)) != 0) {
                width = rect.right - rect.left + 1;
                key2[0] = me->field_13a;
                key2[1] = 0;
                GetFont();
                *found = 0;
                DrawTextClipped(surface, p, xb, ys, width, 0);
                // Suspected original bug: this measures `text` (the first
                // string, [esp+0x4c] at 0x4a681e), not the drawn prefix `p`,
                // so the underline is misplaced when stage selects a later
                // string. The flags 2 branch measures its truncated copy.
                xb += GetTextPixelWidth(text);
                SetTextColors(menu->colours[10], GetTextKeyColor());
                DrawTextClipped(surface, key2, xb, ys, width, 0);
                xb += GetTextPixelWidth(key2);
                SetTextColors(menu->colours[me->colours], GetTextKeyColor());
                DrawTextClipped(surface, found + 1, xb, ys, width, 0);
            } else {
                DrawTextClipped(surface, p, xb, ys, rect.right - rect.left + 1, 0);
            }
        }
    } while (pass--);

    menu->language = menu->values[0];
    if (border) {
        GrayRectangle(surface, &rect);
        FadeRectangle(surface, &rect, -0x14);
    }
}

// FUNCTION: 0x4a69d0
void __stdcall ClearAllStatus(Gui* param_1)
{
    Gadget* entries = param_1->layer->entries;
    Gadget* e = &entries[1];
    for (int i = 1; i < entries->u.count + 1; i++, e++) {
        if (e->type == 1 && e->field_138 != 0) {
            e->field_138 = 0;
            DrawButton(param_1, i);
            param_1->changed = 1;
        }
    }
}

// Sibling of 0x4a69d0, limited to the entries on the same team as `index`.
// FUNCTION: 0x4a6a40
void __stdcall ClearGroupStatus(Gui* param_1, int index)
{
    Gadget* entries = param_1->layer->entries;
    Gadget* e = &entries[1];
    unsigned char team = entries[index].team;
    for (int i = 1; i < entries->u.count + 1; i++, e++) {
        if (e->type == 1 && e->team == team && e->field_138 != 0) {
            e->field_138 = 0;
            DrawButton(param_1, i);
            param_1->changed = 1;
        }
    }
}

static inline int FindKind(Gadget* entries, unsigned char kind)
{
    for (int i = 1; i < entries->u.count + 1; i++) {
        if (entries[i].type == 4 && entries[i].team == kind)
            return i;
    }
    return 0;
}

// Command-button click/key handler for the 0x15b-byte entry table.
// FUNCTION: 0x4a6ae0
int __stdcall HandleButtonInput(Gui* obj, int index, int param_3)
{
    Gadget* entries = obj->layer->entries;
    Gadget* entry = &entries[index];
    if (entry->field_13c & 1)
        goto fail;

    Rect r;
    if (entry->type == 0) {
        r.left = 0;
        r.top = 0;
    } else {
        r.left = entry->x;
        r.top = entry->y;
    }
    r.right = entry->w + r.left - 1;
    r.bottom = entry->h + r.top - 1;

    Point point = obj->point;
    point.x -= entries->x;
    point.y -= entries->y;

    if (point.x >= r.left && point.x <= r.right
        && point.y >= r.top && point.y <= r.bottom) {
        obj->hoverGadgetIndex = index;
        if (IsMouseButtonMessage(obj, 1)) {
            obj->focus = -1;
            TrySetFocus(obj, index);
            SetClickMode(obj, 1);
            obj->clickStatusCache = entry->field_138;
        } else if (IsMouseButtonMessage(obj, 2)) {
            obj->focus = -1;
            TrySetFocus(obj, index);
            SetClickMode(obj, 2);
            obj->clickStatusCache = entry->field_138;
        }
    }

    if (obj->focus == index) {
        if (entry->flags & 0x10) {
            if (!HasMouseKeyFlags(obj, 3))
                goto fail;
            obj->focus = -1;
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom) {
                entry->field_138 = obj->clickStatusCache;
                DrawButton(obj, index);
                return 0;
            }
            entry->field_138 = 1;
            ClearPeerStatus(obj, index);
            DrawButton(obj, index);
            return 1;
        }
        if (entry->flags & 0x40) {
            if (!HasMouseKeyFlags(obj, 3)) {
                obj->focus = -1;
                if (point.x < r.left || point.x > r.right
                    || point.y < r.top || point.y > r.bottom) {
                    entry->field_138 = obj->clickStatusCache;
                    DrawButton(obj, index);
                    return 0;
                }
                entry->field_138 = (obj->clickStatusCache == 0);
                ClearPeerStatus(obj, index);
                DrawButton(obj, index);
                return 1;
            }
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom) {
                if (entry->field_138 == 0)
                    goto fail;
                entry->field_138 = 0;
                DrawButton(obj, index);
                return 0;
            }
            if (entry->field_138 != 0)
                goto fail;
            entry->field_138 = 1;
            DrawButton(obj, index);
            return 0;
        }
        if (entry->flags & 8) {
            if (HasMouseKeyFlags(obj, 3))
                goto fail;
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom)
                goto fail;
            if (entry->field_138 == 1)
                entry->field_138 = 0;
            else if (entry->field_138 == 0)
                entry->field_138 = 1;
            ClearPeerStatus(obj, index);
            DrawButton(obj, index);
            obj->focus = -1;
            return 1;
        }
        if (entry->flags & 0x100) {
            if (!IsMouseButtonMessage(obj, 1))
                goto fail;
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom)
                goto fail;
            GafEntry* p = entry->gaf;
            if (p != 0) {
                if (entry->field_138 < p->count - 1)
                    entry->field_138 += 1;
                else
                    entry->field_138 = 0;
            }
            ClearPeerStatus(obj, index);
            DrawButton(obj, index);
            obj->focus = -1;
            return 1;
        }
        if (!HasMouseKeyFlags(obj, 3)) {
            obj->focus = -1;
            entry->field_138 = 0;
            ClearPeerStatus(obj, index);
            if (point.x >= r.left && point.x <= r.right
                && point.y >= r.top && point.y <= r.bottom
                && !(entry->flags & 0x1800)) {
                if (entry->stage != 0) {
                    entry->stageIndex += 1;
                    if (entry->stageIndex >= entry->stage)
                        entry->stageIndex = 0;
                }
                DrawButton(obj, index);
                return 1;
            }
            DrawButton(obj, index);
            return 0;
        }
        // Keep this if / else-if / else chain: it gives the original block layout.
        if (entry->field_138 != 0 && (entry->flags & 0x2000)) {
            if (DAT_0051fbb0 == GetTicks())
                goto fail;
            DAT_0051fbb0 = GetTicks();
            if (DAT_0051fbac > 0) {
                DAT_0051fbac -= 1;
                return 0;
            }
        } else if (entry->field_138 == 0
                   && point.x >= r.left && point.x <= r.right
                   && point.y >= r.top && point.y <= r.bottom) {
            entry->field_138 = 1;
            DAT_0051fbac = 0xf;
        } else {
            if (entry->field_138 == 0)
                goto fail;
            if (point.x >= r.left && point.x <= r.right
                && point.y >= r.top && point.y <= r.bottom)
                goto fail;
            entry->field_138 = 0;
            DrawButton(obj, index);
            return 0;
        }
        DrawButton(obj, index);
        int flags = entry->flags;
        if (!(flags & 0x1800))
            goto fail;
        // Inline FindKind fed a byte local: fixes the obj/entry register choice.
        unsigned char team = entry->team;
        int found = FindKind(entries, team);
        // FindKind returns 0, not -1, when nothing matches, so this test can
        // never be true and a miss falls through to entry 0 (docs/bugs.md).
        if (found == -1)
            goto fail;
        Gadget* f = &entries[found];
        short off = f->field_140;
        if (flags & 0x1000) {
            if (off > 0)
                f->field_140 = off - 1;
        } else {
            if (off < f->field_136 - 1)
                f->field_140 = off + 1;
        }
        if (obj->layer)
            obj->layer->dirty = 1;
        DrawSliderBar(obj, found);
        SyncAssocGadgets(obj, found);
        if (f->callback)
            f->callback(obj, f->callbackArg);
        return 0;
    } else {
        if (!(obj->focus != -1 && entries[obj->focus].type == 3) || IsKeyDown(0xfb)) {
            if (obj->pathsReady == 1) {
                if (param_3 != 0) {
                    if ((char)tolower((char)entry->field_13a) == (char)param_3
                        || (char)toupper((char)entry->field_13a) == (char)param_3) {
                        if (entry->flags & 0x40) {
                            entry->field_138 = (entry->field_138 == 0);
                            DrawButton(obj, index);
                        } else if (entry->flags & 0x10) {
                            if (entry->field_138 == 0) {
                                entry->field_138 = 1;
                                DrawButton(obj, index);
                            }
                        }
                        ClearPeerStatus(obj, index);
                        PopKey();
                        return 1;
                    }
                }
            }
        }
    }
fail:
    return 0;
}

// FUNCTION: 0x4a7190
void __stdcall BeginTextEdit(Gui* obj, int index)
{
    Gadget* entries = obj->layer->entries;
    Gadget* target = &entries[index];

    SetTextColors(obj->colours[target->colours], GetTextKeyColor());

    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == target->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->fontId);
    }

    TrySetFocus(obj, index);
    obj->layer->current = index;
    CommitTextEdit(obj, index, target->u.text, target->field_138, 0);
    ClearKeyQueue();
}

// A copy of the function at 0x4a1810 (another unit), inlined in HandleTextInput.
static inline int SelectFontForEntry_inlined(Gadget* entries, int index)
{
    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entries[index].tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->fontId);
        i = -1;
    }
    return i;
}

// One mouse button's action on the entry: the rect GetGadgetRectByIndex fills here is
// never read, but its block-scoped local shares the frame slot of the hit
// test's rect, as in the original.
static inline void Activate(Gui* obj, int index)
{
    Rect rect;
    Gadget* ep = obj->layer->entries;
    GetGadgetRectByIndex((char*)ep, index, &rect);
    SetTextColors(obj->colours[ep[index].colours], GetTextKeyColor());
    SelectFontForEntry(ep, index);
    TrySetFocus(obj, index);
    obj->layer->current = index;
    CommitTextEdit(obj, index, ep[index].u.text, ep[index].field_138, 0);
    ClearKeyQueue();
}

// Mouse and key handling for a text entry of the 0x15b-byte entry table: a
// click inside the entry's rect (left or right button) selects its group and
// gives it the focus; with the focus, the typed key is handled and Enter or
// Escape (which also clears the text) end the edit.
// FUNCTION: 0x4a7290
int __stdcall HandleTextInput(Gui* obj, int index, int key)
{
    Gadget* entries = obj->layer->entries;
    Rect rect;
    GetGadgetRectByIndex((char*)entries, index, &rect);
    SelectFontForEntry_inlined(entries, index);

    Point point = obj->point;
    int rel_x = point.x - entries->x;
    int rel_y = point.y - entries->y;

    if (IsPointInRect(&rect, rel_x, rel_y)) {
        obj->hoverGadgetIndex = index;
        if (IsMouseButtonMessage(obj, 1)) {
            Activate(obj, index);
            SetClickMode(obj, 1);
        } else if (IsMouseButtonMessage(obj, 2)) {
            Activate(obj, index);
            SetClickMode(obj, 2);
        }
    }

    if (IsFocused((int)obj, index)) {
        SetTextColors(obj->colours[entries[index].colours],
                     obj->colours[entries[index].image]);
        int r = HandleTextEditKey(obj, index, key);
        if (r == 13) {
            ClearFocus((Dialog*)obj);
            return 1;
        }
        if (r == 27) {
            ClearFocus((Dialog*)obj);
            entries[index].u.text[0] = 0;
            return 1;
        }
        if (obj->layer)
            obj->layer->dirty = 1;
    }
    return 0;
}

// Finds the next entry of type 3 after `index` in a 1-based list of 0x15b-byte
// entries (entry 0 holds the count at +0xb6), wrapping round to the start.
// The parameter itself is the loop counter (the original loads it first and
// keeps a copy of its old value for the wrapped search).
// FUNCTION: 0x4a7560
int __stdcall FindNextTextInput(Gadget* list, int index)
{
    int old = index;
    for (index++; index < list[0].u.count + 1; index++) {
        if (list[index].type == 3)
            break;
    }
    if (index == list[0].u.count + 1) {
        for (index = 1; index < old; index++) {
            if (list[index].type == 3)
                break;
        }
    }
    return index;
}

// FUNCTION: 0x4a75d0
int __stdcall LoadScreenGaf(Gui* obj, char* name)
{
    char path[256];
    path[0] = 0;
    if (obj->str_ab6[0] != 0)
        strncpy(path, obj->str_ab6, 0x100);
    strcat(path, name);
    ChangeExtension(path, path, "GAF");
    if (HAPI_FileLengthByName(path)) {
        obj->layer->entries->u.assets.archive = LoadGaf(path);
        if (obj->layer->entries->u.assets.archive != 0)
            return 1;
    }
    return 0;
}

static inline void DoSelect(Gui* menu, Gadget* entries, int sel)
{
    Gadget* entry = &entries[sel];
    SetTextColors(menu->colours[entry->colours], GetTextKeyColor());
    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entry->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1)
        SetFont(g_guiContext->fontId);
    TrySetFocus(menu, sel);
    menu->layer->current = sel;
    CommitTextEdit(menu, sel, entry->u.text, entry->field_138, 0);
    ClearKeyQueue();
}

// Selects the menu entry named `name` (16 bytes of its name at +0x02 of the
// 0x15b-byte entry, so entry i's name is at entries + i*0x15b + 2). When the
// selected entry is type 3 it makes its group's type-7 entry current and
// refreshes its edit field.
// FUNCTION: 0x4a76b0
void __stdcall SelectGadgetByName(Gui* menu, char* name)
{
    Gadget* entries = menu->layer->entries;
    int index = FindEntry(entries, name);
    if (index != -1) {
        menu->focus = -1;
        menu->layer->current = index;
        if (entries[menu->layer->current].type == 3) {
            // The test reads the local entries; the call re-reads layer->entries and current.
            DoSelect(menu, menu->layer->entries, menu->layer->current);
        }
    }
}

// Selecting the entry with the index the caller passes: remembers the index,
// and if the entry it names is a type 3 (text) control it makes the group's
// type 7 list entry current and puts the entry's own text back into the field.
// FUNCTION: 0x4a7830
void __stdcall SelectGadgetByIndex(Gui* menu, int index)
{
    // The array is loaded twice: this copy for the type test, again inside the branch.
    Gadget* first = menu->layer->entries;
    menu->focus = -1;
    menu->layer->current = index;
    if (first[menu->layer->current].type == 3) {
        int i = menu->layer->current;
        Gadget* entries = menu->layer->entries;
        Gadget* entry = &entries[i];
        int font = GetTextKeyColor();
        SetTextColors(menu->colours[entry->colours], font);

        int n = 0;
        int j = 1;
        for (; j < entries->u.count + 1; j++) {
            if (entries[j].type == 7) {
                if (n == entry->tab) {
                    SetFont(entries[j].u.list.language);
                    break;
                }
                n++;
            }
        }
        if (j == entries->u.count + 1) {
            SetFont(g_guiContext->fontId);
        }

        TrySetFocus(menu, i);
        menu->layer->current = i;
        CommitTextEdit(menu, i, entry->u.text, entry->field_138, 0);
        ClearKeyQueue();
    }
}

// The real BeginTextEdit, inlined here by /Ob2 (the out-of-line function alone is not inlined).
static inline void FUN_004a7190_inlined(Gui* obj, int index)
{
    Gadget* entries = obj->layer->entries;
    Gadget* target = &entries[index];

    SetTextColors(obj->colours[target->colours], GetTextKeyColor());

    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == target->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->fontId);
    }

    TrySetFocus(obj, index);
    obj->layer->current = index;
    CommitTextEdit(obj, index, target->u.text, target->field_138, 0);
    ClearKeyQueue();
}

// FUNCTION: 0x4a7960
void __stdcall SelectAdjacentGadget(Gui* menu, int dir)
{
    int used[200];

    Layer* layer = menu->layer;
    int index = layer->current;
    Gadget* entries = layer->entries;
    if (index == -1)
        return;

    for (int k = 0; k < 50; k++)
        used[k] = 0;

    // Both loops index by i, with no cnt local: count + 1 is written at each use.
    for (int i = 1; i < entries->u.count + 1; i++) {
        int idx = -1;
        for (int j = 1; used[j] != 0; j++) {
            int d = used[j] - entries[i].x;
            if (d < 10 && d > -10) {
                idx = j;
                break;
            }
        }
        if (idx != -1)
            used[i] = used[idx];
        else
            used[i] = entries[i].x;
    }

    int start;
    int bound;
    switch (dir) {
    case 0:
        start = entries[index].x + entries[index].y * 5000;
        bound = start - 0x17d7840;
        break;
    case 2:
        start = entries[index].y + used[index] * 5000;
        bound = start - 0x17d7840;
        break;
    case 1:
        start = entries[index].x + entries[index].y * 5000;
        bound = start + 0x17d7840;
        break;
    case 3:
        start = entries[index].y + used[index] * 5000;
        bound = start + 0x17d7840;
        break;
    }

    {
    int pos;
    for (int i = 1; i < entries->u.count + 1; i++) {
        // up is declared before b, and b is an explicit pointer to w: induction variable order.
        int* up = &used[i];
        char* b = (char*)&entries[i].w;
        if (*(signed char*)(b + 0x12) != 0 && !(*(int*)(b + 4) & 0x400)
            && !(*(unsigned char*)(b - 0x17) == 1 && (*(unsigned char*)(b + 0x125) & 1))
            && !(*(unsigned char*)(b - 0x17) == 4 && *(int*)(b + 0x140) != 0)) {
            if (*(unsigned char*)(b - 0x17) == 3 || *(unsigned char*)(b - 0x17) == 4
                || *(unsigned char*)(b - 0x17) == 1
                || *(unsigned char*)(b - 0x17) == 6
                || *(unsigned char*)(b - 0x17) == 2) {
                if (!(*(unsigned char*)(b - 0x17) == 4
                      && *(short*)b < *(short*)(b + 2))) {
                    if (!(*(unsigned char*)(b - 0x17) == 2 && (*(int*)(b + 4) & 0x100))
                        && !(*(unsigned char*)(b - 0x17) == 1
                             && (*(unsigned char*)(b + 0x125) & 1))) {
                        switch (dir) {
                        case 0:
                        case 1:
                            pos = *(short*)(b - 4) + *(short*)(b - 2) * 5000;
                            break;
                        case 2:
                        case 3:
                            pos = *(short*)(b - 2) + *up * 5000;
                            break;
                        }
                        switch (dir) {
                        case 1:
                        case 3:
                            if (pos <= start)
                                pos += 0x17d7840;
                            if (pos < bound) {
                                bound = pos;
                                index = i;
                            }
                            break;
                        case 0:
                        case 2:
                            if (pos >= start)
                                pos -= 0x17d7840;
                            if (pos > bound) {
                                bound = pos;
                                index = i;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    }

    menu->focus = -1;
    layer->current = index;
    // Call FUN_004a7190_inlined(menu, menu->layer->current) behind this test; no sel local.
    if (entries[menu->layer->current].type == 3)
        FUN_004a7190_inlined(menu, menu->layer->current);
    if (entries[menu->layer->current].type == 3)
        FUN_004a7190_inlined(menu, menu->layer->current);
}

// FUNCTION: 0x4a7ee0
void __stdcall MoveEntryToFront(Gadget* entries, int index)
{
    Gadget temp;
    if (index != -1) {
        temp = entries[index];
        for (int i = index; i > 1; i--) {
            entries[i] = entries[i - 1];
        }
        entries[1] = temp;
    }
}

// Finds the GAF entry that draws a button-like object: the object's own name
// (copied out of obj->name) is looked up in the screen's GAF, then in the
// object's own GAF, and only when both fail does it fall back to "CHECKBOX",
// "stagebuttn%d" or "BUTTONS0".
// FUNCTION: 0x4a7f70
void __stdcall FindButtonGaf(Gui* button, Gadget* obj)
{
    char name[0x10];
    char str[0x20];
    int best;
    GafEntry* entry = 0;
    Gadget* holder = button->layer->entries;
    strncpy(name, obj->name, 0x10);
    name[0xf] = 0;
    obj->field_13b = 0;
    void* gaf = holder->u.assets.archive;
    if (gaf)
        entry = FindGafEntry(gaf, name);
    if (entry == 0) {
        if (button->gaf != 0) {
            entry = FindGafEntry(button->gaf, name);
            if (entry == 0) {
                if (obj->flags & 0x80) {
                    entry = FindGafEntry(button->gaf, "CHECKBOX");
                } else if (obj->stage != 0) {
                    int n = obj->stage < 4 ? obj->stage : 4;
                    sprintf(str, "stagebuttn%d", n);
                    entry = FindGafEntry(button->gaf, str);
                    if (obj->stage == 1) {
                        obj->stage = 2;
                        obj->flags |= 0x4000;
                    }
                } else {
                    strcpy(str, "BUTTONS0");
                    entry = FindGafEntry(button->gaf, str);
                }
                // Both loops stay inside this block: early exits must jump to the tail.
                if (entry != 0) {
                    best = 1000;
                    for (int i = 0; i < entry->count; i++) {
                        Glyph* f = GetGafFrame(entry, i);
                        if (f != 0) {
                            f->yoff = 0;
                            f->xoff = 0;
                        }
                    }
                    for (int j = 0; j < entry->count; j += 4) {
                        Glyph* f = GetGafFrame(entry, j);
                        int d = abs(obj->h - f->height) + abs(obj->w - f->width);
                        if (d < best) {
                            obj->field_13b = (unsigned char)j;
                            best = d;
                        }
                    }
                }
            }
        }
    }
    obj->gaf = entry;
    if (entry != 0) {
        Glyph* f = GetGafFrame(entry, obj->field_13b);
        if (f != 0) {
            obj->w = f->width;
            obj->h = f->height;
        }
    }
}

// Appends a cleared entry of the given type to the GUI entry list (entry 0
// holds the count) and returns its index.
// The memset spelling and the type-before-field_29 order are what RenderLayer
// needs when it inlines this; the standalone function matches with either.
// FUNCTION: 0x4a8150
int __stdcall AddGadgetEntry(Gui* obj, unsigned char type)
{
    Gadget* entries = obj->layer->entries;
    entries->u.count++;
    Gadget* e = &entries[entries->u.count];
    memset(&entries[entries->u.count], 0, sizeof(Gadget));
    e->type = type;
    e->field_29 = 1;
    return entries->u.count;
}

// FUNCTION: 0x4a81b0
void __stdcall GetAnimsPath(Gui* obj, char* out)
{
    *out = 0;
    if (obj->str_ab6[0] != 0)
        strncpy(out, obj->str_ab6, 0x100);
}

// FUNCTION: 0x4a81e0
int __stdcall RenderLayer(Gui* menu, unsigned int flags)
{
    int savedType;
    int orientation;
    Gadget* entries;
    char* name;
    int* pf;
    int i, force;
    GafEntry* g;

    if (!menu->layer)
        return 0;
    entries = menu->layer->entries;
    if ((flags & 0x100) && (flags & 1)) {
        entries[0].y = -1;
        entries[0].x = -1;
    }
    if ((flags & 0x1000) && (flags & 1)) {
        entries[0].y = -2;
        entries[0].x = -2;
    }
    if (-1 == entries[0].x) {
        entries[0].x = (short)((GetScreenWidth() - entries[0].w) / 2);
        entries[0].y = (short)((GetScreenHeight() - entries[0].h) / 2);
    }
    if (entries[0].x == -2) {
        entries[0].x = (short)(((GetScreenWidth() - 0x80 - entries[0].w) / 2) + 0x80);
        entries[0].y = (short)((GetScreenHeight() - entries[0].h) / 2);
    }
    if (entries[0].x + entries[0].w > GetScreenWidth())
        entries[0].x = (short)((GetScreenWidth() - entries[0].w) / 2);
    if (entries[0].y + entries[0].h > GetScreenHeight())
        entries[0].y = (short)((GetScreenHeight() - entries[0].h) / 2);

    force = flags & 1;
    if (force) {

    // The result goes through a variable and is compared against it.
    do {
        i = PopKey();
    } while (i != 0);
    if (menu->clearQuickKeys != 0) {
        for (i = 0; i <= entries[0].u.count; i++) {
            if (entries[i].type == 1)
                entries[i].field_13a = 0;
        }
    }
    if (entries[0].w > GetScreenWidth() || entries[0].h > GetScreenHeight())
        return 0;

    entries[0].u.assets.archive = 0;
    i = 0;
    while (i < entries[0].u.count + 1) {
        // Buffers stay declared at the top of the loop body (scheduling of textbuf stores).
        char stagebuf[0x20];
        char textbuf[0x100];
        char buf1[0x100];
        char buf2[0x100];
        char buf3[0x80];
        buf2[0] = 0;
        if (menu->str_9b6[0])
            strcpy(buf2, menu->str_9b6);
        g = 0;
        if (entries[i].resourceFlags & 1) {
            entries[i].archive = 0;
            entries[i].gaf = 0;
            GetAnimsPath(menu, buf1);
            strcat(buf1, entries[i].name);
            strcat(buf1, "_gadget");
            ChangeExtension(buf1, buf1, "GAF");
            if (HAPI_FileLengthByName(buf1)) {
                entries[i].archive = LoadGaf(buf1);
                if (entries[i].archive)
                    entries[i].gaf = FindGafEntry(entries[i].archive, entries[i].name);
            }
        }
        switch (entries[i].type) {
        case 0:
        case 11: {
            if (0 > entries[0].y)
                entries[0].y += (short)GetScreenHeight();
            GetAnimsPath(menu, buf1);
            strncpy(textbuf, entries[0].name, 0x10);
            textbuf[0x10] = 0;
            strcat(buf1, textbuf);
            ChangeExtension(buf1, buf1, "GAF");
            if (!entries[0].u.assets.archive) {
                if (HAPI_FileLengthByName(buf1))
                    entries[0].u.assets.archive = LoadGaf(buf1);
            }
            strncpy(textbuf, entries[0].u.text + 0x46, 0x10);
            textbuf[0x10] = 0;
            if (entries[0].u.assets.archive)
                g = FindGafEntry(entries[0].u.assets.archive, textbuf);
            if (g == 0) {
                if (0 != menu->gaf) {
                    g = FindGafEntry(menu->gaf, textbuf);
                    if (g == 0) {
                        g = FindGafEntry(menu->gaf, "BackTile");
                        if (g != 0) {
                            for (int frameIndex = 0; frameIndex < g->count; frameIndex++) {
                                Glyph* frame = GetGafFrame(g, frameIndex);
                                frame->yoff = 0;
                                frame->xoff = 0;
                            }
                        }
                    }
                }
            }
            entries[0].u.assets.background = g;
            break;
        }

        case 4: {
            entries[i].field_13b = 0;
            if (entries[0].u.assets.archive)
                g = FindGafEntry(entries[0].u.assets.archive, "SLIDERS");
            if (g == 0 && menu->gaf != 0) {
                g = FindGafEntry(menu->gaf, "SLIDERS");
                if (g != 0) {
                    for (int f = 0; f < g->count; f++) {
                        Glyph* frame = GetGafFrame(g, f);
                        frame->yoff = 0;
                        frame->xoff = 0;
                    }
                    orientation = entries[i].w > entries[i].h ? 10 : 0;
                    Glyph* frame = GetGafFrame(g, orientation);
                    if (entries[i].w < entries[i].h)
                        entries[i].w = frame->width;
                    else
                        entries[i].h = frame->height;
                    entries[i].sliderStyle = (unsigned char)orientation;
                }
            }
            entries[i].sliderGaf = g;
            if (g != 0) {
                Gadget* firstEnd = &entries[AddGadgetEntry(menu, 1)];
                firstEnd->x = entries[i].x;
                firstEnd->y = entries[i].y;
                firstEnd->gaf = g;
                firstEnd->field_13b = entries[i].sliderStyle + 6;
                firstEnd->team = entries[i].team;
                Glyph* frame = GetGafFrame(g, entries[i].sliderStyle + 6);
                firstEnd->w = frame->width;
                firstEnd->h = frame->height;
                firstEnd->flags = 0x3400;
                firstEnd->field_29 = entries[i].field_29;
                Gadget* secondEnd = &entries[AddGadgetEntry(menu, 1)];
                secondEnd->field_29 = entries[i].field_29;
                frame = GetGafFrame(g, entries[i].sliderStyle + 8);
                secondEnd->y = entries[i].y;
                secondEnd->gaf = g;
                secondEnd->field_13b = entries[i].sliderStyle + 8;
                secondEnd->team = entries[i].team;
                secondEnd->flags = 0x2c00;
                secondEnd->w = frame->width;
                secondEnd->h = frame->height;
                frame = GetGafFrame(g, entries[i].sliderStyle + 6);
                if (entries[i].w > entries[i].h) {
                    // Spelled x - (fw - w), not x - fw + w.
                    secondEnd->x = entries[i].x - (frame->width - entries[i].w);
                    entries[i].w += (short)(frame->width * -2);
                    entries[i].x += frame->width;
                    frame = GetGafFrame(g, entries[i].sliderStyle + 5);
                    entries[i].sliderThumb = frame->width;
                    entries[i].field_136 = entries[i].w - entries[i].sliderThumb - 4;
                } else {
                    secondEnd->y = entries[i].y - (frame->height - entries[i].h);
                    secondEnd->x = entries[i].x;
                    entries[i].h += (short)(-2 * frame->height);
                    entries[i].y += frame->height;
                }
            } else {
                entries[i].field_136 = (entries[i].w > entries[i].h ? entries[i].w : entries[i].h) - 6;
            }
            break;
        }

        case 3: {
            GafEntry* input = menu->gaf ? FindGafEntry(menu->gaf, "TEXTINPUT") : 0;
            if (input != 0) {
                for (int f = 0; f < input->count; f++) {
                    Glyph* frame = GetGafFrame(input, f);
                    frame->yoff = 0;
                    frame->xoff = 0;
                }
            }
            entries[i].inputGaf = input;
            if (entries[i].field_138 >= 0x80)
                entries[i].field_138 = 0x7f;
            memset(entries[i].u.text, 0, 0x80);
            break;
        }
        case 2: {
            GafEntry* list = menu->gaf ? FindGafEntry(menu->gaf, "LISTBOX") : 0;
            if (list != 0) {
                for (int f = 0; f < list->count; f++) {
                    Glyph* frame = GetGafFrame(list, f);
                    frame->yoff = 0;
                    frame->xoff = 0;
                }
            }
            entries[i].u.list.gaf = list;
            int j = 1;
            for (; j <= entries[0].u.count; ) {
                Gadget* other = &entries[j];
                if (j != i && other->type == 2) {
                    if (other->team == entries[i].team) {
                        short scroll = other->u.list.scroll > entries[i].u.list.scroll
                                         ? other->u.list.scroll : entries[i].u.list.scroll;
                        entries[i].u.list.scroll = scroll;
                        other->u.list.scroll = scroll;
                    }
                }
                j = j + 1;
            }
            break;
        }
        case 12: {
            entries[i].colours = 0;
            strncpy(textbuf, entries[i].name, 0x10);
            textbuf[0x10] = 0;
            entries[i].u.frame.glyph = 0;
            if (entries[0].u.assets.archive != 0)
                g = FindGafEntry(entries[0].u.assets.archive, textbuf);
            if (0 == g)
                g = FindGafEntry(menu->gaf, textbuf);
            if (g != 0)
                entries[i].u.frame.glyph = GetGafFrame(g, 0);
            break;
        }

        case 1: {
            // pf is built from cur, not &entries[i].flags; the 0x80 test reads entries[i].flags.
            Gadget* cur = &entries[i];
            cur->colours = 0;
            pf = &cur->flags;
            if ((cur->flags & 0x1800) || (cur->resourceFlags & 1))
                break;
            AssignQuickKey(menu, i);
            strncpy(textbuf, entries[i].name, 0x10);
            entries[i].field_13b = 0;
            textbuf[0x10] = 0;
            if (entries[0].u.assets.archive)
                g = FindGafEntry(entries[0].u.assets.archive, textbuf);
            if (g == 0) {
                if (menu->gaf != 0) {
                    g = FindGafEntry(menu->gaf, textbuf);
                    if (0 == g) {
                        if (0x80 & entries[i].flags) {
                            g = FindGafEntry(menu->gaf, "CHECKBOX");
                        } else if (entries[i].stage != 0) {
                            if (strcmp(entries[i].u.text, "Off|On") != 0 && entries[i].stage != 1 && 0 == (*pf & 0x4000)) {
                                int n = entries[i].stage < 4 ? entries[i].stage : 4;
                                sprintf(stagebuf, "stagebuttn%d", n);
                            } else {
                                entries[i].stage = 2;
                                strcpy(stagebuf, "stagebuttn1");
                                *pf |= 0x4000;
                            }
                            g = FindGafEntry(menu->gaf, stagebuf);
                        } else {
                            strcpy(stagebuf, "BUTTONS0");
                            g = FindGafEntry(menu->gaf, stagebuf);
                        }
                        if (g) {
                            int best = 1000;
                            for (int f = 0; f < g->count; ++f) {
                                Glyph* frame = GetGafFrame(g, f);
                                if (frame != 0) {
                                    frame->yoff = 0;
                                    frame->xoff = 0;
                                }
                            }
                            for (int j = 0; j < g->count; j += 4) {
                                Glyph* frame = GetGafFrame(g, j);
                                int distance = abs(entries[i].h - frame->height) + abs(entries[i].w - frame->width);
                                if (distance < best) {
                                    entries[i].field_13b = (unsigned char)j;
                                    best = distance;
                                }
                            }
                        }
                    }
                }
            }
            entries[i].gaf = g;
            if (g != 0) {
                Glyph* frame = GetGafFrame(g, entries[i].field_13b);
                if (frame != 0) {
                    entries[i].w = frame->width;
                    entries[i].h = frame->height;
                }
            }
            if (0 != entries[i].stage) {
                char* p = entries[i].u.text;
                while (*p) {
                    if (*p == '|')
                        *p = 0;
                    p++;
                }
                cur = &menu->layer->entries[i];
                char* dst = buf3;
                char* src = cur->u.text;
                int k = 0;
                for (; k < cur->stage; k++) {
                    strcpy(dst, Translate(src));
                    dst += strlen(dst) + 1;
                    src += strlen(src) + 1;
                }
                memcpy(cur->u.text, buf3, sizeof(buf3));
                *pf = (*pf & 0x4000) | 1;
            }
            break;
        }

        case 7:
            strcpy(buf2, menu->str_bb6);
            strcat(buf2, entries[i].u.text);
            strcat(buf2, ".FNT");
            entries[i].u.list.filebuf = HAPI_LoadFile(buf2, 0);
            break;

        case 8:
            strcat(buf2, entries[i].u.text);
            entries[i].u.list.filebuf = HAPI_LoadFile(buf2, 0);
            break;

        case 13: {
            Gadget* en = menu->layer->entries;
            en[i].u.anim.field_c6 = GetTicks() + en[i].u.anim.field_c2;
            break;
        }

        case 5:
            if (strlen((char*)&entries[i].field_136) == 0)
                entries[i].flags |= 0x10;
            else
                AssignQuickKey(menu, i);
            entries[i].colours = 0;
            break;

        default:
            break;
        }
        i++;
    }

    name = entries[0].name;
    if (0 == name)
        name = "GUI SURFACE";
    entries[0].u.assets.surface = AllocSurface(name, entries[0].w, entries[0].h);
    DrawSurface(entries[0].u.assets.surface, 0, -entries[0].x, -entries[0].y);
    if (!(flags & 0x20)) {
        entries[0].u.assets.saveUnder = AllocSurface("SAVE UNDER", entries[0].w, entries[0].h);
        DrawSurface(entries[0].u.assets.saveUnder, entries[0].u.assets.surface, 0, 0);
    } else {
        entries[0].u.assets.saveUnder = 0;
    }
    }

    if ((flags & 4) != 0 || force || (flags & 0x40)) {
        if (force || (flags & 0x40)) {
            if (menu->layer->surface)
                DrawSurface(entries[0].u.assets.surface, menu->layer->surface, 0, 0);
            else if ((flags & 0x80) == 0)
                DrawListboxFrame(menu, 0, entries[0].u.assets.background);
        }

        for (i = 1; i < 1 + entries[0].u.count; i++) {
            if (entries[i].field_29 == 0)
                continue;
            switch (entries[i].type) {
            case 11:
                DrawListboxFrame(menu, i, entries[i].u.assets.background);
                break;
            case 12:
                DrawGadgetGlyph(menu, i);
                break;
            case 1:
                if (force != 0 || (flags & 0x48) != 0)
                    DrawButton(menu, i);
                break;
            case 2: {
                if (force) {
                    int fh;
                    Gadget* base = menu->layer->entries;
                    int t = 0;
                    int j;
                    base[i].u.list.field_bc = 0;
                    base[i].u.list.field_ba = 0;
                    for (j = 1; j < base[0].u.count + 1; j++) {
                        if (base[j].type == 7) {
                            if (t == base[i].tab) {
                                SetFont(base[j].u.list.language);
                                break;
                            }
                            t++;
                        }
                    }
                    if (j == base[0].u.count + 1)
                        SetFont(g_guiContext->fontId);
                    if (g_guiContext->language == 0)
                        fh = GetFontHeight();
                    else
                        fh = GetGafFrame(g_guiContext->language->glyphs, 0x49)->height + 2;
                    int hh = base[i].h;
                    base[i].h = (short)(hh - ((int)base[i].h) % (fh + 2));
                    base[i].u.list.sortKey = GetTicks();
                }
                if (force || (flags & 0x40))
                    DrawListBox(menu, i);
                break;
            }
            case 3:
                if (force || (flags & 0x40))
                    DrawTextInput(menu, i);
                break;
            case 4:
                if (force) {
                    Gadget* base = menu->layer->entries;
                    base[i].field_140 = 0;
                    base[i].callback = 0;
                    base[i].callbackArg = 0;
                }
                if (force || (0x40 & flags))
                    DrawSlider(menu, i);
                break;
            case 5:
                if (force || (flags & 0x40))
                    DrawLabel(menu, i);
                break;
            case 6:
                if (force) {
                    Gadget* base = menu->layer->entries;
                    base[i].u.t6.f_b6 = 0;
                    base[i].u.t6.f_be = 0;
                    base[i].u.t6.f_c2 = 0;
                    base[i].u.t6.f_c6 = 0;
                }
                if (force || (flags & 0x40))
                    DrawHotspot(menu, i);
                break;
            case 13:
                if (force) {
                    Gadget* base = menu->layer->entries;
                    base[i].u.anim.field_c6 = GetTicks() + base[i].u.anim.field_c2;
                }
                if (force || (flags & 0x40))
                    DrawProgressBarGadget(menu, i);
                break;
            case 10:
                if (force || (flags & 0x40))
                    DrawLineGadget(menu, i, flags);
                break;
            default:
                break;
            }
    }
        if (menu->layer->current != -1 && menu->dirty != 0) {
            savedType = entries[menu->layer->current].type;
            DrawGadgetSelectionBox(menu, menu->layer->current, 8);
            int j = FindEntry(entries, entries[0].u.text + 0x16);
            if (j != -1 && savedType != 1 && entries[i].field_29 != 0)
                DrawGadgetSelectionBox(menu, j, 8);
        }
    }

    if (flags & 2) {
        if (entries[0].u.assets.saveUnder != 0) {
            DrawSurface(0, entries[0].u.assets.saveUnder, entries[0].x, entries[0].y);
            FreeSurface(entries[0].u.assets.saveUnder);
            entries[0].u.assets.saveUnder = 0;
        }
        FreeSurface(entries[0].u.assets.surface);
        entries[0].u.assets.surface = 0;
        for (int j = 0; j < 1 + entries[0].u.count; j = j + 1) {
            if ((entries[j].resourceFlags & 1) && entries[j].archive)
                GameFreeThunk(entries[j].archive);
            switch (entries[j].type) {
            case 0:
                GameFreeThunk(entries[j].u.assets.archive);
                break;
            case 7:
                GameFreeThunk(entries[j].u.list.filebuf);
                break;
            case 8:
                GameFreeThunk(entries[j].u.list.filebuf);
                break;
            default:
                break;
            }
        }
    }

    return 1;
}

// Closes the top GUI screen: runs its close handler, pops it off the stack,
// activates the next one and frees the old node.
// FUNCTION: 0x4a9660
void __stdcall CloseTopScreen(Gui* gui)
{
    if (gui->layer) {
        unsigned int flags = gui->layer->flags;
        gui->hoverGadgetIndex = gui->hotGadgetIndex = gui->focus = -1;
        if (gui->layer->handler)
            gui->layer->handler(gui);
        HideSoftwareCursor();
        RenderLayer(gui, 2);
        ShowSoftwareCursor();
        Layer* old = gui->layer;
        gui->layer = old->next;
        if (gui->layer)
            gui->layer->dirty = 1;
        GameFreeThunk(old);
        if (flags & 0x800)
            RenderLayer(gui, 0x40);
    }
}

// Decrements the scroll offset (field_140) of GUI entry `index`, clamped to
// [0, field_136 - 1]. When the value actually changes it marks the object
// changed, refreshes the gadget and runs the entry's callback (if any).
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
extern int Pad_a96d0_e0;
extern int Pad_a96d0_e1;
extern int Pad_a96d0_e2;

// FUNCTION: 0x4a96d0
void __stdcall DecrementKnobPos(Gui* obj, int index)
{
    Gadget* e = &obj->layer->entries[index];
    short raw = e->field_140;
    // Keep the int copy of the old value: it fixes the register order of the entry address.
    int old = raw;
    e->field_140 = raw - 1;
    if (e->field_140 > e->field_136 - 1) {
        e->field_140 = e->field_136 - 1;
    }
    if (e->field_140 < 0) {
        e->field_140 = 0;
    }
    if (e->field_140 != old) {
        obj->changed = 1;
        DrawSliderBar(obj, index);
        SyncAssocGadgets(obj, index);
    }
    if (e->callback) {
        e->callback(obj, e->callbackArg);
    }
}

// Forward declarations of the list steps below: their symbol ids keep IncrementKnobPos matching.
void __stdcall ListBoxSelectUp(Gui* param_1, int index);
void __stdcall ListBoxSelectDown(Gui* param_1, int index);

// Must stay a static inline helper: written inline it changes the load order.
static inline Gadget* entry_at(Gui* obj, int index)
{
    return &obj->layer->entries[index];
}

// Increments the scroll offset (field_140) of GUI entry `index`, clamped to
// [0, field_136 - 1]. When the value actually changes it marks the object
// changed, refreshes the gadget and runs the entry's callback (if any).
// Note: the upper clamp still uses field_136 - 1, as the copy-paste source of
// this function did, even though this side scrolls the other way.
// FUNCTION: 0x4a9780
void __stdcall IncrementKnobPos(Gui* obj, int index)
{
    Gadget* e = entry_at(obj, index);
    short raw = e->field_140;
    // Keep the int copy of the old value: it fixes the register used for it.
    int old = raw;
    e->field_140 = raw + 1;
    if (e->field_140 > e->field_136 - 1) {
        e->field_140 = e->field_136 - 1;
    }
    if (e->field_140 < 0) {
        e->field_140 = 0;
    }
    if (e->field_140 != old) {
        obj->changed = 1;
        DrawSliderBar(obj, index);
        SyncAssocGadgets(obj, index);
    }
    if (e->callback) {
        e->callback(obj, e->callbackArg);
    }
}

// The list gadget's scroll-up step, the sibling of the scroll-down step
// 0x4a99c0. It first does what 0x4a99c0 does: picks the entry of type 7 whose
// group number matches entry `index` and makes that entry's id the current
// one (falling back to the current id of the list holder). Then it works out
// how far the visible window moves per line and, when the selected line is
// still inside the window and the window has not run off the top, moves the
// selection one line up, scrolls the window if needed and refreshes the
// gadget. A selection on line 0 is not moved, and a line whose text starts
// with "&G" is not moved either.
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
struct Pad_a9830_0 { int field; };
struct Pad_a9830_1 { int field; };
struct Pad_a9830_2 { int field; };
struct Pad_a9830_3 { int field; };
struct Pad_a9830_4 { int field; };
extern int Pad_a9830_e0;
extern int Pad_a9830_e1;
extern int Pad_a9830_e2;
extern int Pad_a9830_e3;
extern int Pad_a9830_e4;
extern int Pad_a9830_e5;

// FUNCTION: 0x4a9830
void __stdcall ListBoxSelectUp(Gui* param_1, int index)
{
    Gadget* entries = param_1->layer->entries;
    Gadget* me = &entries[index];
    int n = 0;
    int i = 1;
    // The `count + 1` condition keeps n in the dead argument slot.
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->fontId);
    }
    int size;
    if (g_guiContext->language == 0) {
        size = GetFontHeight();
    } else {
        // Indexed pg[1], not a pointer local: keeps the +2 offset in the load.
        unsigned short* pg = (unsigned short*)GetGafFrame(g_guiContext->language->glyphs, 0x49);
        size = pg[1] + 2;
    }
    size++;
    int step = (me->h - 2) / size;
    short last = me->u.list.field_bc;
    short sel = me->u.list.field_ba;
    int isel = sel;
    if (sel < last + step && sel >= last) {
        if (me->u.list.field_c0 == 0) {
            return;
        }
        if (sel == 0) {
            return;
        }
        short prev = sel - 1;
        me->u.list.field_ba = prev;
        if (prev < last) {
            last--;
            me->u.list.field_bc = last;
        }
        if (me->u.list.field_c2 != 0) {
            char* line = SkipTextLines(me->u.list.field_c2, prev);
            if (strncmp(DAT_00502a20, line, 2) == 0) {
                me->u.list.field_ba = isel;
            }
        }
        DrawListBox(param_1, index);
        SyncAssocGadgets(param_1, index);
        return;
    }
    if (me->u.list.field_c0 != 0) {
        SetListBoxScrollByName(param_1, me->name, isel);
    }
}

// Forward declarations of the functions below: their symbol ids keep ListBoxSelectDown matching.
int __stdcall HandleGuiCommand(Gui* obj, int cmd);
int __stdcall UpdateMenu(Gui* menu);
void __stdcall SetCurrentGuiContext(Gui* ctx);
int GetGuiContext(void);
void __stdcall SetDefaultFont(int* param_1, int param_2);

// The list gadget's scroll-down step. First it does what 0x4a1810 does: picks
// the entry of type 7 whose group number matches entry `index` and makes that
// entry's id the current one (falling back to the current id of the list
// holder). Then it works out how far the visible window moves per line and,
// when the selected line has fallen below the window but is still inside the
// list, moves the selection one line down, scrolls the window if needed and
// refreshes the gadget. When the selection is already at the last line
// nothing happens. A selection sitting on a line whose text starts with
// "&G" is not moved either.
// FUNCTION: 0x4a99c0
void __stdcall ListBoxSelectDown(Gui* param_1, int index)
{
    Gadget* entries = param_1->layer->entries;
    Gadget* me = &entries[index];
    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->fontId);
    }
    int size = (g_guiContext->language == 0) ? GetFontHeight()
        : (*(unsigned short*)((int)GetGafFrame(g_guiContext->language->glyphs, 0x49) + 2) + 2);
    size++;
    int step = (me->h - 2) / size;
    short last = me->u.list.field_bc;     // last line of the window
    short sel = me->u.list.field_ba;      // selected line
    // Int copy of sel, kept live across the calls below: using sel directly changes the code.
    int isel = sel;
    if (isel < last + step && sel >= last) {
        if (me->u.list.field_c0 != 0) {
            if (isel == me->u.list.field_be + step - 1) {
                return;
            }
            short next = sel + 1;
            me->u.list.field_ba = next;
            if (next > last + step - 1) {
                me->u.list.field_bc = last + 1;
            }
            if (next >= me->u.list.field_c0 - 1) {
                me->u.list.field_ba = me->u.list.field_c0 - 1;
            }
            if (me->u.list.field_c2 != 0) {
                char* line = SkipTextLines(me->u.list.field_c2, me->u.list.field_ba);
                if (strncmp(DAT_00502a20, line, 2) == 0) {
                    me->u.list.field_ba = isel;
                }
            }
            DrawListBox(param_1, index);
            SyncAssocGadgets(param_1, index);
            return;
        }
    }
    if (me->u.list.field_c0 != 0) {
        SetListBoxScrollByName(param_1, me->name, isel);
    }
}

// The GUI layer's command handler, called by 0x4a9fd0 with a decoded key/command
// in `cmd`. It looks up the layer's currently selected gadget (layer->current,
// layer->entries) and switches on the command:
//   9     toggle a checkbox-ish gadget (IsKeyDown(0xf9))
//   0x1b  make the gadget whose stored name matches entries[0].choice2 current
//   0xd   same for entries[0].choice, then fall through into 0x20
//   0x20  activate the selected gadget (types 1, 2, 6); type 1 also cycles its
//         stageIndex sub-index and re-selects it
//   0xf4/0xf6  scroll the list one line up / down (type 4 gadgets)
//   0xf5/0xf7  page the list up / down (type 2 gadgets)
// A handled command is returned as 0, an unhandled one unchanged. The common
// tail refreshes the holder when nothing consumed the command and records the
// newly selected gadget in obj->hotGadgetIndex.
// FUNCTION: 0x4a9b90
int __stdcall HandleGuiCommand(Gui* obj, int cmd)
{
    int newsel = -1;
    int index = obj->layer->current;
    Gadget* entries = obj->layer->entries;
    Gadget* e = &entries[index];
    int type = e->type;

    // Case order follows the original emit order, not ascending.
    switch (cmd) {
    case 9:
        if (IsKeyDown(0xf9))
            SelectAdjacentGadget(obj, 0);
        else
            SelectAdjacentGadget(obj, 1);
        obj->changed = 1;
        cmd = 0;
        break;
    case 0x1b:
        {
            int found = FindEntry(entries, entries[0].u.names.choice2);
            if (found == -1 || entries[found].field_29 == 0)
                break;
            newsel = found;
        }
        cmd = 0;
        break;
    case 0xd:
        if (obj->focus != -1 && entries[obj->focus].type == 3)
            break;
        {
            int found = FindEntry(entries, entries[0].u.names.choice);
            if (found != -1 && entries[found].field_29 != 0
                && !(entries[found].type == 1 && (entries[found].field_13c & 1))) {
                newsel = found;
                cmd = 0;
                break;
            }
        }
        // fall through to case 0x20
    case 0x20:
        if (type == 3)
            break;
        if (type != 1 && type != 2 && type != 6)
            break;
        if (e->field_29 == 0)
            break;
        if (type == 1 && (e->field_13c & 1))
            break;
        newsel = index;
        if (type == 1) {
            if (e->flags & 0x10) {
                e->field_138 = 1;
                ClearPeerStatus(obj, index);
                DrawButton(obj, index);
            }
        }
        if (type == 1 && e->stage != 0) {
            // Wrap through a pointer: a local copy makes the stage reload differ.
            unsigned char* p = &e->stageIndex;
            if (++*p >= e->stage)
                *p = 0;
        }
        cmd = 0;
        break;
    case 0xf5:
        if (type == 2) {
            ListBoxSelectUp(obj, index);
            if (e->u.list.callback)
                e->u.list.callback(obj, e);
        } else {
            SelectAdjacentGadget(obj, 2);
        }
        obj->changed = 1;
        cmd = 0;
        break;
    case 0xf4:
        if (type == 3)
            break;
        if (type == 4 && e->w > e->h)
            DecrementKnobPos(obj, index);
        else
            SelectAdjacentGadget(obj, 0);
        obj->changed = 1;
        cmd = 0;
        break;
    case 0xf7:
        if (type == 2) {
            ListBoxSelectDown(obj, index);
            if (e->u.list.callback)
                e->u.list.callback(obj, e);
        } else {
            SelectAdjacentGadget(obj, 3);
        }
        obj->changed = 1;
        cmd = 0;
        break;
    case 0xf6:
        if (type == 3)
            break;
        if (type == 4 && e->w > e->h)
            IncrementKnobPos(obj, index);
        else
            SelectAdjacentGadget(obj, 1);
        obj->changed = 1;
        cmd = 0;
        break;
    }
    if (cmd == 0 && obj->layer->keyboardInput == 0)
        PopKey();
    if (newsel != -1) {
        obj->hotGadgetIndex = newsel;
        obj->changed = 1;
    }
    return cmd;
}

// BeginTextEdit's body with its group scan left as a call to SelectFontForEntry, as
// on the case 5 path here (and twice in HandleTextInput).
static inline void SelectCurrentByName(Gui* menu, Gadget* entries, int sel)
{
    Gadget* entry = &entries[sel];
    SetTextColors(menu->colours[entry->colours], GetTextKeyColor());
    SelectFontForEntry(entries, sel);
    TrySetFocus(menu, sel);
    menu->layer->current = sel;
    CommitTextEdit(menu, sel, entry->u.text, entry->field_138, 0);
    ClearKeyQueue();
}

// The real UpdateHelpText (0x4a0090), inlined here by /Ob2.
static inline void UpdateHelpText(Gui* obj)
{
    char* text = DAT_005119b8;
    if (obj->hoverGadgetIndex != -1) {
        obj->prevHoverGadget = obj->hoverGadgetIndex;
        text = obj->layer->entries[obj->hoverGadgetIndex].helpKey;
    }
    int found = FindEntry(obj->layer->entries, "HELPTEXT");
    if (found != -1) {
        strcpy(obj->layer->entries[found].u.text, Translate(text));
        obj->changed = 1;
    }
}

// The real TickProgressBar (0x4a4890), inlined here by /Ob2.
static inline void TickProgressBar(Gui* menu, int i)
{
    // Entries in their own local first: the one-expression form shifts registers.
    Gadget* entries = menu->layer->entries;
    Gadget* e = &entries[i];
    if (e->u.anim.field_ce && e->u.anim.field_ba < e->u.anim.field_be) {
        if ((int)GetTicks() > e->u.anim.field_c6) {
            e->u.anim.field_ba += (int)e->u.anim.field_ca;
            if (e->u.anim.field_ba > e->u.anim.field_be) {
                e->u.anim.field_ba = e->u.anim.field_be;
                e->u.anim.field_ce = 0;
            }
            e->u.anim.field_c6 = GetTicks() + e->u.anim.field_c2;
        }
        DrawProgressBarGadget(menu, i);
    }
}

// The real SelectGadgetByIndex, inlined here by /Ob2.
static inline void SelectGadgetByIndex_inlined(Gui* menu, int index)
{
    Gadget* first = menu->layer->entries;
    menu->focus = -1;
    menu->layer->current = index;
    if (first[menu->layer->current].type == 3)
        FUN_004a7190_inlined(menu, menu->layer->current);
}

// The real CloseTopScreen, inlined here by /Ob2.
static inline void CloseTopScreen_inlined(Gui* gui)
{
    if (gui->layer) {
        unsigned int flags = gui->layer->flags;
        gui->hoverGadgetIndex = gui->hotGadgetIndex = gui->focus = -1;
        if (gui->layer->handler)
            gui->layer->handler(gui);
        HideSoftwareCursor();
        RenderLayer(gui, 2);
        ShowSoftwareCursor();
        Layer* old = gui->layer;
        gui->layer = old->next;
        if (gui->layer)
            gui->layer->dirty = 1;
        GameFreeThunk(old);
        if (flags & 0x800)
            RenderLayer(gui, 0x40);
    }
}

// The per-frame update of the current screen: tracks the mouse, runs the key
// command handler, then gives every enabled entry its input handler by type
// and closes the screen when an entry was chosen.
// FUNCTION: 0x4a9fd0
int __stdcall UpdateMenu(Gui* menu)
{
    // Declared at the top of the function.
    int k;
    Gadget* e;
    int i;
    Gadget* entries = 0;

    // Real early returns: the original has several, so no shrink-wrapping.
    if (menu->layer == 0)
        return 0;

    int now = GetTicks();
    menu->animTimer = now - menu->time;
    menu->time = now;
    UpdateCursorAndMouse(menu);

    int key;
    if (menu->layer->keyboardInput == 0) {
        key = PeekKey();
        if (key >= 0xe2 && key <= 0xeb)
            key = 0;
    } else {
        key = PopKey();
    }

    if (menu->layer->keyboardInput != 0 && key != 0 && menu->dirty != 0) {
        key = HandleGuiCommand(menu, key);
        if (key != 0) {
            for (int n = 0; n < 0xe; n++)
                menu->layer->text[n] = menu->layer->text[n + 1];
            menu->layer->lastKey = (char)toupper(key);
            if (menu->layer->cb3b != 0)
                menu->layer->cb3b(menu);
            menu->hotGadgetIndex = -1;
        }
    }

    int sel = menu->hotGadgetIndex;
    if (menu->layer == 0)
        return 1;
    if (menu->changed == 1) {
        menu->changed = 0;
        RenderLayer(menu, menu->layer->flags | 0x40);
    }

    // Read after the changed block, not at the top.
    entries = menu->layer->entries;
    if (entries == 0)
        return 1;

    Point& pt = menu->point;
    {
        // Rect local, not named ints: right/bottom spill into slots shared with `point`.
        Rect box;
        box.left = entries->x;
        box.top = entries->y;
        if (entries->type != 0) {
            box.left *= 2;
            box.top *= 2;
        }
        // Load y early, before right and bottom.
        int ptY = pt.y;
        box.right = entries->w + box.left - 1;
        box.bottom = entries->h + box.top - 1;
        SetCursorHover(menu, pt.x >= box.left && pt.x <= box.right &&
                           ptY >= box.top && ptY <= box.bottom);
    }

    int saved = menu->hoverGadgetIndex;
    menu->hoverGadgetIndex = -1;

    Point point;
    memcpy(&point, &menu->point, 24);
    point.x -= entries->x;
    point.y -= entries->y;

    int elapsed;
    if ((int)GetTicks() - DAT_0051fbb4 > 0) {
        elapsed = 1;
        DAT_0051fbb4 = GetTicks();
    } else {
        elapsed = 0;
    }

    i = 1;
    for (; i < entries->u.count + 1; i++) {
        e = &entries[i];
        if (e->field_29 != 0) {
            int x, y;
            if (e->type == 0) {
                x = 0;
                y = 0;
            } else {
                x = e->x;
                y = e->y;
            }
            int right = e->w + x - 1;
            int bottom = e->h + y - 1;
            if (point.x >= x && point.x <= right && point.y >= y && point.y <= bottom)
                menu->hoverGadgetIndex = i;

            k = IsKeyDown(0xfb) == 0 ? key : 0;
            switch (e->type) {
            case 1:
                if (HandleButtonInput(menu, i, key) == 1)
                    sel = i;
                if (e->colours != 0 && elapsed) {
                    e->colours -= 2;
                    if (e->colours < 0)
                        e->colours = 0;
                    if (menu->layer != 0)
                        menu->layer->dirty = 1;
                }
                break;
            case 2:
                if (HandleListBoxInput(menu, i, 0) == 1)
                    sel = i;
                break;
            case 3:
                if (HandleTextInput(menu, i, k) == 1)
                    sel = i;
                break;
            case 4:
                HandleSliderInput(menu, i);
                break;
            case 5:
                if (HandleLabelInput(menu, i, key) != 0) {
                    sel = -1;
                    int found = FindEntry(entries, (char*)&e->field_136);
                    // Keep this if/else nesting, with sel = i in the else.
                    if (found != sel) {
                        Gadget* me;
                        sel = found;
                        me = &entries[found];
                        if (me->type == 1) {
                            if (me->field_29 == 0 || (me->field_13c & 1) != 0) {
                                sel = -1;
                            } else {
                                me->stageIndex++;
                                if (me->stageIndex >= me->stage)
                                    me->stageIndex = 0;
                                DrawButton(menu, found);
                            }
                        } else {
                            if (me->field_29 != 0) {
                                if (me->type == 4 && me->field_157 != 0) {
                                    sel = -1;
                                } else {
                                    // Helper call, not the inline chain: keeps the group scan an explicit call.
                                    Gadget* entriesNow = menu->layer->entries;
                                    menu->focus = -1;
                                    menu->layer->current = found;
                                    if (entriesNow[menu->layer->current].type == 3)
                                        SelectCurrentByName(menu, menu->layer->entries, menu->layer->current);
                                }
                            } else {
                                sel = -1;
                            }
                        }
                    } else {
                        sel = i;
                    }
                }
                break;
            case 6:
                if (HandleHotspotInput(menu, i) == 1)
                    sel = i;
                break;
            case 13:
                TickProgressBar(menu, i);
                break;
            case 12:
                if (e->colours != 0 && elapsed) {
                        e->colours--;
                        DrawGadgetGlyph(menu, i);
                        if (menu->layer != 0)
                            menu->layer->dirty = 1;
                    }
                break;
            }
        }
        if (sel != -1)
            break;
    }

    if (menu->hoverGadgetIndex != saved)
        UpdateHelpText(menu);

    if (menu->layer->cb1c != 0)
        menu->layer->cb1c();

    if (sel != -1) {
        menu->hotGadgetIndex = sel;
        SelectGadgetByIndex_inlined(menu, sel);
        if (menu->layer->handler != 0)
            menu->layer->handler(menu);
        if (menu->hotGadgetIndex != -1)
            CloseTopScreen_inlined(menu);
    }
    return 1;
}

// Makes `ctx` the current context (g_guiContext) and resets its state.
// FUNCTION: 0x4aa850
void __stdcall SetCurrentGuiContext(Gui* ctx)
{
    g_guiContext = (Root_004a32a0*)ctx;
    ctx->layer = 0;
    ctx->str_9b6[0] = 0;
    ctx->str_ab6[0] = 0;
    ctx->str_bb6[0] = 0;
    ctx->pathsReady = 1;
    ctx->time = GetTicks();
    ctx->animTimer = 0;
    ctx->inputEnabled = 1;
    ctx->gaf = 0;
    ctx->fallback = 0;
    ctx->cachedBgName = 0;
    ctx->knobDragging = 0;
    ctx->hoverGadgetIndex = -1;
    memset(ctx->values, 0, 12);
    ctx->language = 0;
    ctx->dirty = 1;
}

// FUNCTION: 0x4aa8d0
int GetGuiContext(void)
{
    return (int)g_guiContext;
}

// FUNCTION: 0x4aa8e0
void __stdcall SetDefaultFont(int* param_1, int param_2)
{
    *param_1 = param_2;
}
