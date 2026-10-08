// Decompiled by Opus, GPT-5.6-Terra, muse-spark-1.3-free, space-bunny-free, deepseek-v4.1-flash, GPT-6.1-sol, claude-opus-5-5, Sonnet and Haiku. Names are provisional.
// The second gui module (0x4a04f0 to 0x4a1970): the gadget entry accessors
// (text, value, status, stage, rect and flag setters), the entry lookups, the
// font selection helper and the small rectangle predicates.
#include <string.h>

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
struct Dialog {
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

// The inclusive bounding rectangle the gadget helpers fill: x0/y0 at the top
// left, x1/y1 at the bottom right.
struct Rect {
    int x0;                            // +0x0
    int y0;                            // +0x4
    int x1;                            // +0x8
    int y1;                            // +0xc
};

#pragma pack(pop)

void __stdcall FUN_004a03f0(Dialog* menu, int index, int value);
void __stdcall FUN_004a0340(Dialog* obj, int index);
char* __stdcall Translate(char* text);
void __stdcall FatalError(char* path);
void __stdcall DrawLitRectangle(int surface, Rect* rect, int level);
void __stdcall SetFont(int id);
int __cdecl tolower(int c);

extern Dialog* g_guiContext;

static inline int FindEntry(Entry_004a04f0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

static inline Entry_004a04f0* GetEntry(Dialog* obj, int index)
{
    return obj->holder->entries + index;
}

static inline Entry_004a04f0* GetEntries(Dialog* obj)
{
    return obj->holder->entries;
}

static inline char* GetData(Dialog* obj)
{
    return (char*)obj->holder->entries;
}

// FUNCTION: 0x4a04f0
char __stdcall FUN_004a04f0(Dialog* obj, char* name)
{
    Entry_004a04f0* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i == -1) {
        return -1;
    }
    return entries[i].value;
}

// FUNCTION: 0x4a0570
void __stdcall FUN_004a0570(Dialog* obj, char* name, int param_3)
{
    if (obj->holder) {
        int index = FindEntry(obj->holder->entries, name);
        if (index != -1)
            FUN_004a03f0(obj, index, param_3);
    }
}

// The two redundant type re-tests (cmp dl,dl at 0x4a064c and cmp dl,1 at 0x4a0679)
// come from separate if statements that re-test entry->type; the tolower calls
// are compared directly, with no named temporaries.
// FUNCTION: 0x4a05e0
void __stdcall FUN_004a05e0(Dialog* obj, int index)
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
void __stdcall SetGadgetTextByName(Dialog* obj, char* name, char* text)
{
    if (obj->holder) {
        Entry_004a04f0* entries = obj->holder->entries;
        int index = FindEntry(entries, name);
        if (index != -1) {
            strncpy(entries[index].text, text, 0x80);
            obj->changed = 1;
            FUN_004a05e0(obj, index);
        }
    }
}

// 0x4a0880 keeps its own file: the merged file's include set and prelude
// put the entries pointer in eax instead of ecx.
struct Object_004a0880;
void __stdcall SetGadgetText(Object_004a0880* obj, int index, char* text);

// FUNCTION: 0x4a08f0
void __stdcall FUN_004a08f0(Dialog* obj, int index)
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
void __stdcall FUN_004a09c0(Dialog* context, int index, char* source, int value)
{
    if (index == -1 || context->holder == 0)
        return;
    Entry_004a04f0* entries = context->holder->entries;
    char* text = Translate(source);

    switch (entries[index].type) {
    case 5:
        strncpy(entries[index].text, text, 0x80);
        if (entries[index].count_136 != 0)
            FUN_004a05e0(context, index);
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
        FUN_004a05e0(context, index);
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
void __stdcall FUN_004a0bf0(Dialog* obj, char* name, char* param_3, int param_4)
{
    if (obj->holder != 0) {
        int index = FindEntry(obj->holder->entries, name);
        if (index != -1) {
            FUN_004a09c0(obj, index, param_3, param_4);
        }
    }
}

// Finds a gadget by name (as in 0x4a0570), stores a value into it and marks
// the object as changed.
// FUNCTION: 0x4a0c70
void __stdcall FUN_004a0c70(Dialog* obj, char* name, int value)
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
char* __stdcall GetGadgetText(Dialog* obj, char* name, char* buf)
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
void __stdcall FUN_004a0e00(Dialog* obj, char* name, char* text)
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
int __stdcall GetGadgetStatus(Dialog* obj, int index)
{
    return GetEntries(obj)[index].field_138;
}

// Looks up the gadget entry by name (the lookup of 0x49fdf0, inlined) and
// returns its field 0x137 when the entry's state is 1, otherwise -1 (compare
// 0x4a0ff0).
// FUNCTION: 0x4a0f60
int __stdcall GetButtonStageByName(Dialog* obj, char* name)
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
int __stdcall GetButtonStage(Dialog* obj, int index)
{
    Entry_004a04f0* entries = obj->holder->entries;
    if (entries[index].type == 1) {
        return entries[index].field_137;
    }
    return -1;
}

// FUNCTION: 0x4a1030
int __stdcall SetButtonStage(Dialog* obj, int index, char value)
{
    Entry_004a04f0* entries = obj->holder->entries;
    if (entries[index].type == 1) {
        entries[index].field_137 = value;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4a1080
int __stdcall SetButtonStageByName(Dialog* obj, char* name, char value)
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
int __stdcall SetGadgetStatusByName(Dialog* obj, char* name, int value)
{
    Entry_004a04f0* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i != -1) {
        entries[i].field_138 = value;
        obj->changed = 1;
        if (value) {
            FUN_004a0340(obj, i);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4a11c0
void __stdcall SetGadgetStatus(Dialog* param_1, int param_2, short param_3)
{
    *(short*)(GetData(param_1) + param_2 * 347 + 0x138) = param_3;
    FUN_004a0340(param_1, param_2);
}

// FUNCTION: 0x4a1200
void __stdcall FUN_004a1200(Dialog* obj, int index, int value)
{
    GetEntries(obj)[index].flag_bit = value;
    obj->changed = 1;
}

// Finds a gadget by name and sets its flag bit (see 0x4a1080 and 0x4a1200).
// FUNCTION: 0x4a1250
void __stdcall FUN_004a1250(Dialog* obj, char* name, int value)
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
void __stdcall FUN_004a12e0(Dialog* obj, int index, int value)
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
// FUN_004a12e0; byte-identical to 0x4a14c0.
// FUNCTION: 0x4a1450
void __stdcall FUN_004a1450(Dialog* obj, char* name, int param_3)
{
    int index = FindEntry(obj->holder->entries, name);
    if (index != -1)
        FUN_004a12e0(obj, index, param_3);
}

// Looks up a menu entry by name and, when found, passes its index to
// FUN_004a12e0; compare 0x4a0570 and 0x4a1530.
// FUNCTION: 0x4a14c0
void __stdcall FUN_004a14c0(Dialog* obj, char* name, int param_3)
{
    int index = FindEntry(obj->holder->entries, name);
    if (index != -1)
        FUN_004a12e0(obj, index, param_3);
}

// FUNCTION: 0x4a1530
void __stdcall FUN_004a1530(Dialog* obj, char* name, char value)
{
    int i = FindEntry(obj->holder->entries, name);
    obj->holder->entries[i].field_13a = value;
    obj->changed = 1;
}

// FUNCTION: 0x4a15c0
void __stdcall FUN_004a15c0(char* param_1, int param_2, Rect* param_3)
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

// Fills the bounding rectangle of a gadget entry: the entry's position and
// size when it is active, or a degenerate rectangle at the origin otherwise.
// FUNCTION: 0x4a1630
void __stdcall GetGadgetRect(Entry_004a04f0* entry, Rect* rect)
{
    if (entry->type == 0) {
        rect->x0 = 0;
        rect->y0 = 0;
    } else {
        rect->x0 = entry->x;
        rect->y0 = entry->y;
    }
    rect->x1 = entry->width + rect->x0 - 1;
    rect->y1 = entry->height + rect->y0 - 1;
}

// FUNCTION: 0x4a1680
void __stdcall FUN_004a1680(char* param_1, int param_2, Rect* param_3)
{
    char* e = param_1 + param_2 * 0x15b;
    param_3->x0 = *(short*)(e + 0x13);
    param_3->y0 = *(short*)(e + 0x15);
    if (*e != 0) {
        param_3->x0 += *(short*)(param_1 + 0x13);
        param_3->y0 += *(short*)(param_1 + 0x15);
    }
    param_3->x1 = *(short*)(e + 0x17) - 1 + param_3->x0;
    param_3->y1 = *(short*)(e + 0x19) - 1 + param_3->y0;
}

// Marks GUI entry `index` as the selected one and outlines it: every entry of
// type 3 is cleared first, then the selected one is given state 30 and, unless
// its type rules it out, a box is drawn around it in six shrinking steps.
// FUNCTION: 0x4a16f0
void __stdcall FUN_004a16f0(Dialog* obj, int index, int param_3)
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
            rect.x0 = 0;
            rect.y0 = 0;
        } else {
            rect.x0 = e->x;
            rect.y0 = e->y;
        }
        rect.x1 = e->width + rect.x0 - 1;
        rect.y1 = e->height + rect.y0 - 1;
        level = 0x1f;
        for (step = 0; step < 6; step++) {
            rect.x0--;
            rect.y0--;
            rect.x1++;
            rect.y1++;
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
int __stdcall FUN_004a18c0(Entry_004a04f0* entries, int index)
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
int __stdcall FUN_004a1920(Rect* r, int px, int py)
{
    if (px >= r->x0 && px <= r->x1 && py >= r->y0 && py <= r->y1) {
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
