// Decompiled by deepseek-v4.1, edited by deepseek-v4.1-flash. Names are provisional.
//
// MATCH: GUI layer loader (0x4aa8f0, 1762 bytes, byte-identical).
//
// Techniques that mattered (earlier passes, then the pass that finished it):
//  * `int mask = flags & 0x200;` must be declared inside the block with its
//    initializer. MSVC5 then spills it to the +0x18 slot and keeps the 0x21c
//    frame; any other shape CSEs it into a register and the whole frame
//    shifts by 4, which costs 20 points.
//  * The PANEL search is an inlined helper returning an index or -1:
//    `static inline int FindPanel(Layer*)` with
//    `for (i = 1; i < base->count + 1; i++) if (strncmp(base[i].name,
//    "PANEL", 0x10) == 0) return i; return -1;`, called as
//    `int idx = FindPanel(layer);`. That produces the original's EBX = base,
//    ESI = i, EDI = e, the reload of EDI from [S+0x10] on both loop exits,
//    and the shared `cmp ecx,-1` test.
//  * The found arm reloads `layer->entries` per use (write
//    `layer->entries[idx]` out in dx/dy instead of a local `base`), which is
//    what gives the two `mov ecx,[edi+4]` / `mov edx,[edi+4]` reloads.
//  * The final 7/field_28 search is array indexing, not a walking pointer:
//    `int n = base->count + 1; for (j = 1; j < n; j++) { if (base[j].type
//    != 7) continue; if (i == base[1].field_28) {...break;} i++; }`. That
//    spelling puts count+1 in EAX, j in ESI, i in EDX and keeps the `jl` at
//    the bottom; a pointer-walking `e` variable instead spills i to memory.
//  * okName and prevName are both `while (i <= entry->count)` loops (a
//    do/while for prevName emits the mirrored `jg exit; jmp body`).
//  * Other load-bearing shapes: `entry[i].name` indexing in the name
//    searches, `base[1].field_28`, focusName searched with a `while` and
//    `i < entry->count + 1`, the Menu field at +0x60 as a second field
//    (field_64 at +0x64), `if (ret == 1) { ...; return layer; }` with the
//    free path last, and FUN_004c13a0 declared `(int, int)` with a zeroed
//    `unsigned int v` before the byte load.
//
// Suspected original bug: when FUN_004bbc40(layerName) returns 0 (GUI file
// missing) the code jumps to 0x4aac2d, which loads `layer` from [S+0x10]
// before it was ever stored (the only store is the mask-path one at
// 0x4aaa3b) and then writes layer->entries/field_1c/field_24/field_3b
// through it and returns the garbage pointer.
#include <string.h>

#pragma pack(push, 1)

struct Layer_004aa8f0;

// 0x15b-byte GUI control record.
struct Entry_004aa8f0 {
    unsigned char type;            // +0x00
    char unknown_1;
    char name[0x10];               // +0x02
    char unknown_12;
    short x;                       // +0x13
    short y;                       // +0x15
    short w;                       // +0x17
    short h;                       // +0x19
    char unknown_1b[4];
    int field_1f;                  // +0x1f
    char unknown_23[5];
    char field_28;                 // +0x28
    unsigned char field_29;        // +0x29
    char unknown_2a[0x8c];
    short count;                   // +0xb6
    char unknown_b8[4];
    int handle;                    // +0xbc
    char unknown_c0[0x0c];
    char okName[0x10];             // +0xcc
    char prevName[0x10];           // +0xdc
    char focusName[0x10];          // +0xec
    char unknown_fc[0x5f];
};

struct Layer_004aa8f0 {
    Layer_004aa8f0* next;          // +0x00
    Entry_004aa8f0* entries;       // +0x04
    int field_08;
    int field_0c;
    int flags;                     // +0x10
    int field_14;                  // +0x14
    int field_18;                  // +0x18
    int field_1c;                  // +0x1c
    int field_20;                  // +0x20
    int field_24;                  // +0x24
    char unknown_28[0x13];
    int field_3b;                  // +0x3b
};

