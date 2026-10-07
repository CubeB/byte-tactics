// Decompiled by deepseek-v4.1, edited by deepseek-v4.1-flash. Names are provisional.
//
// GUI layer loader.
//
// Suspected original bug: when HAPI_FileLengthByName(layerName) returns 0 (GUI file
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
    // Must stay a second field after field_60.
    int field_64;                  // +0x64
    char unknown_68[0x8b2 - 0x68];
    unsigned char field_8b2[0x104];// +0x8b2
    char name[0x100];              // +0x9b6
};

#pragma pack(pop)

extern void __stdcall FadeRectangle(int handle, int* rect, int mode);
extern char* __stdcall StripPath(char* path);
extern char* __stdcall ChangeExtension(char* out, char* in, char* ext);
extern int __stdcall HAPI_FileLengthByName(char* path);
extern void* __cdecl FUN_004d83b0(const char* path, unsigned int size);
extern int __stdcall ReadGuiFile(void* entry, char* path);
extern void __cdecl FUN_004d85a0(void* p);
extern int __stdcall RenderLayer(Menu_004aa8f0* menu, unsigned int flags);
extern void __cdecl FUN_004c2470(void);
extern void __cdecl FUN_004c2870(void);
extern void __stdcall FUN_004a7960(Menu_004aa8f0* menu, int value);
extern void __stdcall FUN_0049fc50(Menu_004aa8f0* menu, int value);
extern int __cdecl GetTextKeyColor(void);
extern void __stdcall SetTextColors(int a, int b);
extern void __stdcall SetFont(int a);
extern void __cdecl ClearKeyQueue(void);
extern void __stdcall FUN_004ab6c0(Menu_004aa8f0* menu, int a, char* text,
                                   int maxLength, int clear);
extern int* g_guiContext;

// Must stay an inline helper returning an index or -1: sets the search's registers.
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
Layer_004aa8f0* __stdcall LoadGuiLayer(Menu_004aa8f0* menu, const char* name,
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
            FadeRectangle(cur->entries->handle, rect, -0x18);
            if (menu->layer != 0)
                menu->layer->field_14 = 1;
        } else {
            FadeRectangle(0, 0, -0x18);
        }
    }
    strncpy(layerName, menu->name, 0x100);
    strcat(layerName, name);
    strncpy(guiName, name, 0x100);
    StripPath(guiName);
    ChangeExtension(layerName, layerName, "GUI");
    if (HAPI_FileLengthByName(layerName) != 0) {
        // Declared here with its initializer: it spills to the +0x18 slot and keeps the frame.
        int mask = flags & 0x200;
        if (mask != 0) {
            layer = menu->layer;
            entry = &layer->entries[layer->entries->count + 1];
        } else {
            layer = (Layer_004aa8f0*)FUN_004d83b0(guiName, 0x10f57);
            memset(layer, 0, 0x10f57);
            entry = (Entry_004aa8f0*)((char*)layer + 0x3f);
        }
        if (ReadGuiFile(entry, layerName) != 0) {
          if (mask != 0) {
            int idx = FindPanel_004aa8f0(layer);
            if (idx != -1) {
                layer->entries[idx].field_29 = 0;
                flags |= 0x20;
                // Index layer->entries at each use, no local base: gives the reloads.
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
        ret = RenderLayer(menu, flags | 1);
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
            // while, not do/while, as for okName; names indexed as entry[i].name.
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
            // A while over i < count + 1.
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
    // The free path stays last.
    if (ret == 1) {
        if (entry->count == 1 && ((char*)entry)[0x15b] == 3) {
        Entry_004aa8f0* base = menu->layer->entries;
        Entry_004aa8f0* sub = &base[1];
        int r = GetTextKeyColor();
        // Zeroed before the byte load; SetTextColors stays declared (int, int).
        unsigned int v = 0;
        v = menu->field_8b2[sub->field_1f];
        SetTextColors(v, r);
        int i = 0;
        int j;
        // Array indexing, not a walking pointer: a pointer spills i to memory.
        int n = base->count + 1;
        for (j = 1; j < n; j++) {
            if (base[j].type != 7) {
                continue;
            }
            if (i == base[1].field_28) {
                SetFont(*(int*)((char*)&base[j] + 0xd6));
                break;
            }
            i++;
        }
        if (j == base->count + 1)
            SetFont(*g_guiContext);
        FUN_0049fc50(menu, 1);
        menu->layer->field_20 = 1;
        FUN_004ab6c0(menu, 1, (char*)sub + 0xb6,
                     *(short*)((char*)sub + 0x138), 0);
        ClearKeyQueue();
        }
        return layer;
    }
    FUN_004d85a0(layer);
    return 0;
}
