// Decompiled by deepseek-v4.1, deepseek-v4.1-flash, Opus, Haiku, Sonnet and space-bunny-free. Names are provisional.
//
// The dialog's GUI layer loader, the entry-list operations that add and find
// entries, the animation reference and mouse-message helpers, and the
// text-edit key handler.
#include <string.h>
#include <windows.h>

#pragma pack(push, 1)

struct Layer_004aa8f0;

// 0x15b-byte GUI control record: entry 0 holds the count at +0xb6 and the
// dialog's own fields from +0xbc on, the other entries hold NUL terminated
// text there.
struct Entry_004aa8f0 {
    unsigned char type;            // +0x00
    unsigned char group;           // +0x01
    char name[0x10];               // +0x02
    char unknown_12;
    short x;                       // +0x13
    short y;                       // +0x15
    short w;                       // +0x17
    short h;                       // +0x19
    // AddTextGadget stores a dword here, HandleTextEditKey tests a byte.
    union {
        int flags;                 // +0x1b
        unsigned char flag_1b;     // +0x1b
    };
    int field_1f;                  // +0x1f
    int field_23;                  // +0x23
    unsigned char field_27;        // +0x27
    char field_28;                 // +0x28
    unsigned char field_29;        // +0x29
    unsigned char field_2a;        // +0x2a
    char unknown_2b[0xb6 - 0x2b];
    union {
        struct {                   // entry 0: the count and the dialog's fields
            short count;           // +0xb6
            char unknown_b8[4];
            union {
                int handle;        // +0xbc, the surface handle FadeRectangle takes
                void* surface;     // +0xbc, the image DrawSurface takes
            };
            char unknown_c0[0xc];
            char okName[0x10];     // +0xcc
            char prevName[0x10];   // +0xdc
            char focusName[0x10];  // +0xec
            char unknown_fc[0x5f];
        };
        struct {                   // the other entries: the NUL terminated text
            char text[0x80];       // +0xb6
            char unknown_136[2];
            short capacity;        // +0x138
            char unknown_13a[0x21];
        };
    };
};

// The layer a dialog's +0x18 points at: its entry table at +4.
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

struct Src_004ab400;

// The animation reference at +0x30 of a dialog.
struct Ref_004ab400 {
    unsigned short index;          // +0x0
    unsigned short value;          // +0x2
    unsigned char kind;            // +0x4
    char unknown_5[3];
    Src_004ab400* src;             // +0x8
};

struct Flags_004ab400 {
    unsigned int active : 1;
    unsigned int rest : 31;
};

// A mouse event, 0x18 bytes: the point at +0x0/+0x4, the code at +0x8 and
// the message at +0x10.
struct Event_004ab5d0 {
    int data[6];
};

struct Rect_004b6720 {
    int left;
    int top;
    int right;
    int bottom;
};

// The dialog: a layer at +0x18, the animation reference at +0x30, the last
// mouse event at +0x3c, flags at +0x5c, the text cursor at +0x74, and the
// menu's own fields.
struct Menu_004aa8f0 {
    char unknown_0[0x18];
    Layer_004aa8f0* layer;         // +0x18
    int field_1c;                  // +0x1c
    int field_20;                  // +0x20
    int field_24;                  // +0x24
    Src_004ab400* src_28;          // +0x28
    Src_004ab400* src_2c;          // +0x2c
    Ref_004ab400 ref;              // +0x30
    Event_004ab5d0 event;          // +0x3c
    int field_54;                  // +0x54
    int field_58;                  // +0x58
    Flags_004ab400 flags_5c;       // +0x5c
    int field_60;                  // +0x60
    // Must stay a second field after field_60.
    int field_64;                  // +0x64
    char unknown_68[0x74 - 0x68];
    int cursor;                    // +0x74
    char unknown_78[0x9a - 0x78];
    int field_9a;                  // +0x9a
    char unknown_9e[0x8b2 - 0x9e];
    unsigned char field_8b2[0x104];// +0x8b2
    char name[0x100];              // +0x9b6
};

