// Decompiled by space-bunny-free. Names are provisional.
// Marks GUI entry `index` as the selected one and outlines it: every entry of
// type 3 is cleared first, then the selected one is given state 30 and, unless
// its type rules it out, a box is drawn around it in six shrinking steps.

#pragma pack(push, 1)
struct Entry_004a16f0 {               // 0x15b bytes
    char type;                       // +0x00
    char unknown_01[0x12];
    short x;                         // +0x13
    short y;                         // +0x15
    short width;                     // +0x17
    short height;                    // +0x19
    char unknown_1b[4];
    int state;                       // +0x1f
    char unknown_23[0xb6 - 0x23];
    short count;                     // +0xb6 (entry 0 holds the entry count)
    char unknown_b8[4];
    int surface;                     // +0xbc (entry 0 holds the surface)
    char unknown_c0[0x15b - 0xc0];
};
#pragma pack(pop)

struct Holder_004a16f0 {
    char unknown_0[4];
    Entry_004a16f0* entries;          // +0x4
};

#pragma pack(push, 1)
struct Class_004a16f0 {
    char unknown_0[0x18];
    Holder_004a16f0* holder;          // +0x18
    char unknown_1c[0xcca - 0x1c];
    int f_cca;                        // +0xcca
};
#pragma pack(pop)

struct Rect_004a16f0 {
    int left;
    int top;
    int right;
    int bottom;
};

void __stdcall DrawLitRectangle(int surface, Rect_004a16f0* rect, int level);

// FUNCTION: 0x4a16f0
void __stdcall FUN_004a16f0(Class_004a16f0* obj, int index, int param_3)
{
    Entry_004a16f0* entries = obj->holder->entries;
    int i;
    // Declared up here, used only at the bottom: in this scope MSVC keeps the
    // entry field loads in source order instead of hoisting them together.
    Rect_004a16f0 rect;
    int level;
    int step;
    obj->f_cca = 1;
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
        Entry_004a16f0* e = &entries[index];
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
