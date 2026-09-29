// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL: this is a structural first pass at the GUI layer loader.
// FUN_004aa8f0 builds (or appends to) a Layer from a .GUI file: it prefixes
// the menu name with "GUI", parses the file into an entry block, links the
// layer into the menu list and wires the PANEL/PREV/NEXT/Cancel controls.
//
// Best result so far: 32.7 % (1754 vs 1762 bytes), first pass.
//
// What still differs: register allocation and the local-slot map. The
// original keeps the freshly allocated entry block in [esp+0x18] and the
// layer in [esp+0x10]; [esp+0x10] is only ever stored on the flags&0x200
// path (0x4aaa3b), which makes the flags&0x200==0 path look like it reads an
// uninitialised [esp+0x10] at 0x4aac09 / 0x4aac2d and [alloc+4] (zeroed by
// the memset) at 0x4aaab9. Our frame is also 4 bytes larger (add esp,0x220
// vs 0x21c). The entry memcpy and the tail are present but unverified.
//
// Confirmed local slot map (relative to esp after the four register pushes,
// i.e. esp = E-0x22c where E is the entry esp):
//   original: layer [esp+0x10], ret [esp+0x14], mask/alloc [esp+0x18],
//             rect [esp+0x1c], layerName [esp+0x2c], guiName [esp+0x12c].
//             Only THREE dwords precede rect, so `entry` has no stack home:
//             the original keeps it in ebp (xor ebp,ebp at 0x4aa900) and keeps
//             `flags` in ebx. mask and the malloc pointer share slot 0x18
//             (mask is dead on the alloc path, so MSVC colours them together).
//   ours:     layer [esp+0x10], base/i [esp+0x14], n/dst [esp+0x18],
//             ret [esp+0x1c], rect [esp+0x20], layerName [esp+0x30],
//             guiName [esp+0x130].
// The blocker is that our loop temporaries (base, n, i) get stack homes at
// 0x14/0x18 and push ret to 0x1c, making the frame 0x220. Removing the `mask`
// local by writing (flags & 0x200) inline did NOT change the frame or the
// score (still 32.7%), so the extra dword is one of the loop temporaries.
// The original's prologue also loads `flags` into eax BEFORE saving registers
// (mov eax,[esp+0x228]; test ah,8) and uses `xor ebp,ebp` for entry = 0.
#include <string.h>

#pragma pack(push, 1)

// 0x15b-byte GUI control record. Entry 0 of a layer is a header whose
// +0xb6 short is the layer's entry count and whose +0xcc/+0xdc/+0xec hold
// the OK/NEXT, PREV/Cancel and focus-text names.
struct Entry_004aa8f0 {
    unsigned char type;              // +0x00
    char unknown_1;
    char name[0x10];                 // +0x02
    char unknown_12;
    short x;                         // +0x13
    short y;                         // +0x15
    short w;                         // +0x17
    short h;                         // +0x19
    char unknown_1b[0x1f - 0x1b];
    int field_1f;                    // +0x1f
    char unknown_23[0x28 - 0x23];
    unsigned char field_28;          // +0x28
    unsigned char field_29;          // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                     // +0xb6
    char unknown_b8[0xbc - 0xb8];
    int handle;                      // +0xbc
    char unknown_c0[0xcc - 0xc0];
    char okName[0x10];               // +0xcc
    char prevName[0x10];             // +0xdc
    char focusName[0x10];            // +0xec
    char unknown_fc[0x15b - 0xfc];
};

struct Layer_004aa8f0 {
    Layer_004aa8f0* next;            // +0x00
    Entry_004aa8f0* entries;         // +0x04
    void* handler;                   // +0x08
    int field_c;                     // +0x0c
    int flags;                       // +0x10
    int field_14;                    // +0x14
    int field_18;                    // +0x18
    int field_1c;                    // +0x1c
    int field_20;                    // +0x20
    int field_24;                    // +0x24
    char unknown_28[0x3b - 0x28];
    int field_3b;                    // +0x3b
};

struct Menu_004aa8f0 {
    char unknown_0[0x18];
    Layer_004aa8f0* layer;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                    // +0x60
    char unknown_64[0x9b6 - 0x64];
    char name[0x100];                // +0x9b6
};