struct Menu_004aa8f0 {
    char unknown_0[0x18];
    Layer_004aa8f0* layer;         // +0x18
    char unknown_1c[0x44];
    int field_60;                  // +0x60
    int field_64;                  // +0x64
    char unknown_68[0x8b2 - 0x68];
    unsigned char field_8b2[0x104];// +0x8b2
    char name[0x100];              // +0x9b6
};

#pragma pack(pop)

extern void __stdcall FUN_004bf4d0(int handle, int* rect, int mode);
extern char* __stdcall FUN_004bb150(char* path);
extern char* __stdcall FUN_004baff0(char* out, char* in, char* ext);
extern int __stdcall FUN_004bbc40(char* path);
extern void* __cdecl FUN_004d83b0(const char* path, unsigned int size);
extern int __stdcall FUN_004aeac0(void* entry, char* path);
extern void __cdecl FUN_004d85a0(void* p);
extern int __stdcall FUN_004a81e0(Menu_004aa8f0* menu, unsigned int flags);
extern void __cdecl FUN_004c2470(void);
extern void __cdecl FUN_004c2870(void);
extern void __stdcall FUN_004a7960(Menu_004aa8f0* menu, int value);
extern void __stdcall FUN_0049fc50(Menu_004aa8f0* menu, int value);
extern int __cdecl FUN_004c13f0(void);
extern void __stdcall FUN_004c13a0(int a, int b);
extern void __stdcall FUN_004c1420(int a);
extern void __cdecl FUN_004c1a40(void);
extern void __stdcall FUN_004ab6c0(Menu_004aa8f0* menu, int a, char* text,
                                   int maxLength, int clear);
extern int* DAT_0051fba4;

