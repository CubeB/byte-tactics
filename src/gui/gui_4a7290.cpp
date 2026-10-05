// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by claude-opus-5-5. Names are provisional.
// Mouse and key handling for a text entry of the 0x15b-byte entry table: a
// click inside the entry's rect (left or right button) selects its group and
// gives it the focus; with the focus, the typed key is handled and Enter or
// Escape (which also clears the text) end the edit.
//
// MATCH (claude-opus-5-5, from 85.8%). The earlier passes wrote everything
// out by hand and fought the rect block for many sessions (their notes are in
// the file's history). The whole function is real helpers from the same GUI
// unit, defined here without FUNCTION lines and left to /Ob2:
// - The rect at the top is FUN_004a15c0 inlined: its stores and the x0/y0
//   reloads go through the out pointer, and the zero it keeps in eax across
//   both arms is the inlined SelectFontForEntry's `n = 0`, which is the shape every
//   hand-written rect missed. The group scan after it is SelectFontForEntry.
// - The hit test is FUN_004a1920, and the focus test and the two focus
//   resets are FUN_0049fcf0 and FUN_0049fc40 (both have no callers in the
//   exe because /Ob2 inlined every call).
// - Each button's action is one inline helper (Activate) with its own rect.
//   Its FUN_004a15c0 and SelectFontForEntry stay calls because their share of the
//   inline budget is (budget left - Activate's size) / R, and the three
//   focus helper sites after it raise R to 5 (tools/c2prio.py --inline).
//   Passing the outer rect instead keeps it address-taken, and MSVC then
//   holds x0/y0 in registers across the rect block.
// - Some CRT header is needed for the compiler state (<string.h>, <stdio.h>,
//   <stdlib.h> and <windows.h> all match; with none it is 87.2%).
#include <string.h>

#pragma pack(push, 1)

struct Entry_004a7290 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x0;                          // +0x13
    short y0;                          // +0x15
    short x1;                          // +0x17
    short y1;                          // +0x19
    char unknown_1b[0x1f - 0x1b];
    int colourIndex;                   // +0x1f
    int field_23;                      // +0x23
    char unknown_27[0x28 - 0x27];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0: number of entries)
        char text[0x82];               // +0xb6 (a text control)
        struct {
            char pad[0x20];
            int id;                    // +0xd6 (a list entry)
        } list;
    } data;
    short maxLength;                   // +0x138
    char unknown_13a[0x15b - 0x13a];
};

struct Holder_004a7290 {
    int unknown_00;
    Entry_004a7290* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                      // +0x20
};

struct Point_004a7290 {
    int x;                             // +0x00
    int y;                             // +0x04
    int unknown_08[4];
};

struct Object_004a7290 {
    char unknown_00[0x18];
    Holder_004a7290* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a7290 point;              // +0x3c
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64
    int field_68;                      // +0x68
    char unknown_6c[0x8b2 - 0x6c];
    unsigned char colors[16];          // +0x8b2
};
#pragma pack(pop)

struct Out_4a15c0 {
    int x0;
    int y0;
    int x1;
    int y1;
};

struct Dialog {
    int current;                       // +0x00
};

extern Dialog* g_guiContext;

int GetTextKeyColor();
void __stdcall SetTextColors(int colour, int font);
void __stdcall SetFont(int id);
int __stdcall IsMouseButtonMessage(Object_004a7290* obj, unsigned char buttons);
void __stdcall FUN_004ab690(Object_004a7290* obj, int param_2);
void __stdcall FUN_004ab6c0(Object_004a7290* obj, int index, char* text,
                            int maxLength, int clear);
int __stdcall HandleTextEditKey(Object_004a7290* obj, int index, char* text);
int __stdcall FUN_0049fc50(Object_004a7290* obj, int index);
void ClearKeyQueue();

// 0x4a15c0 (matched in its own file).
void __stdcall FUN_004a15c0(char* param_1, int param_2, Out_4a15c0* param_3)
{
    char* e = param_1 + param_2 * 0x15b;
    if (*e == 0) {
        param_3->x0 = 0;
        param_3->y0 = 0;
    } else {
        param_3->x0 = *(short*)(e + 0x13);
        param_3->y0 = *(short*)(e + 0x15);
    }
    param_3->x1 = *(short*)(e + 0x17) - 1 + param_3->x0;
    param_3->y1 = *(short*)(e + 0x19) - 1 + param_3->y0;
}

// 0x4a1810 (matched in its own file).
int __stdcall SelectFontForEntry(Entry_004a7290* entries, int index)
{
    int n = 0;
    int i = 1;
    for (; i < entries->data.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entries[index].group) {
                SetFont(entries[i].data.list.id);
                break;
            }
            n++;
        }
    }
    if (i == entries->data.count + 1) {
        SetFont(g_guiContext->current);
        i = -1;
    }
    return i;
}

// 0x4a1920 (matched in its own file).
int __stdcall FUN_004a1920(Out_4a15c0* r, int px, int py)
{
    if (px >= r->x0 && px <= r->x1 && py >= r->y0 && py <= r->y1) {
        return 1;
    }
    return 0;
}

// 0x49fc40 (matched in its own file).
void __stdcall FUN_0049fc40(Object_004a7290* obj)
{
    obj->focus = -1;
}

// 0x49fcf0 (matched in its own file).
int __stdcall FUN_0049fcf0(Object_004a7290* obj, int index)
{
    int result = 0;
    result = obj->focus == index;
    return result;
}

// One mouse button's action on the entry: the rect FUN_004a15c0 fills here is
// never read, but its block-scoped local shares the frame slot of the hit
// test's rect, as in the original.
static inline void Activate(Object_004a7290* obj, int index)
{
    Out_4a15c0 rect;
    Entry_004a7290* ep = obj->holder->entries;
    FUN_004a15c0((char*)ep, index, &rect);
    SetTextColors(obj->colors[ep[index].colourIndex], GetTextKeyColor());
    SelectFontForEntry(ep, index);
    FUN_0049fc50(obj, index);
    obj->holder->field_20 = index;
    FUN_004ab6c0(obj, index, ep[index].data.text, ep[index].maxLength, 0);
    ClearKeyQueue();
}

// FUNCTION: 0x4a7290
int __stdcall HandleTextInput(Object_004a7290* obj, int index, char* text)
{
    Entry_004a7290* entries = obj->holder->entries;
    Out_4a15c0 rect;
    FUN_004a15c0((char*)entries, index, &rect);
    SelectFontForEntry(entries, index);

    Point_004a7290 point = obj->point;
    int rel_x = point.x - entries->x0;
    int rel_y = point.y - entries->y0;

    if (FUN_004a1920(&rect, rel_x, rel_y)) {
        obj->field_68 = index;
        if (IsMouseButtonMessage(obj, 1)) {
            Activate(obj, index);
            FUN_004ab690(obj, 1);
        } else if (IsMouseButtonMessage(obj, 2)) {
            Activate(obj, index);
            FUN_004ab690(obj, 2);
        }
    }

    if (FUN_0049fcf0(obj, index)) {
        SetTextColors(obj->colors[entries[index].colourIndex],
                     obj->colors[entries[index].field_23]);
        int r = HandleTextEditKey(obj, index, text);
        if (r == 13) {
            FUN_0049fc40(obj);
            return 1;
        }
        if (r == 27) {
            FUN_0049fc40(obj);
            entries[index].data.text[0] = 0;
            return 1;
        }
        if (obj->holder)
            obj->holder->field_14 = 1;
    }
    return 0;
}