// The records AddButtonGadget, FUN_004ab310 and FUN_004ab3a0 copy into an
// entry slot: the slot's first bytes viewed as the control's definition.
struct Record_004ab2b0 {
    unsigned char type;            // +0x0
    char unknown_1[0xb6 - 0x1];
    short count;                   // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x13e - 0xb8];
};

struct Entry_004ab2b0 {
    Record_004ab2b0 record;        // +0x0
    char unknown_13e[0x15b - 0x13e];
};

struct Record_004ab310 {
    unsigned char type;            // +0x0
    char unknown_1[0xb6 - 0x1];
    union {
        short count;               // +0xb6 (only meaningful in entry 0)
        int field_b6;              // +0xb6
    };
    char unknown_ba[0xbe - 0xba];
    int field_be;                  // +0xbe
    int field_c2;                  // +0xc2
    short field_c6;                // +0xc6
    char unknown_c8[0xcc - 0xc8];
};

struct Entry_004ab310 {
    Record_004ab310 record;        // +0x0
    char unknown_cc[0x15b - 0xcc];
};

struct Record_004ab3a0 {
    unsigned char type;            // +0x0
    char unknown_1[0xb6 - 0x1];
    short count;                   // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xd6 - 0xb8];
};

struct Entry_004ab3a0 {
    Record_004ab3a0 record;        // +0x0
    char unknown_d6[0x15b - 0xd6];
};

#pragma pack(pop)

extern int* g_guiContext;

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
extern void __stdcall FUN_004ab6c0(Menu_004aa8f0* menu, int index, char* text,
                                   int maxLength, int clear);

extern int __stdcall PointInRect(Rect_004b6720* r, int x, int y);
extern int __stdcall RectsOverlap(Rect_004b6720* a, Rect_004b6720* b);
extern void __stdcall DrawSurface(void* dest, void* image, int x, int y);

extern void __stdcall InitGafSequence(Ref_004ab400* ref, Src_004ab400* src, int index);
extern int __stdcall GetGafSequenceFrame(Ref_004ab400* ref);
extern void __stdcall FUN_004c2b20(int handle);
int FUN_004c2ba0();

extern void __stdcall AdvanceGafSequence(Ref_004ab400* ref, int step);
extern int __stdcall PeekMouseEvent(Event_004ab5d0* out);
extern void __stdcall GetCurrentMouseEvent(Event_004ab5d0* out);
extern void __stdcall PopMouseEvent(Event_004ab5d0* out);
extern void __stdcall FUN_004a1680(Entry_004aa8f0* table, int index, Rect_004b6720* out);
extern int __stdcall FUN_004a1920(Rect_004b6720* r, int px, int py);

extern void __stdcall DrawTextInput(Menu_004aa8f0* control, int index);
extern int __stdcall GetTextPixelWidth(char* text);
int PopKey();
void* GetDisplay();

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

// GUI layer loader.
//
// Suspected original bug: when HAPI_FileLengthByName(layerName) returns 0 (GUI file
// missing) the code jumps to 0x4aac2d, which loads `layer` from [S+0x10]
// before it was ever stored (the only store is the mask-path one at
// 0x4aaa3b) and then writes layer->entries/field_1c/field_24/field_3b
// through it and returns the garbage pointer.
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

// Returns whether the point stored at +0x3c/+0x40 lies inside the rectangle
// (x, y, width, height) of the object's entry; 0 when there is no entry.
// FUNCTION: 0x4aafe0
int __stdcall IsPointInEntryRect(Menu_004aa8f0* obj)
{
    if (!obj->layer)
        return 0;
    Entry_004aa8f0* info = obj->layer->entries;
    Rect_004b6720 r;
    r.left = info->x;
    r.top = info->y;
    r.right = r.left + info->w - 1;
    r.bottom = r.top + info->h - 1;
    return PointInRect(&r, obj->event.data[0], obj->event.data[1]);
}

// FUNCTION: 0x4ab040
int __stdcall FUN_004ab040(Menu_004aa8f0* obj)
{
    return obj->layer != 0;
}

