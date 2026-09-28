// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: Register allocation and stack layout differences. Original reuses arg1/arg2 slots
// for locals by loading them into ebx/ebp early; this compiler uses different slots. 47.1% match.
// Partial match (47.1%). The logic is correct but the compiler allocates
// stack slots differently than the original. The original reuses arg1/arg2
// stack slots for locals (entries base at [esp+0x3c], entry pointer at
// [esp+0x40]) because it loads them into ebx/ebp early, while this
// compiler uses different slots. The bounds check also uses different
// stack slots ([esp+0x10..0x1c] in the original vs [esp+0x40], [esp+0x10],
// [esp+0x14], [esp+0x18] here). The field_cc6 offset is correct (0xcc6)
// but the compiler generates 0xcc8 due to struct padding differences.
#include <string.h>
#include <ctype.h>

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

struct Object_0049fc50 {
    char unknown_0[0x18];
    Holder_0049fc50* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    int field_3c[6];                   // +0x3c to +0x50
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64
    char unknown_68[0xcc6 - 0x68];
    int field_cc6;                     // +0xcc6
};

int __stdcall FUN_0049fc50(Object_0049fc50* obj, int index);
int __stdcall FUN_004ab510(Object_0049fc50* obj, unsigned char buttons);
int __stdcall FUN_004ab5b0(Object_0049fc50* obj, unsigned int mask);
void __stdcall FUN_004ab690(Object_0049fc50* obj, int param_2);
int FUN_004c1ab0(void);
int __stdcall FUN_004c1b80(int key);

// FUNCTION: 0x4a4440
int __stdcall FUN_004a4440(Object_0049fc50* obj, int index, char key)
{
    Entry_0049fc50* entries = obj->holder->entries;
    Entry_0049fc50* entry = &entries[index];
    int x1, y1, x2, y2;
    int rel_x, rel_y;
    int buf[7];

    if (entry->text[0x147 - 0xb6] == 0 && !(entry->flags & 0x10))
        return 0;

    if (entry->type == 0) {
        x1 = 0;
    } else {
        x1 = entry->x1;
    }
    y1 = entry->y1;
    x2 = entry->x2 + x1 - 1;
    y2 = entry->y2 + y1 - 1;

    memcpy(buf, obj->field_3c, 24);
    buf[0] = y2;

    rel_x = buf[1] - entries->x1;
    rel_y = buf[2] - entries->y1;

    if (FUN_004ab510(obj, 1)) {
        if (rel_x < x1 || rel_x > x2 || rel_y < y1 || rel_y > y2)
            goto fail;
        FUN_0049fc50(obj, index);
        FUN_004ab690(obj, 1);
    } else if (FUN_004ab510(obj, 2)) {
        if (rel_x < x1 || rel_x > x2 || rel_y < y1 || rel_y > y2)
            goto fail;
        FUN_0049fc50(obj, index);
        FUN_004ab690(obj, 2);
    }

fail:
    if (obj->focus != index)
        goto check_queue;
    if (FUN_004ab5b0(obj, 3))
        goto check_queue;
    obj->focus = -1;
    if (rel_x < x1 || rel_x > x2 || rel_y < y1 || rel_y > y2)
        goto check_queue;
    return 1;

check_queue:
    if (obj->focus != -1) {
        Entry_0049fc50* e = &entries[obj->focus];
        if (e->type == 3 && FUN_004c1b80(0xfb)) {
            // fall through
        } else {
            goto final_check;
        }
    } else {
        goto final_check;
    }

final_check:
    if (obj->field_cc6 != 1 || key == 0)
        return 0;
    if (tolower(entry->text[0x147 - 0xb6]) != key &&
        toupper(entry->text[0x147 - 0xb6]) != key)
        return 0;
    FUN_004c1ab0();
    return 1;
}
