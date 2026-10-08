// Decompiled by deepseek-v4.1, deepseek-v4.1-flash, Opus, Haiku, Sonnet, space-bunny-free, LongCat 2.5 Preview Free, GPT-6.1-sol, mimo-v2.6-pro, claude-opus-5-5, GPT-6, Claude Opus 5.5, DeepSeek V4.1 Flash and Space Bunny Free. Names are provisional.
//
// The dialog's GUI layer loader, the entry-list operations that add and find
// entries, the animation reference and mouse-message helpers, the text-edit
// key handler, the dialog creators and handlers (CONFIRM.GUI, YESORNO.GUI,
// MSGBOX.GUI, NOTEXIST.GUI, CHOICE3.GUI, INPUT.GUI), the word-wrap and
// text-fit helpers, and the 16x16 "COLS" colour grid of the palette editor.
#include <string.h>
#include <windows.h>

#pragma pack(push, 1)

struct Layer_004aa8f0;
struct Menu_004aa8f0;

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
    // AddTextGadget stores a dword here, HandleTextEditKey tests a byte; the
    // union stays because its symbol ids keep the later functions' registers.
    union {
        int attribs;               // +0x1b
        unsigned char attribsLow;  // +0x1b
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
            void* surface;         // +0xbc
            char unknown_c0[0xc];
            char okName[0x10];     // +0xcc
            char prevName[0x10];   // +0xdc
            char focusName[0x10];  // +0xec
            char unknown_fc[0x5f];
        };
        struct {                   // the other entries: the NUL terminated text
            char text[0x80];       // +0xb6
            unsigned char stages;  // +0x136
            unsigned char stageIndex; // +0x137
            short capacity;        // +0x138
            char unknown_13a[6];
            unsigned short field_140; // +0x140
            short knobSize;           // +0x142
            void* sliderCallback;     // +0x144 (a typed function pointer here takes symbol ids)
            char unknown_148[0x13];
        };
    };
};

// The layer a dialog's +0x18 points at: its entry table at +4 and the
// installed handler at +8.
struct Layer_004aa8f0 {
    Layer_004aa8f0* next;          // +0x00
    Entry_004aa8f0* entries;       // +0x04
    void (__stdcall* handler)(Menu_004aa8f0*); // +0x08
    int data;                      // +0x0c
    int flags;                     // +0x10
    int field_14;                  // +0x14
    int keyboardInput;             // +0x18
    int field_1c;                  // +0x1c
    int current;                   // +0x20
    int surface;                   // +0x24
    char unknown_28[0x13];
    int textHandler;               // +0x3b
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
// mouse event at +0x3c, the current entry index at +0x60, the text cursor at
// +0x74, and the menu's own fields.
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

// The records AddButtonGadget, AddHotspotGadget and AddBarGadget copy into an
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

extern void __stdcall FadeRectangle(void* surface, int* rect, int mode);
extern char* __stdcall StripPath(char* path);
extern char* __stdcall ChangeExtension(char* out, char* in, char* ext);
extern int __stdcall HAPI_FileLengthByName(char* path);
extern void* __cdecl GameAllocIgnoreTag(const char* path, unsigned int size);
extern int __stdcall ReadGuiFile(void* entry, char* path);
extern void __cdecl GameFreeThunk(void* p);
extern int __stdcall RenderLayer(Menu_004aa8f0* menu, unsigned int flags);
extern void __cdecl HideSoftwareCursor(void);
extern void __cdecl ShowSoftwareCursor(void);
extern void __stdcall SelectAdjacentGadget(Menu_004aa8f0* menu, int value);
extern void __stdcall TrySetFocus(Menu_004aa8f0* menu, int value);
extern int __cdecl GetTextKeyColor(void);
extern void __stdcall SetTextColors(int a, int b);
extern void __stdcall SetFont(int a);
extern void __cdecl ClearKeyQueue(void);
extern void __stdcall CommitTextEdit(Menu_004aa8f0* menu, int index, char* text,
                                   int maxLength, int clear);

extern int __stdcall PointInRect(Rect_004b6720* r, int x, int y);
extern int __stdcall RectsOverlap(Rect_004b6720* a, Rect_004b6720* b);
extern void __stdcall DrawSurface(void* dest, void* image, int x, int y);

extern void __stdcall InitGafSequence(Ref_004ab400* ref, Src_004ab400* src, int index);
extern int __stdcall GetGafSequenceFrame(Ref_004ab400* ref);
extern void __stdcall SetCursorSprite(int handle);
int GetCursorSprite();

extern void __stdcall AdvanceGafSequence(Ref_004ab400* ref, int step);
extern int __stdcall PeekMouseEvent(Event_004ab5d0* out);
extern void __stdcall GetCurrentMouseEvent(Event_004ab5d0* out);
extern void __stdcall PopMouseEvent(Event_004ab5d0* out);
extern void __stdcall GetGadgetScreenRect(Entry_004aa8f0* table, int index, Rect_004b6720* out);
extern int __stdcall IsPointInRect(Rect_004b6720* r, int px, int py);

extern void __stdcall DrawTextInput(Menu_004aa8f0* control, int index);
extern int __stdcall GetTextPixelWidth(unsigned char* text);
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
// 0x4aaa3b) and then writes layer->entries/field_1c/surface/textHandler
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
            FadeRectangle(cur->entries->surface, rect, -0x18);
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
            layer = (Layer_004aa8f0*)GameAllocIgnoreTag(guiName, 0x10f57);
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
            GameFreeThunk(layer);
        }
    }
    layer->entries = entry;
    layer->field_1c = 0;
    layer->surface = 0;
    layer->textHandler = 0;
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
        menu->layer->keyboardInput = 0;
    strncpy((char*)&entry->name[0], guiName, 0x10);
    menu->field_64 = -1;
    if ((flags & 0x400) == 0) {
        HideSoftwareCursor();
        ret = RenderLayer(menu, flags | 1);
        ShowSoftwareCursor();
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
            layer->current = i;
        } else {
            layer->current = 0;
            SelectAdjacentGadget(menu, 1);
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
        TrySetFocus(menu, 1);
        menu->layer->current = 1;
        CommitTextEdit(menu, 1, (char*)&sub->count,
                     *(short*)((char*)sub + 0x138), 0);
        ClearKeyQueue();
        }
        return layer;
    }
    GameFreeThunk(layer);
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
int __stdcall HasLayer(Menu_004aa8f0* obj)
{
    return obj->layer != 0;
}