// Returns 1 when the object's entry has a name (after a two-character
// prefix) equal to `name`, ignoring case, over at most 16 characters.
// FUNCTION: 0x4ab060
int __stdcall IsScreenNamed(Menu_004aa8f0* obj, const char* name)
{
    // The entries pointer, two bytes in, is the first entry's name.
    if (obj->layer && _strnicmp((char*)obj->layer->entries + 2, name, 0x10) == 0)
        return 1;
    return 0;
}

// FUNCTION: 0x4ab0a0
void __stdcall FUN_004ab0a0(void* param_1)
{
    *(int*)((char*)param_1 + 0x60) = -1;
}

// FUNCTION: 0x4ab0b0
int __stdcall FUN_004ab0b0(Layer_004aa8f0* node, void* param_2, Rect_004b6720* param_3)
{
    Rect_004b6720 rect;
    if (node == 0) {
        return 0;
    }
    FUN_004ab0b0(node->next, param_2, param_3);
    Entry_004aa8f0* g = node->entries;
    rect.left = g->x;
    rect.top = g->y;
    rect.right = g->w + rect.left - 1;
    rect.bottom = g->h + rect.top - 1;
    if (param_3 == 0) {
        if (node->field_14 == 1) {
            node->field_14 = 0;
            DrawSurface(param_2, g->surface, g->x, g->y);
        }
    } else {
        if (node->field_14 != 1 && RectsOverlap(&rect, param_3) == 0) {
            goto finish;
        }
        node->field_14 = 0;
        DrawSurface(param_2, g->surface, g->x, g->y);
    }
finish:
    return 1;
}

// FUNCTION: 0x4ab170
void __stdcall FUN_004ab170(Menu_004aa8f0* param_1, unsigned int* param_2, int* param_3)
{
    FUN_004ab0b0(param_1->layer, param_2, (Rect_004b6720*)param_3);
}

// FUNCTION: 0x4ab190
void __stdcall FUN_004ab190(int param_1, int param_2)
{
    *(int*)(param_1 + 0xcc6) = param_2;
}

// Appends a type-5 GUI entry to the holder's 0x15b-byte entry table and fills
// it in: type 5, the unbounded name at +2, x/y at +0x13/+0x15, the width at
// +0x17 (defaults to entry 0's width minus x minus 5), flags at +0x1b, the
// text via strncpy(0x7f) at +0xb6, and assorted small fields.
//
// Suspected original bug: the entry name is only 0x10 bytes (+0x02..+0x11)
// but it is filled with an unbounded strcpy, and the caller at 0x4abe6b
// passes the literal "Player%dController" (0x5029f8, 18 characters plus the
// terminator), which spills past the name into the x field at +0x13.
// FUNCTION: 0x4ab1b0
void __stdcall AddTextGadget(Layer_004aa8f0* obj, char* name, char* text,
                            int x, short y, int w, int flags)
{
    Entry_004aa8f0* entries = obj->entries;
    short n = entries->count;
    int index = n + 1;
    n++;
    entries->count = n;
    Entry_004aa8f0* e = &obj->entries[index];
    e->type = 5;
    e->x = x;
    e->y = y;
    if (w == -1)
        e->w = entries->w - x - 5;
    else
        e->w = w;
    e->h = 0xf;
    e->field_1f = 0xf;
    e->flags = flags;
    e->group = 0;
    e->field_23 = 0;
    e->field_27 = 0;
    e->field_28 = 0;
    e->field_29 = 1;
    e->field_2a = 0;
    strcpy(e->name, name);
    strncpy(e->text, text, 0x7f);
    e->text[0x7f] = 0;
}

// FUNCTION: 0x4ab290
int __stdcall FUN_004ab290(Menu_004aa8f0* menu, int value)
{
    if (menu->layer != 0)
        menu->layer->field_24 = value;
    return 1;
}

// Appends a copy of a 0x13e-byte record to the entry list (at most 200
// entries; entry 0 holds the count) and marks the new entry as type 1.
// FUNCTION: 0x4ab2b0
int __stdcall AddButtonGadget(Menu_004aa8f0* obj, Record_004ab2b0* record)
{
    Entry_004ab2b0* entries = (Entry_004ab2b0*)obj->layer->entries;
    if (entries->record.count == 200) {
        return 0;
    }
    short n = ++entries->record.count;
    entries[n].record = *record;
    entries[n].record.type = 1;
    return 1;
}