static inline int FindPanel_004aa8f0(Layer_004aa8f0* layer)
{
    Entry_004aa8f0* base = layer->entries;
    int i;
    for (i = 1; i < base->count + 1; i++) {
        if (strncmp(base[i].name, "PANEL", 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4aa8f0
Layer_004aa8f0* __stdcall FUN_004aa8f0(Menu_004aa8f0* menu, const char* name,
                                       unsigned int flags)
{
    Layer_004aa8f0* layer;
    int ret = 1;
    int mask;
    int rect[4];
    char layerName[0x100];
    char guiName[0x100];
    Entry_004aa8f0* entry = 0;

    if (flags & 0x800) {
        Layer_004aa8f0* cur = menu->layer;
        if (cur != 0) {
            Entry_004aa8f0* e = cur->entries;
            if (e->type == 0) {
                rect[0] = 0;
                rect[1] = 0;
            } else {
                rect[0] = e->x;
                rect[1] = e->y;
            }
            rect[2] = rect[0] + e->w - 1;
            rect[3] = rect[1] + e->h - 1;
            FUN_004bf4d0(cur->entries->handle, rect, -0x18);
            if (menu->layer != 0)
                menu->layer->field_14 = 1;
        } else {
            FUN_004bf4d0(0, 0, -0x18);
        }
    }
    strncpy(layerName, menu->name, 0x100);
    strcat(layerName, name);
    strncpy(guiName, name, 0x100);
    FUN_004bb150(guiName);
    FUN_004baff0(layerName, layerName, "GUI");
    if (FUN_004bbc40(layerName) != 0) {
        int mask = flags & 0x200;
        if (mask != 0) {
            layer = menu->layer;
            entry = &layer->entries[layer->entries->count + 1];
        } else {
            layer = (Layer_004aa8f0*)FUN_004d83b0(guiName, 0x10f57);
            memset(layer, 0, 0x10f57);
            entry = (Entry_004aa8f0*)((char*)layer + 0x3f);
        }
        if (FUN_004aeac0(entry, layerName) != 0) {
          if (mask != 0) {
            int idx = FindPanel_004aa8f0(layer);
            if (idx != -1) {
                layer->entries[idx].field_29 = 0;
                flags |= 0x20;
                int dx = (layer->entries[idx].w - entry->w) / 2 + layer->entries[idx].x;
                int dy = (layer->entries[idx].h - entry->h) / 2 + layer->entries[idx].y;
                int j = 1;
                if (j <= entry->count) {
                    Entry_004aa8f0* e = &entry[1];
                    do {
                        e->x += dx;
                        e->y += dy;
                        j++;
                        e++;
                    } while (j <= entry->count);
                }
            } else {
                int j = 1;
                if (j <= entry->count) {
                    Entry_004aa8f0* e = &entry[1];
                    do {
                        e->x += entry->x;
                        e->y += entry->y;
                        j++;
                        e++;
                    } while (j <= entry->count);
                }
            }
            layer->entries->count += entry->count;
            memcpy(entry, &entry[1], entry->count * 0x15b);
            entry = layer->entries;
          }
        } else {
            FUN_004d85a0(layer);
        }
    }
    layer->entries = entry;
    layer->field_1c = 0;
    layer->field_24 = 0;
    layer->field_3b = 0;
    if ((flags & 0x200) == 0) {
        layer->next = menu->layer;
        menu->layer = layer;
    }
    layer->flags = 0;
    if ((flags & 0x200) == 0 && (flags & 0x80) != 0)
        layer->flags = 0x80;
    layer->flags |= flags & 0x800;
    if (menu->layer != 0)
        menu->layer->field_14 = 1;
    if (menu->layer != 0)
        menu->layer->field_18 = 0;
    strncpy((char*)entry + 2, guiName, 0x10);
    menu->field_64 = -1;
    if ((flags & 0x400) == 0) {
        FUN_004c2470();
        ret = FUN_004a81e0(menu, flags | 1);
        FUN_004c2870();
    }
    {
        char* dst = entry->okName;
        if (strlen(dst) == 0) {
            int i = 1;
            while (i <= entry->count) {
                if (entry[i].type == 1 && (_strnicmp(entry[i].name, "OK", 2) == 0
                        || _strnicmp(entry[i].name, "NEXT", 4) == 0)) {
                    strcpy(dst, entry[i].name);
                    break;
                }
                i++;
            }
        }
        dst = entry->prevName;
        if (strlen(dst) == 0) {
            int i = 1;
            while (i <= entry->count) {
                if (entry[i].type == 1 && (_strnicmp(entry[i].name, "PREV", 4) == 0
                        || _strnicmp(entry[i].name, "Cancel", 6) == 0)) {
                    strcpy(dst, entry[i].name);
                    break;
                }
                i++;
            }
        }
        dst = entry->focusName;
        if (strlen(dst) != 0) {
            int i = 1;
            Entry_004aa8f0* e = &entry[1];
            while (i < entry->count + 1) {
                if (strncmp(e->name, dst, 0x10) == 0)
                    goto focusFound;
                i++;
                e++;
            }
            i = -1;
        focusFound:
            layer->field_20 = i;
        } else {
            layer->field_20 = 0;
            FUN_004a7960(menu, 1);
        }
    }
    menu->field_60 = -1;
    if (ret == 1) {
        if (entry->count == 1 && ((char*)entry)[0x15b] == 3) {
        Entry_004aa8f0* base = menu->layer->entries;
        Entry_004aa8f0* sub = &base[1];
        int r = FUN_004c13f0();
        unsigned int v = 0;
        v = menu->field_8b2[sub->field_1f];
        FUN_004c13a0(v, r);
        int i = 0;
        int j;
        int n = base->count + 1;
        for (j = 1; j < n; j++) {
            if (base[j].type != 7) {
                continue;
            }
            if (i == base[1].field_28) {
                FUN_004c1420(*(int*)((char*)&base[j] + 0xd6));
                break;
            }
            i++;
        }
        if (j == base->count + 1)
            FUN_004c1420(*DAT_0051fba4);
        FUN_0049fc50(menu, 1);
        menu->layer->field_20 = 1;
        FUN_004ab6c0(menu, 1, (char*)sub + 0xb6,
                     *(short*)((char*)sub + 0x138), 0);
        FUN_004c1a40();
        }
        return layer;
    }
    FUN_004d85a0(layer);
    return 0;
}
