// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6. Names are
// provisional. MATCH, 765 bytes. Capture g_guiContext in a root local before searching its entry
// names, and pass that captured root to FUN_004a03f0. The final scroll call uses a fresh global
// lookup, as in the original. This fixes the earlier register family and permits a normal integer
// remain local instead of overwriting param_1 with the remaining height. Initialize remain before
// storing me->first to reproduce the final stack-store ordering.
// The missing-name path calls the fatal-error routine FatalError, which
// exits the process. Its following null-entry accesses are unreachable.
#include <string.h>

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

struct List_004a32a0 {
    char unknown_0[0x0c];
    unsigned short* glyphs; // +0x0c
};

struct Glyph_004a32a0 {
    unsigned short width;
    unsigned short height; // +0x02
};

struct Holder_004a32a0 {
    int unknown_0;
    Entry_004a32a0* entries; // +0x04
};

struct Root_004a32a0 { // g_guiContext
    int current;       // +0x00
    char unknown_04[0x14 - 0x04];
    List_004a32a0* language; // +0x14
    Holder_004a32a0* holder; // +0x18
};

struct Class_004a32a0 {
    char unknown_00[0x18];
    Holder_004a32a0* holder; // +0x18
};

extern Root_004a32a0* g_guiContext;

void __stdcall FatalError(char* msg);
int GetFontHeight();
int __stdcall GetGafFrame(unsigned short* glyphs, int c);
void __stdcall FUN_004a03f0(Root_004a32a0* menu, int index, int value);
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
void __stdcall FUN_004a32a0(Class_004a32a0* param_1, char* name, int bitmap, int count, int flag) {
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
    Root_004a32a0* root = g_guiContext;
    if (root->holder != 0) {
        int j2 = FindName_004a32a0(root->holder->entries, text);
        if (j2 != -1) {
            FUN_004a03f0(root, j2, remain < 0);
        }
    }
    if (remain < 0) {
        DrawSlider(g_guiContext, found);
    }
}