// Appends a copy of a 0xcc-byte record to the entry list (at most 200
// entries; entry 0 holds the count) and marks the new entry as type 6.
// Same shape as 0x4ab2b0 (type 1) and 0x4ab3a0 (type 0xd).
// FUNCTION: 0x4ab310
int __stdcall FUN_004ab310(Menu_004aa8f0* obj, Record_004ab310* record)
{
    Entry_004ab310* entries = (Entry_004ab310*)obj->layer->entries;
    if (entries->record.count == 200) {
        return 0;
    }
    short n = ++entries->record.count;
    entries[n].record = *record;
    entries[n].record.type = 6;
    Entry_004ab310* dst = (Entry_004ab310*)&obj->layer->entries[n];
    dst->record.field_b6 = 0;
    dst->record.field_be = 0;
    dst->record.field_c2 = 0;
    dst->record.field_c6 = 0;
    return 1;
}

// Appends a copy of a 0xd6-byte record to the entry list (at most 200
// entries; entry 0 holds the count) and marks the new entry as type 0xd.
// Same shape as 0x4ab2b0, which appends a 0x13e-byte type 1 record.
// FUNCTION: 0x4ab3a0
int __stdcall FUN_004ab3a0(Menu_004aa8f0* obj, Record_004ab3a0* record)
{
    Entry_004ab3a0* entries = (Entry_004ab3a0*)obj->layer->entries;
    if (entries->record.count == 200) {
        return 0;
    }
    short n = ++entries->record.count;
    entries[n].record = *record;
    entries[n].record.type = 0xd;
    return 1;
}

// Compare 0x4ab4e0: stores the source, points the reference at +0x30 at its
// first entry, sets the active flag and passes the entry's value on to
// FUN_004c2b20.
// FUNCTION: 0x4ab400
void __stdcall FUN_004ab400(Menu_004aa8f0* p, Src_004ab400* src)
{
    p->src_2c = src;
    InitGafSequence(&p->ref, src, 0);
    p->flags_5c.active = 1;
    FUN_004c2b20(GetGafSequenceFrame(&p->ref));
}

// Picks the source at +0x28 (alt) or +0x2c; with a source, points the
// reference at +0x30 at it (compare 0x4ab400) and sets the active flag,
// otherwise passes the plain value at +0x24 or +0x20 on to FUN_004c2b20
// (compare 0x4ab4e0) and clears the flag. Nothing happens when the source
// or value is already current.
static inline void SetSource(Menu_004aa8f0* p, Src_004ab400* src)
{
    if (p->ref.src == src)
        return;
    InitGafSequence(&p->ref, src, 0);
    FUN_004c2b20(GetGafSequenceFrame(&p->ref));
    p->flags_5c.active = 1;
}

static inline void SetValue(Menu_004aa8f0* p, int value)
{
    if (FUN_004c2ba0() == value)
        return;
    FUN_004c2b20(value);
    p->ref.src = 0;
    p->flags_5c.active = 0;
}

// FUNCTION: 0x4ab440
void __stdcall FUN_004ab440(Menu_004aa8f0* p, int alt)
{
    if (alt) {
        if (p->src_28)
            SetSource(p, p->src_28);
        else
            SetValue(p, p->field_24);
    } else {
        if (p->src_2c)
            SetSource(p, p->src_2c);
        else
            SetValue(p, p->field_20);
    }
}

// A free __stdcall function (param comes off the stack, ecx unused): waits
// on a handle then clears a 1-bit flag inside a dword bitfield (the load of
// the whole dword but an `and al, 0xfe` on just the low byte is the
// dword-bitfield clear idiom from the guide).
// FUNCTION: 0x4ab4c0
void __stdcall FUN_004ab4c0(Menu_004aa8f0* p)
{
    FUN_004c2b20(p->field_1c);
    p->flags_5c.active = 0;
}