// Returns 1 when the object's entry has a name (after a two-character
// prefix) equal to `name`, ignoring case, over at most 16 characters.
// FUNCTION: 0x4ab060
int __stdcall IsScreenNamed(Menu_004aa8f0* obj, const char* name)
{
    // The entries pointer, two bytes in, is the first entry's name.
    if (obj->layer && _strnicmp((char*)&obj->layer->entries->name[0], name, 0x10) == 0)
        return 1;
    return 0;
}

// FUNCTION: 0x4ab0a0
void __stdcall ClearSelectedGadget(void* param_1)
{
    *(int*)((char*)param_1 + 0x60) = -1;
}

// FUNCTION: 0x4ab0b0
int __stdcall BlitLayers(Layer_004aa8f0* node, void* param_2, Rect_004b6720* param_3)
{
    Rect_004b6720 rect;
    if (node == 0) {
        return 0;
    }
    BlitLayers(node->next, param_2, param_3);
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
void __stdcall BlitMenuLayers(Menu_004aa8f0* param_1, unsigned int* param_2, int* param_3)
{
    BlitLayers(param_1->layer, param_2, (Rect_004b6720*)param_3);
}

// FUNCTION: 0x4ab190
void __stdcall SetDescListCleanupFlag(int param_1, int param_2)
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
    e->attribs = flags;
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
int __stdcall SetBackgroundSurface(Menu_004aa8f0* menu, int value)
{
    if (menu->layer != 0)
        menu->layer->surface = value;
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
int __stdcall AddHotspotGadget(Menu_004aa8f0* obj, Record_004ab310* record)
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
int __stdcall AddBarGadget(Menu_004aa8f0* obj, Record_004ab3a0* record)
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
// SetCursorSprite.
// FUNCTION: 0x4ab400
void __stdcall SetCursorAnimation(Menu_004aa8f0* p, Src_004ab400* src)
{
    p->src_2c = src;
    InitGafSequence(&p->ref, src, 0);
    p->flags_5c.active = 1;
    SetCursorSprite(GetGafSequenceFrame(&p->ref));
}

// Picks the source at +0x28 (alt) or +0x2c; with a source, points the
// reference at +0x30 at it (compare 0x4ab400) and sets the active flag,
// otherwise passes the plain value at +0x24 or +0x20 on to SetCursorSprite
// (compare 0x4ab4e0) and clears the flag. Nothing happens when the source
// or value is already current.
static inline void SetSource(Menu_004aa8f0* p, Src_004ab400* src)
{
    if (p->ref.src == src)
        return;
    InitGafSequence(&p->ref, src, 0);
    SetCursorSprite(GetGafSequenceFrame(&p->ref));
    p->flags_5c.active = 1;
}

static inline void SetValue(Menu_004aa8f0* p, int value)
{
    if (GetCursorSprite() == value)
        return;
    SetCursorSprite(value);
    p->ref.src = 0;
    p->flags_5c.active = 0;
}

// FUNCTION: 0x4ab440
void __stdcall SetCursorHover(Menu_004aa8f0* p, int alt)
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
void __stdcall ApplySharedCursorFrame(Menu_004aa8f0* p)
{
    SetCursorSprite(p->field_1c);
    p->flags_5c.active = 0;
}

// Compare 0x4ab4c0 and 0x4ab400: sets the three values at +0x1c..+0x24,
// passes the value on to SetCursorSprite and clears the active flag.
// FUNCTION: 0x4ab4e0
void __stdcall InitCursorFrames(Menu_004aa8f0* p, int value)
{
    p->field_1c = value;
    p->field_20 = value;
    p->field_24 = value;
    SetCursorSprite(value);
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
int __stdcall HasMouseKeyFlags(Menu_004aa8f0* obj, unsigned int mask)
{
    return (obj->field_54 & mask) != 0;
}

// One tick of a UI element: advances the reference at +0x30 when the active
// flag is set and forwards the entry it lands on, then peeks the next event,
// tests its point against the element's rectangle (from the entry at +0x18)
// and, on a hit or a plain code, pops the event into the element at +0x3c.
// FUNCTION: 0x4ab5d0
void __stdcall UpdateCursorAndMouse(Menu_004aa8f0* p)
{
    if (p->flags_5c.active) {
        int old = p->ref.index;
        AdvanceGafSequence(&p->ref, p->field_9a);
        if (p->ref.index != old)
            SetCursorSprite(GetGafSequenceFrame(&p->ref));
    }

    Event_004ab5d0 e;
    if (PeekMouseEvent(&e) != 0) {
        if (p->layer != 0) {
            Rect_004b6720 r;
            GetGadgetScreenRect(p->layer->entries, 0, &r);
            if (IsPointInRect(&r, e.data[0], e.data[1]) != 0 || e.data[2] == 0) {
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
void __stdcall SetClickMode(void* param_1, int param_2)
{
    *(int*)((char*)param_1 + 0x58) = param_2;
    *(int*)(*(int*)((char*)param_1 + 0x18) + 0x37) = param_2;
}

// FUNCTION: 0x4ab6b0
int __stdcall GetClickMode(Menu_004aa8f0* obj)
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
void __stdcall CommitTextEdit(Menu_004aa8f0* control, int index, char* text,
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

    if (holder->keyboardInput == 0)
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
                            int width = GetTextPixelWidth((unsigned char*)text);
                            while (width > entry->w) {
                                if (strlen(text) == 0)
                                    break;
                                text[strlen(text) - 1] = 0;
                                width = GetTextPixelWidth((unsigned char*)text);
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
                if (entry->attribsLow & 2) {
                    if (!isalnum(key) && key != '_' && key != ' ' && key != '\'')
                        break;
                }
                // A plain char c[2]: it shares the dead key parameter's slot.
                char c[2];
                c[0] = (char)key;
                c[1] = 0;
                // entry->text, not the text local: sets the evaluation order of the two calls.
                int width = GetTextPixelWidth((unsigned char*)entry->text) + GetTextPixelWidth((unsigned char*)c);
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

// The headers and types of the dialog creators sit here, after the text-edit
// handler: 0x4ab720 only matches with the symbol ids it had before them.
// stdio.h and math.h are included for their symbol counts, not for their
// declarations (docs/c2-regalloc.md).
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(push, 1)
// One palette entry.
struct PalEntry_004aa8f0 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char flags;
};

// The palette copy and the remap table built from it.
struct Palette_004aa8f0 {
    char unknown_0[0xb2];
    PalEntry_004aa8f0 dest[256];   // +0xb2
    char unknown_4b2[0x400];
    unsigned char table[256];      // +0x8b2
};

// One channel slider: its value byte at +0x140.
struct Slider_004aa8f0 {
    char unknown_0[0x140];
    unsigned char value;           // +0x140
};

// The palette editor dialog: the selected entry at +0x9b2 and the three
// sliders at +0xcb6.
struct PaletteDialog_004aa8f0 {
    char unknown_0[0x9b2];
    int index;                     // +0x9b2
    char unknown_9b6[0xcb6 - 0x9b6];
    Slider_004aa8f0* red;          // +0xcb6
    Slider_004aa8f0* green;        // +0xcba
    Slider_004aa8f0* blue;         // +0xcbe
};

#pragma pack(pop)

extern char DAT_00502ae8[];        // "OK"

int __stdcall FindGadgetIndex(Entry_004aa8f0* entries, const char* name, int type);
int __stdcall IsGadgetNamed(Entry_004aa8f0* entries, int index, char* name);
char* __stdcall Translate(char* text);
char* __stdcall WordWrapText(Menu_004aa8f0* menu, char* text, int width, int index);
int __stdcall GetFontHeight();
int GetScreenWidth();
int GetScreenHeight();
void __stdcall SetGadgetActiveByName(Menu_004aa8f0* menu, const char* name, int flag);
void __stdcall SetKeyboardInput(Menu_004aa8f0* menu, int flag);
void __stdcall MarkChanged(Menu_004aa8f0* menu);
void __stdcall UpdateMenu(Menu_004aa8f0* menu);
int __stdcall SelectFontForEntry(Entry_004aa8f0* entries, int index);
int GetFont();
int __stdcall GetTextWidth(int font, unsigned char* text);
int __stdcall FillRectangle(void* surface, Rect_004b6720* rect, int color);
int __stdcall SetPaletteColors(unsigned char* src, int start, int count);

struct FileHandle;
unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* data, unsigned int size);

void __stdcall YesNoDialogHandler(Menu_004aa8f0* menu);
void __stdcall MessageBoxHandler(Menu_004aa8f0* menu);
void __stdcall NotExistDialogHandler(Menu_004aa8f0* menu);
void __stdcall Choice3DialogHandler(Menu_004aa8f0* menu);
void __stdcall InputDialogHandler(Menu_004aa8f0* menu);

// Opens the confirmation dialog (CONFIRM.GUI) and sets its title text.
// FUNCTION: 0x4abb20
int __stdcall OpenConfirmDialog(Menu_004aa8f0* sub, char* title)
{
    Layer_004aa8f0* layer = LoadGuiLayer(sub, "CONFIRM.GUI", 0);
    if (layer) {
        Entry_004aa8f0* gadgets = layer->entries;
        int i = FindGadgetIndex(gadgets, "TITL", 5);
        strcpy(gadgets[i].text, title);
        gadgets[i].x = -1;
        return 1;
    }
    return 0;
}

// Same shape as 0x4ac300: tests the current GUI entry against "CHC1" and
// "CHC2".
// FUNCTION: 0x4abba0
void __stdcall YesNoDialogHandler(Menu_004aa8f0* gadget)
{
    Entry_004aa8f0* entries = gadget->layer->entries;
    int index = gadget->field_60;
    IsGadgetNamed(entries, index, "CHC1");
    IsGadgetNamed(entries, index, "CHC2");
}

// Opens the YESORNO.GUI dialog, fills its CHC1 / CHC2 / TITL text fields and
// installs YesNoDialogHandler as the handler. Same shape as 0x4abb20 / 0x4ac130.
// FUNCTION: 0x4abbd0
int __stdcall OpenYesNoDialog(Menu_004aa8f0* sub, char* param_2, char* param_3, char* param_4)
{
    Layer_004aa8f0* layer = LoadGuiLayer(sub, "YESORNO.GUI", 0x800);
    if (layer) {
        Entry_004aa8f0* entries = sub->layer->entries;
        Entry_004aa8f0* p1 = &entries[FindGadgetIndex(entries, "CHC1", 1)];
        Entry_004aa8f0* p2 = &entries[FindGadgetIndex(entries, "CHC2", 1)];
        Entry_004aa8f0* p3 = &entries[FindGadgetIndex(entries, "TITL", 5)];
        strcpy(p1->text, param_4);
        strcpy(p2->text, param_3);
        strcpy(p3->text, param_2);
        layer->handler = YesNoDialogHandler;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4abd00
void __stdcall MessageBoxHandler(Menu_004aa8f0* menu)
{
    Layer_004aa8f0* layer = menu->layer;
    IsGadgetNamed(layer->entries, menu->field_60, DAT_00502ae8);
}

// Returns 1 when the object's entry is named "MSGBOX.GUI" (after a
// two-character prefix); 0 when it has no entry.
// FUNCTION: 0x4abd20
int __stdcall IsMessageBoxScreen(Menu_004aa8f0* obj)
{
    if (!obj->layer)
        return 0;
    return strcmp((char*)&obj->layer->entries->name[0], "MSGBOX.GUI") == 0;
}

// Builds the MSGBOX.GUI dialog: word-wraps the (translated) message text, adds
// one TEXT entry per line, sizes the dialog from the longest line, centres it
// on the screen, stretches every text entry to the dialog width, positions the
// OK button and installs MessageBoxHandler as the handler.
//
// Suspected original bug: the max-line-width loop measures the entries at
// entries[count+1] .. entries[count+lines] (0x4abeb8 walks forward from
// count+1, one 0x15b step per line), while the text entries just added live at
// entries[count-lines+1] .. entries[count], so it measures one entry past the
// last line it added.
// Note: the two "OK" strings go to the table base's fields at +0xcc and +0xdc
// (0x4abfba, 0x4abfd8 address them from the table pointer, not from the entry
// that FindGadgetIndex returned, unlike the x/y fields just above them). If those
// are entry 0's own button-name fields that is deliberate, otherwise the OK
// entry is left unnamed.
// FUNCTION: 0x4abd90
int __stdcall OpenMessageBox(Menu_004aa8f0* gui, char* text, int wrapWidth, int centre, int autoHeight)
{
    Layer_004aa8f0* layer = LoadGuiLayer(gui, "MSGBOX.GUI", 0x800);
    if (layer) {
        char name[0x100];
        char buf[0x100];
        strcpy(name, Translate(text));
        char* wrapped = WordWrapText(gui, name, wrapWidth, -1);
        RenderLayer(gui, 2);
        strncpy(buf, wrapped, 0xfe);
        Entry_004aa8f0* entries = gui->layer->entries;
        char* line = strtok(buf, "\n");
        int lines = 0;
        int y = 0x14;
        int next = layer->entries->count + 1;
        if (line) {
            do {
                AddTextGadget(layer, "TEXT", line, 0, y, -1, 2);
                y += GetFontHeight() + 5;
                line = strtok(0, "\n");
                lines++;
            } while (line);
        }
        int width;
        if (autoHeight) {
            width = 0;
            if (lines > 0) {
                char* p = entries[next].text;
                int i = lines;
                do {
                    if (width <= GetTextPixelWidth((unsigned char*)p))
                        width = GetTextPixelWidth((unsigned char*)p);
                    p += 0x15b;
                } while (--i);
            }
            width += 0x14;
        } else {
            width = wrapWidth;
        }
        entries->w = width;
        entries->h = lines * 25 + entries[1].h + 0x28;
        entries->x = (GetScreenWidth() - entries->w) / 2;
        entries->y = (GetScreenHeight() - entries->h) / 2;
        int i;
        for (i = 0; i <= entries->count; i++) {
            if (entries[i].type == 5) {
                entries[i].w = entries->w;
                entries[i].attribs = 2;
            }
        }
        if (centre) {
            int k = FindGadgetIndex(entries, "OK", 0xe);
            if (k != -1) {
                entries[k].y = entries->h - entries[k].h - 0xf;
                entries[k].x = entries->w - entries[k].w - 0xf;
                strcpy(entries->okName, "OK");
                strcpy(entries->prevName, "OK");
            }
        } else {
            SetGadgetActiveByName(gui, "OK", 0);
        }
        SetKeyboardInput(gui, 1);
        RenderLayer(gui, 1);
        layer->handler = MessageBoxHandler;
        MarkChanged(gui);
        UpdateMenu(gui);
        GameFreeThunk(wrapped);
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4ac080
void __stdcall NotExistDialogHandler(Menu_004aa8f0* obj)
{
    IsGadgetNamed(obj->layer->entries, obj->field_60, DAT_00502ae8);
}

// Opens the "file does not exist" dialog (NOTEXIST.GUI), puts the name in
// its NAME gadget and installs NotExistDialogHandler as its handler; compare 0x4abb20.
// FUNCTION: 0x4ac0a0
int __stdcall OpenNotExistDialog(Menu_004aa8f0* sub, char* name)
{
    Layer_004aa8f0* dialog = LoadGuiLayer(sub, "NOTEXIST.GUI", 0);
    if (dialog) {
        Entry_004aa8f0* gadgets = sub->layer->entries;
        int i = FindGadgetIndex(gadgets, "NAME", 5);
        strcpy(gadgets[i].text, name);
        gadgets[i].x = -1;
        dialog->handler = NotExistDialogHandler;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4ac130
void __stdcall Choice3DialogHandler(Menu_004aa8f0* param_1)
{
    Entry_004aa8f0* p = param_1->layer->entries;
    FindGadgetIndex(p, "CHC1", 0xe);
    FindGadgetIndex(p, "CHC2", 0xe);
    FindGadgetIndex(p, "CHC3", 0xe);
}

// Creates the CHOICE3.GUI dialog, copies the title and three choice strings
// into the CHC1 / CHC2 / CHC3 / TITL gadget entries (each 0x15b bytes, text
// at +0xb6), clears the TITL entry's +0x13 field and installs Choice3DialogHandler
// as the dialog handler. Returns 1 if the dialog was created, else 0.
// FUNCTION: 0x4ac170
int __stdcall OpenChoice3Dialog(Menu_004aa8f0* menu, const char* title, const char* choice1,
                           const char* choice2, const char* choice3)
{
    Layer_004aa8f0* dialog = LoadGuiLayer(menu, "CHOICE3.GUI", 0);
    if (dialog != 0) {
        Entry_004aa8f0* entries = menu->layer->entries;
        RenderLayer(menu, 1);
        Entry_004aa8f0* e1 = &entries[FindGadgetIndex(entries, "CHC1", 1)];
        Entry_004aa8f0* e2 = &entries[FindGadgetIndex(entries, "CHC2", 1)];
        Entry_004aa8f0* e3 = &entries[FindGadgetIndex(entries, "CHC3", 1)];
        Entry_004aa8f0* et = &entries[FindGadgetIndex(entries, "TITL", 5)];
        strcpy(e1->text, choice1);
        strcpy(e2->text, choice2);
        strcpy(e3->text, choice3);
        strcpy(et->text, title);
        et->x = 0xffff;
        dialog->handler = Choice3DialogHandler;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4ac300
void __stdcall InputDialogHandler(Menu_004aa8f0* param_1)
{
    Entry_004aa8f0* esi = param_1->layer->entries;
    int edi = param_1->field_60;
    IsGadgetNamed(esi, edi, "CHC1");
    IsGadgetNamed(esi, edi, "CHC2");
    IsGadgetNamed(esi, edi, "INPT");
}

// Opens the player setup dialog (INPUT.GUI), fills the TITL / INPT / CHC1 /
// CHC2 gadget texts and installs InputDialogHandler as its handler. Returns 1 if the
// dialog was created, else 0.
// FUNCTION: 0x4ac340
int __stdcall OpenInputDialog(Menu_004aa8f0* sub, char* title, char* input, char* chc2, char* chc1, int unused)
{
    Layer_004aa8f0* dialog = LoadGuiLayer(sub, "INPUT.GUI", 0);
    if (dialog) {
        Entry_004aa8f0* gadgets = sub->layer->entries;
        Entry_004aa8f0* g = &gadgets[FindGadgetIndex(gadgets, "INPT", 3)];
        strcpy(g->text, input);
        g = &gadgets[FindGadgetIndex(gadgets, "TITL", 5)];
        strcpy(g->text, title);
        g->x = -1;
        Entry_004aa8f0* g1 = &gadgets[FindGadgetIndex(gadgets, "CHC1", 1)];
        Entry_004aa8f0* g2 = &gadgets[FindGadgetIndex(gadgets, "CHC2", 1)];
        strcpy(g1->text, chc1);
        strcpy(g2->text, chc2);
        dialog->handler = InputDialogHandler;
        return 1;
    }
    return 0;
}

// Builds a word-wrapped copy of `text` in a buffer allocated from
// the pool: the number of characters per line is width / (width of one
// digit), and a line is broken at a space, newline or '-' when the measured
// line would exceed `width` pixels. `index` selects the current font entry
// (SelectFontForEntry) when it is not -1; measurements go through the same
// GetTextPixelWidth / GetFont+GetTextWidth pair the sibling text fitter
// 0x4ac610 uses.
// The preheader size estimate `len + 3 * (len / (width / w)) + 2` allocates
// room for the CRLF pairs; the wrap-back loop overwrites the break character
// with the CR, so `s` is only advanced past it once.
// FUNCTION: 0x4ac4c0
char* __stdcall WordWrapText(Menu_004aa8f0* menu, char* text, int width, int index)
{
    Entry_004aa8f0* gadgets = menu->layer->entries;
    // Net-zero update: makes the allocator count an early use of text.
    text += 1; text -= 1;
    int len = strlen(text);
    if (index != -1)
        SelectFontForEntry(gadgets, index);
    int w;
    if (index == -1)
        w = GetTextPixelWidth((unsigned char*)"d");
    else
        w = GetTextWidth(GetFont(), (unsigned char*)"d");
    int size = len + 3 * (len / (width / w)) + 2;
    char* buf = (char*)GameAllocIgnoreTag("WordWrap", size);
    memset(buf, 0, size);
    int i = 0;
    char c = *text;
    char* s = text;
    char* p = buf;
    while (c != 0) {
        if (*s == (char)0xff)
            break;
        buf[i] = *s;
        char next = s[1];
        // i is incremented before s: the source order sets the instruction order.
        i++;
        s++;
        if (next == ' ' || next == '\n' || next == '-') {
            int wrapped = 0;
            int m;
            if (index == -1)
                m = GetTextPixelWidth((unsigned char*)p);
            else
                m = GetTextWidth(GetFont(), (unsigned char*)p);
            if (width <= m) {
                buf[i] = 0;
                wrapped = 1;
                char ch = s[-1];
                i--;
                s--;
                while (ch != ' ' && ch != '-') {
                    // Net-zero update: makes the allocator count an extra use of i.
                    i += 1; i -= 1;
                    buf[i] = 0;
                    ch = s[-1];
                    i--;
                    s--;
                }
                buf[i] = '\r';
                i++;
                buf[i] = '\n';
                i++;
                s++;
            }
            if (wrapped)
                p = buf + i;
        }
        c = *s;
        if (c == '\n')
            p = buf + i + 1;
    }
    buf[i] = 0;
    return buf;
}

// Fit `text` into `limit` pixels: if it is too wide, chop characters off the
// end until it (plus the "..." marker, when flag is set) fits, then append the
// marker. Charset -1 means the default font (GetTextPixelWidth), anything else
// selects the holder entry (SelectFontForEntry) and measures with GetTextWidth.
static inline int Measure_004ac610(unsigned char* text, int charset)
{
    if (charset == -1)
        return GetTextPixelWidth(text);
    return GetTextWidth(GetFont(), text);
}

// FUNCTION: 0x4ac610
void __stdcall TruncateTextWithEllipsis(Menu_004aa8f0* obj, unsigned char* text, int limit,
                            int charset, int flag)
{
    Entry_004aa8f0* entries = obj->layer->entries;
    if (charset != -1)
        SelectFontForEntry(entries, charset);

    // Declared before width: the allocator then knows width is dead at the truncation test.
    int dots;
    int width;
    if (charset == -1) {
        width = GetTextPixelWidth(text);
        if (width < limit)
            return;
    } else {
        width = GetTextWidth(GetFont(), text);
        if (width < limit)
            return;
    }

    dots = flag ? Measure_004ac610((unsigned char*)"...", charset) : 0;

    unsigned char* end = text;
    while (*end)
        end++;

    while (width + dots > limit) {
        if (end == text)
            break;
        end--;
        *end = 0;
        width = Measure_004ac610(text, charset);
    }

    if (flag)
        strcpy((char*)end, "...");
}

// Unused here: forward declarations of later functions of this file. Their
// symbol ids in front of each palette helper (here and before 0x4ac7d0,
// 0x4ac8c0 and 0x4acbe0) set its register allocation (docs/c2-regalloc.md).
void __stdcall CopyPaletteEntries(char* obj, void* dest);

// Builds a 256-entry remap table: for each colour of the source palette,
// the index of the closest colour (sum of absolute RGB differences) in the
// destination palette.
// FUNCTION: 0x4ac710
void __stdcall BuildColorRemapTable(PalEntry_004aa8f0* src, PalEntry_004aa8f0* dest, unsigned char* table)
{
    for (int n = 256; n != 0; n--) {
        int i = 0;
        int best = 9999999;
        PalEntry_004aa8f0* p = dest;
        int b = src->b;
        int g = src->g;
        int r = src->r;
        int bestIndex;
        for (; i < 256; i++, p++) {
            int d = abs(b - p->b) + abs(g - p->g) + abs(r - p->r);
            if (d < best) {
                best = d;
                bestIndex = i;
            }
        }
        src++;
        *table++ = (unsigned char)bestIndex;
    }
}

void __stdcall ApplySlidersToPaletteEntry(PaletteDialog_004aa8f0* obj, PalEntry_004aa8f0* palette);

// Copies a 256-entry palette into the object and then builds a 256-byte remap
// table: for each colour of the copied palette, the index of the closest
// colour (sum of absolute RGB differences) in the source palette.
// FUNCTION: 0x4ac7d0
void __stdcall RemapPaletteToClosestIndices(Palette_004aa8f0* pal, PalEntry_004aa8f0* src, PalEntry_004aa8f0* copy)
{
    memcpy(pal->dest, copy, 0x400);

    unsigned char* table = pal->table;
    PalEntry_004aa8f0* p = pal->dest;
    for (int n = 256; n != 0; n--) {
        int i = 0;
        int best = 9999999;
        PalEntry_004aa8f0* q = src;
        int bestIndex;
        for (; i < 256; i++, q++) {
            int d = abs(p->r - q->r) + abs(p->b - q->b) + abs(p->g - q->g);
            if (d < best) {
                best = d;
                bestIndex = i;
            }
        }
        p++;
        *table++ = (unsigned char)bestIndex;
    }
}

// FUNCTION: 0x4ac8a0
void __stdcall CopyPaletteEntries(char* obj, void* dest)
{
    memcpy(dest, obj + 0xb2, 0x100 * sizeof(int));
}

void __stdcall ShowPaletteEntryRgb(Menu_004aa8f0* obj, unsigned char* colors, int index);
void __stdcall LerpPaletteRange(unsigned char* data, int a, int b);
void __stdcall WriteTabs(FileHandle* file, int depth);

// Draws the 16 x 16 palette grid: one 8 x 8 cell per colour, at the "COLS"
// gadget's position.
// FUNCTION: 0x4ac8c0
void __stdcall DrawColorGrid(Menu_004aa8f0* obj)
{
    // grid before gadgets: operand order of the prologue sums follows symbol order.
    Entry_004aa8f0 *grid, *gadgets;
    gadgets = obj->layer->entries;
    int index = FindGadgetIndex(gadgets, "COLS", 6);
    grid = &gadgets[index];
    void* surface = gadgets->surface;
    int x0 = grid->x + gadgets->x;
    int y = gadgets->y + grid->y;
    Rect_004b6720 rect;
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            rect.left = x0 + col * 8;
            rect.top = y + row * 8;
            rect.right = rect.left + 7;
            rect.bottom = rect.top + 7;
            FillRectangle(surface, &rect, row * 16 + col);
        }
    }
}

// Writes the red, green and blue bytes of one entry of a palette (4 bytes per
// entry) into the field_140 word of the "RED", "GREN" and "BLUE" GUI entries,
// then refreshes the object's gadget state.
// FUNCTION: 0x4aca20
void __stdcall ShowPaletteEntryRgb(Menu_004aa8f0* obj, unsigned char* colors, int index)
{
    Entry_004aa8f0* entries = obj->layer->entries;
    // Entry via a local: addressing entries[i] directly changes the allocation.
    Entry_004aa8f0* e;
    e = &entries[FindGadgetIndex(entries, "RED", 4)];
    e->field_140 = colors[index * 4];
    e = &entries[FindGadgetIndex(entries, "GREN", 4)];
    e->field_140 = colors[index * 4 + 1];
    e = &entries[FindGadgetIndex(entries, "BLUE", 4)];
    e->field_140 = colors[index * 4 + 2];
    RenderLayer(obj, 4);
}

// Interpolates the 256-entry RGB palette between entry `lo` and entry `hi`.
// Note: the two 0x400-byte copies run on a 768-byte stack buffer (see the bug
// note in the pull request); the original really does emit them, so they stay.
// FUNCTION: 0x4acae0
void __stdcall LerpPaletteRange(unsigned char* data, int a, int b)
{
    unsigned char buf[768];
    memcpy(buf, data, 0x400);

    int lo = a < b ? a : b;
    int hi = a > b ? a : b;

    int base[3];
    int delta[3];
    int den = hi - lo;
    int i0 = lo * 3;
    base[0] = buf[i0 + 0];
    base[1] = buf[i0 + 1];
    base[2] = buf[i0 + 2];
    unsigned char* dst = &buf[i0];
    int i1 = hi * 3;
    delta[0] = buf[i1 + 0] - base[0];
    delta[1] = buf[i1 + 1] - base[1];
    delta[2] = buf[i1 + 2] - base[2];

    for (int i = lo; i < hi; i++) {
        for (int j = 0; j < 3; j++) {
            *dst = (unsigned char)(base[j] + delta[j] * (i - lo) / den);
            dst++;
        }
    }

    memcpy(data, buf, 0x400);
}


// Unused here: the type of the neighbour 0x4ac970, which has a file of its own.
struct Object_004ac970;

// Converts a screen position to the index of a cell in the 16x16 "COLS"
// colour grid gadget (row * 16 + column, each cell 8 pixels).
// FUNCTION: 0x4acbe0
int __stdcall GetColorCellAt(Menu_004aa8f0* obj, int x, int y)
{
    Entry_004aa8f0* gadgets = obj->layer->entries;
    int index = FindGadgetIndex(gadgets, "COLS", 6);
    Entry_004aa8f0* grid = &gadgets[index];
    int col = (x - gadgets->x - grid->x) / 8;
    int row = (y - gadgets->y - grid->y) / 8;
    if (col < 0) {
        col = 0;
    }
    if (col > 15) {
        col = 15;
    }
    if (row < 0) {
        row = 0;
    }
    if (row > 15) {
        row = 15;
    }
    return row * 16 + col;
}

// Copies three slider values into one palette entry and applies it.
// FUNCTION: 0x4acc70
void __stdcall ApplySlidersToPaletteEntry(PaletteDialog_004aa8f0* obj, PalEntry_004aa8f0* palette)
{
    PalEntry_004aa8f0 c;
    c.r = obj->red->value;
    c.g = obj->green->value;
    c.b = obj->blue->value;
    palette[obj->index] = c;
    SetPaletteColors((unsigned char*)&c, obj->index, 1);
}

// Writes `depth` tab characters to a file (sibling of 0x4acda0).
// FUNCTION: 0x4accd0
void __stdcall WriteTabs(FileHandle* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        HAPI_WriteFile(file, &tab, 1);
}