#pragma pack(pop)

extern void* FUN_004bf4d0(int handle, int* rect, int mode);
extern char* FUN_004bb150(char* path);
extern char* FUN_004baff0(char* out, char* in, char* ext);
extern int FUN_004bbc40(char* path);
extern void* FUN_004d83b0(char* path, int size);
extern int FUN_004aeac0(void* entry, char* path);
extern void FUN_004d85a0(void* p);
extern int FUN_004a81e0(Menu_004aa8f0* menu, int flags);
extern void FUN_004c2470(void);
extern void FUN_004c2870(void);
extern void FUN_004a7960(Menu_004aa8f0* menu, int value);
extern void FUN_0049fc50(Menu_004aa8f0* menu, int value);
extern int FUN_004c13f0(void);
extern void FUN_004c13a0(int a, int b);
extern void FUN_004c1420(int a);
extern void FUN_004c1a40(void);
extern void FUN_004ab6c0(Menu_004aa8f0* menu, int a, char* text, int maxLength, int clear);
extern int* DAT_0051fba4;

// FUNCTION: 0x4aa8f0
Layer_004aa8f0* __stdcall FUN_004aa8f0(Menu_004aa8f0* menu, const char* name, unsigned int flags)
{
    int ret = 1;
    Layer_004aa8f0* layer = 0;
    Entry_004aa8f0* entry = 0;
    int rect[4];
    char layerName[0x100];
    char guiName[0x100];

    if (flags & 0x800) {
        Layer_004aa8f0* cur = menu->layer;
        if (cur == 0) {
            FUN_004bf4d0(0, 0, -0x18);
        } else {
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
            FUN_004bf4d0(e->handle, rect, -0x18);
            if (menu->layer != 0)
                menu->layer->field_14 = 1;
        }
    }
    strncpy(layerName, menu->name, 0x100);
    strncpy(guiName, name, 0x100);
    FUN_004bb150(guiName);
    FUN_004baff0(layerName, layerName, "GUI");
    if (FUN_004bbc40(layerName) != 0) {
        int mask = flags & 0x200;
        if (mask == 0) {
            entry = (Entry_004aa8f0*)FUN_004d83b0(guiName, 0x10f57);
            layer = (Layer_004aa8f0*)entry;
            memset(entry, 0, 0x10f57);
            entry = (Entry_004aa8f0*)((char*)entry + 0x3f);
        } else {
            layer = menu->layer;
            entry = (Entry_004aa8f0*)((char*)layer->entries
                    + (*(short*)((char*)layer->entries + 0xb6) + 1) * 0x15b);
        }
        if (FUN_004aeac0(entry, layerName) == 0) {
            FUN_004d85a0(layer);
        } else if (mask != 0) {
            Entry_004aa8f0* base = layer->entries;
            int idx = -1;
            int n = *(short*)((char*)base + 0xb6) + 1;
            if (n > 1) {
                Entry_004aa8f0* e = (Entry_004aa8f0*)((char*)base + 0x15d);
                int i = 1;
                do {
                    if (strncmp((char*)e, "PANEL", 0x10) == 0) {
                        idx = i;
                        break;
                    }
                    i++;
                    e = (Entry_004aa8f0*)((char*)e + 0x15b);
                } while (i < n);
            }
            if (idx == -1) {
                int i = 1;
                if (entry->count >= 1) {
                    short* p = (short*)((char*)entry + 0x170);
                    do {
                        p[-1] += entry->x;
                        p[0] += entry->y;
                        i++;
                        p = (short*)((char*)p + 0x15b);
                    } while (i <= entry->count);
                }
            } else {
                Entry_004aa8f0* sel = (Entry_004aa8f0*)((char*)base + idx * 0x15b);
                sel->field_29 = 0;
                flags |= 0x20;
                int dx = ((int)sel->w - (int)entry->w) / 2 + (int)sel->x;
                int dy = ((int)sel->h - (int)entry->h) / 2 + (int)sel->y;
                int i = 1;
                if (entry->count >= 1) {
                    short* p = (short*)((char*)entry + 0x170);
                    do {
                        p[-1] += (short)dx;
                        p[0] += (short)dy;
                        i++;
                        p = (short*)((char*)p + 0x15b);
                    } while (i <= entry->count);
                }
            }
            *(short*)((char*)base + 0xb6) += entry->count;
            memcpy(entry, (char*)entry + 0x15b, entry->count * 0x15b);
            entry = (Entry_004aa8f0*)layer->entries;
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
    menu->field_60 = -1;
    if ((flags & 0x400) == 0) {
        FUN_004c2470();
        ret = FUN_004a81e0(menu, flags | 1);
        FUN_004c2870();
    }
    {
        Entry_004aa8f0* base = layer->entries;
        char* dst;
        int i;
        // OK / NEXT
        dst = base->okName;
        if (*dst == 0) {
            i = 1;
            if (base->count >= 1) {
                Entry_004aa8f0* e = (Entry_004aa8f0*)((char*)base + 0x15d);
                do {
                    if (e[-0].type == 1 && (_strnicmp((char*)e, "OK", 2) == 0
                            || _strnicmp((char*)e, "NEXT", 4) == 0)) {
                        strcpy(dst, (char*)base + i * 0x15b + 2);
                        break;
                    }
                    i++;
                    e = (Entry_004aa8f0*)((char*)e + 0x15b);
                } while (i <= base->count);
            }
        }
        // PREV / Cancel
        dst = base->prevName;
        if (*dst == 0) {
            i = 1;
            if (base->count >= 1) {
                Entry_004aa8f0* e = (Entry_004aa8f0*)((char*)base + 0x15d);
                do {
                    if (e->type == 1 && (_strnicmp((char*)e, "PREV", 4) == 0
                            || _strnicmp((char*)e, "Cancel", 6) == 0)) {
                        strcpy(dst, (char*)base + i * 0x15b + 2);
                        break;
                    }
                    i++;
                    e = (Entry_004aa8f0*)((char*)e + 0x15b);
                } while (i <= base->count);
            }
        }
        // focus control name
        dst = base->focusName;
        if (*dst == 0) {
            layer->field_20 = 0;
            FUN_004a7960(menu, 1);
        } else {
            int found = -1;
            int n = base->count + 1;
            if (n > 1) {
                Entry_004aa8f0* e = (Entry_004aa8f0*)((char*)base + 0x15d);
                i = 1;
                do {
                    if (strncmp((char*)e, dst, 0x10) == 0) {
                        found = i;
                        break;
                    }
                    i++;
                    e = (Entry_004aa8f0*)((char*)e + 0x15b);
                } while (i < n);
            }
            layer->field_20 = found;
        }
    }
    menu->field_60 = -1;
    if (ret != 1) {
        FUN_004d85a0(layer);
        return 0;
    }
    if (layer->entries->count == 1 && ((char*)layer->entries)[0x15b] == 3) {
        Entry_004aa8f0* base = layer->entries;
        Entry_004aa8f0* e1 = (Entry_004aa8f0*)((char*)base + 0x15b);
        int r = FUN_004c13f0();
        FUN_004c13a0(*(unsigned char*)(e1->field_1f + (char*)menu + 0x8b2), r);
        int i = 0;
        int j = 1;
        int n = e1->count + 1;
        if (n > 1) {
            Entry_004aa8f0* e = (Entry_004aa8f0*)((char*)base + 0x15b);
            do {
                if (e->type == 7) {
                    if (i == base->field_28) {
                        FUN_004c1420(*(int*)((char*)e + 0xd6));
                        break;
                    }
                    i++;
                }
                j++;
                e = (Entry_004aa8f0*)((char*)e + 0x15b);
            } while (j < n);
        }
        if (j == n)
            FUN_004c1420(*DAT_0051fba4);
        FUN_0049fc50(menu, 1);
        menu->layer->field_20 = 1;
        FUN_004ab6c0(menu, 1, (char*)e1 + 0xb6, *(short*)((char*)e1 + 0x138), 0);
        FUN_004c1a40();
    }
    return layer;
}