// Compare 0x4ab4c0 and 0x4ab400: sets the three values at +0x1c..+0x24,
// passes the value on to FUN_004c2b20 and clears the active flag.
// FUNCTION: 0x4ab4e0
void __stdcall FUN_004ab4e0(Menu_004aa8f0* p, int value)
{
    p->field_1c = value;
    p->field_20 = value;
    p->field_24 = value;
    FUN_004c2b20(value);
    p->flags_5c.active = 0;
    p->field_54 = 0;
    p->src_28 = 0;
    p->src_2c = 0;
}

// Returns 1 when the object's message is a button-down or double-click of a
// button selected by the mask (1 = left, 2 = right). The message is read
// through a reference taken up front, which is why the object pointer is
// loaded before the first test.
// FUNCTION: 0x4ab510
int __stdcall IsMouseButtonMessage(Menu_004aa8f0* obj, unsigned char buttons)
{
    int& msg = obj->event.data[4];
    if (buttons & 1) {
        if (msg == WM_LBUTTONDOWN)
            return 1;
        if (msg == WM_LBUTTONDBLCLK)
            return 1;
    } else if (buttons & 2) {
        if (msg == WM_RBUTTONDOWN)
            return 1;
        if (msg == WM_RBUTTONDBLCLK)
            return 1;
    }
    return 0;
}

// Returns 1 when the object's message is a double-click of a button
// selected by the mask (1 = left, 2 = right); compare 0x4ab510.
// FUNCTION: 0x4ab570
int __stdcall IsDoubleClickMessage(Menu_004aa8f0* obj, unsigned char buttons)
{
    int& msg = obj->event.data[4];
    if (buttons & 1) {
        if (msg == WM_LBUTTONDBLCLK)
            return 1;
    } else if (buttons & 2) {
        if (msg == WM_RBUTTONDBLCLK)
            return 1;
    }
    return 0;
}

// Returns 1 when any of the mask bits are set in the object's field at +0x54.
// FUNCTION: 0x4ab5b0
int __stdcall FUN_004ab5b0(Menu_004aa8f0* obj, unsigned int mask)
{
    return (obj->field_54 & mask) != 0;
}

// One tick of a UI element: advances the reference at +0x30 when the active
// flag is set and forwards the entry it lands on, then peeks the next event,
// tests its point against the element's rectangle (from the entry at +0x18)
// and, on a hit or a plain code, pops the event into the element at +0x3c.
// FUNCTION: 0x4ab5d0
void __stdcall FUN_004ab5d0(Menu_004aa8f0* p)
{
    if (p->flags_5c.active) {
        int old = p->ref.index;
        AdvanceGafSequence(&p->ref, p->field_9a);
        if (p->ref.index != old)
            FUN_004c2b20(GetGafSequenceFrame(&p->ref));
    }

    Event_004ab5d0 e;
    if (PeekMouseEvent(&e) != 0) {
        if (p->layer != 0) {
            Rect_004b6720 r;
            FUN_004a1680(p->layer->entries, 0, &r);
            if (FUN_004a1920(&r, e.data[0], e.data[1]) != 0 || e.data[2] == 0) {
                PopMouseEvent(&e);
                p->field_54 = e.data[2];
                p->event = e;
            }
        }
    } else {
        GetCurrentMouseEvent(&p->event);
    }
}

// FUNCTION: 0x4ab690
void __stdcall FUN_004ab690(void* param_1, int param_2)
{
    *(int*)((char*)param_1 + 0x58) = param_2;
    *(int*)(*(int*)((char*)param_1 + 0x18) + 0x37) = param_2;
}

// FUNCTION: 0x4ab6b0
int __stdcall FUN_004ab6b0(Menu_004aa8f0* obj)
{
    return obj->field_58;
}

