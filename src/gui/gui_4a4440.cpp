// Decompiled by GPT-5.6-Terra. Names are provisional.
#include <string.h>

int __cdecl tolower(int);
int __cdecl toupper(int);

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

struct Point_0049fc50 {
    int x;
    int y;
    int unknown_8[4];
};

#pragma pack(push, 1)
struct Object_0049fc50 {
    char unknown_0[0x18];
    Holder_0049fc50* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_0049fc50 point;              // +0x3c to +0x50
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64
    char unknown_68[0xcc6 - 0x68];
    int field_cc6;                     // +0xcc6
};
#pragma pack(pop)

int __stdcall FUN_0049fc50(Object_0049fc50* obj, int index);
int __stdcall IsMouseButtonMessage(Object_0049fc50* obj, unsigned char buttons);
int __stdcall FUN_004ab5b0(Object_0049fc50* obj, unsigned int mask);
void __stdcall FUN_004ab690(Object_0049fc50* obj, int param_2);
int PopKey(void);
int __stdcall IsKeyDown(int key);

// FUNCTION: 0x4a4440
int __stdcall FUN_004a4440(Object_0049fc50* obj, int index, char key)
{
    Entry_0049fc50* entries = obj->holder->entries;
    Entry_0049fc50* entry = &entries[index];
    struct Rect_0049fc50 { int x1, y1, x2, y2; } rect;
    Point_0049fc50 point;
    int rel_x, rel_y;

    if (entry->text[0x147 - 0xb6] == 0 && (entry->flags & 0x10))
        return 0;

    if (entry->type == 0) {
        rect.x1 = 0;
        rect.y1 = 0;
    } else {
        rect.x1 = entry->x1;
        rect.y1 = entry->y1;
    }
    rect.x2 = entry->x2 + rect.x1 - 1;
    rect.y2 = entry->y2 + rect.y1 - 1;

    memcpy(&point, &obj->point, 24);

    rel_x = point.x - entries->x1;
    rel_y = point.y - entries->y1;

    if (IsMouseButtonMessage(obj, 1)) {
        if (rel_x < rect.x1 || rel_x > rect.x2 || rel_y < rect.y1 || rel_y > rect.y2)
            goto fail;
        FUN_0049fc50(obj, index);
        FUN_004ab690(obj, 1);
    } else if (IsMouseButtonMessage(obj, 2)) {
        if (rel_x < rect.x1 || rel_x > rect.x2 || rel_y < rect.y1 || rel_y > rect.y2)
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
    if (rel_x < rect.x1 || rel_x > rect.x2 || rel_y < rect.y1 || rel_y > rect.y2)
        goto check_queue;
    return 1;

check_queue:
    if (obj->focus != -1) {
        Entry_0049fc50* e = &entries[obj->focus];
        if (e->type == 3 && !IsKeyDown(0xfb))
            return 0;
    }

final_check:
    if (obj->field_cc6 != 1 || *(int*)&key == 0)
        return 0;
    if ((char)tolower(entry->text[0x147 - 0xb6]) != key &&
        (char)toupper(entry->text[0x147 - 0xb6]) != key)
        return 0;
    PopKey();
    return 1;
}