// Stores the length of a text in a control (or empties the text when it is
// too long or the flag is set), then refreshes the control. MSVC duplicates
// the shared call into both branches itself.
// The original calls this from LoadGuiLayer (0x4aa8f0), so it must not be
// inlined into it now that both live in one file.
#pragma auto_inline(off)
// FUNCTION: 0x4ab6c0
void __stdcall FUN_004ab6c0(Menu_004aa8f0* control, int index, char* text,
                            int maxLength, int clear)
{
    int length = strlen(text);
    if (clear == 0 && length <= maxLength) {
        control->cursor = length;
    } else {
        *text = 0;
        control->cursor = 0;
    }
    DrawTextInput(control, index);
}
#pragma auto_inline(on)

// Text-edit key handler for one GUI entry (stride 0x15b, text at +0xb6).
// __stdcall(control, entryIndex, key): when the holder has no pending
// event source it pulls keys from PopKey. Handles backspace, escape,
// delete, home, end, left, right, clipboard paste and plain character
// insertion, then saves the text back through DrawTextInput.
// FUNCTION: 0x4ab720
int __stdcall HandleTextEditKey(Menu_004aa8f0* control, int index, int key)
{
    Layer_004aa8f0* holder = control->layer;
    Entry_004aa8f0* entry = &holder->entries[index];
    // Declared before text: makes the subscript the addressing-mode index.
    int i;
    char* text = entry->text;
    int changed = 0;
    int last = 0;

    if (holder->field_18 == 0)
        key = PopKey();
    if (key != 0) {
        changed = 1;
        while (key != 0) {
            last = key;
            int len = strlen(text);
            switch (key) {
            case 0xf1:
                control->cursor = len;
                break;
            case 0xf0:
                control->cursor = 0;
                break;
            case 8:
                if (control->cursor != 0) {
                    control->cursor--;
                    int n = strlen(text);
                    for (i = control->cursor; i < n - 1; i++)
                        text[i] = text[i + 1];
                    text[n - 1] = 0;
                }
                break;
            case 0xf4:
                if (control->cursor != 0)
                    control->cursor--;
                break;
            case 0xf6:
                if (control->cursor < len)
                    control->cursor++;
                break;
            case 0xef:
                if (len != 0 && control->cursor < len) {
                    int n = strlen(text);
                    for (i = control->cursor; i < n - 1; i++)
                        text[i] = text[i + 1];
                    text[n - 1] = 0;
                }
                break;
            case 0xbf:
            case 0xee: {
                HWND hwnd = *(HWND*)((char*)GetDisplay() + 0x40);
                if (OpenClipboard(hwnd)) {
                    HANDLE hMem = GetClipboardData(1);
                    if (hMem != 0) {
                        DWORD size = GlobalSize(hMem);
                        if (size != 0) {
                            char* src = (char*)GlobalLock(hMem);
                            memset(text, 0, 0x80);
                            memcpy(text, src, (int)size < entry->capacity - 1 ? (int)size : entry->capacity - 1);
                            int width = GetTextPixelWidth(text);
                            while (width > entry->w) {
                                if (strlen(text) == 0)
                                    break;
                                text[strlen(text) - 1] = 0;
                                width = GetTextPixelWidth(text);
                            }
                            GlobalUnlock(hMem);
                        }
                    }
                    CloseClipboard();
                }
                break;
            }
            case 0x1b:
                goto done;
            default: {
                if (len == entry->capacity)
                    break;
                if (key < 0x20 || key > 0x7f)
                    break;
                if (entry->flag_1b & 2) {
                    if (!isalnum(key) && key != '_' && key != ' ' && key != '\'')
                        break;
                }
                // A plain char c[2]: it shares the dead key parameter's slot.
                char c[2];
                c[0] = (char)key;
                c[1] = 0;
                // entry->text, not the text local: sets the evaluation order of the two calls.
                int width = GetTextPixelWidth(entry->text) + GetTextPixelWidth(c);
                if (width > entry->w - 4)
                    break;
                int n = entry->capacity - 1;
                for (i = n; i > control->cursor; i--)
                    text[i] = text[i - 1];
                text[control->cursor] = (char)key;
                control->cursor++;
                break;
            }
            }
            key = PopKey();
        }
    }
done:
    if (changed)
        DrawTextInput(control, index);
    return last;
}
